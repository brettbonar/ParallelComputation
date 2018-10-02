#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <vector>
#include <limits> 
#define MCW MPI_COMM_WORLD

using namespace std;

std::vector<int> sort(bool down, const std::vector<int>& sortList)
{
  if (sortList.size() <= 1)
  {
    sortList = localSort(down, sortList);
  }
}

int main(int argc, char **argv){
  int rank, size;
  int data;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  int length = 16;
  int sortSize = length / size;

  std::vector<int> bitonicList(length);
  std::vector<int> sortList(sortSize);
  if (rank == 0)
  {
    // Form the initial bitonic sequence
    int num = 1;
    for (int i = 0; i < length; i++)
    {
      if (i == length / 2) {
        num--;
      }

      bitonicList[i] = num;

      num += i < length / 2 ? 2 : -2;
    }
  }
  
  MPI_Scatter(bitonicList.data(), sortSize, MPI_INT,
    sortList.data(), sortSize, MPI_INT, 0, MCW);
    
  sortList = sort(rank % 2 === 0, sortList);

  MPI_Gather(sortList.data(), sortSize, MPI_INT,
    bitonicList.data(), sortSize, MPI_INT, 0, MCW);

  if (rank == 0)
  {
    for (int i = 0; i < length; i++)
    {
      std::cout << bitonicList[i] << std::endl;
    }
  }

  MPI_Finalize();

  return 0;
}
