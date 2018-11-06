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
const int TOKEN = 1;
const int DONE = 1;

int handleToken(int rank, int size, int& token, bool& isWhite, MPI_Request& sendRequest, MPI_Request& tokenRequest)
{
  int tokenFlag = 0;
  MPI_Test(&tokenRequest, &tokenFlag, MPI_STATUS_IGNORE);
  if (tokenFlag)
  {
    std::cerr << rank << " received token " << token << std::endl;
    if (rank == 0)
    {
      if (token == 1)
      {
        int done = 1;
        std::cerr << "Finishing..." << std::endl;
        for (int process = 0; process < size; process++)
        {
          MPI_Send(&done, 1, MPI_INT, process, DONE, MCW);
        }
        return 0;
      }
      else
      {
        token = 1;
      }
    }
    else if (!isWhite)
    {
      token = 0;
    }

    std::cerr << rank << " sent token " << token << " to " << (rank + 1) % size << std::endl;
    MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, TOKEN, MCW);
    isWhite = true;
    std::cerr << rank << " is white " << std::endl;
  }

  return tokenFlag;
}

int main(int argc, char **argv){
  int rank, size;
  int data;
  int sendData = 1;
  int token = 1;
  int doneFlag = 0;
  int done = 0;
  bool isWhite = true;

  MPI_Request jobRequest;
  MPI_Request tokenRequest;
  MPI_Request sendRequest;
  MPI_Request doneRequest;
-
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  srand(time(nullptr) * rank);
  int numTasks = rand() % 32 + 16;

  std::cerr << "Rank: " << rank << ", Start Tasks: " << numTasks << std::endl;

  if (rank == 0)
  {
    MPI_Isend(&token, 1, MPI_INT, (rank + 1) % size, TOKEN, MCW, &sendRequest);
  }

  MPI_Irecv(&token, 1, MPI_INT, MPI_ANY_SOURCE, TOKEN, MCW, &tokenRequest);
  MPI_Irecv(&sendData, 1, MPI_INT, MPI_ANY_SOURCE, JOB, MCW, &jobRequest);
  MPI_Irecv(&done, 1, MPI_INT, 0, DONE, MCW, &doneRequest);

  while (numTasks || !doneFlag)
  {
    if (numTasks)
    {
      if (numTasks > TASK_THRESHOLD)
      {
        int count = 2;
        for (int i = 0; i < count; i++)
        {
          int target = rand() % size;
          if (target < rank)
          {
            isWhite = false;
            std::cerr << rank << " is black " << std::endl;
          }
          //std::cerr << rank << " sending task to " << target << std::endl;
          MPI_Isend(&sendData, 1, MPI_INT, target, JOB, MCW, &sendRequest);
        }
        numTasks -= count;
        //std::cerr << "Rank: " << rank << ", # Tasks: " << numTasks << std::endl;
      }

      int jobFlag = 0;

      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      numTasks--;
      //std::cerr << rank << " completed a task, now has " << numTasks << std::endl;

      MPI_Test(&jobRequest, &jobFlag, MPI_STATUS_IGNORE);
      while (jobFlag)
      {
        numTasks++;
        //std::cerr << rank << " received a task, now has " << numTasks << std::endl;
        MPI_Irecv(&sendData, 1, MPI_INT, MPI_ANY_SOURCE, JOB, MCW, &jobRequest);
        MPI_Test(&jobRequest, &jobFlag, MPI_STATUS_IGNORE);
      }
    }

    MPI_Test(&doneRequest, &doneFlag, MPI_STATUS_IGNORE);

    if (handleToken(rank, size, token, isWhite, sendRequest, tokenRequest))
    {
      MPI_Irecv(&token, 1, MPI_INT, MPI_ANY_SOURCE, TOKEN, MCW, &tokenRequest);
    }
  }

  std::cerr << "Rank: " << rank << " is done" << std::endl;

  MPI_Finalize();

  return 0;
}
