#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <utility>
#include <vector>
#include <cctype>
#include <map>
#include <cassert>
#include <iomanip>
#include <algorithm>
#include <Eigen/Dense>
#include "Components.hpp"
#include "Node.hpp"
#include "Analysis.hpp"

using namespace std;
using namespace Eigen;

/*
pair<string, string> read_nodes(string &line);
void read_input(Nodes *circuit, Analysis *command);
void node_voltages(Nodes *circuit);
*/

pair<string, string> read_nodes(string &linern)
{
    stringstream ssrn(linern);
    string first_node, second_node;
    
    getline(ssrn, first_node, ' ');
    
    getline(ssrn, first_node, ' ');
    getline(ssrn, second_node, ' ');

    return make_pair(first_node, second_node);
}

//function used for direct input: returns a map of the circuit and the analysis type.
Analysis *read_input(Nodes *circuit, Analysis *command)
{
    string lineri;
    
    while(getline(cin, lineri))
    {
        if(lineri[0] != '*' && lineri[0] != '.')
            circuit->add_branch(read_nodes(lineri), create_component(lineri));
        else
        {
            //save analysis type data
 
            if(lineri[0] == '.' && lineri != ".end")
                command = choose_analysis(lineri);
        }
    }
    
    return command;
}

void node_voltages(Nodes *circuit)
{
    /*
    do inverse matrix and multiply with current vector to find voltage vector
    (use eigen library)
    */
        
    MatrixXd cm(circuit->get_Size(), circuit->get_Size());
    for (int i = 0; i < circuit->get_Size(); i++)
        for(int j = 0; j < circuit->get_Size(); j++)
            cm(i, j) = circuit->get_conductance(i, j);
        
    VectorXd vi(circuit->get_Size());
        for (int i = 0; i < circuit->get_Size(); i++)
            vi(i) = circuit->get_current(i);
        
    VectorXd vv(circuit->get_Size());
    //choose different types of solutions to Ax = b from Eigen
    vv = cm.colPivHouseholderQr().solve(vi);
        
    for(int i = 0; i < circuit->get_Size(); i++)
        circuit->add_voltage(i, vv(i));
}

int main()
{
    Nodes *circuit = new Nodes();
    Analysis *command;
    
    command = read_input(circuit, command);
    
    if(command->get_type() == ".op")
    {
        node_voltages(circuit);
        //output voltages;
        circuit->print_voltages();
        
        cout << endl;
        
        circuit->print_conductances();
        
        cout << endl;
        
        circuit->print_currents();
    }
    
    if(command->get_type() == ".tran")
    {
        //do transient analysis using discrete time
    }
    
    delete circuit;
    delete command;
}
