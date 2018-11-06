#include <iostream>
#include <unistd.h>
#include <cmath>
#include <mpi.h>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#define MCW MPI_COMM_WORLD

const int TASK_THRESHOLD = 16;
const int JOB = 0;

int main(int argc, char **argv){
  int rank, size;
  int data;
  int sendData = 1;

  MPI_Request myRequest;
  MPI_Request sendRequest;
  MPI_Status myStatus;
-
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  // TODO: use MPI_Iprobe instead? Use different tags to check for available work and actual work?
  // Each process generates a number of tasks?
  // Process turns black only if it has sent work to a lower process. Because token can't be aware of lower things
  // or something like that.

  srand(time(nullptr) * rank);
  int numTasks = rand() % 32;

  std::cerr << "Rank: " << rank << ", Start Tasks: " << numTasks << std::endl;

  while (numTasks)
  {
    if (numTasks > TASK_THRESHOLD)
    {
      int count = numTasks - TASK_THRESHOLD;
      for (int i = 0; i < count; i++)
      {
        int target = rand() % size;
        std::cerr << rank << " sending task to " << target << std::endl;
        MPI_Isend(&sendData, 1, MPI_INT, target, JOB, MCW, &sendRequest);
      }
      numTasks -= count;
      std::cerr << "Rank: " << rank << ", # Tasks: " << numTasks << std::endl;
    }

    MPI_Irecv(&sendData, 1, MPI_INT, MPI_ANY_SOURCE, JOB, MCW, &myRequest);
    int jobFlag = 0;
    while (numTasks && !jobFlag)
    {
      // Do work then check for more work
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      numTasks--;
      std::cerr << rank << " completed a task" << std::endl;
      MPI_Test(&myRequest, &jobFlag, &myStatus);
    }
    if (jobFlag)
    {
      numTasks++;
    }
  }

  std::cerr << "Rank: " << rank << " is done" << std::endl;

  MPI_Finalize();

  return 0;
}
