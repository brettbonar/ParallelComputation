#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <vector>
#define MCW MPI_COMM_WORLD

using namespace std;

int main(int argc, char **argv){
  int rank, size;
  int data;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  int length = 64;
  int sortSize = length / size;

  std::vector<int> fullList(length);
  std::vector<int> sortList(sortSize);
  if (rank == 0)
  {
    for (int i = 0; i < length; i++)
    {
      fullList[i] = std::rand() % length;
    }
  }
  
  MPI_Scatter(fullList.data(), sortSize, MPI_INT, sortList.data(), sortSize, MPI_INT, 0, MCW);

  std::sort(sortList.begin(), sortList.end());

  MPI_Gather(sortList.data(), sortSize, MPI_INT, fullList.data(), sortSize, MPI_INT, 0, MCW);

  if (rank == 0)
  {
    for (int i = 0; i < length; i++)
    {
      std::cerr << fullList[i] << std::endl;
    }
  }

  MPI_Finalize();

  return 0;
}
