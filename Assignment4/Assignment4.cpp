#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <vector>
#include <limits> 
#define MCW MPI_COMM_WORLD

using namespace std;

void bitonicSort()

int main(int argc, char **argv){
  int rank, size;
  int data;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  int length = 64;
  int sortSize = length / size;

  std::vector<int> sortedList;
  std::vector<int> aggregateList(length);
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

      aggregateList[i] = num;

      num += i < length / 2 ? 2 : -2;
    }
  }
  
  // MPI_Scatter(aggregateList.data(), sortSize, MPI_INT,
  //   sortList.data(), sortSize, MPI_INT, 0, MCW);

  // std::sort(sortList.begin(), sortList.end());

  // MPI_Gather(sortList.data(), sortSize, MPI_INT,
  //   aggregateList.data(), sortSize, MPI_INT, 0, MCW);

  if (rank == 0)
  {
    for (int i = 0; i < length; i++)
    {
      std::cout << aggregateList[i] << std::endl;
    }
  }

  MPI_Finalize();

  return 0;
}
