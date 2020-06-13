#ifndef node_hpp
#define node_hpp

//  Created by Mihai-Catalin Stretcu and Youngmin Song
//  Department of Electrical and Electronic Engineering, Imperial College London
//  June 2020


/*
 Nodes is a class that can hold all necessary information about a circuit entity
 */
class Nodes
{
private:
    //Holds all components between a set of nodes
    map<pair<string, string>, vector<Component>> Branch;
    
    //Holds voltage sources and their respective id
    vector<pair<Component*, int>> voltage_sources;
    
    vector<Component*> current_sources;
    vector<Component*> non_linear;
    vector<Eigen::VectorXd> voltage_history;
    vector<double> time_history;
    Eigen::MatrixXd Conductances;
    Eigen::VectorXd Currents;
    Eigen::VectorXd Voltages;
    
    //Holds the index of arbitrary labels
    map<string, int> Labels;
    
    //Holds labels assosicated with an int value for printing voltage labels
    map<int, string> intLabels;
    
    int Size;
    int nSources;
    double dt;
    
    //Indicates presence of non_linear components in the circuit and is used to check the need for newton_iterate()
    bool is_non_linear;

    //Resizes Conductances, Voltages and Currents according to Size
    void Resize()
    {
        /*
         Using conservativeResizeLike() to conserve intial values on their initial position
         */
        Currents.conservativeResizeLike(Eigen::VectorXd::Zero(Size));
        Voltages.conservativeResizeLike(Eigen::VectorXd::Zero(Size));
        Conductances.conservativeResizeLike(Eigen::MatrixXd::Zero(Size, Size));
    }
    
    /*
     Returns an int node number according to a string.
     Function enables the simulator to use arbitrary labels such as 'V_in', 'V_out'
     */
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
                /*
                 check label
                 if new, add new label with number Size+1
                */
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
    
    //Adds a conductance to matrix 'Conductances' (initial guess for diodes)
    void add_conductance(int &node1, int &node2, Component *c)
    {
        if(c->get_type() == 'R' || c->get_type() == 'D')
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
    }
    
    //Adds a current to vector 'Currents' at time-step &time
    void add_I(double &time)
    {
        for(int i = 0; i < current_sources.size(); i++)
        {
            Component *c = current_sources[i];
            
            if(c->get_type() == 'I')
            {
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
            if(c->get_type() == 'C' && voltage_history.size()>0)
            {
                int node1 = node_number(c->get_n1());
                int node2 = node_number(c->get_n2());
                
                Eigen::VectorXd v1 = voltage_history[voltage_history.size()-1];
                
                double current;
                
                if(node1 && node2)
                    current = c->get_C()*(v1(node1-1) - v1(node2-1))/dt;
                else
                {
                    if(node1)
                        current = c->get_C()*v1(node1-1)/dt;
                    else
                        current = c->get_C()*(-v1(node2-1))/dt;
                }
                
                if(node1)
                    Currents(node1-1) += current;
                if(node2)
                    Currents(node2-1) -= current;
            }
        }
        for(int i = 0; i < non_linear.size(); i++)
        {
            Component *c = non_linear[i];
            
            if(c->get_type() == 'D')
            {
                int node1 = node_number(c->get_n1());
                int node2 = node_number(c->get_n2());
                
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
        }
    }
    
    //Adds a voltage source in the circuit; modifies both matrix 'Conductances' and vector 'Currents'
    void add_V(double &time)
    {
        for(int i = 0; i < voltage_sources.size(); i++)
        {
            Component *c = voltage_sources[i].first;
            
            if(c->get_type() == 'V')
            {
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
            if(c->get_type() == 'L')
            {
                int node1 = node_number(c->get_n1());
                int node2 = node_number(c->get_n2());
                
                int id = voltage_sources[i].second;
                double lv = 0;
                
                if(voltage_history.size() > 0)
                {
                    Eigen::VectorXd v1 = voltage_history[voltage_history.size()-1];
                    lv = -v1(id-1)*c->get_L()/dt;
                    Conductances(id-1, id-1) = -c->get_L()/dt;
                }
                
                if(node1 && node2)
                {
                    Conductances(id-1, node1-1) = 1;
                    Conductances(node1-1, id-1) = 1;
                    
                    Conductances(id-1, node2-1) = -1;
                    Conductances(node2-1, id-1) = -1;
                    
                    Currents(id-1) = lv;
                }
                else
                {
                    if(node1)
                    {
                        Conductances(id-1, node1-1) = 1;
                        Conductances(node1-1, id-1) = 1;
                        
                        Currents(id-1) = lv;
                    }
                    else
                    {
                        Conductances(id-1, node2-1) = 1;
                        Conductances(node2-1, id-1) = 1;
                        
                        Currents(id-1) = -lv;
                    }
                }
            }
        }
    }
    
    //clears conductance matrix and current vector for a new iteration of newton_iterate()
    void nl_clear(Component *c)
    {
        int node1 = node_number(c->get_n1());
        int node2 = node_number(c->get_n2());
        
        if(node1 && node2)
            Conductances(node1-1, node2-1) += c->compute_conductance();
            Conductances(node2-1, node1-1) += c->compute_conductance();
        
        if(node1)
        {
            Conductances(node1-1, node1-1) -= c->compute_conductance();
            Currents(node1-1) -= c->get_I();
        }
        if(node2)
        {
            Conductances(node2-1, node2-1) -= c->compute_conductance();
            Currents(node2-1) += c->get_I();
        }
    }
    
    //fills conductance matrix and current vector during new iteration of newton_iterate()
    void nl_fill(Component *c)
    {
        int node1 = node_number(c->get_n1());
        int node2 = node_number(c->get_n2());
        
        if(node1 && node2)
            Conductances(node1-1, node2-1) -= c->compute_conductance();
            Conductances(node2-1, node1-1) -= c->compute_conductance();
        
        if(node1)
        {
            Conductances(node1-1, node1-1) += c->compute_conductance();
            Currents(node1-1) += c->get_I();
        }
        if(node2)
        {
            Conductances(node2-1, node2-1) += c->compute_conductance();
            Currents(node2-1) -= c->get_I();
        }
    }
    
    //iterates Newton-Raphson method until reaching a good approximation
    void newton_iterate()
    {
        bool ok = 1;
        
        while (ok)
        {
            Eigen::VectorXd nlv = Voltages;
            double vd;
            
            for(int i = 0; i < non_linear.size(); i++)
            {
                Component *c = non_linear[i];
                
                int node1 = node_number(c->get_n1());
                int node2 = node_number(c->get_n2());
                
                if(node1 && node2)
                    vd = nlv(node1-1) - nlv(node2-1);
                else
                {
                    if(node1)
                        vd = nlv(node1-1);
                    else
                        vd = -nlv(node2-1);
                }
                
                nl_clear(c);
                c->d_iterate(vd);
                nl_fill(c);
            }
            
            Voltages = Conductances.householderQr().solve(Currents);
            
            ok = 0;
            for(int i = 0; i < non_linear.size(); i++)
            {
                Component *c = non_linear[i];
                
                int node1 = node_number(c->get_n1());
                int node2 = node_number(c->get_n2());
                
                if(abs((Voltages(node1-1) - Voltages(node2-1))-(nlv(node1-1) - nlv(node2-1))) > 0.00001)
                    ok = 1;
            }
        }
    }
    
public:
    Nodes()
    {
        Size = 0;
        nSources = 0;
        dt = 0;
        is_non_linear = 0;
    }
    
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
        
        if(c->get_type() == 'I' || c->get_type() == 'C')
            current_sources.push_back(c);
        
        if(c->get_type() == 'R' || c->get_type() == 'D')
            add_conductance(node1, node2, c);
        
        if(c->get_type() == 'V' || c->get_type() == 'L')
        {
            nSources++;
            voltage_sources.push_back(make_pair(c, nSources));
        }
        
        if(c->get_type() == 'D')
        {
            is_non_linear = 1;
            non_linear.push_back(c);
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
    
    //Saves time-step value and initialises 'Conductances' to treat capacitors as open-circuit
    void set_dt(double &t)
    {
        dt = t;
        
        for(int i = 0; i < current_sources.size(); i++)
        {
            Component *c = current_sources[i];
            
            if(c->get_type() == 'C')
            {
                int node1 = node_number(c->get_n1());
                int node2 = node_number(c->get_n2());
                
                double conductance = c->get_C()/t;
                
                if(node1 && node2)
                {
                    Conductances(node1-1, node2-1) -= conductance;
                    Conductances(node2-1, node1-1) -= conductance;
                }
                
                if(node1)
                    Conductances(node1-1, node1-1) += conductance;
                if(node2)
                    Conductances(node2-1, node2-1) += conductance;
            }
        }
    }
    
    //Calculates voltages at a discrete point in time. Makes use of Ax = b solver implementation from Eigen.
    void compute_voltages(double &time)
    {
        time_history.push_back(time);
        
        Currents = Eigen::VectorXd::Zero(Size);
        add_I(time);
        add_V(time);
        
        Voltages = Conductances.householderQr().solve(Currents);
        
        if(is_non_linear)
            newton_iterate();
        
        voltage_history.push_back(Voltages);
    }
    
    //Prints output to file data_out.txt
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
        //Used to test
        for(int i = 0; i < Size; i++)
            cout << Currents(i) << endl;
    }
    
    void print_conductances()
    {
        //Used to test
        for(int i = 0; i < Size; i++)
        {
            for(int j = 0; j < Size; j++)
                cout << Conductances(i, j) << " ";
            
            cout << endl;
        }
    }
};

#endif
