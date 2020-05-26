//
// Created by hhhxiao on 5/2/20.
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

/*
 * the first most important is to understand the blocking strategy
 *
 * r = n % p
 * num = n /p ==> n = num * p + r
 *[0 --- r-1] ==> num+1 = n/p+1
 * [r --- p-1] ==> num
 *
 * abs(num(pi) - num(pj)) <= 1
 *
 */



int main(int argc, char *argv[]) {
    int count;//local primer count
    double elapsed_time;
    long first;
    int global_count;
    int index;
    char *marked;
    int proc0_size;
    long prime;
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

    //get max number
    long n = atol(argv[1]);


    //from 2 to n [2,n]
    //right shift 2
    //number of numbers is (n-2)+1 = n -1
    long low_value = 2 + BLOCK_LOW(id, p, n - 1); //current min val id 2
    long high_value = 2 + BLOCK_HIGH(id, p, n - 1);
    long size = BLOCK_SIZE(id, p, n - 1);

    //stop in proper time
    //the size of proc 0
   //  proc0_size = (n - 1) / p;
    //here to make sure all  the factors  are in the 0 process
    //if we have too many process then some factor may be in process 1
//    if ((2 + proc0_size) * (2 + proc0_size) < n) {
//        if (id != 0)printf("too many processes or the number is bigger");
//        MPI_Finalize();
//        exit(1);
//    }

    marked = (char *) malloc(size);
    if (!marked) {
        printf("alloc failure\n");
        MPI_Finalize();
        exit(2);
    }

    //init the memory,each element is marked as 0
    for (int i = 0; i < size; ++i)marked[i] = 0;
    //core algorithm
    if (!id)index = 0;
    prime = 2;
    do {
        //the default check range is (prime*prime,highest)
        //[lower_value... prime^2.....high_value]
        if (prime * prime > low_value) {
            first = prime * prime - low_value;
        } else {
            //
            if (!(low_value % prime))first = 0;
            else first = prime - low_value % prime;
        }
        //marked
        for (long i = first; i < size; i += prime) marked[i] = 1;
        //none process update the factor
        if (!id) {
            while (marked[++index]);
            prime = index + 2; //set the prime as the next prime number (offset 2 once)
        }
        MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } while (prime * prime <= n);
    //print the result
    count = 0;
    for (int j = 0; j < size; ++j) {
        if (!marked[j]) {
            //  printf("prime number: %d \n", j+2);
            count++;
        }

    }
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();
    if (id == 0) {
        printf("%d %.4f", global_count, elapsed_time);
    }
    MPI_Finalize();
    return 0;
}

