/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Simulated_Annealing.h"

#include "vector"

//"Main function"
double Simulated_Annealing(vector< vector <double> >& route_time, vector<node_ID>& current_node_path){
    
  
    //set up a random instance
    
    TSP_Instance tsp_instance;
 
    
    tsp_instance.createNew(route_time, current_node_path.size(), current_node_path);
 
    
    //Initialize all related classes
    Optimizer optimizer;
    
    //Initialize three strategies of moving
    ChainReverseMove move1;
    SwapCityMove move2;
    RotateCityMove move3;
 
    
    //Add these strategies into optimize plan "moves" 
    optimizer.add_move(&move1);
    
    optimizer.add_move(&move2);
    
    optimizer.add_move(&move3);
    
    
    //Set up the parameter for annealing, including initial temperature, 
    //end temperature and delta, which is constant
    Geometric_Cooling_Action action(150, 1e-2, 0.95);
    
    //put the cooling schedule into optimizer
    optimizer.cooling_action = & action;
    
    //outer loop define the times to lower the temperature 
    optimizer.outer_loop = 100;
    
    //inner loop defines the 
    optimizer.inner_loop = 5000;
        
    //Start optimization
    double best = optimizer.Optimize(tsp_instance, current_node_path);   

    return best;
    
    
}


double calculate_tour_length(const std::vector<node_ID>& tour){
    
//    assert(tour.size() == cities.size());
    
    double result = 0;

//********************************Do not consider first and last depot*********    
    for(size_t index = 0; index < tour.size() -1 ; index++){
        

        result = result + globalVar.travel_distance[tour[index]][tour[index+1]];
    }
    //We need to consider the starting and ending depot
    result += globalVar.depot_distance[tour[0]];
    result += globalVar.depot_distance[tour[tour.size() - 1]];
    
    return double(result);
    
}


double time_difference_chain_reverse(vector< vector<double> > & route_time, std::vector<node_ID> & state, node_ID i, node_ID j){
   
    double time_difference = 0;
    
    if(i == 0){
        time_difference -= globalVar.depot_distance[state[i]];
        time_difference += globalVar.depot_distance[state[j]];
        
    }
    
    
    
    //calculate the conjunction point
    else{
        time_difference -= route_time[state[i-1]][state[i]];
        time_difference += route_time[state[i-1]][state[j]];
        
    }
    
    //calculate the middle part
    for(node_ID d = i; d <= j - 1; d++){
        time_difference -= route_time[state[d]][state[d+1]];
        time_difference += route_time[state[d+1]][state[d]];
               
    }
    
    //if j is not the last index
    if(j <= state.size() -2 ){
        time_difference -= route_time[state[j]][state[j+1]];
        time_difference += route_time[state[i]][state[j+1]];
    }
       
    //else j is the end point
    else{
        time_difference -= globalVar.depot_distance[state[j]];
        time_difference += globalVar.depot_distance[state[i]];
    }
    
    
    return time_difference;
       
    
}

double time_difference_swap(vector< vector<double> > & route_time, std::vector<node_ID> & state, node_ID i, node_ID j){
    
    double time_difference = 0;
    
    //Take depot into consideration
    if(i == 0){
        time_difference -= globalVar.depot_distance[state[i]];
        time_difference += globalVar.depot_distance[state[j]];
        
    }
    
    //i is not starting point
    else{
        time_difference -= route_time[state[i-1]][state[i]];
        time_difference += route_time[state[i-1]][state[j]];
        
    }
    
    //If they are adjacent, just swap them
    if(j == i+1){
        time_difference -= route_time[state[i]][state[j]];
        time_difference += route_time[state[j]][state[i]];
    }

    //They are not adjacent
    else{
        time_difference -= route_time[state[j-1]][state[j]];
        time_difference += route_time[state[j]][state[i+1]];
        
        time_difference -= route_time[state[i]][state[i+1]];
        time_difference += route_time[state[j-1]][state[i]];
        
    }
    
    if(j <= state.size()-2){
        time_difference -= route_time[state[j]][state[j+1]];
        time_difference += route_time[state[i]][state[j+1]];
    }
    
    //else j is the end point
    else{
        time_difference -= globalVar.depot_distance[state[j]];
        time_difference += globalVar.depot_distance[state[i]];
    }
    
    return time_difference;
 
}

double time_difference_rotate(vector< vector<double> > & route_time, std::vector<node_ID> & state, node_ID i, node_ID j, node_ID k){
    
    double time_difference = 0;
    
    //take the depot into consideration
    
    if(i == 0){
        
        time_difference -= globalVar.depot_distance[state[i]];
        time_difference += globalVar.depot_distance[state[j]];
    }
    
    
    //i is not the starting point
    else{
        time_difference -= route_time[state[i-1]][state[i]];
        time_difference += route_time[state[i-1]][state[j]];
        
    }
            
    time_difference -= route_time[state[j-1]][state[j]];       
    
    if(k <= state.size()-2){
        time_difference -= route_time[state[k]][state[k+1]];
        time_difference += route_time[state[j-1]][state[k+1]];
    }
    
    //k is the end point, take depot into consideration
    else{
        time_difference -= globalVar.depot_distance[state[k]];
        time_difference += globalVar.depot_distance[state[j-1]];
        
    }
    
    time_difference += route_time[state[k]][state[i]];
 
    
    return time_difference;
}


//*****************The core function:: Optimizer********************************

double Optimizer::Optimize(const TSP_Instance & instance, std::vector<node_ID> & result) {
//    
   globalVar.Tabu_List.clear();
   globalVar.Tabu_List_Corresponding_Value.clear();
    
    
    //First get the number of cities
    int n = static_cast<int>(instance.get_cities().size());
    
    assert(n > 0);
    
    assert(moves.size() > 0);
    
    
    //Set up the run time configuration
    Config config;
    
    
    
    //Set up the initial path
    config.state.resize(n);
    
    config.best_state.resize(n);
    
    
    //******************Parameter needs to test
   int tabu_tenure = sqrt(n);
    
    
    
//     Resize the TabuList's tabu_tenure
   globalVar.Tabu_List.resize(tabu_tenure);
   globalVar.Tabu_List_Corresponding_Value.resize(tabu_tenure, 0);
    
    
    
    //Put the node pattern into the state
    for(int index = 0; index < n;  index++){
        
        config.state[index] = instance.cities[index];
        config.best_state[index] = instance.cities[index];
        
    }

    //Calculate the initial energy, which is the tour length
    //Remember to add the start and end depot
    config.energy = calculate_tour_length(config.state);

    config.best_energy = config.energy;
    
    //Set up the initial temperature
    config.temp = cooling_action->initial_temp();
    

    std::mt19937 g({std::random_device{}()});
    
    //set up an initial distribution over the possible moves, range from 0 to moves index
    std::uniform_int_distribution<int> moveDist(0, static_cast<int> (moves.size() -1));
    
    //A uniform distribution for the acceptance probability, range from 0 to 1  
    std::uniform_real_distribution<double> uniformDist(0.0, 1.0);
    
    
    //set up the mover service, n means the number of nodes
    Optimizer::Move_Service* service = new Optimizer::Move_Service(n);   
    
    
    for(size_t i = 0; i < moves.size(); i++){
        
        moves[i]->set_Move_Service(service);
        
    }

    //The current proposal
    std::vector<node_ID> SA_proposal;
   std::vector<node_ID> Tabu_Proposal;
   double Tabu_best_energy;
   
    
    //start the optimization, outer loop is to adjust the temperature
    for(config.out_loop = 0; config.out_loop < outer_loop; config.out_loop++){
        
        //Determine the next temperature, if smaller than end........
        config.temp = cooling_action-> next_temp(config);
        
       int i = 0;
       int j = 0;
       
       i++;
       vector<int> best_neighbor = config.state;
       
       while (true){      
       /*********Tabu selection of best neighbors****************************/
          
           //moves[1] is swap, and proposal will be iterated to be the 
           //best neighbor. config is passed to change the current energy (config.energy))
         Tabu_best_energy = Tabu_Generate_the_best_neighbour(config.state, moves[1], config, best_neighbor);
  
         //if tabu best neighbor is better than current best energy, no need to run this loop;
         if(Tabu_best_energy < config.best_energy){

             config.energy =  Tabu_best_energy;
             
             config.state = best_neighbor;
             
             j++;             
             
           //renew the Tabu_List
           aspiration_Rule_Selection(best_neighbor, Tabu_best_energy);
          
         }
         
        else{
             cout << "not found" << endl;
             break;
        }

   }




        
        //Simulate the process of Markov Chain
        for(config.inner_loop = 0; config.inner_loop < inner_loop; config.inner_loop++){
            
            //proposal means the current result, which is not optimal
            SA_proposal = config.state;
             
            //propose a new neighbor according to some moves
            //choose a move
            int m = moveDist(g);
          
            
            //return the path difference
            const double delta = moves[m]->propose(SA_proposal);         
            
            //Get the energy of the new proposal
            const double energy = config.energy + delta;         
            
            /*******************************/
            assert(energy > 0);
             /***************************/
            
            //It means that the new path takes less time
            if (delta <= 0){
                
                //Accept the move
                config.state = SA_proposal;
                
                config.energy = energy;                
            
                //If new solution energy is better, renew the best energy and best state 
            if(energy < config.best_energy){
                    
                //This energy is better
                config.best_energy = energy;
                config.best_state = SA_proposal;                   
                }
            }
             else  {
                
                
                //Accept the proposal with certain probability
                
                double u = uniformDist(g);

                if(u <= std::exp(-0.8 /config.temp * delta)){
                    
                    config.state = SA_proposal;
                    config.energy = energy;
                    
                    
                }
                
             }
            
            
            /***********************Renew the Tabu_List************************/
            
            aspiration_Rule_Selection(config.state, config.energy);
          
        
        }
            
        
        
        auto current = std::chrono::high_resolution_clock::now();    
            
        auto alpha = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
        
        if(alpha.count() >= 0.95 * TIME_LIMIT)
          
            return config.best_energy;
         
    }
   
    DELETE_PTR(service);
    
    for(size_t i = 0; i < moves.size(); i++){
        
        moves[i]-> set_Move_Service(0);
    }
    
    result = config.best_state;

    return config.best_energy;
    
}

bool if_in_Tabu_List(vector<int> & current_solution){
    
    for(int i = 0; i < globalVar.Tabu_List.size(); i++){
        if(current_solution == globalVar.Tabu_List[i])
            return true;
    }
     
    return false;
}


int Optimizer:: return_Best_Solution_Index_in_Tabu_List(){
    
    auto smallest_iterator = std::min_element(globalVar.Tabu_List_Corresponding_Value.begin(), 
            globalVar.Tabu_List_Corresponding_Value.end()); 
    
    int smallest_index = std::distance(globalVar.Tabu_List_Corresponding_Value.begin(),
            smallest_iterator);
    
    
    return smallest_index;
}


void Optimizer:: routine_Replacement_of_Tabu_List_and_Energy(vector<int> & current_solution, double & current_energy){
    
    //cout << "Tabu_List size:" << globalVar.Tabu_List.size() << endl;
    
    //Do the routine replacement
    for(int index = globalVar.Tabu_List.size() - 2; index >= 0; index--){
        
        //Replace the old one with the solution right before itself
        globalVar.Tabu_List[index + 1] = globalVar.Tabu_List[index]; 
        globalVar.Tabu_List_Corresponding_Value[index + 1] = globalVar.Tabu_List_Corresponding_Value[index];
    }
    
    //Add the solution into Tabu_List
    globalVar.Tabu_List[0] = current_solution;
    globalVar.Tabu_List_Corresponding_Value[0] = current_energy;
    
}

 double Optimizer::Tabu_Generate_the_best_neighbour(vector<int> & current_solution, Move* & swap, Config & config, vector<int> & best_neighbor){

     /****We do not need to check the Tabu_List for generating best neighbor***/
    
    //Initialize the best energy of current path
    double best_energy = calculate_tour_length(current_solution);
    
    vector<int> proposal;
    
    /*******************The number 100 should be adjusted**************************/
    //We only pick 20 potential solutions
    for (int i = 0; i < 30; i++) {

        //Every time use the current solution to calculate neighbor
        proposal = current_solution;

        //Generate a new neighbor according to swap
        swap->propose(proposal);
     
        //If this leagal path energy is better
        if (calculate_tour_length(proposal) < best_energy) {

            //Replace the original neighbor with the better one
            best_energy = calculate_tour_length(proposal);

             best_neighbor = proposal;
        }

    }   
      return best_energy;
 }

//Used only for Tabu Part
void Optimizer:: aspiration_Rule_Selection(vector<int> & current_solution, double & current_energy){
    // if the potential best solution is better than the best, aspire it.
    if(current_energy < config.best_energy){
        return;
    }
    if the generated neighbor solution is acceptable according to
    //self tested parameter**********************************

   //Find if the solution is in the Tabu_List
   if(if_in_Tabu_List(current_solution) == true){
       //aspire the solution with best energy
       int smallest_index = return_Best_Solution_Index_in_Tabu_List();

       //Use the tabu-best solution to replace current solution
       current_solution = globalVar.Tabu_List[smallest_index];

       //Remember to return the current energy
       current_energy = globalVar.Tabu_List_Corresponding_Value[smallest_index];         
       

       //Reset the removed solution, set to none
       globalVar.Tabu_List[smallest_index] = {};       

       //and corresponding energy, set to -1
       globalVar.Tabu_List_Corresponding_Value[smallest_index] = 0;
       
       return;
       
   }
    //routine procedure, release the one first coming into the Tabu_List 
    routine_Replacement_of_Tabu_List_and_Energy(current_solution, current_energy);
    
    return;
    
}
