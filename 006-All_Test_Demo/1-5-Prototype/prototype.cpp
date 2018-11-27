#include "prototype.h"
#include <iostream>
using namespace std;

Prototype::Prototype()
{

}

Prototype::~Prototype()
{

}

Prototype *Prototype::clone() const
{
    return 0;
}

ConcretePrototype::ConcretePrototype()
{

}

ConcretePrototype::ConcretePrototype(const ConcretePrototype &cp)
{
    cout<<__PRETTY_FUNCTION__<<"copy..."<<endl;
}

ConcretePrototype::~ConcretePrototype()
{

}

Prototype *ConcretePrototype::clone() const
{
    return new ConcretePrototype(*this);
}
