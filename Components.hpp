#ifndef components_hpp
#define components_hpp

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
                    /*
                    case 'µ':
                        d *= pow(10, -6);
                        break;
                     */
                    case 'm':
                        d *= pow(10, -3);
                    case 'c':
                        d *= pow(10, -2);
                        break;
                    case 'd':
                        if(s[i+1] != 'a')
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
                    case 'T':
                        d*= pow(10, 12);
                        break;
                }
            }
            break;
        }
    }
    return d;
}

//General component object
class Component
{
protected:
    string name;
public:
    char get_type()
    {
        return name[0];
    }
    
    void rename(string &new_name)
    {
        name = new_name;
    }
    
    /*
    Implement these for specific components:
     */
    virtual double get_V()
    {
        return 0;
    }
    virtual double get_I()
    {
        return 0;
    }
    virtual double compute_conductance()
    {
        return 0;
    }
    virtual string get_np()
    {
        return {};
    }
    virtual string get_nm()
    {
        return {};
    }
};

class Resistor: public Component
{
protected:
    string n1, n2;
    double resistance;
public:
    Resistor();
    Resistor(stringstream &ss)
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
    
    double compute_conductance()
    {
        return 1.0/resistance;
    }
    ~Resistor();
};

class Capacitor: public Component
{
protected:
    string n1, n2;
    double capacitance;
public:
    Capacitor();
    Capacitor(stringstream &ss)
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
};

class Inductor: public Component
{
protected:
    string n1, n2;
    double inductance;
public:
    Inductor();
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
};

class vSource: public Component
{
protected:
    string np, nm;
    double V;
public:
    vSource();
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
    double get_V()
    {
        return V;
    }
    string get_np()
    {
        return np;
    }
    string get_nm()
    {
        return nm;
    }
    ~vSource();
};

class iSource: public Component
{
protected:
    string nin, nout;
    double I;
public:
    iSource();
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
    
    double get_I()
    {
        return I;
    }
    
    ~iSource();
};

//create constructor for SineV and SineI !!!!!! CHECK TO SEE IF INHERITANCE WORKS
class SineV: public Component
{
protected:
    string np, nm;
    double V;
    double Vamp, freq;
public:
    SineV();
    SineV(stringstream &ss)
    {
        string *w = new string;
        
        getline (ss, *w, ' ');
        name = *w;
        
        getline (ss, *w, ' ');
        np = *w;
        
        getline (ss, *w, ' ');
        nm = *w;
        
        getline (ss, *w, '(');
        getline (ss, *w, ' ');
        V = string_to_double(*w);
        
        getline (ss, *w, ' ');
        Vamp = string_to_double(*w);
        
        getline (ss, *w, ')');
        freq = string_to_double(*w);
        delete w;
    }
    string get_np()
    {
        return np;
    }
    string get_nm()
    {
        return nm;
    }
    ~SineV();
};

class SineI: public Component
{
protected:
    string nin, nout;
    double I;
    double Iamp, freq;
public:
    SineI();
    SineI(stringstream &ss)
    {
       string *w = new string;
        
        getline (ss, *w, ' ');
        name = *w;
        
        getline (ss, *w, ' ');
        nin = *w;
        
        getline (ss, *w, ' ');
        nout = *w;
        
        getline (ss, *w, '(');
        getline (ss, *w, ' ');
        I = string_to_double(*w);
        
        getline (ss, *w, ' ');
        Iamp = string_to_double(*w);
        
        getline (ss, *w, ')');
        freq = string_to_double(*w);
        delete w;
    }
    ~SineI();
};

//chooses source type (for now, between DC and sine)
Component *choose_source(string &linecs, stringstream &sscs)
{
    stringstream new_ss(linecs);
    string wcs, typecs;
    getline (new_ss, typecs, ' ');
    getline (new_ss, wcs, ' ');
    getline (new_ss, wcs, ' ');
    
    getline (new_ss, wcs, ' ');
    if(typecs[0] == 'V')
    {
        if(wcs[0] == 'S' || wcs[0] == 's')
        {
            return new SineV(sscs);
        }
        else
        {
            return new vSource(sscs);
        }
    }
    else
    {
        if(wcs[0] == 'S' || wcs[0] == 's')
        {
            return new SineI(sscs);
        }
        else
        {
            return new iSource(sscs);
        }
    }
}

//creates a specific component object from a line of input
Component *create_component(string &linecc)
{
    stringstream sscc(linecc);
    
    switch (linecc[0])
    {
        case 'R':
            return new Resistor(sscc);
        case 'C':
            return new Capacitor(sscc);
        case 'L':
            return new Inductor(sscc);
            
        case 'V':
            return choose_source(linecc, sscc);
        case 'I':
            return choose_source(linecc, sscc);

        //add diode and transistor support;
        default:
            cerr << "Unspecified component." << endl;
            break;
    }
    return nullptr;
}


#endif
