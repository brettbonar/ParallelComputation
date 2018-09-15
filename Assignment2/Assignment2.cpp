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
    std::cout << rank << " received bomb with " << bomb << " seconds left!" << std::endl;
    if (!bomb)
    {
      break;
    }
    bomb--;
    if (!bomb)
    {
      std::cout << rank << " loses!" << std::endl;
      break;
    }

    MPI_Bcast(&bomb, 1 , MPI_INT, rank, MCW);
  }

  MPI_Finalize();

  return 0;
}
