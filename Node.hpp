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
public:
    void add_branch(const pair<string, string> &p, const Component &c)
    {
        Branch[p].push_back(c);
    }
}

#endif
