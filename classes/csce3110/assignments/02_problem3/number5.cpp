#include<iostream>
using namespace std;
int main()
{
   int n=10, sum=0;
   for (int i=0; i < n; i++)
      for (int j=0; j < i*i; j++)
         for (int k=0; k < j; k++)
            sum++;
   cout << "Sum is " << sum << endl;
   return 0;
}
