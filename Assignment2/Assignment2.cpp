#include <iostream>
#include <mpi.h>
#include <stdlib.h>   
#define MCW MPI_COMM_WORLD

using namespace std;

int main(int argc, char **argv){
  int rank, size;
  int data;
  int bomb = std::rand() % 1000 + 1; // Random timer from 1 - 1000
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size); 

  if (rank == 0)
  {
    MPI_Send(&bomb, 1 , MPI_INT, std::rand() % size, 0, MCW);
    std::cout << "Start timer: " << bomb << std::endl;
  }


  while (true)
  {
    MPI_Recv(&bomb, 1, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);
    if (!bomb)
    {
      break;
    }
    
    std::cout << rank << " received bomb with " << bomb << " seconds left!" << std::endl;
    bomb--;
    if (!bomb)
    {
      std::cout << rank << " loses!" << std::endl;
      for (int i = 0; i < size; i++)
      {
        if (i != rank)
        {
          MPI_Send(&bomb, 1, MPI_INT, i, 0, MCW);
        }
      }
      break;
    }
    
    int target = std::rand() % size;
    while (target == rank)
    {
      target = std::rand() % size;
    }
    MPI_Send(&bomb, 1, MPI_INT, target, 0, MCW);
  }

  MPI_Finalize();

  return 0;
}
