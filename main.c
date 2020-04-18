#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char **argv) {

    int size = 0;
    MPI_Init(&argc, &argv);
    int numOfProcs, myRank;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if(argc == 1){
        if(myRank == 0) {
            printf("OMG!!!\n");
        }
        return 1;
    }

    char * endptr = strstr(argv[1], "\0");
    int seriesLength = (int) strtol(argv[1], &endptr, 10);
    int * from;
    int * to;
    from = malloc(numOfProcs * sizeof(int));
    to = malloc(numOfProcs * sizeof(int));
    float * parts = malloc(numOfProcs * sizeof(float));
    double dTime;
    if (myRank == 0) {
        dTime = MPI_Wtime();
        int previousEnd = 0;
        for (int i = 0; i < numOfProcs; i++) {
            from[i] = previousEnd;
            previousEnd = previousEnd + (seriesLength - previousEnd) / (numOfProcs - i);
            to[i] = previousEnd;
        }
    }

    from[0] = 1;

//    if (myRank == 0) {
//        for (int i = 0; i < numOfProcs; i++) {
//            printf("%d ", from[i]);
//        }
//        printf("\n");
//        for (int i = 0; i < numOfProcs; i++) {
//            printf("%d ", to[i]);
//        }
//        printf("\n");
//    }

    int myFrom, myTo;
    MPI_Scatter(from, 1, MPI_INT, &myFrom, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(to, 1, MPI_INT, &myTo, 1, MPI_INT, 0, MPI_COMM_WORLD);

    float sum = 0;

    for(int i = myTo-1; i >= myFrom; i--){
        sum += (float) 6 * ((float)M_1_PI) * ((float)M_1_PI)  / ((float)i * (float)i);
    }

    MPI_Gather(&sum, 1, MPI_FLOAT, parts, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (myRank == 0) {
        float result = 0;
        for (int i = numOfProcs - 1; i >= 0; i--) {
            result +=  parts[i];
        }
        dTime = MPI_Wtime() - dTime;
        printf("result = %f\ntime = distributionTime + transmissionTime + receivingTime = %f sec\n", result, dTime);
    }

    free(to);
    free(from);
    free(parts);
    MPI_Barrier(MPI_COMM_WORLD);
    return 0;
}
