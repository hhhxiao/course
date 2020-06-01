//
// Created by hhhxiao on 5/10/20.
//
//cache opt
// Created by hhhxiao on 5/6/20.
//

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define BLOCK_LOW(id, p, n) ((id)*(n)/(p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id, p, n) (BLOCK_LOW((id)+1,p,n)-BLOCK_LOW(id,p,n))


void get_range_begin(long prime, long first, long *begin) {
    if (prime * prime > first) {
        *begin = prime * prime;
    } else {
        if (first % prime == 0) {
            *begin = first;
        } else {
            *begin = prime - first % prime + first;
        }
    }
}

int main(int argc, char *argv[]) {
    int count;//local primer count
    double elapsed_time;
    int global_count;
    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    int id;
    int p;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    if (argc != 3) {
        if (id == 0)printf("command line %s <num> <cache line>\n", argv[0]);
        MPI_Finalize();
    }

    long n = atol(argv[1]);
    long cache_line = atol(argv[2]);

    long min_size = (long) sqrt((double) n);
    min_size = (min_size - 1) / 2;
    n = (n + 1) / 2;
    long low_value = 1 + BLOCK_LOW(id, p, n - 1); //current min val id 2
    long high_value = 1 + BLOCK_HIGH(id, p, n - 1);
    long size = BLOCK_SIZE(id, p, n - 1);

    char *marked = (char *) malloc(size * sizeof(char));
    char *min_primer_list = (char *) malloc(sizeof(char) * min_size);
    if (!marked || !min_primer_list) {
        printf("alloc failure\n");
        MPI_Finalize();
        exit(2);
    }

    for (int i = 0; i < size; ++i)marked[i] = 0;
    for (int i = 0; i < min_size; ++i) min_primer_list[i] = 0;


    //find all the prime number within sqrt(n)
    int idx = 0;
    int prime = 3;
    do {
        for (int i = prime * prime; i <= 2 * min_size + 3; i += prime) {
            if (i % 2 == 1) {
                min_primer_list[(i - 3) / 2] = 1;
            }
        }

        while (min_primer_list[++idx]);
        prime = idx * 2 + 3;
    } while (prime * prime <= 2 * n + 3);

    //cache line calculator


    long line_num = size / cache_line;
    //cal the complete part
    long first, last;
    for (int j = 0; j <= line_num; ++j) {
        //iterator every cache_line
        //range is [j * cache_size --- (j+1)*cachesize]
        first = j * cache_line + low_value;
        last = (j + 1) * (cache_line) + low_value - 1;
        if (j == line_num)
            last = high_value;
        // printf("p(%d) block[%d]:(%ld,%ld)\n", id, j, first, last);
        long prime_num, begin;
        for (int k = 0; k < min_size;) {
            prime_num = 2 * k + 3;
            get_range_begin(prime_num, first * 2 + 1, &begin);
            //  printf("p(%d) block[%d]:range(%ld,%ld),prime = %ld\n", id, j, begin, last, prime_num);
            if (begin < last * 2 + 1)
                for (long i = begin; i <= last * 2 + 1; i += prime_num) {
                    if (i % 2 == 1) {
                        marked[(i - low_value * 2 - 1) / 2] = 1;
                        // printf("prime: %ld mark %ld\n", prime_num, i);
                    }
                }
            while (min_primer_list[++k]);
        }

    }

    // count num
    count = 0;
    for (int j = 0; j < size; ++j)
        if (!marked[j]) count++;
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();
    if (id == 0) {
        printf("%d %.4f", global_count + 1, elapsed_time);
    }
    MPI_Finalize();
    return 0;
}


