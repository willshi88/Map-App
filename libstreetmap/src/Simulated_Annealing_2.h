/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Simulated_Annealing.h
 * Author: zhan3795
 *
 * Created on March 31, 2018, 4:12 PM
 * 
 * Original Source: https://github.com/TobyPDE/simulated-annealing-tsp
 */

#include "m4.h"
#include "m1.h"
#include "m3.h"
#include "global.h"
#include <thread>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <random>
#include <fstream>
#define DELETE_PTR(p) if((p) != 0) {delete (p); (p) = 0;}
#define DELETE_PTRA(p) if((p) != 0) {delete[] (p); (p) = 0;}




typedef int route_ID;
typedef int delivery_ID;
typedef int segment_ID;
typedef int intersection_ID;
typedef int node_ID;

extern global globalVar;




double time_difference_chain_reverse_2(vector< vector<double> > & route_time, std::vector<node_ID> & state, node_ID i, node_ID j);

double time_difference_swap_2(vector< vector<double> > & route_time, std::vector<node_ID> & state, node_ID i, node_ID j);

double time_difference_rotate_2(vector< vector<double> > & route_time, std::vector<node_ID> & state, node_ID i, node_ID j, node_ID k);

double Simulated_Annealing_2(vector< vector <double> >& route_time, vector<node_ID>& current_node_path);


//This is the random generator to return a random city

class RanGen_2{
public:   
    std::mt19937 generator;
   
    std::uniform_int_distribution<unsigned> distribution;   
    RanGen_2(unsigned num_of_cities):
    generator({std::random_device{}()}), distribution(0, num_of_cities - 1){
        
    }
    
    unsigned generate_2(){
        return distribution(generator);
    }
    
};


class TSP_Instance_2{
    
public:   
    
    //Used to store the original node_id of each point
    std::vector<node_ID> cities;
    
    //the computed route time
    vector< vector<double> > route_time;
       
    //number of nodes
    int node_size;
    
    
    
    void createNew_2(vector< vector<double> > & route_time_, 
    int node_size_,  vector<node_ID> & nodePath_cur){
        cities = nodePath_cur;
        route_time = route_time_;

        node_size = node_size_;
 
   
    }
    
    
    
    

    
    const std:: vector<node_ID> & get_cities_2() const {
        return cities;
    }
    

    

};


double calculate_tour_length_2(const std::vector<node_ID> & tour);





class Optimizer_2{
public:    
   
    friend class SwapCityMove;
    
    
    class Config_2{
    public:
        //constructor innitialization
        Config_2(): temp(0), out_loop(0), inner_loop(0), energy(0),best_energy(0), 
                finish(false){}
       
        //The current temperature
        double temp;
        
        //The current out loop
        int out_loop;
        
        //The current inner loop
        int inner_loop;
        
        //The current energy
        double energy;
        
        //The currently best energy 
        double best_energy;
                
        //current state, which is used to store the index of pick up and drop off
        std::vector<node_ID> state;
        
        //The best state found ever        
        std::vector<node_ID> best_state;
        
        bool finish;
        
       
    };
    

    //This is used to define cooling procedure
    class Cooling_Action_2{
        
    public:
        virtual double next_temp_2(const Config_2 & config) const = 0;
        
        virtual double initial_temp_2() const = 0;
    

    };    
    
    
    
    
    //This class is used to allows random sampling of cities
    class Move_Service_2{
    public:
        
        
        std::mt19937 generator;
   
        std::uniform_int_distribution<int> distribution;
        
        
        Move_Service_2(unsigned num_of_cities):
        generator({std::random_device{}()}), distribution(0, num_of_cities -1){
        
    }
        
        unsigned sampling_2(){
            return distribution(generator);
        }
                
    };
    
    
    //This class conduct only a single move
    class Move_2{
    public:
      
        
        //Computes a random neighbor according to defined moving strategies
        virtual double propose_2 (std::vector<node_ID> & state) const = 0;

        
        //set the random service
        void set_Move_Service_2(Move_Service_2* _service){
            service = _service;
        }
        

        
    protected: 
        Move_Service_2* service;  
        
    };
                    
    
    
      //constructor
    
    Optimizer_2() :cooling_action(0), 
            outer_loop(100),
            inner_loop(1000),
            notification_cycle(250) {};
    
            
            //The cooling action
            Cooling_Action_2* cooling_action;
            
            //The number of outer iteration
            int outer_loop;
            
            //The number of inner iteration
            int inner_loop;
            
            //The notification cycle, which records every c iteration
            int notification_cycle;
                             
            
            double Optimize_2(const TSP_Instance_2 & instance, std::vector<node_ID> & result);             
            
            void add_move_2(Move_2* move){
                
                return moves.push_back(move);
            }
            
/***************Tabu search part for optimizing path selection*****************/            
//   
//           
//
//        
//            //Return the solution in Tabu_List with best energy
//            int return_Best_Solution_Index_in_Tabu_List_2();
//            
//            
//            //Routine process, remove the first-in solution and move each solution
//            //one index upward, then push the current solution into Tabu_List
//            void routine_Replacement_of_Tabu_List_and_Energy_2(vector<int> & current_solution, double & current_energy);
//            
//            
//            
//            
//            double Tabu_Generate_the_best_neighbour_2(vector<int> & current_solution, Move_2* & swap, Config_2 & config,  vector<int> & proposed_solution);
//            
//            
//            
//            //This is the function to decide whether a solution should be pushed
//            //into tabu list or whether an already tabu solution should be
//            //aspired for optimizing global solution            
//            void aspiration_Rule_Selection_2(vector<int> & current_solution, double & current_energy);
            
            
            
            
            
            
            
            
/******************************************************************************/           
            
//Store the possible moves according different neighbor-generating strategies           
private:      
    
            std::vector<Move_2*> moves;   
            
            
    
};


//Function to test whether the current solution is in the tabu list
//bool if_in_Tabu_List_2(vector<int> & current_solution);





class Geometric_Cooling_Action_2 : public Optimizer_2::Cooling_Action_2 {
    

public:    
       Geometric_Cooling_Action_2(double initial_temp_, double end_temp_, double alpha_) :
        
               Init_temp(initial_temp_),  
                end_temp(end_temp_), 
                        
                alpha(alpha_){}
        
        
        
        virtual double next_temp_2(const Optimizer_2::Config_2 & config) const {
            
            return std::max(config.temp * alpha, end_temp);
        }

        virtual double initial_temp_2() const{
            
            return Init_temp;
        }
    
private: 
    
        double Init_temp;
        
        double end_temp;
        
        //Decreasing constant
        double alpha;

                       
};










class ChainReverseMove_2 : public Optimizer_2::Move_2{
    
    
    //strategy to compute a random neighbour
    
    virtual double propose_2(std::vector<node_ID> & state) const {
                
        vector<int> ID_TO_INDEX(state.size(), 0);
        
        //Store the vector of pick_ID to drop_index
        for(unsigned n = 0; n < state.size(); n++){
            //If we find drop ID 
            if((state[n] %2) == 1){
                node_ID pick_ID = state[n] - 1;
                ID_TO_INDEX[pick_ID] = n;
            }
         }

        
        
        unsigned random1, random2;
        bool legal = false;
        
        while(!legal){
            
            
            random1 = service -> sampling_2();
            random2 = service -> sampling_2();
            


            //We need to make sure random1 < random2, and random2 is at least 
            //2 bigger than random 1, and they are index in state
            
            if(random1 >= random2)
                continue;
            
            legal = true;
            
            for(unsigned index = random1; index <= random2; index++){
                node_ID node_id = state[index];
                
                //If this is a pick up point
                if((node_id % 2) == 0){
                    
                    unsigned drop_Index = ID_TO_INDEX[node_id];
                    
                    
                    //Can be optimized
                    if(drop_Index <= random2){
                        
                        legal = false;
                        break;
                    }                       
                    
                }                
            }        
            
            
//            vector<int> potential_solution = state;
//            
//            reverse(potential_solution.begin() + random1, potential_solution.begin() + random2 + 1);
//            
//            if (if_in_Tabu_List_2(potential_solution) == true)
//                legal == false;
//                
                
                
            
            
            
            
        }         
       
        double travel_distance_temp = time_difference_chain_reverse_2(globalVar.travel_distance, state, random1, random2);              
        //********************Exclude depot cases****************************
       
//         cout << "before reverse:" << endl;
//        cout << "node1: " << random1 << "  " << "node2: " << random2;
//        cout << endl;
//        
        std::reverse(state.begin() + random1, state.begin() + random2 + 1);
        return travel_distance_temp;
        
    }
    
};







class SwapCityMove_2 : public Optimizer_2::Move_2 {
    
public:
    
    virtual double propose_2(std::vector<node_ID> & state) const{
    vector<int> ID_TO_INDEX(state.size(), 0);
        
        for(unsigned n = 0; n < state.size(); n++){
            //If we find drop ID 
            if((state[n] %2) == 1){
                node_ID pick_ID = state[n] - 1;
                ID_TO_INDEX[pick_ID] = n;
            }
            else{
                node_ID drop_ID = state[n] +1;
                ID_TO_INDEX [drop_ID] = n;
            }
         }
        
        unsigned random1, random2;
        bool legal = false;
        
    
        while(!legal){
            
            random1 = service->sampling_2();
            random2 = service->sampling_2();
            
            //guarantee random1 smaller than random2
            //Do not ever change the first point
            if(random1 >= random2)
                continue;
            
            
            
            
            //if this point is pick up
            if(state[random1] % 2 == 0){
               
                node_ID pick_ID = state[random1];
            
            unsigned drop_Index = ID_TO_INDEX[pick_ID];
            
            
            
            if(drop_Index <= random2)
                continue;
            
            }
            
            //this point is drop off point
            if(state[random2] % 2 == 1){
                node_ID drop_ID = state[random2];
                
                unsigned pick_Index = ID_TO_INDEX[drop_ID];
                
                if(pick_Index >= random1 && pick_Index < random2){
                    continue;
                }                
                
            }
            
//            vector<int> potential_solution = state;
//            swap(potential_solution[random1], potential_solution[random2]);
//            
//            //If in Tabu_List, not allowed
//            if (if_in_Tabu_List_2(potential_solution) != true)
            
                legal = true;
        }
        
        
        
        
        
        
        
        
        
        
        
        
        double travel_distance_temp = time_difference_swap_2(globalVar.travel_distance, state, random1, random2);
        
        
        
        //exclude the depot 
//         cout << "before swap:" << endl;
//        cout << "node1: " << random1 << "  " << "node2: " << random2;
//        cout << endl;
        
        std::swap(state[random1], state[random2]);
        
        return travel_distance_temp;
     }
    
    
    
};


class RotateCityMove_2 : public Optimizer_2::Move_2{
public:
    
    virtual double propose_2(std::vector<node_ID> & state) const{
    vector<int> ID_TO_INDEX(state.size(), 0);
        
        for(unsigned n = 0; n < state.size(); n++){
            //If we find drop ID 
            if((state[n] %2) == 1){
                node_ID pick_ID = state[n] - 1;
                ID_TO_INDEX[pick_ID] = n;
            }
         }       
    

        bool legal = false;
        vector<unsigned> index_vector;
    
        while(!legal){
           std::vector<unsigned> random({service->sampling_2(),service->sampling_2(),service->sampling_2()});
           std::sort(random.begin(), random.end());
           
           //if any of two random number are same, continue
           if(random[0] == random[1] || random[1] == random[2] || random[0] == random[2])
               continue;
            
            
            legal = true;
           
           
           for(unsigned index = random[0]; index < random[1]; index++){
               node_ID node_id = state[index];
               
               //if this is a pick up point
               if(node_id % 2 == 0){
               //check if previous node exists a pick up point
                   unsigned drop_Index = ID_TO_INDEX[node_id];
                   
                   //This is to certify that the drop off is not in the second segment
                   if(drop_Index <= random[2] && drop_Index >= random[1]){
                       legal = false;
                       break;
                   }
               }                            
           }
            
           
//           vector<int> potential_solution = state;
//           rotate(potential_solution.begin()+random[0], potential_solution.begin()
//                   + random[1], potential_solution.begin() + random[2] + 1);
//            
//           
//           
//            //If in Tabu_List, not allowed
//            if (if_in_Tabu_List_2(potential_solution) == true)
//                legal == false;
//           
           
           
           if(legal)
               index_vector = random;
        }
    
        
       
        
        
        //**********exclude depot***********************************
        
        //********************************************make the time to compute the time to be global
       
//        cout << "before rotate:" << endl;
//        
//        
//        for(int i = 0; i < index_vector.size(); i++){
//            cout << "node" << i << ": " << index_vector[i]<< "  ";
//            
//        }
//        
//        cout << endl;
        
        
    
        
        
        
        double travel_distance_temp = time_difference_rotate_2(globalVar.travel_distance, state, index_vector[0], index_vector[1], index_vector[2]);

        std::rotate(state.begin()+index_vector[0], state.begin() + index_vector[1], state.begin() + index_vector[2] + 1);
        
       
        
        return travel_distance_temp;
    }
  
    
};


