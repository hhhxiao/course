//
// Created by hhhxiao on 5/5/20.
//
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) ((a)<(b)?(a):(b))
//process id num of numbers num of processor
#define BLOCK_LOW(id, p, n) ((id)*(n)/(p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id, p, n) (BLOCK_LOW((id)+1,p,n)-BLOCK_LOW(id,p,n))
#define BLOCK_OWNER(index, p, n) (((p)*((index)+1)-1)/(n))

int main(int argc, char *argv[]) {
    int count;//local primer count
    double elapsed_time;
    int first;
    int global_count;
    int index;
    char *marked;
    int proc0_size;
    int prime;
    MPI_Init(&argc, &argv);
    //start timer
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    int id;
    int p;
    MPI_Comm_rank(MPI_COMM_WORLD, &id); //current process id
    MPI_Comm_size(MPI_COMM_WORLD, &p); //num of processor
    //process parameter error
    if (argc != 2) {
        if (id == 0)printf("Command line %s <m>\n", argv[0]);
        MPI_Finalize();
    }
    long n = atol(argv[1]);
    n = (n + 1) / 2;
    long low_value = 1 + BLOCK_LOW(id, p, n - 1); //current min val id 2
    long high_value = 1 + BLOCK_HIGH(id, p, n - 1);
    long size = BLOCK_SIZE(id, p, n - 1);
    //if we have too many process then some factor may be in process 1
    //    if ((2 + proc0_size) * (2 + proc0_size) < n) {
    //        if (id != 0)printf("too many processes or the number is bigger");
    //        MPI_Finalize();
    //        exit(1);
    //    }
    marked = (char *) malloc(size * sizeof(size));
    if (!marked) {
        printf("alloc failure\n");
        MPI_Finalize();
        exit(2);
    }
    for (int i = 0; i < size; ++i)marked[i] = 0;
    if (!id)index = 0;
    prime = 3; //this primer number is actually
    do {
        //the default check range is (prime*prime,highest)
        //[lower_value... prime^2.....high_value]
        long min_test_number;
        //low_val * 2 + 1 is the origin val
        if (prime * prime > low_value * 2 + 1) {
            min_test_number = prime * prime;
        } else {
            if ((low_value * 2 + 1) % prime == 0) {
                min_test_number = low_value * 2 + 1;
            } else {
                min_test_number = prime - (low_value * 2 + 1) % prime + low_value * 2 + 1;
            }
        }
        //printf("id: %d primer:%d range is (%d,%d)\n", id, prime, min_test_number, high_value * 2 + 1);
        for (long i = min_test_number; i <= high_value * 2 + 1; i += prime) {
            if (i % 2 == 1) {
                // printf("id %d: marked %d (%d)\n", (i - low_value * 2 - 1) / 2, id, i);
                marked[(i - low_value * 2 - 1) / 2] = 1;
            }
        }
        if (!id) {
            while (index < size && marked[++index]);
            prime = index * 2 + 3; //set the prime as the next prime number (offset 2 once)
        }
        //broadcast the primer number
        MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } while (prime * prime <= 2 * n + 1);
    //print the result
    count = 0;
    for (int j = 0; j < size; ++j)
        if (!marked[j]) count++;
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();
    if (id == 0) {
        //need add 2
        printf("%d %.4f", global_count + 1, elapsed_time);
    }
    MPI_Finalize();
    return 0;
}
