#ifndef node_hpp
#define node_hpp
/*
Convert to Eigen instead of normal vectors
*/

class Nodes
{
private:
    map<pair<string, string>, vector<Component>> Branch;
    vector<vector<double>> Conductances;
    vector<double> Currents;
    vector<double> Voltages;
    map<string, int> Labels;
    //number of nodes in the circuit
    unsigned int Size;
    
public:
    Nodes()
    {
        Size = 0;
    }
    
    /*
        Modify all get_ methods to const types if possible.
    */
    
    int get_Size()
    {
        return Size;
    }
    
    void Resize()
    {
        Currents.resize(Size);
        Voltages.resize(Size);
        Conductances.resize(Size);
        for(int i = 0; i < Conductances.size(); i++)
            Conductances[i].resize(Size);
    }
    
    int node_number(string &s)
    {
        if(s != "0")
        {
            if(s[0] == 'n' || s[0] == 'N')
            {
                s.erase(s.begin());
                if(stoi(s) > Size)
                {
                    Size = stoi(s);
                    Resize();
                }
                return stoi(s);
            }
            else
            {
                //check label
                //if new, add new label with number Size+1
                if(Labels[s])
                    return Labels[s];
                else
                {
                    Size++;
                    Labels[s] = Size;
                    Resize();
                    return Size;
                }
            
            }
        }
        return 0;
    }
    
    double get_conductance(int &i, int &j)
    {
        return Conductances[i][j];
    }
    
    double get_current(int &i)
    {
        return Currents[i];
    }
    
    double get_voltage(int &i)
    {
        return Voltages[i];
    }
    
    void add_voltage(int &i, double &value)
    {
        Voltages[i] = value;
    }
    
    void add_branch(pair<string, string> p, Component *c)
    {
        Branch[p].push_back(*c);
        
        int node1 = node_number(p.first);
        int node2 = node_number(p.second);
        
        //dynamicly calculate Currents and Conductances
        /*
         Current sources only, for now
        */
        if(c->get_type() == 'I')
        {
            if(node1)
                Currents[node1-1] += c->get_I();
            if(node2)
                Currents[node2-1] -= c->get_I();
        }
        else
        {
            //add to conductance matrix
            /*
             Resistors only, for now
            */
            assert(c->get_type() == 'R');
            
            if(node1 && node2)
            {
                Conductances[node1-1][node2-1] -= c->compute_conductance();
                Conductances[node2-1][node1-1] -= c->compute_conductance();
            }
            
            if(node1)
                Conductances[node1-1][node1-1] += c->compute_conductance();
            if(node2)
                Conductances[node2-1][node2-1] += c->compute_conductance();
        }
    }
    
    void print_voltages()
    {
        for(int i = 0; i < Size; i++)
            cout << setprecision(10) << Voltages[i] << endl;
    
    
    }
    
    void print_currents()
    {
        for(int i = 0; i < Currents.size(); i++)
            cout << Currents[i] << endl;
    }
    
    void print_conductances()
    {
        for(int i = 0; i < Conductances.size(); i++)
        {
            for(int j = 0; j < Conductances.size(); j++)
                cout << Conductances[i][j] << " ";
            
            cout << endl;
        }
    }
};

#endif
