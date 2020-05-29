#ifndef analysis_hpp
#define analysis_hpp

#include "Components.hpp"
#include <iostream>
#include <vector>
#include <cstring>
#include <cmap>
#include <utility>
#include <sstream>

//general Analysis class
class Analysis
{
protected:
    string type;
public:
    Analysis();
    ~Analysis();
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
        case ".tran":
            return Tran(line);
            
        //add support for different types of analysis like operating point or AC-sweep
        default:
            cerr << "Unspecified analysis." << endl;
            break;
    }
}
#endif
