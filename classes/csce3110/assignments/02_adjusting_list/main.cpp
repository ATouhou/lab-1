#include<iostream>
#include "AList.h"

int main()
{
   AList L;
       
   cout << "insert test" << endl;
   for (int i=1; i <= 10; i++)
   {
      L.insert(i);
   }
   
   L.printAList();
   
   cout << "finding a value" << endl;
   L.find(5);   
   L.printAList();
      
   return 0;
}
