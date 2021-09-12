#include "agentQL.hpp"


agentQL::agentQL(int env_rows, int env_cols, int env_actions, float learning_rate, float discount_factor, float epsilon, bool bDebug){
    this->env_rows = env_rows;
    this->env_cols = env_cols;
    this->env_actions = env_actions;
    this->learning_rate = learning_rate;
    this->discount_factor = discount_factor;
    this->epsilon = epsilon;
    this->bDebug = bDebug;

    // Ready randon number generator
    srand(time(0));

    // Initialize Q-Values to 0
    q_values = std::vector<std::vector<std::vector<float> > > (env_rows, std::vector<std::vector<float> >(env_cols, std::vector<float>(env_actions, 0.0)));
            
    // Fill rewards from csv file
    rewards = std::vector<std::vector<int> > (env_rows, std::vector<int>(env_cols,0));
    std::ifstream fp("board.csv");
    for (unsigned int i=0; i<env_rows; i++)
        for (unsigned int j=0; j<env_cols; j++)
            fp >> rewards[i][j];

}



agentQL::~agentQL(){}



void agentQL::print_qvalues(){
    std::cout << std::endl << "    Q-Values" << std::endl; 
    for (unsigned int i=0; i<q_values.size(); i++){
        for (unsigned int j=0; j<q_values[i].size(); j++){
            std::cout << "\t";
            for (unsigned int k=0; k<q_values[i][j].size(); k++)
                std::cout << q_values[i][j][k] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}



void agentQL::print_rewards(){
    std::cout << std::endl << "    Rewards" << std::endl; 
    for (unsigned int i=0; i<rewards.size(); i++){
        for (unsigned int j=0; j<rewards[i].size(); j++)
            std::cout << "\t" << rewards[i][j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}



bool agentQL::is_terminal_state(){
    // Determines if specified location is a terminal state
    if (rewards[row][col]==-1)
        return false;
    else
        return true;
}



void agentQL::get_starting_location(){
    // Determines a random not terminal starting location    
    int rowt = 0;
    int colt = 0;
            
    do {
        rowt = rand() % 11;
        colt = rand() % 11;
    }
    while (rewards[rowt][colt] != -1);

    row = rowt;
    col = colt;
}





void agentQL::get_next_action(){
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


void agentQL::get_next_location(){
    // Get new location based on last action
    int row1 = row;
    int col1 = col;
    if (action==0 and row>0)
        row1 -= 1;
    else if (action==1 and col<(env_cols-1))
        col1 += 1;
    else if (action==2 and row<(env_rows-1))
        row1 += 1;
    else if (action==3 and col>0)
        col1 -= 1;
    row = row1;
    col = col1;
}

    
void agentQL::train(int train_episodes){
    for (unsigned int episode=0; episode<train_episodes; episode++){

        get_starting_location();

        if (bDebug) std::cout << std::endl << "Episode " << episode << "\t" << row << "-" << col << "\t";

        do {
            row1 = row;
            col1 = col;

            get_next_action();
            get_next_location();

            if (bDebug) std::cout << action << " " << row << "-" << col << "\t";

            int reward = rewards[row][col];
            float old_q_value = q_values[row1][col1][action];

            float max_q_value = 0.0;
            for (unsigned int i=0; i<4; i++)
                if (q_values[row][col][i] > max_q_value)
                    max_q_value = q_values[row][col][i];

            float temporal_difference = reward +  (discount_factor * max_q_value) - old_q_value;
            float new_q_value = old_q_value + (learning_rate * temporal_difference);
            q_values[row1][col1][action] = new_q_value;

        }
        while (!is_terminal_state());
    }

    std::cout << std::endl << "Training complete!" << std::endl << std::endl;
}


std::vector<std::vector<int> > agentQL::get_shortest_path(int strow, int stcol){
    // Get shortest path between locations
    std::vector<std::vector<int> > path;

    row = strow;
    col = stcol;
            
    if (!is_terminal_state()) {
        path.push_back(std::vector<int> {row,col});

        do {
            get_next_action();
            get_next_location();
            path.push_back(std::vector<int> {row,col});     
        }
        while (!is_terminal_state());
    }

    return path;
}
