#include <iostream>
#include <unistd.h>
#include <cmath>
#include <mpi.h>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <fstream>
#define MCW MPI_COMM_WORLD

const int WORLD_SIZE = 1024;

int printWorld(int world[][WORLD_SIZE], int it)
{
  std::ofstream file;
  file.open(std::to_string(it) + ".pbm");
  file << "P1" << std::endl;
  file << WORLD_SIZE << " " << WORLD_SIZE << std::endl;

  for(int i = 0; i < WORLD_SIZE; ++i)
  {
    for(int j = 0; j < WORLD_SIZE; ++j)
    {
      file << world[i][j] << " ";
    }
    file << std::endl;
  }
}

int countNeighbors(int world[][WORLD_SIZE], int x, int y, int localSize)
{
  int count = 0;
  if (x > 0)
  {
    count += world[x - 1][y];
  }
  if (x < localSize - 1)
  {
    count += world[x + 1][y];
  }

  if (y > 0)
  {
    count += world[x][y - 1];
  }
  if (y < WORLD_SIZE - 1)
  {
    count += world[x][y + 1];
  }

  return count;
}

int updateCell(int world[][WORLD_SIZE], int x, int y, int localSize)
{
  int neighbors = countNeighbors(world, x, y, localSize);
  if (neighbors <= 1 || neighbors >= 4)
  {
    return 0;
  }

  if (world[x][y] == 0)
  {
    if (neighbors == 3)
    {
      return 1;
    }
    return 0;
  }

  return 1;
}

void updateWorld(int world[][WORLD_SIZE], int localSize)
{
  for (int x = 0; x < localSize; x++)
  {
    for (int y = 0; y < WORLD_SIZE; y++)
    {
      world[x][y] = updateCell(world, x, y, localSize);
    }
  }
}

int main(int argc, char **argv){
  int rank, size;
  int data;
  int world[WORLD_SIZE][WORLD_SIZE] = {};
  int iterations = 100;
-
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  int localSize = WORLD_SIZE / size;

  if (size > 1)
  {
    localSize++;
  }

  if (rank > 0 && rank < size - 1)
  {
    localSize++;
  }
  auto localWorld = new int[localSize][WORLD_SIZE]();
  // for (int x = 0; x < localSize; x++)
  // {
  //   for (int y = 0; y < WORLD_SIZE; y++)
  //   {
  //     if (rand() % 15 == 0)
  //     {
  //       localWorld[x][y] = 1;
  //     }
  //   }
  // }

  localWorld[500][500] = 1;
  localWorld[501][500] = 1;
  localWorld[502][500] = 1;
  localWorld[502][499] = 1;
  localWorld[501][498] = 1;

  printWorld(localWorld, 0);
  for (int i = 1; i < iterations; i++)
  {
    updateWorld(localWorld, localSize);
    printWorld(localWorld, i);
  }

  //std::srand(rank * std::time(nullptr));

  //std::cerr << "Rank: " << rank << ", Value: " << myValue << std::endl;

  
  MPI_Finalize();

  return 0;
}
