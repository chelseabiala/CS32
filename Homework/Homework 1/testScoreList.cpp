
#include "Sequence.h"
#include "ScoreList.h"
#include <iostream>
using namespace std;

int main()
{
    ScoreList s;
    s.add(0);
    s.add(100);
    s.add(80);
    s.add(90);
    cout << s.minimum();
    cout << s.maximum();
}
