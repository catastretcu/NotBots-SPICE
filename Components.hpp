#ifndef components_hpp
#define components_hpp

#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <cctype>

using namespace std;

double string_to_double(string &s)
{
    double d;
    string to_d;
    
    for (int i = 0; i < s.length(); i++)
    {
        if((isdigit(s[i]) || s[i] == '.') && i != s.length()-1)
            to_d += s[i];
        else
        {
            if(i == s.length()-1 && isdigit(s[i]))
            {
                to_d += s[i];
                d = stod(to_d);
            }
            else
            {
                d = stod(to_d);
                switch(s[i])
                {
                    case 'p':
                        d *= pow(10, -12);
                        break;
                    case 'n':
                        d *= pow(10, -9);
                        break;
                    case 'u':
                        d *= pow(10, -6);
                        break;
                    case 'µ':
                        d *= pow(10, -6);
                        break;
                    case 'm':
                        d *= pow(10, -3);
                    case 'c':
                        d *= pow(10, -2);
                        break
                    case 'd':
                        if(s[i+1] != a)
                            d *= pow(10, -1);
                        else
                            d *= pow(10, 1);
                        break;
                    case 'h':
                        d *= pow(10, 2);
                        break;
                    case 'k':
                        d *= pow(10, 3);
                        break;
                    case 'M':
                        d*= pow(10, 6);
                        break;
                    case 'G':
                        d*= pow(10, 9);
                        break;
                    case 'T';
                        d*= pow(10, 12);
                        break;
                }
            }
            break;
        }
    }
    return d;
}

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
    {
        string *w = new string;
        
        getline (ss, *w, ' ');
        name = *w;
        
        getline (ss, *w, ' ');
        n1 = *w;
        
        getline (ss, *w, ' ');
        n2 = *w;
        
        getline (ss, *w, ' ');
        resistance = string_to_double(*w);
        delete w;
    }
    ~Resistor();
}

class Capacitor: public Component
{
protected:
    string n1, n2;
    double capacitance;
public:
    Capacitor(stringstream &ss);
    {
        string *w = new string;
        
        getline (ss, *w, ' ');
        name = *w;
        
        getline (ss, *w, ' ');
        n1 = *w;
        
        getline (ss, *w, ' ');
        n2 = *w;
        
        getline (ss, *w, ' ');
        capacitance = string_to_double(*w);
        delete w;
    }
    ~Capacitor();
}

class Inductor: public Component
{
protected:
    string n1, n2;
    double inductance;
public:
    Inductor(stringstream &ss)
    {
        string *w = new string;
        
        getline (ss, *w, ' ');
        name = *w;
        
        getline (ss, *w, ' ');
        n1 = *w;
        
        getline (ss, *w, ' ');
        n2 = *w;
        
        getline (ss, *w, ' ');
        inductance = string_to_double(*w);
        delete w;
    }
    ~Inductor();
}

class vSource: public Component
{
protected:
    string np, nm;
    double V;
public:
    vSource(stringstream &ss)
    {
        string *w = new string;
        
        getline (ss, *w, ' ');
        name = *w;
        
        getline (ss, *w, ' ');
        np = *w;
        
        getline (ss, *w, ' ');
        nm = *w;
        
        getline (ss, *w, ' ');
        V = string_to_double(*w);
        delete w;
    }
    ~vSource();
}

class iSource: public Component
{
protected:
    string nin, nout;
    double I;
public:
    iSource(stringstream &ss)
    {
        string *w = new string;
        
        getline (ss, *w, ' ');
        name = *w;
        
        getline (ss, *w, ' ');
        nin = *w;
        
        getline (ss, *w, ' ');
        nout = *w;
        
        getline (ss, *w, ' ');
        I = string_to_double(*w);
        delete w;
    }
    ~iSource();
}

//create constructor for SineV and SineI
class SineV: public vSource
{
protected:
    double Vamp, freq;
public:
    SineV();
    ~SineV();
}

class SineI: public vSource
{
protected:
    double Iamp, freq;
public:
    SineI();
    ~SineI();
}

#endif
