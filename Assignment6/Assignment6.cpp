#include <iostream>
#include <unistd.h>
#include <cmath>
#include <mpi.h>
#include <vector>
#define MCW MPI_COMM_WORLD
#define PIXELS 512

using namespace std;

struct Complex{
  double r;
  double i;
};

Complex operator * (Complex a, Complex b){
  Complex c;
  c.r = a.r*b.r-a.i*b.i;
  c.i = a.r*b.i+a.i*b.r;
  return c;
}

Complex operator + (Complex a, Complex b){
  Complex c;
  c.r = a.r+b.r;
  c.i = a.i+b.i;
  return c;
}

Complex operator - (Complex a, Complex b){
  Complex c;
  c.r = a.r-b.r;
  c.i = a.i-b.i;
  return c;
}

int mbrot_iters(Complex c){
  int i=0;
  Complex z = c;
  while(z.r*z.r+z.i*z.i<2.0*2.0 && i<1024){
    z = z*z+c;
    i++;
  }
  return i;
}


int main(int argc, char **argv){
  int rank, size;
  int data;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank); 
  MPI_Comm_size(MCW, &size);

  Complex c1,c2,cx,cdiff;
  double rinc;
  double iinc;
  int iters;
  c1.r = -2.0;
  c2.r = 0.0;
  c1.i = 1.0;
  c2.i = -1.0;

  cdiff = c2 - c1;
  rinc = cdiff.r / PIXELS;
  iinc = cdiff.i / PIXELS;

  int rowsPerProcess = PIXELS / size;
  int numLocalPixels = rowsPerProcess * PIXELS;
  int numGlobalPixels = PIXELS * PIXELS;
  std::vector<int> localColors(numLocalPixels * 3);
  std::vector<int> globalColors(numGlobalPixels * 3);

  cout << "Rows Per Process: " << rowsPerProcess << endl;
  cout << "Local Pixels: " << numLocalPixels << endl;
  cout << "Global Pixels: " << numGlobalPixels << endl;

  for(int i = rank * rowsPerProcess; i < rank * rowsPerProcess + rowsPerProcess; ++i)
  {
    for(int j = 0; j < PIXELS; ++j)
    {
      cx.i = c1.i + j * iinc;
      cx.r = c1.r + i * rinc;
      iters = mbrot_iters(cx);
      
      int r,g,b;
      r = (log(iters) / log(1024)) * 255;
      g = 0;
      b = 0;
      if (iters == 1024)
      {
        r = 0;
        g = 0;
        b = 0;
      }

      localColors.push_back(r);
      localColors.push_back(g);
      localColors.push_back(b);
    }
  }

  MPI_Gather(localColors.data(), numLocalPixels, MPI_INT,
    globalColors.data(), numGlobalPixels, MPI_INT, 0, MCW);

  if (rank == 0)
  {
    for (auto&& color : globalColors)
    {
      cout << color << " ";
    }
    cout << endl;
  }
  
  return 0;
}
