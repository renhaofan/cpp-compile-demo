// @file: test.c
#include <iostream>
using namespace std;

#define PI 3.14
#ifdef USING_FLOAT64
typedef double scalar;
#else
typedef float scalar;
#endif

int main() {
  scalar n;
  cout << "Hello world!" << endl;
  return 0;
}
