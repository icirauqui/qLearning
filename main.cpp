
#include <iostream>
#include <fstream>
#include <vector>

#include "agentQL.hpp"


const int environment_rows = 11;
const int environment_cols = 11;
const int action_dimension = 4;


/*
    Create a 3D numpy array to hold the current Q-Vañies for each state and action pair: Q(s,a)
    The array contains 11 rows and 11 columns (environment shape) as well as a third "action" dimension.
    The "action" dimension consists of 4 layers that will allow us to keep track of the Q-values for each possible action in each state.
    The value of each (state,action) pair is initialized to 0.
*/
float q_values[environment_rows][environment_cols][action_dimension];

/*
    Actions
    0 = up
    1 = right
    2 = down
    3 = left
*/

// Create a 2D array to hold the rewards for each state
// The array is 11x11, matching the shape of the environment
int rewards[environment_rows][environment_cols];


int row;
int col;
int action = 0;

float epsilon = 0.9;
float discount_factor = 0.9;
float learning_rate = 0.9;






bool is_terminal_state(int row, int col){
    // Determines if specified location is a terminal state
    if (rewards[row][col]==-1)
        return false;
    else
        return true;
}


void get_starting_location(int &row, int &col){
    // Determines a random not terminal starting location    
    int rowt = 0;
    int colt = 0;
    
    do {
        rowt = rand() % 11;
        colt = rand() % 11;
        //std::cout << rowt << "-" << colt << std::endl;
    }
    while (rewards[rowt][colt] != -1);

    row = rowt;
    col = colt;
}


void get_next_action(int row, int col, int &action, float epsilon){
    // If a randomly chosen value between 0 and 1 is less than epsilon
    // then choose the most promising value from the Q-table for this state.
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    if (r<epsilon) {
        float valmax = 0;
        int idxmax = 0;
        for (unsigned int i=0; i<4; i++)
            if (q_values[row][col][i] > valmax){
                valmax = q_values[row][col][i];
                idxmax = i;
            }
        action = idxmax;
    }
    else {
        action = rand() % 4;
    }   
}

void get_next_location(int &row, int &col, int action){
    // Get new location based on last action
    int row1 = row;
    int col1 = col;

    if (action==0 and row>0)
        row1 -= 1;
    else if (action==1 and col<(environment_cols-1))
        col1 += 1;
    else if (action==2 and row<(environment_rows-1))
        row1 += 1;
    else if (action==3 and col>0)
        col1 -= 1;

    row = row1;
    col = col1;
}



void printQvalues(){
    for (unsigned int i=0; i<environment_rows; i++){
        for (unsigned int j=0; j<environment_cols; j++){
            std::cout << "\t";
            for (unsigned int k=0; k<action_dimension; k++){
                std::cout << q_values[i][j][k] << " ";
            }
        }
        std::cout << std::endl;
    }
}





void qtrain(int train_episodes, bool bDebug){
    for (unsigned int episode=0; episode<train_episodes; episode++){



        get_starting_location(row, col);

        if (bDebug)
            std::cout << std::endl << "Episode " << episode << "\t" << row << "-" << col << "\t";

        

        do {
            
            get_next_action(row,col,action,epsilon);
            

            int row0 = row;
            int col0 = col;

            get_next_location(row,col,action);
            if (bDebug)
                std::cout << action << " " << row << "-" << col << "\t";

            int reward = rewards[row][col];
            float old_q_value = q_values[row0][col0][action];

            float max_q_value = 0.0;
            for (unsigned int i=0; i<4; i++)
                if (q_values[row][col][i] > max_q_value)
                    max_q_value = q_values[row][col][i];

            float temporal_difference = reward +  (discount_factor * max_q_value) - old_q_value;

            float new_q_value = old_q_value + (learning_rate * temporal_difference);
            q_values[row0][col0][action] = new_q_value;

        }
        while (!is_terminal_state(row,col));
    }

    std::cout << std::endl << "Training complete!" << std::endl;
}





std::vector<std::vector<int> > get_shortest_path(int strow, int stcol){
    // Get shortest path between locations
    std::vector<std::vector<int> > path;
    
    if (!is_terminal_state(strow,stcol)) {
        int crow = strow;
        int ccol = stcol;

        std::vector<int> pos{crow,ccol};
        path.push_back(pos);

        do {
            get_next_action(crow,ccol,action,epsilon);
            get_next_location(crow,ccol,action);
            pos = std::vector<int> {crow,ccol};
            path.push_back(pos);     
        }
        while (!is_terminal_state(crow,ccol));
    }

    return path;
}





int main(){

    std::cout << "12" << std::endl;
    agentQL ql(11,11,4,0.5,0.9,0.8,false);

    

    std::cout << ql.q_values.size() << std::endl;
    std::cout << ql.q_values[0].size() << std::endl;
    std::cout << ql.q_values[0][0].size() << std::endl;
   
    /*
    for (unsigned int i=0; i<ql.q_values.size(); i++){
        for (unsigned int j=0; j<ql.q_values[i].size(); j++){
            for (unsigned int k=0; k<ql.q_values[i][j].size(); k++){
                //std::cout << " " << ql.q_values[i][j][k];
                std::cout << "a ";
            }
            std::cout << "\t";
        }
        std::cout << std::endl; 
    }

    ql.printQvalues();
    */


    /*
    //srand(time(0));
    int train_episodes = 1000000;

    // Fill rewards array with board info from csv file
    std::ifstream fp("board.csv");
    for (unsigned int i=0; i<environment_rows; i++){
        for (unsigned int j=0; j<environment_cols; j++){
            fp >> rewards[i][j];
        }
    }

    qtrain(train_episodes,true);
    //printQvalues();

    std::vector<std::vector<int> > shortest_path;
    shortest_path = get_shortest_path(9, 5);
    for (unsigned int i=0; i<shortest_path.size(); i++)
        std::cout << "  " << shortest_path[i][0] << "-" << shortest_path[i][1];

    */

    return 0;
}