#include "Components.hpp"
#include "Node.hpp"
#include "Analysis.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <utility>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

void node_voltages(Nodes &circuit)
{
    /*
    do inverse matrix and multiply with current vector to find voltage vector
    (use eigen library)
    */
        
    MatrixXd cm(circuit.get_Size(), circuit.get_Size());
    for (int i = 0; i < circuit.get_Size(); i++)
        for(int j = 0; j < circuit.get_Size(); i++)
            m(i, j) = circuit.get_conductance(i, j);
        
    VectorXd vi(circuit.get_Size());
        for (int i = 0; i < circuit.get_Size(); i++)
            vi(i) = circuit.get_current(i);
        
    VectorXd vv(circuit.get_Size());
    //choose different types of solutions to Ax = b from Eigen
    vv = cm.colPivHouseholderQr().solve(vi);
        
    for(int i = 0; i < voltages.size(); i++)
        circuit.add_voltage(i, vv(i));
    
}
