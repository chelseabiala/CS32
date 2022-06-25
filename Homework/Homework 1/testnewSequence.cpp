
#include "newSequence.h"
   #include <iostream>
   #include <cassert>
   using namespace std;

   int main()
   {
       Sequence s(100);
       assert(s.empty());
       s.insert(1);
       s.insert(3);
       s.insert(2);
       s.insert(0);
       s.insert(4, 4);
       s.erase(0);
       s.erase(3);
       s.insert(3);
       s.remove(3);
       s.insert(2);
       s.find(2);
       s.set(2, 3);
       Sequence n;
       n.insert(4);
       n.insert(5);
       n.insert(6);
       n.insert(7);
       n.dump();
       s.dump();
       s.swap(n);
       n.dump();
       s.dump();
   }
