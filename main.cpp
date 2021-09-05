
#include <iostream>
#include <fstream>
#include <vector>

#include "agentQL.hpp"


int main(){

    agentQL ql(11,11,4,0.5,0.9,0.8,false);

    ql.train(1000);


    std::vector<std::vector<int> > shortest_path;
    shortest_path = ql.get_shortest_path(9, 5);
    for (unsigned int i=0; i<shortest_path.size(); i++)
        std::cout << "  " << shortest_path[i][0] << "-" << shortest_path[i][1];
    std::cout << std::endl << std::endl;


    return 0;
}