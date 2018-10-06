#include <iostream>
#include <unistd.h>
#include <cmath>
#include <mpi.h>
#include <vector>
#define MCW MPI_COMM_WORLD
#define PIXELS 1024

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
  c1.r = -1.5;
  c2.r = -0.5;
  c1.i = 1.0;
  c2.i = 0;

  cdiff = c2 - c1;
  rinc = cdiff.r / PIXELS;
  iinc = cdiff.i / PIXELS;

  int rowsPerProcess = PIXELS / size;
  int numLocalColors = PIXELS * PIXELS / size;
  int numGlobalColors = PIXELS * PIXELS;
  std::vector<int> localColors(numLocalColors);
  std::vector<int> globalColors(numGlobalColors);

  for(int i = rank * rowsPerProcess, row = 0; i < rank * rowsPerProcess + rowsPerProcess; ++i, ++row)
  {
    for(int j = 0; j < PIXELS; ++j)
    {
      cx.i = c1.i + j * iinc;
      cx.r = c1.r + i * rinc;
      iters = mbrot_iters(cx);
      
      int r,g,b;
      if (iters == 1024)
      {
        r = 0;
        g = 0;
        b = 0;
      }
      else
      {
        r = (log(iters) / log(1024)) * 255;
        g = 0;
        b = 255 - (log(iters) / log(1024)) * 255;
      }

      localColors[row * PIXELS + j] = (r << 16) + (g << 8) + b;
    }
  }

  MPI_Gather(localColors.data(), numLocalColors, MPI_INT,
    globalColors.data(), numLocalColors, MPI_INT, 0, MCW);

  if (rank == 0)
  {
    cout << "P3" <<endl;
    cout << PIXELS << " " << PIXELS << endl;
    cout << "255" <<endl;

    for(int i = 0; i < PIXELS; ++i)
    {
      for(int j = 0; j < PIXELS; ++j)
      {
        int red = (globalColors[i * PIXELS + j] >> 16) & 0xFF;
        int green = (globalColors[i * PIXELS + j] >> 8) & 0xFF;
        int blue = globalColors[i * PIXELS + j] & 0xFF;
        cout << red << " " << green << " " << blue << " ";
      }
      cout << endl;
    }
  }
  
  MPI_Finalize();

  return 0;
}
