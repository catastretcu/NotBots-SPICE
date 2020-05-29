#ifndef node_hpp
#define node_hpp

#include "Components.hpp"
#include <iostream>
#include <vector>
#include <cstring>
#include <cmap>
#include <utility>

using namespace std;

//can be modified to resemble a matrix by adding components at both a[i][j] and a[j][i] nodes in inverse order
class Nodes
{
private:
    map<pair<string, string>, vector<Component>> Branch;
    vector<vector<double>> Conductances;
    vector<double> Currents;
    map<string, int> Labels;
    unsigned int Size;
public:
    Nodes()
    {
        Size = 0;
    }
    void add_branch(const pair<string, string> &p, const Component &c)
    {
        Branch[p].push_back(c);
        //dynamicly calculate Currents and Conductances
    }
}

int node_number(string &s)
{
    if(s[0] == 'n' || s[0] == 'N')
        //map to number after n
    else
    {
        //check label
        //if new, add new label with number Size+1
    }
}

#endif
