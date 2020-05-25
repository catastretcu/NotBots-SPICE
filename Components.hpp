#ifndef components_hpp
#define components_hpp

#include <iostream>
#include <cstring>
#include <sstream>

//General component obeject
class Component
{
protected:
    string name;
public:
    Component();
    virtual ~Component() =0;
}

class Resistor: public Component
{
protected:
    string n1, n2;
    double resistance;
public:
    Resistor(stringstream &ss);
    ~Resistor();
}

class Capacitor: public Component
{
protected:
    string n1, n2;
    double capacitance;
public:
    Capacitor(stringstream &ss);
    ~Capacitor();
}

class Inductor: public Component
{
protected:
    string n1, n2;
    double inductance;
public:
    Inductor(stringstream &ss);
    ~Inductor();
}

class vSource: public Component
{
protected:
    string np, nm;
    double V;
public:
    vSource(stringstream &ss);
    ~vSource();
}

class iSource: public Component
{
protected:
    string nin, nout;
    double I;
public:
    iSource(stringstream &ss);
    ~iSource();
}

#endif
