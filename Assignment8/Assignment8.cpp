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

int printWorld(int world[], int it)
{
  std::ofstream file;
  file.open(std::to_string(it) + ".pbm");
  file << "P1" << std::endl;
  file << WORLD_SIZE << " " << WORLD_SIZE << std::endl;

  for(int i = 0; i < WORLD_SIZE; ++i)
  {
    for(int j = 0; j < WORLD_SIZE; ++j)
    {
      file << world[i * WORLD_SIZE + j] << " ";
    }
    file << std::endl;
  }
}

int countNeighbors(int world[][WORLD_SIZE], int x, int y, int localSize,
  int front[], int back[])
{
  int count = 0;
  if (x > 0)
  {
    count += world[x - 1][y];
    if (y > 0)
    {
      count += world[x - 1][y - 1];
    }
    else if (front)
    {
      count += front[x - 1];
    }

    if (y < WORLD_SIZE - 1)
    {
      count += world[x - 1][y + 1];
    }
    else if (back)
    {
      count += back[x - 1];
    }
  }
  if (x < localSize - 1)
  {
    count += world[x + 1][y];

    if (y > 0)
    {
      count += world[x + 1][y - 1];
    }
    else if (front)
    {
      count += front[x + 1];
    }

    if (y < WORLD_SIZE - 1)
    {
      count += world[x + 1][y + 1];
    }
    else if (back)
    {
      count += back[x + 1];
    }
  }

  if (y > 0)
  {
    count += world[x][y - 1];
  }
  else if (front)
  {
    count += front[x];
  }

  if (y < WORLD_SIZE - 1)
  {
    count += world[x][y + 1];
  }
  else if (back)
  {
    count += back[x];
  }

  return count;
}

int updateCell(int world[][WORLD_SIZE], int x, int y, int localSize,
  int front[], int back[])
{
  int neighbors = countNeighbors(world, x, y, localSize, front, back);
  // if (world[x][y])
  // {
  //   std::cerr << neighbors << std::endl;
  // }

  if (world[x][y] == 1)
  {
    if (neighbors <= 1 || neighbors >= 4)
    {
      return 0;
    }
    return 1;
  }
  else
  {
    if (neighbors == 3)
    {
      return 1;
    }
    return 0;
  }

  return 1;
}

void updateWorld(int world[][WORLD_SIZE], int targetWorld[][WORLD_SIZE], int localSize,
  int front[], int back[])
{
  for (int x = 0; x < localSize; x++)
  {
    for (int y = 0; y < WORLD_SIZE; y++)
    {
      targetWorld[x][y] = updateCell(world, x, y, localSize, front, back);
    }
  }
}

int main(int argc, char **argv){
  int rank, size;
  int data;
  auto world = new int[WORLD_SIZE * WORLD_SIZE]();
  int iterations = 100;
-
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  int localSize = WORLD_SIZE / size;
  int frontTag = 0;
  int backTag = 1;
  
  auto sourceWorld = new int[localSize][WORLD_SIZE]();
  auto targetWorld = new int[localSize][WORLD_SIZE]();
  std::cerr << "Rank: " << rank << ", Local Size: " << localSize << std::endl;

  srand(rank * time(NULL));
  for (int x = 0; x < localSize; x++)
  {
    for (int y = 0; y < WORLD_SIZE; y++)
    {
      if (rand() % 5 == 0)
      {
        sourceWorld[x][y] = 1;
      }
    }
  }

  int* front = nullptr;
  int* back = nullptr;

  if (rank > 0)
  {
    front = new int[WORLD_SIZE];
  }
  if (rank < size - 1)
  {
    back = new int[WORLD_SIZE];
  }

  int displacements[size];
  int recvCounts[size];
  for (int i = 0; i < size; i++)
  {
    recvCounts[i] = WORLD_SIZE;
  }

  for (int i = 0; i < localSize; i++)
  {
    for (int p = 0; p < size; p++)
    {
      displacements[p] = p * localSize * WORLD_SIZE + i * WORLD_SIZE;
    }

    MPI_Gatherv(sourceWorld[i], WORLD_SIZE, MPI_INT, world, recvCounts,
      displacements, MPI_INT, 0, MCW);
  }
  if (rank == 0)
  {
    printWorld(world, 0);
  }

  for (int i = 1; i < iterations; i++)
  {
    if (rank == 0)
    {
      std::cerr << "Start iteration " << i << std::endl;
    }
    if (rank < size - 1)
    {
      MPI_Send(sourceWorld[localSize - 1], WORLD_SIZE , MPI_INT, rank + 1, frontTag, MCW);
    }
    if (rank > 0)
    {
      MPI_Recv(front, WORLD_SIZE, MPI_INT, rank - 1, frontTag, MCW, MPI_STATUS_IGNORE);
    }

    if (rank > 0)
    {
      MPI_Send(&sourceWorld[0], WORLD_SIZE, MPI_INT, rank - 1, backTag, MCW);
    }
    if (rank < size - 1)
    {
      MPI_Recv(back, WORLD_SIZE, MPI_INT, rank + 1, backTag, MCW, MPI_STATUS_IGNORE);
    }

    updateWorld(sourceWorld, targetWorld, localSize, front, back);
    auto temp = sourceWorld;
    sourceWorld = targetWorld;
    targetWorld = temp;

    for (int i = 0; i < localSize; i++)
    {
      for (int p = 0; p < size; p++)
      {
        displacements[p] = p * localSize * WORLD_SIZE + i * WORLD_SIZE;
      }

      MPI_Gatherv(sourceWorld[i], WORLD_SIZE, MPI_INT, world, recvCounts,
        displacements, MPI_INT, 0, MCW);
    }
    if (rank == 0)
    {
      printWorld(world, i);
      std::cerr << "End iteration " << i << std::endl;
    }
  }

  MPI_Finalize();

  return 0;
}
