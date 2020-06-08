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
    vector<Eigen::VectorXd> op_voltage_history;
    vector<Eigen::VectorXd> op_current_history;
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
    
   //helper function for add_I for active components
    void active_I()
    {
        //capacitors only
        for(int i = 0; i < current_sources.size(); i++)
        {
            Component *c = current_sources[i];
            
            if(c->get_type() == 'C' && op_voltage_history.size() >= 2)
            {
                int node1 = node_number(c->get_n1());
                int node2 = node_number(c->get_n2());
        
                Eigen::VectorXd v1 = op_voltage_history[op_voltage_history.size()-1];
                Eigen::VectorXd v2 = op_voltage_history[op_voltage_history.size()-2];
                
                double V11 = 0, V12 = 0, V21 = 0, V22 = 0;
                
                if(node1)
                {
                    V11 = v1(node1-1);
                    V12 = v2(node1-1);
                }
                if(node2)
                {
                    V21 = v1(node2-1);
                    V22 = v2(node2-1);
                }
        
                double t1 = time_history[time_history.size()-1];
                double t2 = time_history[time_history.size()-2];
        
                double dV = (V11 - V21) - (V12 - V22);
                double dt = t1 - t2;
        
                if(node1)
                    Currents(node1-1) -= c->get_C()*dV/dt;
                
                if(node2)
                    Currents(node2-1) += c->get_C()*dV/dt;
            }
        }
    }
    
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
        }
    }
    
    void active_V()
    {
        for(int i = 0; i < voltage_sources.size(); i++)
        {
            Component *c = voltage_sources[i].first;;
            
            if(c->get_type() == 'L' && op_current_history.size() >= 2)
            {
                int node1 = node_number(c->get_n1());
                int node2 = node_number(c->get_n2());
            
                int id = voltage_sources[i].second;
                
                Eigen::VectorXd i1 = op_current_history[op_current_history.size()-1];
                Eigen::VectorXd i2 = op_current_history[op_current_history.size()-2];
                
                double i11 = 0, i12 = 0, i21 = 0, i22 = 0;
                
                if(node1)
                {
                    i11 = i1(node1-1);
                    i12 = i2(node1-1);
                }
                if(node2)
                {
                    i21 = i1(node2-1);
                    i22 = i2(node2-1);
                }
            
                double t1 = time_history[time_history.size()-1];
                double t2 = time_history[time_history.size()-2];
                
                double di = (i11 - i21) - (i12 - i22);
                double dt = t1 - t2;
                
                double voltage = c->get_L()*di/dt;
            
                if(node1 && node2)
                {
                    Conductances(id-1, node1-1) = 1;
                    Conductances(node1-1, id-1) = 1;
                
                    Conductances(id-1, node2-1) = -1;
                    Conductances(node2-1, id-1) = -1;
                
                    Currents(id-1) = voltage;
                }
                else
                {
                    if(node1)
                    {
                        Conductances(id-1, node1-1) = 1;
                        Conductances(node1-1, id-1) = 1;
                    
                        Currents(id-1) = voltage;
                    }
                    else
                    {
                        Conductances(id-1, node2-1) = 1;
                        Conductances(node2-1, id-1) = 1;
                                   
                        Currents(id-1) = -voltage;
                    }
                }
            }
        }
    }
    
    void add_V(double &time)
    {
        for(int i = 0; i < voltage_sources.size(); i++)
        {
            Component *c = voltage_sources[i].first;;
            
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
        
        if(c->get_type() == 'I' || c->get_type() == 'C')
            current_sources.push_back(c);
        else
        {
            if(c->get_type() == 'R')
                add_conductance(node1, node2, c);
            else
            {
                //still needs testing for voltage sources in series
                assert(c->get_type() == 'V' || c->get_type() == 'L');
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
    
    void get_fullCurrents(Eigen::VectorXd &fc)
    {
        for(int i = 0; i < voltage_sources.size(); i++)
        {
            Component *c = voltage_sources[i].first;
            
            int node1, node2;
            
            if(c->get_type() == 'V')
            {
                node1 = node_number(c->get_np());
                node2 = node_number(c->get_nm());
            }
            else
            {
                node1 = node_number(c->get_n1());
                node2 = node_number(c->get_n2());
            }
          
            if(node1)
                fc(node1-1) += Voltages(Size-nSources+i);
            if(node2)
                fc(node2-1) -= Voltages(Size-nSources+i);
        }
    }
    
    void compute_voltages(double &time)
    {
         time_history.push_back(time);
        
        //empty current vecotor before each iteration
        Currents = Eigen::VectorXd::Zero(Size);
        add_I(time);
        add_V(time);
        Voltages = Conductances.colPivHouseholderQr().solve(Currents);
        
        op_voltage_history.push_back(Voltages);
        
        Eigen::VectorXd fullCurrents = Currents;
        get_fullCurrents(fullCurrents);
        op_current_history.push_back(fullCurrents);
        
        active_I();
        active_V();
        
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
