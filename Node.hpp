#ifndef node_hpp
#define node_hpp
/*
Converting to Eigen instead of normal vectors and using MNA
*/

class Nodes
{
private:
    map<pair<string, string>, vector<Component>> Branch;
    vector<pair<Component*, int>> voltage_sources;
    vector<Component*> current_sources;
    vector<Eigen::VectorXd> voltage_history;
    vector<double> time_history;
    Eigen::MatrixXd Conductances;
    Eigen::VectorXd Currents;
    Eigen::VectorXd Voltages;
    map<string, int> Labels;
    map<int, string> intLabels;
    int Size;
    int nSources;

    void Resize()
    {
        Currents.conservativeResizeLike(Eigen::VectorXd::Zero(Size));
        Voltages.conservativeResizeLike(Eigen::VectorXd::Zero(Size));
        Conductances.conservativeResizeLike(Eigen::MatrixXd::Zero(Size, Size));
    }
    
    int node_number(string s)
    {
        assert (!s.empty());
        
        string slabel = s;
        
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
                intLabels[stoi(s)] = slabel;
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
                    intLabels[Size] = slabel;
                    return Size;
                }
            
            }
        }
        return 0;
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
    
    void add_I(double &time)
    {
        for(int i = 0; i < current_sources.size(); i++)
        {
            Component *c = current_sources[i];
            
            int node1 = node_number(c->get_nin());
            int node2 = node_number(c->get_nout());
            
            if(node1)
            {
                double *current = new double;
                *current = c->get_I(time);
                
                Currents(node1-1) += *current;
                
                delete current;
            }
            
            if(node2)
            {
                double *current = new double;
                *current = -c->get_I(time);
                
                Currents(node2-1) += *current;
                
                delete current;
            }
        }
    }
    
    void add_V(double &time)
    {
        for(int i = 0; i < voltage_sources.size(); i++)
        {
            Component *c = voltage_sources[i].first;;
            
            int node1 = node_number(c->get_np());
            int node2 = node_number(c->get_nm());
            
            int id = voltage_sources[i].second;
            
            if(node1 && node2)
            {
                Conductances(id-1, node1-1) = 1;
                Conductances(node1-1, id-1) = 1;
                
                Conductances(id-1, node2-1) = -1;
                Conductances(node2-1, id-1) = -1;
                
                Currents(id-1) = c->get_V(time);
            }
            else
            {
                if(node1)
                {
                    Conductances(id-1, node1-1) = 1;
                    Conductances(node1-1, id-1) = 1;
                    
                    Currents(id-1) = c->get_V(time);
                }
                else
                {
                    Conductances(id-1, node2-1) = 1;
                    Conductances(node2-1, id-1) = 1;
                                   
                    Currents(id-1) = -c->get_V(time);
                }
            }
        }
    }
    
public:
    Nodes()
    {
        Size = 0;
        nSources = 0;
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
            current_sources.push_back(c);
        else
        {
            if(c->get_type() == 'R')
                add_conductance(node1, node2, c);
            else
            {
                //still needs testing for voltage sources in series
                assert(c->get_type() == 'V');
                nSources++;
                voltage_sources.push_back(make_pair(c, nSources));
            }
        }
        
    }
    
    void compute_size()
    {
        for(int i = 0; i < voltage_sources.size(); i++)
        {
            voltage_sources[i].second += Size;
            intLabels[voltage_sources[i].second] = "I(" + voltage_sources[i].first->get_name() + ")";
        }
        Size += nSources;
        Resize();
    }
    
    void compute_voltages()
    {
        double time = 0;
        Currents = Eigen::VectorXd::Zero(Size);
        add_I(time);
        add_V(time);
        Voltages = Conductances.colPivHouseholderQr().solve(Currents);
        
        voltage_history.push_back(Voltages);
    }
    
    void compute_voltages(double &time)
    {
        time_history.push_back(time);
        
        //empty current vecotor before each iteration
        Currents = Eigen::VectorXd::Zero(Size);
        add_I(time);
        add_V(time);
        Voltages = Conductances.colPivHouseholderQr().solve(Currents);
        
        voltage_history.push_back(Voltages);
    }
    
    void print_voltages()
    {
        fout << "time" << "\t";
        
        for(int i = 0; i < Size; i++)
        {
            if(i < Size - voltage_sources.size())
                fout << "V(" << intLabels[i+1] << ")" << "\t";
            else
                fout << intLabels[i+1] << "\t";
        }
        fout << endl;
        
        for(int i = 0; i < voltage_history.size(); i++)
        {
            fout << time_history[i] << "\t";
            Eigen::VectorXd xv = voltage_history[i];
            
            for(int j = 0; j < Size; j++)
                fout << setprecision(10) << xv(j) << "\t";
            
            fout << endl;
        }
    
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
