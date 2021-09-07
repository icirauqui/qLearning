#include <iostream>
#include <vector>
#include <fstream>


class agentQL{


    public:

        int env_rows = 11;
        int env_cols = 11;
        int env_actions = 4;

        int row, row1;
        int col, col1;
        int action, action1;


        float learning_rate = 0.9;
        float discount_factor = 0.9;
        float epsilon = 0.9;

        bool bDebug = false;



        /*
            Create an array to hold the current Q-Vañies for each state and action pair: Q(s,a)
            The array contains 11 rows and 11 columns (environment shape) as well as a third "action" dimension.
            The "action" dimension consists of 4 layers that will allow us to keep track of the Q-values for each possible action in each state.
            The value of each (state,action) pair is initialized to 0.
        */
        std::vector<std::vector<std::vector<float> > > q_values;

        // Create a 2D array to hold the rewards for each state
        // The array is 11x11, matching the shape of the environment
        std::vector<std::vector<int> > rewards;

        /*
            Actions
            0 = up
            1 = right
            2 = down
            3 = left
        */



    public:

        agentQL(int env_rows1, int env_cols1, int env_actions1, float learning_rate1, float discount_factor1, float epsilon1, bool bDebug1 = false):
            env_rows(env_rows1), env_cols(env_cols1), env_actions(env_actions1), learning_rate(learning_rate1), discount_factor(discount_factor1), epsilon(epsilon1), bDebug(bDebug1) {

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

        ~agentQL(){};


        void print_qvalues(){
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

        void print_rewards(){
            std::cout << std::endl << "    Rewards" << std::endl; 
            for (unsigned int i=0; i<rewards.size(); i++){
                for (unsigned int j=0; j<rewards[i].size(); j++)
                    std::cout << "\t" << rewards[i][j];
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }



        bool is_terminal_state(){
            // Determines if specified location is a terminal state
            if (rewards[row][col]==-1)
                return false;
            else
                return true;
        }



        void get_starting_location(){
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





        void get_next_action(){
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


        void get_next_location(){
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

    
        void train(int train_episodes){
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


        std::vector<std::vector<int> > get_shortest_path(int strow, int stcol){
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

};