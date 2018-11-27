#ifndef PROTOTYPE_H
#define PROTOTYPE_H


class Prototype
{
public:
    virtual ~Prototype();
    virtual Prototype* clone() const = 0;
protected:
    Prototype();
private:
};

class ConcretePrototype : public Prototype
{
public:
    ConcretePrototype();
    ConcretePrototype(const ConcretePrototype& cp);
    ~ConcretePrototype();
    Prototype* clone() const;

protected:
private:

};

#endif // PROTOTYPE_H












