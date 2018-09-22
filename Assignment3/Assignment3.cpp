#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <array>
#define MCW MPI_COMM_WORLD

using namespace std;

int main(int argc, char **argv){
  int rank, size;
  int data;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  int length = 1024;

  std::array<int, length> inputList;
  std::array<int, length / size> sortList;
  if (rank == 0)
  {
    for (int i = 0; i < 1024; i++)
    {
      list[i] = std::rand() % length;
    }
  }
  
  MPI_Scatter(inputList.data(), length / size, MPI_INT, sortList.data(), length / size, MPI_INT, 0, MCW);

  std::sort(sortList.begin(), sortList.end());

  int MPI_Gather(void *sendbuf, int sendcnt, MPI_Datatype sendtype, 
               void *recvbuf, int recvcnt, MPI_Datatype recvtype, 
               int root, MPI_Comm comm)
               
  MPI_Finalize();

  return 0;
}
