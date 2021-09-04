
#include <iostream>


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


int main(){

    for (unsigned int i=0; i<environment_rows; i++){
        for (unsigned int j=0; j<environment_cols; j++){
            for (unsigned int k=0; k<action_dimension; k++){
                std::cout << " " << q_values[i][j][k];
            }
            std::cout << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}