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
    vector<vector<int>> okC;
    vector<double> Currents;
    vector<double> Voltages;
    map<string, int> Labels;
    //number of nodes in the circuit
    unsigned int Size;
    
    //helper function for add_current
    void to_current(int &node, double &val)
    {
        if(!okC[node-1][node-1])
            Currents[node-1] += val;
        else
            if(okC[node-1][node-1] != node)
                to_current(okC[node-1][node-1], val);
    }
    
    void add_current(int &node1, int &node2, Component *c)
    {
        if(node1)
        {
            double *current = new double;
            *current = c->get_I();
            
            to_current(node1, *current);
            
            delete current;
        }
        
        if(node2)
        {
            double *current = new double;
            *current = -c->get_I();
            
            to_current(node2, *current);
            
            delete current;
        }
    }
    
    //helper function for add_conductance
    void to_conductance(int &node1, int &node2, double &con)
    {
        if(!okC[node1-1][node2-1])
            Conductances[node1-1][node2-1] += con;
        else
            if(okC[node1-1][node2-1] != node1)
                to_conductance(okC[node1-1][node2-1], node2, con);
    }
    
    void add_conductance(int &node1, int &node2, Component *c)
    {
        if(node1 && node2)
        {
            double *conductance = new double;
            *conductance = -c->compute_conductance();
            
            to_conductance(node1, node2, *conductance);
            to_conductance(node2, node1, *conductance);
            
            delete conductance;
        }
        
        if(node1)
        {
            double *conductance = new double;
            *conductance = c->compute_conductance();
            
            to_conductance(node1, node1, *conductance);
            
            delete conductance;
        }
        if(node2)
        {
            double *conductance = new double;
            *conductance = c->compute_conductance();
            
            to_conductance(node2, node2, *conductance);
            
            delete conductance;
        }
    }
    
    void add_source(int &node1, int &node2, Component *c)
    {
        if(node1 && node2)
        {
            if(okC[node1-1][node1-1] != node2 && okC[node2-1][node2-1] != node1)
            {
                for(int i = 0; i < Size; i++)
                    Conductances[node2-1][i] += Conductances[node1-1][i];
                
                fill(Conductances[node1-1].begin(), Conductances[node1-1].end(), 0);
                fill(okC[node1-1].begin(), okC[node1-1].end(), node2);
                
                Conductances[node1-1][node2-1] = -1;
                Conductances[node1-1][node1-1] = 1;
                
                Currents[node2-1] += Currents[node1-1];
                Currents[node1-1] = c->get_V();
            }
            else
                cerr << "Voltage sources in parallel! Check input." << endl;
        }
        else
        {
            if(node1)
            {
                if(okC[node1-1][node1-1] != node1)
                {
                    fill(Conductances[node1-1].begin(), Conductances[node1-1].end(), 0);
                    fill(okC[node1-1].begin(), okC[node1-1].end(), node1);
                    
                    Conductances[node1-1][node1-1] = 1;
                    Currents[node1-1] = c->get_V();
                }
                else
                    cerr << "Voltage sources in parallel! Check input." << endl;
            }
            else
            {
                if(okC[node2-1][node2-1] != node2)
                {
                    fill(Conductances[node2-1].begin(), Conductances[node2-1].end(), 0);
                    fill(okC[node2-1].begin(), okC[node2-1].end(), node2);
                
                    Conductances[node2-1][node2-1] = 1;
                    Currents[node2-1] = -c->get_V();
                }
                else
                    cerr << "Voltage sources in parallel! Check input." << endl;
            }
        }
    }

    
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
        okC.resize(Size);
        for(int i = 0; i < Conductances.size(); i++)
        {
            Conductances[i].resize(Size);
            if(!okC[i].empty())
                okC[i].resize(Size, okC[i][0]);
            else
                okC[i].resize(Size);
        }
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
         Current sources + Voltage sources
        */
        
        if(c->get_type() == 'I')
            add_current(node1, node2, c);
        else
        {
            //add to conductance matrix
            /*
             Resistors and voltage sources only
            */
            
            if(c->get_type() == 'R')
                add_conductance(node1, node2, c);
            else
            {
                //still needs testing for voltage sources in series
                assert(c->get_type() == 'V');
                add_source(node1, node2, c);
            }
        }
    }
    
    void print_voltages()
    {
        for(int i = 0; i < Size; i++)
            cout << setprecision(10) << Voltages[i] << endl;
    
    
    }
    
    void print_currents()
    {
        //using to test
        for(int i = 0; i < Currents.size(); i++)
            cout << Currents[i] << endl;
    }
    
    void print_conductances()
    {
        //using to test
        for(int i = 0; i < Conductances.size(); i++)
        {
            for(int j = 0; j < Conductances.size(); j++)
                cout << Conductances[i][j] << " ";
            
            cout << endl;
        }
    }
    
    void print_ok()
    {
        //using to test
        for(int i = 0; i < okC.size(); i++)
        {
            for(int j = 0; j < okC.size(); j++)
                cout << okC[i][j] << " ";
                   
            cout << endl;
        }
    }
};

#endif
