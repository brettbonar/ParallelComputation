#include <iostream>
#include <unistd.h>
#include <cmath>
#include <mpi.h>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>
#define MCW MPI_COMM_WORLD

int main(int argc, char **argv){
  int rank, size;
  int data;

  int jobFlag;
  MPI_Request myRequest;
  MPI_Status myStatus;
-
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  // TODO: use MPI_Iprobe instead? Use different tags to check for available work and actual work?
  // Each process generates a number of tasks?
  // Process turns black only if it has sent work to a lower process. Because token can't be aware of lower things
  // or something like that.

  if (rank == 0)
  {
    job = 0;
    job++;
    MPI_Send(&job, 1, MPI_INT, 0, JOB, MCW);
    // job++, MPI_Send ...
  }

  while (1)
  {
    MPI_Irecv(&newJob, 1, MPI_INT, MPI_ANY_SOURCE, JOB, MCW, &myRequest);
    MPI_Test(&myRequest, &jobFlag, &myStatus);
    if (!jobFlag) break;
  }

  MPI_Finalize();

  return 0;
}
