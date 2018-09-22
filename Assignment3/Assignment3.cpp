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

  std::vector<int> sortedList(length);
  std::vector<int> aggregateList(length);
  std::vector<int> sortList(sortSize);
  if (rank == 0)
  {
    for (int i = 0; i < length; i++)
    {
      aggregateList[i] = std::rand() % length;
    }
  }
  
  MPI_Scatter(aggregateList.data(), sortSize, MPI_INT, sortList.data(), sortSize, MPI_INT, 0, MCW);

  std::sort(sortList.begin(), sortList.end());

  MPI_Gather(sortList.data(), sortSize, MPI_INT, aggregateList.data(), sortSize, MPI_INT, 0, MCW);

  if (rank == 0)
  {
    // Init sorted list with first sublist
    for (int i = 0; i < sortSize; i++)
    {
      sortedList[i] = aggregateList[i];
    }

    // Aggregate list will contain size sublists of length sortSize
    for (int i = sortSize; i < length; i += sortSize)
    {
      int pos = 0;
      // Iterate over each item in sublist
      for (int j = 0; j < sortSize; j++)
      {
        sortedList.insert(
          std::upper_bound(sortedList.begin(), sortedList.end(), aggregateList[i + j]),
          aggregateList[i + j]);
      }
    }

    for (int i = 0; i < length; i++)
    {
      std::cout << sortedList[i] << std::endl;
    }
  }

  MPI_Finalize();

  return 0;
}
