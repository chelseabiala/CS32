#include <iostream>
#include <cassert>
#include <iostream>
#include "Sequence.h"
using namespace std;

int main()
{
    Sequence s1;
    s1.eraseEvens();
    s1.insert("1");
    s1.eraseEvens();
    s1.dump();
    s1.insert("2");
    s1.insert("3");
    s1.insert("4");
    s1.eraseEvens();
    s1.dump();
    
}
