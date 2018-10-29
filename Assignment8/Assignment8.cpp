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
  std::cerr << "countNeighbors1" << std::endl;
    count += world[x - 1][y];
  }
  if (x < localSize - 1)
  {
  std::cerr << "countNeighbors2" << std::endl;
    count += world[x + 1][y];
  }

  if (y > 0)
  {
  std::cerr << "countNeighbors3" << std::endl;
    count += world[x][y - 1];
  }
  if (y < WORLD_SIZE - 1)
  {
  std::cerr << "countNeighbors4" << std::endl;
    count += world[x][y + 1];
  }

  std::cerr << "countNeighbors5" << std::endl;
  return count;
}

int updateCell(int world[][WORLD_SIZE], int x, int y, int localSize)
{
  std::cerr << "Test2" << std::endl;
  int neighbors = countNeighbors(world, x, y, localSize);
  if (neighbors <= 1 || neighbors >= 4)
  {
  std::cerr << "Test3" << std::endl;
    return 0;
  }

  if (neighbors == 3)
  {
    if (world[x][y] == 0)
    {
  std::cerr << "Test4" << std::endl;
      return 1;
    }
  std::cerr << "Test5" << std::endl;
    return 0;
  }

  std::cerr << "Test6" << std::endl;
  return 1;
}

void updateWorld(int world[][WORLD_SIZE], int localSize)
{
  std::cerr << "Test1" << std::endl;
  for (int x = 0; x < localSize; x++)
  {
    for (int y = 0; y < WORLD_SIZE; y++)
    {
      std::cerr << "Update" << " " << x << " " << y << std::endl;
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

  std::cerr << "1" << std::endl;
  int localSize = WORLD_SIZE / size;
  std::cerr << "2" << std::endl;

  if (size > 1)
  {
    localSize++;
  }

  std::cerr << "2" << std::endl;
  if (rank > 0 && rank < size - 1)
  {
    localSize++;
  }
  std::cerr << "2" << std::endl;
  auto localWorld = new int[localSize][WORLD_SIZE]();

  std::cerr << "2" << std::endl;
  for (int i = 0; i < iterations; i++)
  {
  std::cerr << "2" << std::endl;
    updateWorld(localWorld, localSize);
    printWorld(world, i);
  }

  //std::srand(rank * std::time(nullptr));

  //std::cerr << "Rank: " << rank << ", Value: " << myValue << std::endl;

  
  MPI_Finalize();

  return 0;
}
