//  Created by Mihai-Catalin Stretcu and Youngmin Song
//  Department of Electrical and Electronic Engineering, Imperial College London
//  June 2020

#include <fstream>
std::ofstream fout("data_out.txt");

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

//Helper function for read_input
pair<string, string> read_nodes(string &linern)
{
    stringstream ssrn(linern);
    string first_node, second_node;
    
    getline(ssrn, first_node, ' ');
    
    getline(ssrn, first_node, ' ');
    getline(ssrn, second_node, ' ');

    return make_pair(first_node, second_node);
}

//Function used for direct input: returns a map of the circuit and the analysis type.
Analysis *read_input(Nodes *circuit, Analysis *command)
{
    string lineri;
    
    while(getline(cin, lineri))
    {
        if(lineri[0] != '*' && lineri[0] != '.')
            circuit->add_branch(read_nodes(lineri), create_component(lineri));
        else
        {
            //Saves analysis type data
            if(lineri[0] == '.' && lineri != ".end")
                command = choose_analysis(lineri);
        }
    }
    
    return command;
}

//Reads input, creates circuit, saves analysis type and parameters and simulates according to input specs.
int main()
{
    Nodes *circuit = new Nodes();
    Analysis *command;
    
    command = read_input(circuit, command);
    
    if(command->get_type() == ".op")
    {
        double time = 0;
        
        circuit->compute_size();
        circuit->compute_voltages(time);
        
        //Outputs voltages
        circuit->print_voltages();
    }
    
    if(command->get_type() == ".tran")
    {
        //Does transient analysis using discrete time
        double tinterval = command->get_interval();
        double tduration = command->get_duration();
        double time = 0;
    
        circuit->compute_size();
        circuit->set_dt(tinterval);
        
        while(time <= tduration)
        {
            circuit->compute_voltages(time);
            
            time += tinterval;
        }
        
        circuit->print_voltages();
    }
    
    delete circuit;
    delete command;
}
