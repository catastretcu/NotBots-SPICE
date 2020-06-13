#ifndef analysis_hpp
#define analysis_hpp

//  Created by Mihai-Catalin Stretcu and Youngmin Song
//  Department of Electrical and Electronic Engineering, Imperial College London
//  June 2020


//general Analysis class
class Analysis
{
public:
    string type;

    const string get_type()
    {
        return type;
    }
    
    virtual const double get_interval()
    {
        return 0;
    }
    
    virtual const double get_duration()
    {
        return 0;
    }
};

//operating point specific class
class Op: public Analysis
{
public:
    Op()
    {
        type = ".op";
    }
    ~Op();
};

//transient analysis specific class;
class Tran: public Analysis
{
protected:
    double interval, duration;
public:
    Tran();
    Tran(string &line)
    {
        type = ".tran";
        
        stringstream ss(line);
        string atype;
        
        getline(ss, atype, ' ');
        getline(ss, atype, ' ');
        
        getline(ss, atype, ' ');
        
        duration = string_to_double(atype);
        
        getline(ss, atype, ' ');
        
        getline(ss, atype, ' ');
        interval = string_to_double(atype);
        
        if(interval > duration)
            cerr << "Timestep value larger than stop-time.";
        
        assert(interval <= duration);
    }
    
    const double get_interval()
    {
        return interval;
    }
    
    const double get_duration()
    {
        return duration;
    }
    
    ~Tran();
};

//Creates a new instance of an analysis type
Analysis *choose_analysis(string &line)
{
    stringstream ss(line);
    string atype;
    
    getline(ss, atype, ' ');
    
    if(atype == ".op")
        return new Op();
            
    if(atype == ".tran")
        return new Tran(line);
        
    //add support for different types of analysis like operating point or AC-sweep
    cerr << "Unspecified analysis." << endl;
    return nullptr;
}
#endif
