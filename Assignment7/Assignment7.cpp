#include <iostream>
#include <unistd.h>
#include <cmath>
#include <mpi.h>
#include <string>
#include <vector>
#include <ctime>
#define MCW MPI_COMM_WORLD

using namespace std;

int main(int argc, char **argv){
  int rank, size;
  int data;
  int MAX = 1000;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  std::srand(rank * std::time(nullptr));
  int myValue = std::rand() % MAX;

  std::cout << "Rank: " << rank << ", Value: " << myValue << std::endl;

  int mySum = myValue;
  int recvSum = 0;
  int target = (rank + 1) % size;

  while (target != rank)
  {
    MPI_Send(&mySum, 1 , MPI_INT, target, 0, MCW);
    MPI_Recv(&recvSum, 1, MPI_INT, MPI_ANY_SOURCE, 0, MCW, MPI_STATUS_IGNORE);

    mySum += recvSum;
    target = (target + 1) % size;
  }

  std::cout << "Rank: " << rank << ", Sum: " << mySum << std::endl;
  
  MPI_Finalize();

  return 0;
}
