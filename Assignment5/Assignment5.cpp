#include <iostream>
#include <unistd.h>
#include <cmath>
//#include "/usr/local/include/mpi.h"
#define MCW MPI_COMM_WORLD
#define PIXELS 512

using namespace std;

void printColor(int i){
  int r,g,b;
  r=(log(i)/log(1024))*255;
  g=0;
  b=0;
  if (i==1024) {r=0; g=0; b=0;}
  cout << r << " ";
  cout << g << " ";
  cout << b << " ";  
}

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
  Complex c1,c2,cx,cdiff;
  double rinc;
  double iinc;
  int iters;
  c1.r = -3.0;
  c2.r = 1.0;
  c1.i = 1.0;
  c2.i = -2.0;

  cdiff = c2-c1;
  rinc = cdiff.r/PIXELS;
  iinc = cdiff.i/PIXELS;

  cout << "P3" <<endl;
  cout << PIXELS << " " << PIXELS << endl;
  cout << "255" <<endl;


  for(int i=0;i<PIXELS;++i){
    for(int j=0;j<PIXELS;++j){
      cx.i = c1.i+j*iinc;
      cx.r = c1.r+i*rinc;
      iters = mbrot_iters(cx);        
      printColor(iters);
    }
    cout << endl;
  }
  


  return 0;
}
