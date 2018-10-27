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

int print(int[][] world, int it)
{
  std::ofstream file;
  ofstream.open(std::to_string(it) + ".pbm");
  file << "P1" <<endl;
  file << WORLD_SIZE << " " << WORLD_SIZE << endl;

  for(int i = 0; i < WORLD_SIZE; ++i)
  {
    for(int j = 0; j < WORLD_SIZE; ++j)
    {
      file << world[i][j] << " ";
    }
    file << endl;
  }
}

int main(int argc, char **argv){
  int rank, size;
  int data;
  int world[WORLD_SIZE][WORLD_SIZE] = {};

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  //std::srand(rank * std::time(nullptr));

  //std::cout << "Rank: " << rank << ", Value: " << myValue << std::endl;
  print(world, 1);

  
  MPI_Finalize();

  return 0;
}
