// fib.cpp
#include<iostream>
#include <ctime>

using namespace std;

unsigned long int recurse(unsigned long int a) {
   if (a == 0)
      return 0;
   else if (a == 1)
      return 1;
   else
      return recurse(a-1) + recurse(a-2);
   
}
int main(int argc, char* argv[]) {
   unsigned long int a, b;
   clock_t start, finish;
   double time;
   
   if (argc > 1)
      a = atoi(argv[1]);
   
   start = clock();
   b = recurse(a);
   finish = clock();
   time = (double(finish)-double(start))/CLOCKS_PER_SEC;
   
   cout << "Fibonacci number " << a << " is " << b << " in " << time << " seconds." << endl;
}