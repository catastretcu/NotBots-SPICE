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
    Resistor();
    ~Resistor();
}

class Capacitor: public Component
{
protected:
    string n1, n2;
    double capacitance;
public:
    Capacitor();
    ~Capacitor();
}

class Inductor: public Component
{
protected:
    string n1, n2;
    double inductance;
public:
    Inductor();
    ~Inductor();
}

class vSource: public Component
{
protected:
    string np, nm;
    double V;
public:
    vSource();
    ~vSource();
}

class iSource: public Component
{
protected:
    string nin, nout;
    double I;
public:
    iSource();
    ~iSource();
}

#endif
