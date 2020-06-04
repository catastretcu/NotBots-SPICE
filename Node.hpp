#ifndef node_hpp
#define node_hpp
/*
Converting to Eigen instead of normal vectors and using MNA
*/

class Nodes
{
private:
    map<pair<string, string>, vector<Component>> Branch;
    vector<Component*> voltage_sources;
    Eigen::MatrixXd Conductances;
    Eigen::VectorXd Currents;
    Eigen::VectorXd Voltages;
    map<string, int> Labels;
    int Size;

    void Resize()
    {
        Currents.conservativeResizeLike(Eigen::VectorXd::Zero(Size));
        Voltages.conservativeResizeLike(Eigen::VectorXd::Zero(Size));
        Conductances.conservativeResizeLike(Eigen::MatrixXd::Zero(Size, Size));
    }
    
    int node_number(string s)
    {
        assert (!s.empty());
        
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
    
    void add_current(int &node1, int &node2, Component *c)
    {
        if(node1)
        {
            double *current = new double;
            *current = c->get_I();
            
            Currents(node1-1) += *current;
            
            delete current;
        }
        
        if(node2)
        {
            double *current = new double;
            *current = -c->get_I();
            
            Currents(node2-1) += *current;
            
            delete current;
        }
    }
    
    void add_conductance(int &node1, int &node2, Component *c)
    {
        if(node1 && node2)
        {
            double *conductance = new double;
            *conductance = -c->compute_conductance();
            
            Conductances(node1-1, node2-1) += *conductance;
            Conductances(node2-1, node1-1) += *conductance;
            
            delete conductance;
        }
        
        if(node1)
        {
            double *conductance = new double;
            *conductance = c->compute_conductance();
            
            Conductances(node1-1, node1-1) += *conductance;
            
            delete conductance;
        }
        if(node2)
        {
            double *conductance = new double;
            *conductance = c->compute_conductance();
            
            Conductances(node2-1, node2-1) += *conductance;
            
            delete conductance;
        }
    }
    
    void add_V()
    {
        for(int i = 0; i < voltage_sources.size(); i++)
        {
            Component *c = voltage_sources[i];
            
            int node1 = node_number(c->get_np());
            int node2 = node_number(c->get_nm());
            
            Size++;
            Resize();
            
            if(node1 && node2)
            {
                Conductances(Size-1, node1-1) = 1;
                Conductances(node1-1, Size-1) = 1;
                
                Conductances(Size-1, node2-1) = -1;
                Conductances(node2-1, Size-1) = -1;
                
                Currents(Size-1) = c->get_V();
            }
            else
            {
                if(node1)
                {
                    Conductances(Size-1, node1-1) = 1;
                    Conductances(node1-1, Size-1) = 1;
                    
                    Currents(Size-1) = c->get_V();
                }
                else
                {
                    Conductances(Size-1, node2-1) = 1;
                    Conductances(node2-1, Size-1) = 1;
                                   
                    Currents(Size-1) = -c->get_V();
                }
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
    
    double get_conductance(int &i, int &j)
    {
        return Conductances(i, j);
    }
    
    double get_current(int &i)
    {
        return Currents(i);
    }
    
    double get_voltage(int &i)
    {
        return Voltages(i);
    }
    
    void add_voltage(int &i, double &value)
    {
        Voltages(i) = value;
    }
    
    void add_branch(pair<string, string> p, Component *c)
    {
        Branch[p].push_back(*c);
        
        int node1 = node_number(p.first);
        int node2 = node_number(p.second);
        
        if(c->get_type() == 'I')
            add_current(node1, node2, c);
        else
        {
            if(c->get_type() == 'R')
                add_conductance(node1, node2, c);
            else
            {
                //still needs testing for voltage sources in series
                assert(c->get_type() == 'V');
                voltage_sources.push_back(c);
            }
        }
        
    }
    
    void compute_voltages()
    {
        add_V();
        
        Voltages = Conductances.colPivHouseholderQr().solve(Currents);
    }
    
    void print_voltages()
    {
        for(int i = 0; i < Size; i++)
            cout << setprecision(10) << Voltages(i) << endl;
    
    
    }
    
    void print_currents()
    {
        //using to test
        for(int i = 0; i < Size; i++)
            cout << Currents(i) << endl;
    }
    
    void print_conductances()
    {
        //using to test
        for(int i = 0; i < Size; i++)
        {
            for(int j = 0; j < Size; j++)
                cout << Conductances(i, j) << " ";
            
            cout << endl;
        }
    }
};

#endif
