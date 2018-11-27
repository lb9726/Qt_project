#include <iostream>
#include "prototype.h"
using namespace std;

int main(int argc, char *argv[])
{
    Prototype* p = new ConcretePrototype();
    if (p)
    {
        Prototype* p1 = p->clone();
        delete p;
        p = 0;
        delete p1;
        p1 = 0;
    }

    return 0;
}
