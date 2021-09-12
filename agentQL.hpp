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

        agentQL(int env_rows, int env_cols, int env_actions, float learning_rate, float discount_factor, float epsilon, bool bDebug = false);

        ~agentQL();


        void print_qvalues();
        void print_rewards();

        bool is_terminal_state();
        void get_starting_location();
        void get_next_action();
        void get_next_location();

        void train(int train_episodes);


        std::vector<std::vector<int> > get_shortest_path(int strow, int stcol);

};