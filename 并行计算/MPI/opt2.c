//
// Created by hhhxiao on 5/6/20.
//

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define BLOCK_LOW(id, p, n) ((id)*(n)/(p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id, p, n) (BLOCK_LOW((id)+1,p,n)-BLOCK_LOW(id,p,n))


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
    if (argc != 2) {
        if (id == 0)printf("command line %s <m>\n", argv[0]);
        MPI_Finalize();
    }

    long n = atol(argv[1]);
    long min_size = (long) sqrt((double) n);
    min_size = (min_size + 1) / 2;
    n = (n + 1) / 2;
    long low_value = 1 + BLOCK_LOW(id, p, n - 1); //current min val id 2
    long high_value = 1 + BLOCK_HIGH(id, p, n - 1);
    long size = BLOCK_SIZE(id, p, n - 1);

    char *marked = (char *) malloc(size);
    char *min_primer_list = (char *) malloc(min_size);

    if (!marked || !min_primer_list) {
        printf("alloc failure\n");
        MPI_Finalize();
        exit(2);
    }

    for (int i = 0; i < size; ++i)marked[i] = 0;
    for (int j = 0; j < min_size; ++j) min_primer_list[j] = 0;


    int idx = 0;
    int prime = 3;
    do {
        long min_test_number;
        for (int i = prime * prime; i <= 2 * min_size + 3; i += prime) {
            if (i % 2 == 1)
                min_primer_list[(i - 3) / 2] = 1;

        }
        if (prime * prime > low_value * 2 + 1) {
            min_test_number = prime * prime;
        } else {
            if ((low_value * 2 + 1) % prime == 0) {
                min_test_number = low_value * 2 + 1;
            } else {
                min_test_number = prime - (low_value * 2 + 1) % prime + low_value * 2 + 1;
            }
        }

        for (long i = min_test_number; i <= high_value * 2 + 1; i += prime) {
            if (i % 2 == 1) {
                marked[(i - low_value * 2 - 1) / 2] = 1;
            }
        }

        while (min_primer_list[++idx]);
        prime = idx * 2 + 3;
    } while (prime * prime <= 2 * n + 3);


    //count num
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

