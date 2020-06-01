#ifndef analysis_hpp
#define analysis_hpp

//general Analysis class
class Analysis
{
//protected:
public:
    string type;

    const string get_type()
    {
        return type;
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
    double interval, time;
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
        interval = string_to_double(atype);
        
        getline(ss, atype, ' ');
        
        getline(ss, atype, ' ');
        
    }
    ~Tran();
};

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
