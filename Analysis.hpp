#ifndef analysis_hpp
#define analysis_hpp

#include "Components.hpp"
#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <utility>
#include <sstream>

//general Analysis class
class Analysis
{
protected:
    string type;
public:
    Analysis();
    const string get_type()
    {
        return type;
    }
    ~Analysis();
}

//operating point specific class
class Op: public Analysis
{
public:
    Op(string &line)
    {
        type = ".op";
    }
    ~Op();
}

//transient analysis specific class;
class Tran: public Analysis
{
protected:
    double interval, time;
public:
    Tran(string &line)
    {
        type = ".tran";
        
        stringstream ss(line);
        string atype;
        
        getline(ss, atype, ' ');
        getline(ss, atype, ' ');
        
        getline(ss, atype, ' ');
        interval = string_to_double(atype);
        
        getline(ss, atype, ' ');
        
        getline(ss, atype, ' ');
        
    }
    ~Tran();
}

Analysis choose_analysis(string &line)
{
    stringstream ss(line);
    string atype;
    
    getline(ss, atype, ' ');
    switch(atype)
    {
        case ".op":
            return Op(line);
            
        case ".tran":
            return Tran(line);
        
        //add support for different types of analysis like operating point or AC-sweep
        default:
            cerr << "Unspecified analysis." << endl;
            break;
    }
}
#endif
