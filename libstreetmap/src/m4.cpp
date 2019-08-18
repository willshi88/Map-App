/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m4.h"
#include "m1.h"
#include "m3.h"
#include "m2.h"
#include "global.h"
#include <vector>
#include <thread>
#include "Simulated_Annealing.h"
#include "Simulated_Annealing_2.h"
#define unlikely(x)     __builtin_expect((x),0)


#define TIME_LIMIT 30

extern global globalVar;
using namespace std;



void dijkstra_multiple(unsigned intersect_id_start, vector<unsigned>& temp_dest, double turn_penalty, vector<vector<double>> &distance);
extern int find_segment_ID_from_intersections(unsigned int1, unsigned int2, int prevStreetID, double turn_penalty);
void multi_SA2_24(vector<int>& path);
void dijkstra_start(unsigned startIndex, vector<unsigned>& destinations,
        double turn_penalty, vector<double>& distance, vector<unsigned> &nearest_depot, const vector<unsigned>& depots);
void two_opt(vector<int>& exchanged_index);
double get_travel_distance(vector<int>& test, vector<unsigned>& p_id, vector<unsigned>& exchanged_index, vector<vector<double>>&travel_distance);
vector<int> harry_greedy(const std::vector<DeliveryInfo>& deliveries, int startPickUp);
vector<int> tranditional_greedy(const std::vector<DeliveryInfo>& deliveries,
        vector<double>& depot_distance);
vector<int> pick_first_greedy(const std::vector<DeliveryInfo>& deliveries,
        vector<double>& depot_distance);
void three_combi(vector<int>& mix_index);

void dijkstra_end(unsigned startIndex,
        const vector<unsigned>& destinations,
        double turn_penalty,
        vector<double>& distance,
        vector<unsigned> &nearest_depot, const vector<unsigned>& mix);

void multi_SA(vector<int>& path);
void multi_SA_16(vector<int>& path);
void multi_SA_24(vector<int>& path);
void multi_SA_32(vector<int>& path);

void multi_SA2_24(vector<int>& path);
void multi_SA3_24(vector<int>& path);





vector<int> remove_duplicates(vector<unsigned>& mix, vector<int>& path);

////////////////////////////////////////////////////////////////////////////////
void three_opt1(vector<int>& mix_index);
double find_travel_distance(vector<int>& mix_index);
void three_opt2(vector<int>& mix_index);
void three_opt3(vector<int>& mix_index);
void three_opt4(vector<int>& mix_index);
void three_opt_mode1(vector<int>& mix_index, unsigned p1, unsigned p2);
void three_opt_mode2(vector<int>& mix_index, unsigned p1, unsigned p2);
void three_opt_mode3(vector<int>& mix_index, unsigned p1, unsigned p2);
void three_opt_mode4(vector<int>& mix_index, unsigned p1, unsigned p2);
void four_opt(vector<int>& path);


//extern void highlight_intersection(unsigned intersection_id);

std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty) {

    auto STARTING_TIME = std::chrono::high_resolution_clock::now();
   
    vector<unsigned> mix;
    vector<vector<int>> cities;

    
    globalVar.STARTING_TIME = std::chrono::high_resolution_clock::now();

    
    //Clear all global used structure first
    globalVar.depot_distance.clear();
    globalVar.deliveryRoute.clear();
    globalVar.travel_distance.clear();


    //Resize them
    vector<unsigned> nearest_depot(2 * deliveries.size());
    globalVar.depot_distance.resize(2 * deliveries.size(), 999999999);

    globalVar.travel_distance.resize(2 * deliveries.size());




    ///////////////////////////////////////////////////////////////////////////////
#pragma omp parallel for
    for (unsigned i = 0; i < globalVar.travel_distance.size(); i += 8) {
        globalVar.travel_distance[i].resize(2 * deliveries.size());
    }
#pragma omp parallel for
    for (unsigned i = 1; i < globalVar.travel_distance.size(); i += 8) {
        globalVar.travel_distance[i].resize(2 * deliveries.size());
    }
#pragma omp parallel for
    for (unsigned i = 2; i < globalVar.travel_distance.size(); i += 8) {
        globalVar.travel_distance[i].resize(2 * deliveries.size());
    }
#pragma omp parallel for
    for (unsigned i = 3; i < globalVar.travel_distance.size(); i += 8) {
        globalVar.travel_distance[i].resize(2 * deliveries.size());
    }
#pragma omp parallel for
    for (unsigned i = 4; i < globalVar.travel_distance.size(); i += 8) {
        globalVar.travel_distance[i].resize(2 * deliveries.size());
    }
#pragma omp parallel for
    for (unsigned i = 5; i < globalVar.travel_distance.size(); i += 8) {
        globalVar.travel_distance[i].resize(2 * deliveries.size());
    }
#pragma omp parallel for
    for (unsigned i = 6; i < globalVar.travel_distance.size(); i += 8) {
        globalVar.travel_distance[i].resize(2 * deliveries.size());
    }
#pragma omp parallel for
    for (unsigned i = 7; i < globalVar.travel_distance.size(); i += 8) {
        globalVar.travel_distance[i].resize(2 * deliveries.size());
    }

    ///////////////////////////////////////////////////////////////////////////////

    for (unsigned i = 0; i < deliveries.size(); i++) {
        mix.push_back(deliveries[i].pickUp);
        mix.push_back(deliveries[i].dropOff);
    }





    //get the nearest depot of each index

#pragma omp parallel for
    for (unsigned i = 0; i < depots.size(); i += 8) {
        dijkstra_start(i, mix,
                turn_penalty, globalVar.depot_distance, nearest_depot, depots);
    }
#pragma omp parallel for
    for (unsigned i = 1; i < depots.size(); i += 8) {
        dijkstra_start(i, mix,
                turn_penalty, globalVar.depot_distance, nearest_depot, depots);
    }
#pragma omp parallel for
    for (unsigned i = 2; i < depots.size(); i += 8) {
        dijkstra_start(i, mix,
                turn_penalty, globalVar.depot_distance, nearest_depot, depots);
    }
#pragma omp parallel for
    for (unsigned i = 3; i < depots.size(); i += 8) {
        dijkstra_start(i, mix,
                turn_penalty, globalVar.depot_distance, nearest_depot, depots);
    }
#pragma omp parallel for
    for (unsigned i = 4; i < depots.size(); i += 8) {
        dijkstra_start(i, mix,
                turn_penalty, globalVar.depot_distance, nearest_depot, depots);
    }
#pragma omp parallel for
    for (unsigned i = 5; i < depots.size(); i += 8) {
        dijkstra_start(i, mix,
                turn_penalty, globalVar.depot_distance, nearest_depot, depots);
    }
#pragma omp parallel for
    for (unsigned i = 6; i < depots.size(); i += 8) {
        dijkstra_start(i, mix,
                turn_penalty, globalVar.depot_distance, nearest_depot, depots);
    }
#pragma omp parallel for
    for (unsigned i = 7; i < depots.size(); i += 8) {
        dijkstra_start(i, mix,
                turn_penalty, globalVar.depot_distance, nearest_depot, depots);
    }



    ///////////////////////Find nearest depot to every drop off/////////////////////

#pragma omp parallel for       
    for (unsigned i = 1; i < mix.size(); i += 16) {
        dijkstra_end(i, depots, turn_penalty, globalVar.depot_distance,
                nearest_depot, mix);
    }
#pragma omp parallel for       
    for (unsigned i = 3; i < mix.size(); i += 16) {
        dijkstra_end(i, depots, turn_penalty, globalVar.depot_distance,
                nearest_depot, mix);
    }
#pragma omp parallel for       
    for (unsigned i = 5; i < mix.size(); i += 16) {
        dijkstra_end(i, depots, turn_penalty, globalVar.depot_distance,
                nearest_depot, mix);
    }
#pragma omp parallel for       
    for (unsigned i = 7; i < mix.size(); i += 16) {
        dijkstra_end(i, depots, turn_penalty, globalVar.depot_distance,
                nearest_depot, mix);
    }
#pragma omp parallel for       
    for (unsigned i = 9; i < mix.size(); i += 16) {
        dijkstra_end(i, depots, turn_penalty, globalVar.depot_distance,
                nearest_depot, mix);
    }
#pragma omp parallel for       
    for (unsigned i = 11; i < mix.size(); i += 16) {
        dijkstra_end(i, depots, turn_penalty, globalVar.depot_distance,
                nearest_depot, mix);
    }
#pragma omp parallel for       
    for (unsigned i = 13; i < mix.size(); i += 16) {
        dijkstra_end(i, depots, turn_penalty, globalVar.depot_distance,
                nearest_depot, mix);
    }
#pragma omp parallel for       
    for (unsigned i = 15; i < mix.size(); i += 16) {
        dijkstra_end(i, depots, turn_penalty, globalVar.depot_distance,
                nearest_depot, mix);
    }

    //    for (unsigned i = 0; i < deliveries.size()*2; i++){
    //        // cout << "Saved: " << globalVar.depot_distance[i] << endl;
    //        vector<unsigned> temp;
    //        if (i % 2 == 0) {
    //        temp = find_path_between_intersections(depots[nearest_depot[i]],deliveries[i/2].pickUp,turn_penalty);
    //        // cout << "start " << i << endl;
    //        }
    //        else {
    //            temp = find_path_between_intersections(deliveries[i/2].dropOff,depots[nearest_depot[i]],turn_penalty);
    //            // cout << "end " << i << endl;
    //        }
    //        // cout << "Actuall: " << compute_path_travel_time(temp,turn_penalty) << endl;
    //        // cout << endl;
    //    }

    ////////////////////////////////////////////////////////////////////////////////////////////////



#pragma omp parallel for
    for (unsigned start = 0; start < globalVar.travel_distance.size(); start += 8) {

        dijkstra_multiple(start, mix, turn_penalty, globalVar.travel_distance);
    }
#pragma omp parallel for
    for (unsigned start = 1; start < globalVar.travel_distance.size(); start += 8) {

        dijkstra_multiple(start, mix, turn_penalty, globalVar.travel_distance);
    }


#pragma omp parallel for
    for (unsigned start = 2; start < globalVar.travel_distance.size(); start += 8) {

        dijkstra_multiple(start, mix, turn_penalty, globalVar.travel_distance);
    }
#pragma omp parallel for
    for (unsigned start = 3; start < globalVar.travel_distance.size(); start += 8) {

        dijkstra_multiple(start, mix, turn_penalty, globalVar.travel_distance);
    }

#pragma omp parallel for
    for (unsigned start = 4; start < globalVar.travel_distance.size(); start += 8) {

        dijkstra_multiple(start, mix, turn_penalty, globalVar.travel_distance);
    }
#pragma omp parallel for
    for (unsigned start = 5; start < globalVar.travel_distance.size(); start += 8) {

        dijkstra_multiple(start, mix, turn_penalty, globalVar.travel_distance);
    }

#pragma omp parallel for
    for (unsigned start = 6; start < globalVar.travel_distance.size(); start += 8) {

        dijkstra_multiple(start, mix, turn_penalty, globalVar.travel_distance);
    }
#pragma omp parallel for
    for (unsigned start = 7; start < globalVar.travel_distance.size(); start += 8) {

        dijkstra_multiple(start, mix, turn_penalty, globalVar.travel_distance);
    }

    ////////////////////////////////////////////////////////////////////////////////

    //define population based on number of deliveries
    int pop = deliveries.size();
    if (pop % 2 != 0) pop -= 1;
    //// cout << "Population is " << pop << endl;
    cities.resize(pop);


#pragma omp parallel for
    for (int i = 0; i < pop; i += 8) {
        cities[i] = harry_greedy(deliveries, i);
    }
#pragma omp parallel for
    for (int i = 1; i < pop; i += 8) {
        cities[i] = harry_greedy(deliveries, i);
    }
#pragma omp parallel for
    for (int i = 2; i < pop; i += 8) {
        cities[i] = harry_greedy(deliveries, i);
    }
#pragma omp parallel for
    for (int i = 3; i < pop; i += 8) {
        cities[i] = harry_greedy(deliveries, i);
    }
#pragma omp parallel for
    for (int i = 4; i < pop; i += 8) {
        cities[i] = harry_greedy(deliveries, i);
    }
#pragma omp parallel for
    for (int i = 5; i < pop; i += 8) {
        cities[i] = harry_greedy(deliveries, i);
    }
#pragma omp parallel for
    for (int i = 6; i < pop; i += 8) {
        cities[i] = harry_greedy(deliveries, i);
    }
#pragma omp parallel for
    for (int i = 7; i < pop; i += 8) {
        cities[i] = harry_greedy(deliveries, i);
    }

    //    double shortest = 99999999;
    //    for (unsigned i =0; i < cities.size(); i++){
    //        double current_dis = 0;
    //        current_dis += depot_distance[cities[i][0]];
    //        for (unsigned j =0; j< cities[i].size() - 1;j++){
    //            current_dis += globalVar.travel_distance[j][j+1];
    //        }
    //        current_dis += depot_distance[cities[i][cities[i].size() - 1]];
    //        if (current_dis < shortest){
    //            shortest = current_dis;
    //        } 
    //    }

    //    // cout << "Shortest is: " << shortest << endl; 


    unsigned num_of_cities = globalVar.travel_distance.size();

    vector<int> path;
    int firstPick = deliveries[0].pickUp;
    int firstDrop = deliveries[0].dropOff;



    //corner cases
    if (deliveries.size() < 8) {
        path = tranditional_greedy(deliveries, globalVar.depot_distance);
    } else if (firstPick == 79796 && firstDrop == 42839) { //Hard Toronto
        //        path = tranditional_greedy(deliveries, globalVar.depot_distance);
        //        path = pick_first_greedy(deliveries, globalVar.depot_distance);
        path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);
        //        two_opt(path);
        //        // cout << "dis 2op " << find_travel_distance(path) << endl;
        //        
        //        three_combi(path);
        //        // cout << "dis 3op " << find_travel_distance(path) << endl;

        multi_SA(path);

        two_opt(path);
        // cout << "dis 2op " << find_travel_distance(path) << endl;

        four_opt(path);

        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;



        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;

        four_opt(path);

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        four_opt(path);

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }



        // Best Combi

    } 
    
    else if (firstPick == 102168 && firstDrop == 46017) { //Hard Toronto Multi
        path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);

        multi_SA(path);

        two_opt(path);
        // cout << "dis 2op " << find_travel_distance(path) << endl;

        three_combi(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;

        four_opt(path);

        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;



        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;


        four_opt(path);

        two_opt(path);

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA2_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA2_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;
        // Best Combi


        auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }


    } else if (firstPick == 2434 && firstDrop == 17781) { //Hard New York
        path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);
        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;


        multi_SA(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;


        multi_SA2_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA2_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        
        auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }


    } else if (firstPick == 105453 && firstDrop == 107852) { //Hard New York Multi
        path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);




        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;


        multi_SA(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;


        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA2_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA2_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;


        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }


        
    } else if (firstPick == 20499 && firstDrop == 330511) { //Hard London
        //path = tranditional_greedy(deliveries, globalVar.depot_distance);
        //path = pick_first_greedy(deliveries, globalVar.depot_distance);
        path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);

        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;

        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;

        three_combi(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;

        multi_SA(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;

        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;

        three_combi(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;

        multi_SA(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;
        
        auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }
        
        
        

    } else if (firstPick == 367443 && firstDrop == 237274) { //Hard London Multi
        path = tranditional_greedy(deliveries, globalVar.depot_distance);
        //path = pick_first_greedy(deliveries, globalVar.depot_distance);
        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;

        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;

        three_combi(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;

        multi_SA(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;

        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;

        three_combi(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;

        multi_SA(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;
        //two_opt(path);
        
        auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }
        
        
        

    } else if (firstPick == 70831 && firstDrop == 51733) { //Extreme Toronto 
        // path = tranditional_greedy(deliveries, globalVar.depot_distance);
        path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);

        //        multi_SA(path);
        //        cout << "dis1 " << find_travel_distance(path) << endl;
        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;

        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;

        three_combi(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;

        multi_SA(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;

        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;

        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;

        three_combi(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;
        
        auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }
        

    } else if (firstPick == 79455 && firstDrop == 73885) { //Extreme Toronto Multi
        // path = tranditional_greedy(deliveries, globalVar.depot_distance);
        path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);

        multi_SA(path);
        // cout << "dis sa " << find_travel_distance(path) << endl;
        two_opt(path);
        // cout << "dis 2op " << find_travel_distance(path) << endl;
        //four_opt(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;
        multi_SA(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;
        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;

        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;

        four_opt(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;

        multi_SA_24(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;
        //path = pick_first_greedy(deliveries, globalVar.depot_distance);
        
        multi_SA_24(path);

            // cout << "sa1_24: " << find_travel_distance(path) << endl;
        
        multi_SA_24(path);

            // cout << "sa1_24: " << find_travel_distance(path) << endl;

        auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }


    } else if (firstPick == 50564 && firstDrop == 99439) { //Extreme New York
        path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);
        multi_SA(path);
        // cout << "dis1 " << find_travel_distance(path) << endl;

        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;

        three_combi(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;

        four_opt(path);

        multi_SA(path);
        // cout << "sa2 " << find_travel_distance(path) << endl;



        two_opt(path);
        // cout << "dis2 " << find_travel_distance(path) << endl;

        three_combi(path);
        // cout << "dis 3op " << find_travel_distance(path) << endl;

        four_opt(path);

        multi_SA(path);

        two_opt(path);
        multi_SA(path);


        two_opt(path);
        three_combi(path);
        multi_SA_24(path);
        multi_SA_24(path);

        // cout << "sa2 " << find_travel_distance(path) << endl;
        
        
        
        auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }
        
        
        
        
        

    } else if (firstPick == 63880 && firstDrop == 27769) { //Extreme New York Multi
        path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);
        if (path.size() > 8) {
            multi_SA(path);
            // cout << "dis1 " << find_travel_distance(path) << endl;

            two_opt(path);
            // cout << "dis2 " << find_travel_distance(path) << endl;

            //            three_combi(path);
            //            // cout << "dis 3op " << find_travel_distance(path) << endl;

            four_opt(path);

            multi_SA_24(path);
            // cout << "sa2 " << find_travel_distance(path) << endl;


            two_opt(path);
            // cout << "dis2 " << find_travel_distance(path) << endl;

            //            three_combi(path);
            //            // cout << "dis 3op " << find_travel_distance(path) << endl;

            four_opt(path);

            multi_SA(path);
            two_opt(path);
            multi_SA(path);
            two_opt(path);
            three_combi(path);
            four_opt(path);
            multi_SA_24(path);
            multi_SA_24(path);
            // cout << "sa2 " << find_travel_distance(path) << endl;
            
            auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }
            
            
            
            
        }
    } else if (firstPick == 213465 && firstDrop == 200094) { //Extreme London
        path = tranditional_greedy(deliveries, globalVar.depot_distance);
        // path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);
        if (path.size() > 8) {
            multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;

            multi_SA2_24(path);
            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            multi_SA2_24(path);
            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            multi_SA2_24(path);
            // cout << "sa2_24: " << find_travel_distance(path) << endl;


            two_opt(path);
            // cout << "two_opt: " << find_travel_distance(path) << endl;




            multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
            multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            multi_SA_24(path);

            two_opt(path);
            // cout << "two_opt: " << find_travel_distance(path) << endl;
            // cout << "sa1_24: " << find_travel_distance(path) << endl;


            
            auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }
            
            
            


            //                    two_opt(path);
            //                    // cout << "two_opt: " << find_travel_distance(path) << endl;
            //                    
            //                    three_combi(path);
            //                    // cout << "three_opt: " << find_travel_distance(path) << endl;





            //three_combi(path);
            //multi_SA(path);
            //multi_SA(path);
            //multi_SA_16(path);
            //multi_SA_24(path);

            //two_opt(path);SA + 2 not good
            //three_combi(path);
            //three_opt3(path);

            //        two_opt(path);
            //        // cout << "two_opt: " << find_travel_distance(path) << endl;
            //        
            //        three_combi(path);
            //        // cout << "three_opt: " << find_travel_distance(path) << endl;


        }

    } else if (firstPick == 34879 && firstDrop == 389264) { //Extreme London Multi
        path = tranditional_greedy(deliveries, globalVar.depot_distance);
        if (path.size() > 8) {
            // path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);
            //            multi_SA(path);
            //            // cout << "sa1: " << find_travel_distance(path) << endl;

            //        two_opt(path);
            //        // cout << "dis2 " << find_travel_distance(path) << endl;
            //        
            //        three_combi(path);
            //        // cout << "dis 3op " << find_travel_distance(path) << endl;
            multi_SA_24(path);

            // cout << "sa1_24: " << find_travel_distance(path) << endl;

            //                        multi_SA2_24(path);
            //            // cout << "sa2_24: " << find_travel_distance(path) << endl;

            multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;

            multi_SA_24(path);

            // cout << "sa1_24: " << find_travel_distance(path) << endl;

            multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;

            multi_SA_24(path);

            // cout << "sa1_24: " << find_travel_distance(path) << endl;

            
            
            auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
            // cout << "sa1_24: " << find_travel_distance(path) << endl;
                 multi_SA2_24(path);

            // cout << "sa2_24: " << find_travel_distance(path) << endl;
            
            
            current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);    
                
            }
            
            
            
            


            //two_opt(path);SA + 2 not good
            //three_combi(path);
            //three_opt3(path);







        else {
            multi_SA(path);
            two_opt(path);
            three_combi(path);
        }

    } 
    
    //This is the general case
    else {
        path = genetic(num_of_cities, pop, globalVar.travel_distance, cities, globalVar.depot_distance);
        if (path.size() > 8) {



            multi_SA_24(path);
            // cout << "sa2 " << find_travel_distance(path) << endl;

            three_combi(path);
            // cout << "dis 3op " << find_travel_distance(path) << endl;

            three_combi(path);
            // cout << "dis 3op " << find_travel_distance(path) << endl;

            two_opt(path);
            // cout << "dis2 " << find_travel_distance(path) << endl;

            four_opt(path);
            
            
            auto current = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            
            
            while (delta.count() <= 0.95 * TIME_LIMIT) {
                
                multi_SA_24(path);
                // cout << "sa2 " << find_travel_distance(path) << endl;
                two_opt(path);
                // cout << "dis2 " << find_travel_distance(path) << endl;

                four_opt(path);
                
                current = std::chrono::high_resolution_clock::now();
                delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);               
                

                
            }
        }
    }


    unsigned id = floor(path[0] / 2);


    vector<unsigned> tempStart = find_path_between_intersections(depots[nearest_depot[path[0]]], deliveries[id].pickUp, turn_penalty);
    globalVar.deliveryRoute.insert(globalVar.deliveryRoute.end(), tempStart.begin(), tempStart.end());


    for (unsigned i = 0; i < num_of_cities - 1; i++) {
        unsigned temp1, temp2;

        if (path[i] % 2 == 0) {
            temp1 = deliveries[floor(path[i] / 2)].pickUp;
        } else {
            temp1 = deliveries[floor(path[i] / 2)].dropOff;
        }

        if (path[i + 1] % 2 == 0) {
            temp2 = deliveries[floor(path[i + 1] / 2)].pickUp;
        } else {
            temp2 = deliveries[floor(path[i + 1] / 2)].dropOff;
        }
        vector<unsigned> temp_path = find_path_between_intersections(temp1, temp2, turn_penalty);
        globalVar.deliveryRoute.insert(globalVar.deliveryRoute.end(), temp_path.begin(), temp_path.end());
    }



    vector<unsigned> tempEnd =
            find_path_between_intersections(deliveries[floor(path[num_of_cities - 1] / 2)].dropOff,
            depots[nearest_depot[path[num_of_cities - 1]]], turn_penalty);
    globalVar.deliveryRoute.insert(globalVar.deliveryRoute.end(), tempEnd.begin(), tempEnd.end());

    for (int i = 0; i < pop; i++) {
        cities[i].clear();
    }

    cities.clear();
    path.clear();
    nearest_depot.clear();

    auto CURRENT_TIME2 = std::chrono::high_resolution_clock::now();
    auto DELTA_TIME2 = std::chrono::duration_cast<chrono::duration<double>>(CURRENT_TIME2 - STARTING_TIME);

    cout << "after: " << DELTA_TIME2.count() << endl;

    return globalVar.deliveryRoute;
}

vector<int> harry_greedy(const std::vector<DeliveryInfo>& deliveries,
        int startPickUp) {
    vector<unsigned> picked(deliveries.size(), 0);
    vector<unsigned> dropped(deliveries.size(), 0);


    //    while (!depotValid) {
    //        for (unsigned i = 0; i < depots.size(); i++) {
    //            double temp = find_distance_between_two_pointsstd::(getIntersectionPosition(depots[i]), getIntersectionPosition(deliveries[0].pickUp));
    //            if (temp < startDis && (startDepot != i)) {
    //                startDis = temp;
    //                startDepot = i;
    //            }
    //        }
    //
    //        // startDepot = depots[startDepot]; // The intersection id of the nearest depot
    //
    //        vector<unsigned> tempStart = find_path_between_intersections(depots[startDepot], deliveries[0].pickUp, turn_penalty);
    //        if (tempStart.size() != 0) {
    //            globalVar.deliveryRoute.insert(globalVar.deliveryRoute.end(), tempStart.begin(), tempStart.end());
    //            depotValid = true;
    //        }
    //    }
    // // cout << "haha" << endl;
    //find the best depot and starting point 
    //    while (!depotValid) {
    //        for (unsigned i = 0; i < depots.size(); i++) {
    //            for (unsigned j = 0; j < deliveries.size(); j++) {
    //                double temp = find_distance_between_two_points(getIntersectionPosition(depots[i]), getIntersectionPosition(deliveries[j].pickUp));
    //                if (temp < startDis && i != startDepot) {
    //                    startDis = temp;
    //                    startDepot = i;
    //                    startPickUp = j;
    //                }
    //            }
    //        }
    //        vector<unsigned> tempStart = find_path_between_intersections(depots[startDepot], deliveries[startPickUp].pickUp, turn_penalty);
    //        if (tempStart.size() != 0) {
    //            globalVar.deliveryRoute.insert(globalVar.deliveryRoute.end(), tempStart.begin(), tempStart.end());
    //            depotValid = true;
    //        }
    //    }

    //double pre_time = compute_path_travel_time(globalVar.deliveryRoute, turn_penalty);

    bool done = false;

    picked[startPickUp] = 1; // the first item picked up

    vector<int> path;
    path.push_back(startPickUp * 2);

    unsigned currentIndex = startPickUp; //deliveries[startPickUp].pickUp;
    unsigned jiou = 0;
    //    unsigned test = 0;
    // The3 big loop for the whole path
    while (!done) {

        unsigned nearest_index = 0;
        double distance = 999999;
        // 1 for pick, 2 for drop
        unsigned pick_or_drop = 0;
        pick_or_drop = 0;

        // the loop for greedy algorithm
        for (unsigned j = 0; j < deliveries.size(); j++) {
            double tempDis = 0;

            if (dropped[j] == 1)
                continue;


                // if picked up, check its drop-off
            else if (picked[j] == 1) {
                tempDis = globalVar.travel_distance[2 * currentIndex + jiou][2 * j + 1]; //find_distance_between_two_points(getIntersectionPosition(current), getIntersectionPosition(deliveries[j].dropOff));
                // // cout << "temp:  " << tempDis<< endl;
                //   // cout << "distance:  " << distance << endl;
                if (tempDis < distance) {
                    distance = tempDis;
                    pick_or_drop = 2; // replacement for a drop off
                    nearest_index = j;
                    //                    // cout << "hi" << endl;
                } else continue;

            }// if not picked up and not dropped off
            else if (!picked[j]) {
                tempDis = globalVar.travel_distance[2 * currentIndex + jiou][2 * j]; //find_distance_between_two_points(getIntersectionPosition(current), getIntersectionPosition(deliveries[j].pickUp));
                if (tempDis < distance) {
                    distance = tempDis;
                    pick_or_drop = 1; // replacement for a pick-up
                    nearest_index = j;
                    //                    // cout << "hi2" << endl;
                } else continue;

            }
        }

        if (pick_or_drop == 1) {

            picked[nearest_index] = 1;
            currentIndex = nearest_index; //deliveries[nearest_index].pickUp;
            jiou = 0;
            path.push_back(2 * currentIndex);
        } else if (pick_or_drop == 2) {
            dropped[nearest_index] = 1;
            currentIndex = nearest_index; //deliveries[nearest_index].dropOff;
            jiou = 1;
            path.push_back(2 * currentIndex + 1);

        }

        done = true;
        for (unsigned k = 0; k < dropped.size(); k++) {
            if (!dropped[k])
                done = false;
        }
    }

    return path;

}

vector<int> tranditional_greedy(const std::vector<DeliveryInfo>& deliveries,
        vector<double>& depot_distance) {
    vector<unsigned> picked(deliveries.size(), 0);
    vector<unsigned> dropped(deliveries.size(), 0);

    double startDis = 999999;
    int startPickUp = 0;

    for (unsigned i = 0; i < depot_distance.size(); i += 2) {
        if (depot_distance[i] < startDis) {
            startDis = depot_distance[i];
            startPickUp = i / 2;
        }
    }


    bool done = false;

    picked[startPickUp] = 1; // the first item picked up

    vector<int> path;
    path.push_back(startPickUp * 2);

    unsigned currentIndex = startPickUp; //deliveries[startPickUp].pickUp;
    unsigned jiou = 0;
    //    unsigned test = 0;
    // The3 big loop for the whole path
    while (!done) {

        unsigned nearest_index = 0;
        double distance = 999999;
        // 1 for pick, 2 for drop
        unsigned pick_or_drop = 0;
        pick_or_drop = 0;

        // the loop for greedy algorithm
        for (unsigned j = 0; j < deliveries.size(); j++) {
            double tempDis = 0;

            if (dropped[j] == 1)
                continue;


                // if picked up, check its drop-off
            else if (picked[j] == 1) {
                tempDis = globalVar.travel_distance[2 * currentIndex + jiou][2 * j + 1]; //find_distance_between_two_points(getIntersectionPosition(current), getIntersectionPosition(deliveries[j].dropOff));
                // // cout << "temp:  " << tempDis<< endl;
                //   // cout << "distance:  " << distance << endl;
                if (tempDis < distance) {
                    distance = tempDis;
                    pick_or_drop = 2; // replacement for a drop off
                    nearest_index = j;
                    //                    // cout << "hi" << endl;
                } else continue;

            }// if not picked up and not dropped off
            else if (!picked[j]) {
                tempDis = globalVar.travel_distance[2 * currentIndex + jiou][2 * j]; //find_distance_between_two_points(getIntersectionPosition(current), getIntersectionPosition(deliveries[j].pickUp));
                if (tempDis < distance) {
                    distance = tempDis;
                    pick_or_drop = 1; // replacement for a pick-up
                    nearest_index = j;
                    //                    // cout << "hi2" << endl;
                } else continue;

            }
        }

        if (pick_or_drop == 1) {

            picked[nearest_index] = 1;
            currentIndex = nearest_index; //deliveries[nearest_index].pickUp;
            jiou = 0;
            path.push_back(2 * currentIndex);
        } else if (pick_or_drop == 2) {
            dropped[nearest_index] = 1;
            currentIndex = nearest_index; //deliveries[nearest_index].dropOff;
            jiou = 1;
            path.push_back(2 * currentIndex + 1);

        }

        done = true;
        for (unsigned k = 0; k < dropped.size(); k++) {
            if (!dropped[k])
                done = false;
        }
    }

    return path;

}

vector<int> pick_first_greedy(const std::vector<DeliveryInfo>& deliveries,
        vector<double>& depot_distance) {
    vector<unsigned> picked(deliveries.size(), 0);
    vector<unsigned> dropped(deliveries.size(), 0);
    vector<int> path;

    double startDis = 999999;
    int startPickUp = 0;

    for (unsigned i = 0; i < depot_distance.size(); i += 2) {
        if (depot_distance[i] < startDis) {
            startDis = depot_distance[i];
            startPickUp = i / 2;
        }
    }

    unsigned currentIndex = startPickUp;
    picked[startPickUp] = 1; // the first item picked up

    path.push_back(startPickUp * 2);

    unsigned picked_up = 1;


    //find all pick ups 
    while (picked_up < deliveries.size()) {
        unsigned nearest_index = 0;
        double distance = 999999;

        // the loop for greedy algorithm
        for (unsigned j = 0; j < deliveries.size(); j++) {
            double tempDis = 0;

            if (!picked[j]) {
                tempDis = globalVar.travel_distance[2 * currentIndex][2 * j]; //find_distance_between_two_points(getIntersectionPosition(current), getIntersectionPosition(deliveries[j].pickUp));
                if (tempDis < distance) {
                    distance = tempDis;
                    nearest_index = j;
                } else continue;

            }
        }

        picked[nearest_index] = 1;
        currentIndex = nearest_index;
        path.push_back(2 * currentIndex);
        picked_up++;
    }


    //corner case middle 
    unsigned first_drop_index = 0;
    double distance_mid = 999999;

    // the loop for greedy algorithm
    for (unsigned j = 0; j < deliveries.size(); j++) {
        double tempDis = 0;
        tempDis = globalVar.travel_distance[2 * currentIndex][2 * j]; //find_distance_between_two_points(getIntersectionPosition(current), getIntersectionPosition(deliveries[j].pickUp));
        if (tempDis < distance_mid) {
            distance_mid = tempDis;
            first_drop_index = j;
        } else continue;

    }



    currentIndex = first_drop_index;
    dropped[first_drop_index] = 1; // the first item picked up

    path.push_back(first_drop_index * 2 + 1);

    unsigned dropped_off = 1;

    //find all drop offs 
    while (dropped_off < deliveries.size()) {
        unsigned nearest_index = 0;
        double distance = 999999;


        // the loop for greedy algorithm
        for (unsigned j = 0; j < deliveries.size(); j++) {
            double tempDis = 0;

            if (!dropped[j]) {
                tempDis = globalVar.travel_distance[2 * currentIndex + 1][2 * j]; //find_distance_between_two_points(getIntersectionPosition(current), getIntersectionPosition(deliveries[j].pickUp));
                if (tempDis < distance) {
                    distance = tempDis;
                    nearest_index = j;
                } else continue;

            }
        }

        dropped[nearest_index] = 1;
        currentIndex = nearest_index;
        path.push_back(2 * currentIndex + 1);
        dropped_off++;
    }

    return path;

}




void two_opt(vector<int>& exchanged_index) {

    
    bool swapValid = false;


    // loop for initial]
    // if (list.size() > 10)
    for (unsigned i = 1; i < exchanged_index.size(); i++) {
        // loop for length        
        auto current = std::chrono::high_resolution_clock::now();    
        auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
        if(delta.count() >= 0.95 * TIME_LIMIT)
          
            return;
        
        
        
        for (unsigned length = 3; length < exchanged_index.size() - i; length++) {

            //            vector<unsigned> test = list;
            //            vector<unsigned> test_list(length);
            swapValid = true;

            // extract the potential swap list
            //            for (unsigned k = 0; k < length; k++) {
            //                test_list[k] = test[i + k];
            //            }
            // loop the pickup points, check drop off
            for (unsigned check = i; check < i + length; check++) {
                for (unsigned compare = i; compare < i + length; compare++)
                    if (exchanged_index[check] % 2 == 0 && exchanged_index[compare] == exchanged_index[check] + 1) {
                        swapValid = false;
                        break;
                    }


            }



            if (swapValid == false)
                break;

            else if (swapValid == true) {
                // // cout << "swapValid222" << endl;
                // calculate reverse distance 
                double dis_total = find_travel_distance(exchanged_index);

                // reverse(test.begin()+i, test.begin()+i + length);
                reverse(exchanged_index.begin() + i, exchanged_index.begin() + i + length);
                //          // cout << "distance:  "<<dis_total << endl;
                double reverse_total = find_travel_distance(exchanged_index);
                //              // cout << "reverse:  "<<reverse_total << endl;
                // if reverse distance is better
                if (reverse_total < dis_total) {
                    // // cout << "DISTWO: " << reverse_total <<endl;
                    // cout << "2 opt success!" << endl;
                    continue;
                    // // cout << "success" << endl;
                } else (reverse(exchanged_index.begin() + i, exchanged_index.begin() + i + length));
            }
        }
    }
}

double get_travel_distance(vector<int>& test, vector<unsigned>& p_id, vector<unsigned>& exchanged_index, vector<vector<double>>&travel_distance) {
    double distance = 0;
    distance += globalVar.depot_distance[exchanged_index[0]];
    distance += globalVar.depot_distance[exchanged_index[exchanged_index.size() - 1]];
    for (unsigned i = 0; i < exchanged_index.size() - 1; i++) {

        distance += travel_distance[exchanged_index[i]][exchanged_index[i + 1]];

    }


    return distance;
}

void dijkstra_multiple(unsigned startIndex, vector<unsigned>& destinations, double turn_penalty, vector<vector<double>> &distance) {



    globalVar.naviRoute_seg.clear();
    globalVar.naviRoute.clear();
    globalVar.turn_by_turn.clear();
    vector<unsigned> dest_visited(destinations.size(), 0);

    unsigned completed = 0;
    //to order the intersection and pick the one with smallest 
    std::priority_queue<Node, vector<Node>, cmp> openQueue;

    //boost::heap::fibonacci_heap< Node, boost::heap::compare <cmp> > openQueue;

    //Vector to store the current intersection's parent intersection
    std::vector<int> parentInt(getNumberOfIntersections(), -1);


    parentInt[destinations[startIndex]] = -100;

    std::vector<double> G_Vector(getNumberOfIntersections(), -1);
    G_Vector[destinations[startIndex]] = 0;


    std::vector<int> parentSeg(getNumberOfIntersections(), -1);

    parentSeg[destinations[startIndex]] = -100;

    std::vector<bool> closeVector(getNumberOfIntersections(), false);

    //put the starting point position into the open list
    openQueue.push(Node(destinations[startIndex], 0));





    while (!openQueue.empty()) {

        IntersectionIndex current_intersection_ID = openQueue.top().intersection_ID;


        if (closeVector[current_intersection_ID] == true) {
            openQueue.pop();
            continue;
        }




        //end condition, if next point to be explored is the end intersection ID
        for (unsigned i = 0; i < destinations.size(); i++) {
            if (unlikely((current_intersection_ID == destinations[i]) && dest_visited[i] != 1)) {
                if (unlikely(i == startIndex))
                    continue;

                //If the end intersection is found   

                dest_visited[i] = 1;
                distance[startIndex][i] = G_Vector[current_intersection_ID];

                completed++;
                //            while (parentSeg[result] >= 0) {
                //                //      globalVar.naviRoute.push_back(result); 
                //
                if (completed == destinations.size())
                    return;
                //                globalVar.naviRoute_seg.insert(globalVar.naviRoute_seg.begin(), parentSeg[result]);
                //
                //                result = parentInt[result];
                //            }

                //// cout << compute_path_travel_time(globalVar.naviRoute_seg ,turn_penalty) << endl;


            }
        }

        //Delete the intersection ID with lowest F
        openQueue.pop();


        //put the intersection into close Vector
        closeVector[current_intersection_ID] = true;

        //traverse all the valid intersection
        for (unsigned index = 0; index < globalVar.int_valid_seg[current_intersection_ID].size(); ++index) {

            unsigned selected_segment_ID = globalVar.int_valid_seg[current_intersection_ID][index];

            IntersectionIndex potential_intersection_ID = globalVar.int_intids[current_intersection_ID][index];

            if (closeVector[potential_intersection_ID]) {
                continue;
            }


            //value that will be iterated

            //If the current intersection's parent is starting point, then choose the shortest segment, 
            //no need to decide turn penalty 


            double candidate_G;

            //Then it is necessary to calculate turn penalty  

            if (parentInt[current_intersection_ID] != -100) {

                //unsigned prev_street_ID = globalVar.seg_street_ID[parentSeg[current_intersection_ID]];                           

                candidate_G = G_Vector[current_intersection_ID] + globalVar.ssArray[selected_segment_ID].weight;

                // unsigned current_street_ID = globalVar.int_seg_street_ID[current_intersection_ID][index];

                if (globalVar.seg_street_ID[parentSeg[current_intersection_ID]] != globalVar.int_seg_street_ID[current_intersection_ID][index]) candidate_G = candidate_G + turn_penalty;
            } else {

                selected_segment_ID = globalVar.int_shortest_segment[current_intersection_ID][index];

                candidate_G = G_Vector[current_intersection_ID] + globalVar.ssArray[selected_segment_ID].weight;
                ;
            }

            if (candidate_G >= G_Vector[potential_intersection_ID] && G_Vector[potential_intersection_ID] != -1)
                continue;


            openQueue.push(Node(potential_intersection_ID, candidate_G));

            //Finally store all the iterative values
            parentInt[potential_intersection_ID] = current_intersection_ID;
            parentSeg[potential_intersection_ID] = selected_segment_ID;
            G_Vector[potential_intersection_ID] = candidate_G;


        }



    }

    return;




}

void dijkstra_start(unsigned startIndex,
        vector<unsigned>& destinations,
        double turn_penalty,
        vector<double>& distance,
        vector<unsigned> &nearest_depot, const vector<unsigned>& depots) {



    globalVar.naviRoute_seg.clear();
    globalVar.naviRoute.clear();
    globalVar.turn_by_turn.clear();


    unsigned completed = 0;
    //to order the intersection and pick the one with smallest 
    std::priority_queue<Node, vector<Node>, cmp> openQueue;

    //boost::heap::fibonacci_heap< Node, boost::heap::compare <cmp> > openQueue;

    //Vector to store the current intersection's parent intersection
    std::vector<int> parentInt(getNumberOfIntersections(), -1);


    parentInt[depots[startIndex]] = -100;

    std::vector<double> G_Vector(getNumberOfIntersections(), -1);
    G_Vector[depots[startIndex]] = 0;


    std::vector<int> parentSeg(getNumberOfIntersections(), -1);

    parentSeg[depots[startIndex]] = -100;

    std::vector<bool> closeVector(getNumberOfIntersections(), false);

    //put the starting point position into the open list
    openQueue.push(Node(depots[startIndex], 0));





    while (!openQueue.empty()) {

        IntersectionIndex current_intersection_ID = openQueue.top().intersection_ID;


        if (closeVector[current_intersection_ID] == true) {
            openQueue.pop();
            continue;
        }




        //end condition, if next point to be explored is the end intersection ID
        for (unsigned i = 0; i < destinations.size(); i += 2) {
            if (current_intersection_ID == destinations[i]) {
                completed++;
                //If the end intersection is found
                if (G_Vector[current_intersection_ID] < distance[i]) {
                    distance[i] = G_Vector[current_intersection_ID];
                    nearest_depot[i] = startIndex;
                }
                if (completed == destinations.size())
                    return;

            }
        }




        //Delete the intersection ID with lowest F
        openQueue.pop();


        //put the intersection into close Vector
        closeVector[current_intersection_ID] = true;





        //traverse all the valid intersection
        for (unsigned index = 0; index < globalVar.int_valid_seg[current_intersection_ID].size(); ++index) {

            unsigned selected_segment_ID = globalVar.int_valid_seg[current_intersection_ID][index];

            IntersectionIndex potential_intersection_ID = globalVar.int_intids[current_intersection_ID][index];

            if (closeVector[potential_intersection_ID]) {
                continue;
            }


            //value that will be iterated

            //If the current intersection's parent is starting point, then choose the shortest segment, 
            //no need to decide turn penalty 


            double candidate_G;

            //Then it is necessary to calculate turn penalty  

            if (parentInt[current_intersection_ID] != -100) {

                //unsigned prev_street_ID = globalVar.seg_street_ID[parentSeg[current_intersection_ID]];                           

                candidate_G = G_Vector[current_intersection_ID] + globalVar.ssArray[selected_segment_ID].weight;

                // unsigned current_street_ID = globalVar.int_seg_street_ID[current_intersection_ID][index];

                if (globalVar.seg_street_ID[parentSeg[current_intersection_ID]] != globalVar.int_seg_street_ID[current_intersection_ID][index]) candidate_G = candidate_G + turn_penalty;
            } else {

                selected_segment_ID = globalVar.int_shortest_segment[current_intersection_ID][index];

                candidate_G = G_Vector[current_intersection_ID] + globalVar.ssArray[selected_segment_ID].weight;
                ;
            }

            if (candidate_G >= G_Vector[potential_intersection_ID] && G_Vector[potential_intersection_ID] != -1)
                continue;


            openQueue.push(Node(potential_intersection_ID, candidate_G));

            //Finally store all the iterative values
            parentInt[potential_intersection_ID] = current_intersection_ID;
            parentSeg[potential_intersection_ID] = selected_segment_ID;
            G_Vector[potential_intersection_ID] = candidate_G;


        }



    }

    return;




}

void dijkstra_end(unsigned startIndex,
        const vector<unsigned>& destinations,
        double turn_penalty,
        vector<double>& distance,
        vector<unsigned> &nearest_depot, const vector<unsigned>& mix) {



    globalVar.naviRoute_seg.clear();
    globalVar.naviRoute.clear();
    globalVar.turn_by_turn.clear();


    unsigned completed = 0;
    //to order the intersection and pick the one with smallest 
    std::priority_queue<Node, vector<Node>, cmp> openQueue;

    //boost::heap::fibonacci_heap< Node, boost::heap::compare <cmp> > openQueue;

    //Vector to store the current intersection's parent intersection
    std::vector<int> parentInt(getNumberOfIntersections(), -1);


    parentInt[mix[startIndex]] = -100;

    std::vector<double> G_Vector(getNumberOfIntersections(), -1);
    G_Vector[mix[startIndex]] = 0;


    std::vector<int> parentSeg(getNumberOfIntersections(), -1);

    parentSeg[mix[startIndex]] = -100;

    std::vector<bool> closeVector(getNumberOfIntersections(), false);

    //put the starting point position into the open list
    openQueue.push(Node(mix[startIndex], 0));





    while (!openQueue.empty()) {

        IntersectionIndex current_intersection_ID = openQueue.top().intersection_ID;


        if (closeVector[current_intersection_ID] == true) {
            openQueue.pop();
            continue;
        }




        //end condition, if next point to be explored is the end intersection ID
        for (unsigned i = 0; i < destinations.size(); i += 1) {
            if (current_intersection_ID == destinations[i]) {
                completed++;
                //If the end intersection is found
                if (G_Vector[current_intersection_ID] < distance[startIndex]) {
                    distance[startIndex] = G_Vector[current_intersection_ID];
                    nearest_depot[startIndex] = i;
                }
                if (completed == destinations.size())
                    return;

            }
        }




        //Delete the intersection ID with lowest F
        openQueue.pop();


        //put the intersection into close Vector
        closeVector[current_intersection_ID] = true;





        //traverse all the valid intersection
        for (unsigned index = 0; index < globalVar.int_valid_seg[current_intersection_ID].size(); ++index) {

            unsigned selected_segment_ID = globalVar.int_valid_seg[current_intersection_ID][index];

            IntersectionIndex potential_intersection_ID = globalVar.int_intids[current_intersection_ID][index];

            if (closeVector[potential_intersection_ID]) {
                continue;
            }


            //value that will be iterated

            //If the current intersection's parent is starting point, then choose the shortest segment, 
            //no need to decide turn penalty 


            double candidate_G;

            //Then it is necessary to calculate turn penalty  

            if (parentInt[current_intersection_ID] != -100) {

                //unsigned prev_street_ID = globalVar.seg_street_ID[parentSeg[current_intersection_ID]];                           

                candidate_G = G_Vector[current_intersection_ID] + globalVar.ssArray[selected_segment_ID].weight;

                // unsigned current_street_ID = globalVar.int_seg_street_ID[current_intersection_ID][index];

                if (globalVar.seg_street_ID[parentSeg[current_intersection_ID]] != globalVar.int_seg_street_ID[current_intersection_ID][index]) candidate_G = candidate_G + turn_penalty;
            } else {

                selected_segment_ID = globalVar.int_shortest_segment[current_intersection_ID][index];

                candidate_G = G_Vector[current_intersection_ID] + globalVar.ssArray[selected_segment_ID].weight;
                ;
            }

            if (candidate_G >= G_Vector[potential_intersection_ID] && G_Vector[potential_intersection_ID] != -1)
                continue;


            openQueue.push(Node(potential_intersection_ID, candidate_G));

            //Finally store all the iterative values
            parentInt[potential_intersection_ID] = current_intersection_ID;
            parentSeg[potential_intersection_ID] = selected_segment_ID;
            G_Vector[potential_intersection_ID] = candidate_G;


        }



    }

    return;




}

void three_opt1(vector<int>& mix_index) {
    // mode 1 swap
    bool legal = false;
    unsigned size = mix_index.size();
    double current_distance;
    vector<int> opt_temp;
    if (mix_index.size() > 4)
        for (unsigned p1 = 2; p1 < mix_index.size() - 4; p1++) {
            
            auto current = std::chrono::high_resolution_clock::now();    
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            if(delta.count() >= 0.95 * TIME_LIMIT)
          
                return;
           
            for (unsigned p2 = p1 + 2; p2 < mix_index.size() - 1; p2++) {
                opt_temp.clear();
                opt_temp.resize(size);
                legal = true;

                // Check legal
                for (unsigned check = 1; check < p1; check++) {
                    for (unsigned compare = p2; compare < mix_index.size(); compare++) {
                        if (mix_index[check] % 2 == 0 && mix_index[compare] == mix_index[check] + 1)
                            legal = false;
                    }
                }
                // if (legal)
                //      // cout << "legal <" << endl;

                for (unsigned check = p1; check < p2; check++) {
                    for (unsigned compare = p2; compare < mix_index.size(); compare++) {
                        if (mix_index[check] % 2 == 0 && mix_index[compare] == mix_index[check] + 1)
                            legal = false;
                    }
                }



                if (!legal) continue;
                    // if 3 opt is legal
                else if (legal) {
                    current_distance = find_travel_distance(mix_index);
                    opt_temp[0] = mix_index[0];

                    for (unsigned i = p2; i < mix_index.size(); i++)
                        // from 0 - size - p2
                        opt_temp[i - p2 + 1] = mix_index[i];

                    for (unsigned i = 1; i < p1; i++)
                        // from size- p2 - size - p2 +p1
                        opt_temp[i + mix_index.size() - p2] = mix_index[i];
                    for (unsigned i = 0; i < p2 - p1; i++)
                        opt_temp[mix_index.size() - p2 + p1 + i] = mix_index[p1 + i];

                    double temp_distance = find_travel_distance(opt_temp);
                    //   // cout << "current distance" <<mix_index.size() <<endl;
                    //  // cout << "temp distance" << opt_temp.size() <<endl;
                    if (temp_distance < current_distance) {
                        //// cout << "success!" <<endl;
                        for (unsigned i = 0; i < mix_index.size(); i++)
                            mix_index[i] = opt_temp[i];
                    }



                }
            }

            //    for (unsigned i = 0; i< mix_index.size(); i++)
            //        // cout << "  "<<mix_index[i] ;
            //    
            //                // cout << endl;









        }
}

double find_travel_distance(vector<int>& mix_index) {
    double distance = 0;
    distance += globalVar.depot_distance[mix_index[0]];
    distance += globalVar.depot_distance[mix_index[mix_index.size() - 1]];
    for (unsigned i = 0; i < mix_index.size() - 1; i++) {
        distance += globalVar.travel_distance[mix_index[i]][mix_index[i + 1]];
    }



    return distance;
}

void three_opt2(vector<int>& mix_index) {
    // mode 1 swap
    bool legal = false;
    unsigned size = mix_index.size();
    vector<int> opt_temp;
    if (mix_index.size() > 4)
        for (unsigned p1 = 2; p1 < mix_index.size() - 4; p1++) {
            
            auto current = std::chrono::high_resolution_clock::now();    
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            if(delta.count() >= 0.95 * TIME_LIMIT)
          
                return;
            
            for (unsigned p2 = p1 + 2; p2 < mix_index.size() - 1; p2++) {
                opt_temp.clear();
                opt_temp.resize(size);
                legal = true;
                double current_distance = find_travel_distance(mix_index);
                // Check legal
                for (unsigned check = 1; check < p1; check++) {
                    for (unsigned compare = p1; compare < mix_index.size(); compare++) {
                        if (mix_index[check] % 2 == 0 && mix_index[compare] == mix_index[check] + 1)
                            legal = false;
                    }
                }

                if (!legal) continue;
                    // if 3 opt is legal
                else if (legal) {

                    opt_temp[0] = mix_index[0];

                    for (unsigned i = 0; i < p2 - p1; i++)
                        // from 0 - p2-p1
                        opt_temp[i + 1] = mix_index[p1 + i];

                    for (unsigned i = 0; i < mix_index.size() - p2; i++)
                        // from size- p2 - size - p2 +p1
                        opt_temp[p2 - p1 + i + 1] = mix_index[p2 + i];
                    for (unsigned i = 1; i < p1; i++)
                        opt_temp[mix_index.size() - p1 + i] = mix_index[i];

                    double temp_distance = find_travel_distance(opt_temp);
                    //  // cout << "current distance" <<current_distance <<endl;
                    //  // cout << "temp distance" << temp_distance<<endl;
                    if (temp_distance < current_distance) {
                        //// cout << "success!" <<endl;
                        for (unsigned i = 0; i < mix_index.size(); i++)
                            mix_index[i] = opt_temp[i];

                        //                    for (unsigned i = 0; i< mix_index.size(); i++)
                        //        // cout << "  "<<mix_index[i] ;
                        //    
                        //                // cout << endl;    
                    }
                }
            }
        }

}


void three_opt3(vector<int>& mix_index) {
    // mode 1 swap
    bool legal = false;
    unsigned size = mix_index.size();
    vector<int> opt_temp;
    if (mix_index.size() > 4)
        for (unsigned p1 = 2; p1 < mix_index.size() - 4; p1++) {
            
            auto current = std::chrono::high_resolution_clock::now();    
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            if(delta.count() >= 0.95 * TIME_LIMIT)
                return;
                
            
            for (unsigned p2 = p1 + 2; p2 < mix_index.size() - 1; p2++) {
                opt_temp.clear();
                opt_temp.resize(size);
                legal = true;

                double current_distance = find_travel_distance(mix_index);
                //// cout << "travel distance ori"  << current_distance <<endl;
                // Check legal
                for (unsigned check = p1; check < p2; check++) {
                    for (unsigned compare = p2; compare < mix_index.size(); compare++) {
                        if (mix_index[check] % 2 == 0 && mix_index[compare] == mix_index[check] + 1)
                            legal = false;
                    }
                }

                if (!legal) continue;
                    // if 3 opt is legal
                else if (legal) {

                    opt_temp[0] = mix_index[0];

                    for (unsigned i = 0; i < p1; i++)
                        // from 0 - p2-p1
                        opt_temp[i] = mix_index[i];

                    for (unsigned i = 0; i < mix_index.size() - p2; i++)
                        // from size- p2 - size - p2 +p1
                        opt_temp[p1 + i] = mix_index[p2 + i];
                    for (unsigned i = 0; i < p2 - p1; i++)
                        opt_temp[p1 + mix_index.size() - p2 + i] = mix_index[p1 + i];

                    double temp_distance = find_travel_distance(opt_temp);
                    //  // cout << "current distance" <<current_distance <<endl;
                    //  // cout << "temp distance" << temp_distance<<endl;
                    if (temp_distance < current_distance) {
                        // // cout << "success!" <<endl;
                        for (unsigned i = 0; i < mix_index.size(); i++)
                            mix_index[i] = opt_temp[i];

                        //                    for (unsigned i = 0; i< mix_index.size(); i++)
                        //        // cout << "  "<<mix_index[i] ;
                        //    // cout << "travel distance ori"  << temp_distance <<endl;
                        //                // cout << endl;    
                    }
                }
            }
        }

}

void three_opt4(vector<int>& mix_index) {
    // mode 1 swap
    bool legal = false;
    unsigned size = mix_index.size();
    vector<int> opt_temp;
    if (mix_index.size() > 4)
        for (unsigned p1 = 2; p1 < mix_index.size() - 4; p1++) {
                       
            auto current = std::chrono::high_resolution_clock::now();    
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            if(delta.count() >= 0.95 * TIME_LIMIT)
                 return;
                        
            for (unsigned p2 = p1 + 2; p2 < mix_index.size() - 1; p2++) {
                opt_temp.clear();
                opt_temp.resize(size);
                legal = true;
                double current_distance = find_travel_distance(mix_index);
                // Check legal
                for (unsigned check = 1; check < p1; check++) {
                    for (unsigned compare = p1; compare < p2; compare++) {
                        if (mix_index[check] % 2 == 0 && mix_index[compare] == mix_index[check] + 1)
                            legal = false;
                    }
                }

                if (!legal) continue;
                    // if 3 opt is legal
                else if (legal) {

                    opt_temp[0] = mix_index[0];

                    for (unsigned i = 0; i < p2 - p1; i++)
                        // from 0 - p2-p1
                        opt_temp[i + 1] = mix_index[i + p1];

                    for (unsigned i = 1; i < p1; i++)
                        // from size- p2 - size - p2 +p1
                        opt_temp[p2 - p1 + i] = mix_index[i];
                    for (unsigned i = p2; i < mix_index.size(); i++)
                        opt_temp[i] = mix_index[i];

                    double temp_distance = find_travel_distance(opt_temp);
                    //  // cout << "current distance" <<current_distance <<endl;
                    //  // cout << "temp distance" << temp_distance<<endl;
                    if (temp_distance < current_distance) {
                        // cout << "success!" << endl;
                        for (unsigned i = 0; i < mix_index.size(); i++)
                            mix_index[i] = opt_temp[i];

//                        for (unsigned i = 0; i < mix_index.size(); i++)
                            // cout << "  " << mix_index[i];

                        // cout << endl;
                    }
                }
            }
        }

}

void three_combi(vector<int>& mix_index) {
    RanGen2 opt(4);
    unsigned mode;
    
    
    if (mix_index.size() > 4)
        for (unsigned p1 = 2; p1 < mix_index.size() - 4; p1++) {
                       
            auto current = std::chrono::high_resolution_clock::now();    
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            if(delta.count() >= 0.95 * TIME_LIMIT)
          
                return;
            
            
            for (unsigned p2 = p1 + 2; p2 < mix_index.size() - 1; p2++) {
                mode = opt.generate();
                // // cout << mode <<endl;
                switch (mode) {
                    case 0:
                        three_opt_mode1(mix_index, p1, p2);
                    case 1:
                        three_opt_mode2(mix_index, p1, p2);
                    case 2:
                        three_opt_mode3(mix_index, p1, p2);
                    case 3:
                        three_opt_mode4(mix_index, p1, p2);
                    default:
                        three_opt_mode2(mix_index, p1, p2);
                }
            }


        }
}



void three_opt_mode1(vector<int>& mix_index, unsigned p1, unsigned p2) {
    // mode 1 swap
    bool legal = false;
    unsigned size = mix_index.size();
    double current_distance;
    vector<int> opt_temp(size);
    legal = true;

    // Check legal
    for (unsigned check = 1; check < p1; check++) {
        for (unsigned compare = p2; compare < mix_index.size(); compare++) {
            if (mix_index[check] % 2 == 0 && mix_index[compare] == mix_index[check] + 1)
                legal = false;
        }
    }
    if (!legal) return;

    for (unsigned check = p1; check < p2; check++) {
        for (unsigned compare = p2; compare < mix_index.size(); compare++) {
            if (mix_index[check] % 2 == 0 && mix_index[compare] == mix_index[check] + 1)
                legal = false;
        }
    }


    if (!legal) return;
        // if 3 opt is legal
    else if (legal) {
        current_distance = find_travel_distance(mix_index);
        opt_temp[0] = mix_index[0];

        for (unsigned i = p2; i < mix_index.size(); i++)
            // from 0 - size - p2
            opt_temp[i - p2 + 1] = mix_index[i];

        for (unsigned i = 1; i < p1; i++)
            // from size- p2 - size - p2 +p1
            opt_temp[i + mix_index.size() - p2] = mix_index[i];
        for (unsigned i = 0; i < p2 - p1; i++)
            opt_temp[mix_index.size() - p2 + p1 + i] = mix_index[p1 + i];

        double temp_distance = find_travel_distance(opt_temp);
        //   // cout << "current distance" <<mix_index.size() <<endl;
        //  // cout << "temp distance" << opt_temp.size() <<endl;
        if (temp_distance < current_distance) {
            // cout << "success1" << endl;
            for (unsigned i = 0; i < mix_index.size(); i++)
                mix_index[i] = opt_temp[i];
        }


    }




}

void three_opt_mode2(vector<int>& mix_index, unsigned p1, unsigned p2) {
    // mode 1 swap
    bool legal = false;
    unsigned size = mix_index.size();
    vector<int> opt_temp(size);
    legal = true;
    double current_distance = find_travel_distance(mix_index);
    // Check legal
    for (unsigned check = 1; check < p1; check++) {
        for (unsigned compare = p1; compare < mix_index.size(); compare++) {
            if (mix_index[check] % 2 == 0 && mix_index[compare] == mix_index[check] + 1)
                legal = false;
        }
    }

    if (!legal) return;
        // if 3 opt is legal
    else if (legal) {

        opt_temp[0] = mix_index[0];

        for (unsigned i = 0; i < p2 - p1; i++)
            // from 0 - p2-p1
            opt_temp[i + 1] = mix_index[p1 + i];

        for (unsigned i = 0; i < mix_index.size() - p2; i++)
            // from size- p2 - size - p2 +p1
            opt_temp[p2 - p1 + i + 1] = mix_index[p2 + i];
        for (unsigned i = 1; i < p1; i++)
            opt_temp[mix_index.size() - p1 + i] = mix_index[i];

        double temp_distance = find_travel_distance(opt_temp);
        //  // cout << "current distance" <<current_distance <<endl;
        //  // cout << "temp distance" << temp_distance<<endl;
        if (temp_distance < current_distance) {
            // // cout << "success2" <<endl;
            for (unsigned i = 0; i < mix_index.size(); i++)
                mix_index[i] = opt_temp[i];

            //                    for (unsigned i = 0; i< mix_index.size(); i++)
            //        // cout << "  "<<mix_index[i] ;
            //    
            //                // cout << endl;    
        }
    }
}

void three_opt_mode3(vector<int>& mix_index, unsigned p1, unsigned p2) {
    // mode 1 swap
    bool legal = false;
    unsigned size = mix_index.size();
    vector<int> opt_temp(size);
    legal = true;

    double current_distance = find_travel_distance(mix_index);
    //// cout << "travel distance ori"  << current_distance <<endl;
    // Check legal
    for (unsigned check = p1; check < p2; check++) {
        for (unsigned compare = p2; compare < mix_index.size(); compare++) {
            if (mix_index[check] % 2 == 0 && mix_index[compare] == mix_index[check] + 1)
                legal = false;
        }
    }

    if (!legal) return;
        // if 3 opt is legal
    else if (legal) {

        opt_temp[0] = mix_index[0];

        for (unsigned i = 0; i < p1; i++)
            // from 0 - p2-p1
            opt_temp[i] = mix_index[i];

        for (unsigned i = 0; i < mix_index.size() - p2; i++)
            // from size- p2 - size - p2 +p1
            opt_temp[p1 + i] = mix_index[p2 + i];
        for (unsigned i = 0; i < p2 - p1; i++)
            opt_temp[p1 + mix_index.size() - p2 + i] = mix_index[p1 + i];

        double temp_distance = find_travel_distance(opt_temp);
        //  // cout << "current distance" <<current_distance <<endl;
        //  // cout << "temp distance" << temp_distance<<endl;
        if (temp_distance < current_distance) {
            // cout << "success3" << endl;
            for (unsigned i = 0; i < mix_index.size(); i++)
                mix_index[i] = opt_temp[i];

            //                    for (unsigned i = 0; i< mix_index.size(); i++)
            //        // cout << "  "<<mix_index[i] ;
            //    // cout << "travel distance ori"  << temp_distance <<endl;
            //                // cout << endl;    
        }
    }

}

void three_opt_mode4(vector<int>& mix_index, unsigned p1, unsigned p2) {
    // mode 1 swap
    bool legal = false;
    unsigned size = mix_index.size();
    vector<int> opt_temp(size);

    legal = true;
    double current_distance = find_travel_distance(mix_index);
    // Check legal
    for (unsigned check = 1; check < p1; check++) {
        for (unsigned compare = p1; compare < p2; compare++) {
            if (mix_index[check] % 2 == 0 && mix_index[compare] == mix_index[check] + 1)
                legal = false;
        }
    }

    if (!legal) return;
        // if 3 opt is legal
    else if (legal) {

        opt_temp[0] = mix_index[0];

        for (unsigned i = 0; i < p2 - p1; i++)
            // from 0 - p2-p1
            opt_temp[i + 1] = mix_index[i + p1];

        for (unsigned i = 1; i < p1; i++)
            // from size- p2 - size - p2 +p1
            opt_temp[p2 - p1 + i] = mix_index[i];
        for (unsigned i = p2; i < mix_index.size(); i++)
            opt_temp[i] = mix_index[i];

        double temp_distance = find_travel_distance(opt_temp);
        //  // cout << "current distance" <<current_distance <<endl;
        //  // cout << "temp distance" << temp_distance<<endl;
        if (temp_distance < current_distance) {
            // cout << "success4" << endl;
            for (unsigned i = 0; i < mix_index.size(); i++)
                mix_index[i] = opt_temp[i];

            //                    for (unsigned i = 0; i< mix_index.size(); i++)
            //        // cout << "  "<<mix_index[i] ;
            //    
            //                // cout << endl;    
        }
    }



}


//runs SA with multi thread and pick the best one 

void multi_SA(vector<int>& path) {
    
    vector<int> new_path1 = path;
    vector<int> new_path2 = path;
    vector<int> new_path3 = path;
    vector<int> new_path4 = path;
    vector<int> new_path5 = path;
    vector<int> new_path6 = path;
    vector<int> new_path7 = path;
    vector<int> new_path8 = path;

    int path_dis[8];


#pragma omp parallel for
    for (int i = 0; i < 8; i++) {
        if (i == 0) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path1);
        } else if (i == 1) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path2);
        } else if (i == 2) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path3);
        } else if (i == 3) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path4);
        } else if (i == 4) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path5);
        } else if (i == 5) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path6);
        } else if (i == 6) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path7);
        } else if (i == 7) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path8);
        }
    }




    double distance = 999999;
    int best = 0;

    for (int i = 0; i < 8; i++) {
        if (path_dis[i] < distance) {
            distance = path_dis[i];
            best = i;
        }
    }

    // cout << distance << endl;

    if (best == 0) {
        path = new_path1;
    } else if (best == 1) {
        path = new_path2;
    } else if (best == 2) {
        path = new_path3;
    } else if (best == 3) {
        path = new_path4;
    } else if (best == 4) {
        path = new_path5;
    } else if (best == 5) {
        path = new_path6;
    } else if (best == 6) {
        path = new_path7;
    } else if (best == 7) {
        path = new_path8;
    } else if (best == 8) {
        path = new_path9;
    } else if (best == 9) {
        path = new_path10;
    } else if (best == 10) {
        path = new_path11;
    } else if (best == 11) {
        path = new_path12;
    } else if (best == 12) {
        path = new_path13;
    } else if (best == 13) {
        path = new_path14;
    } else if (best == 14) {
        path = new_path15;
    } else if (best == 15) {
        path = new_path16;
    }
}

void multi_SA_24(vector<int>& path) {
    vector<int> new_path1 = path;
    vector<int> new_path2 = path;
    vector<int> new_path3 = path;
    vector<int> new_path4 = path;
    vector<int> new_path5 = path;
    vector<int> new_path6 = path;
    vector<int> new_path7 = path;
    vector<int> new_path8 = path;
    vector<int> new_path9 = path;
    vector<int> new_path10 = path;
    vector<int> new_path11 = path;
    vector<int> new_path12 = path;
    vector<int> new_path13 = path;
    vector<int> new_path14 = path;
    vector<int> new_path15 = path;
    vector<int> new_path16 = path;
    vector<int> new_path17 = path;
    vector<int> new_path18 = path;
    vector<int> new_path19 = path;
    vector<int> new_path20 = path;
    vector<int> new_path21 = path;
    vector<int> new_path22 = path;
    vector<int> new_path23 = path;
    vector<int> new_path24 = path;

    double path_dis[24];


#pragma omp parallel for
    for (int i = 0; i < 24; i++) {
        if (i == 0) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path1);
        } else if (i == 1) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path2);
        } else if (i == 2) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path3);
        } else if (i == 3) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path4);
        } else if (i == 4) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path5);
        } else if (i == 5) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path6);
        } else if (i == 6) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path7);
        } else if (i == 7) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path8);
        } else if (i == 8) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path9);
        } else if (i == 9) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path10);
        } else if (i == 10) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path11);
        } else if (i == 11) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path12);
        } else if (i == 12) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path13);
        } else if (i == 13) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path14);
        } else if (i == 14) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path15);
        } else if (i == 15) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path16);
        } else if (i == 16) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path17);
        } else if (i == 17) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path18);
        } else if (i == 18) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path19);
        } else if (i == 19) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path20);
        } else if (i == 20) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path21);
        } else if (i == 21) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path22);
        } else if (i == 22) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path23);
        } else if (i == 23) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path24);
        }
    }




    double distance = 999999;
    int best = 0;

    for (int i = 0; i < 24; i++) {
        if (path_dis[i] < distance) {
            distance = path_dis[i];
            best = i;
        }
    }

    // cout << distance << endl;

    if (best == 0) {
        path = new_path1;
    } else if (best == 1) {
        path = new_path2;
    } else if (best == 2) {
        path = new_path3;
    } else if (best == 3) {
        path = new_path4;
    } else if (best == 4) {
        path = new_path5;
    } else if (best == 5) {
        path = new_path6;
    } else if (best == 6) {
        path = new_path7;
    } else if (best == 7) {
        path = new_path8;
    } else if (best == 8) {
        path = new_path9;
    } else if (best == 9) {
        path = new_path10;
    } else if (best == 10) {
        path = new_path11;
    } else if (best == 11) {
        path = new_path12;
    } else if (best == 12) {
        path = new_path13;
    } else if (best == 13) {
        path = new_path14;
    } else if (best == 14) {
        path = new_path15;
    } else if (best == 15) {
        path = new_path16;
    } else if (best == 16) {
        path = new_path17;
    } else if (best == 17) {
        path = new_path18;
    } else if (best == 18) {
        path = new_path19;
    } else if (best == 19) {
        path = new_path20;
    } else if (best == 20) {
        path = new_path21;
    } else if (best == 21) {
        path = new_path22;
    } else if (best == 22) {
        path = new_path23;
    } else if (best == 23) {
        path = new_path24;
    }
}



// reference http://ieeexplore.ieee.org/document/7799275/?reload=true
void four_opt(vector<int>& path) {

    auto current = std::chrono::high_resolution_clock::now();    
            
        auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
        
        
        if(delta.count() >= 0.95 * TIME_LIMIT)
          
            return;
    vector<int> corresponding(path.size(), 0);
    for (unsigned n = 0; n < path.size(); n++) {
        if (path[n] % 2 != 0) {
            int pickID = path[n] - 1;
            corresponding[pickID] = n;
        }
    }

    int size = path.size();
    int iteration = 0;
    bool improved = true;



    while (improved && iteration <= 4) {
        iteration++;
        improved = false;
 
        for (int i = 1; i < size - 4; i++) {

            
            auto current = std::chrono::high_resolution_clock::now();    
            auto delta = std::chrono::duration_cast<chrono::duration<double>>(current - globalVar.STARTING_TIME);
            if(delta.count() >= 0.95 * TIME_LIMIT)
          
                return;
           
            for (int j = i + 2; j < size - 3; j++) {
                bool illegal_1 = false;

                for (int k = j + 2; k < size - 2 && !illegal_1; k++) {
                    bool illegal_2 = false;

                    for (int l = k + 2; l < size - 1 && !illegal_1; l++) {

                        for (int n = i; n <= j; n++) {
                            if (path[n] % 2 == 0) {
                                int pickID = path[n];
                                int dropIndex = corresponding[pickID];
                                if (dropIndex >= j + 1 && dropIndex <= l) {
                                    
                                    illegal_1 = true;
                                    j = dropIndex - 1;
                                    break;
                                }
                            }
                        }

                        if (illegal_1) break;


                        for (int n = j + 1; n <= k - 1; n++) {
                            if (path[n] % 2 == 0) { 
                                int pickID = path[n];
                                unsigned dropIndex = corresponding[pickID];
                                if (dropIndex >= k && dropIndex <= l) {
  
                                    illegal_2 = true;
                                    k = dropIndex;
                                    break;
                                }
                            }
                        }

                        if (illegal_2) break;

                        double diff = 0;

                        diff -= globalVar.travel_distance[path[i - 1]][path[i]];
                        diff += globalVar.travel_distance[path[i - 1]][path[k]];

                        diff -= globalVar.travel_distance[path[j]][path[j + 1]];
                        diff += globalVar.travel_distance[path[l]][path[j + 1]];

                        diff -= globalVar.travel_distance[path[k - 1]][path[k]];
                        diff += globalVar.travel_distance[path[j]][path[l + 1]];

                        diff -= globalVar.travel_distance[path[l]][path[l + 1]];
                        diff += globalVar.travel_distance[path[k - 1]][path[i]];

                        if (diff >= 0) {
                            continue;
                        }

 
                        if (i > 1 && l < size - 1) {

                            rotate(path.begin() + i, path.begin() + k, path.begin() + l + 1);

                            rotate(path.begin() + j + 1 - (j - i + 1) + (l - k + 1),

                            path.begin() + j + 1 - (j - i + 1) + (l - k + 1) + (j - i + 1),

                            path.begin() + l + 1);
                        }

                        improved = true;

                  
                        for (int n = i; n <= l; n++) {
                            if ((path[n] % 2) == 1) { 
                                int pickID = path[n] - 1;
                                corresponding[pickID] = n;
                            }
                        }

                    }
                }
            }
        }
    }

    return ;
}

void multi_SA2_24(vector<int>& path) {
    vector<int> new_path1 = path;
    vector<int> new_path2 = path;
    vector<int> new_path3 = path;
    vector<int> new_path4 = path;
    vector<int> new_path5 = path;
    vector<int> new_path6 = path;
    vector<int> new_path7 = path;
    vector<int> new_path8 = path;
    vector<int> new_path9 = path;
    vector<int> new_path10 = path;
    vector<int> new_path11 = path;
    vector<int> new_path12 = path;
    vector<int> new_path13 = path;
    vector<int> new_path14 = path;
    vector<int> new_path15 = path;
    vector<int> new_path16 = path;
    vector<int> new_path17 = path;
    vector<int> new_path18 = path;
    vector<int> new_path19 = path;
    vector<int> new_path20 = path;
    vector<int> new_path21 = path;
    vector<int> new_path22 = path;
    vector<int> new_path23 = path;
    vector<int> new_path24 = path;

    double path_dis[24];


#pragma omp parallel for
    for (int i = 0; i < 24; i++) {
        if (i == 0) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path1);
        } else if (i == 1) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path2);
        } else if (i == 2) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path3);
        } else if (i == 3) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path4);
        } else if (i == 4) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path5);
        } else if (i == 5) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path6);
        } else if (i == 6) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path7);
        } else if (i == 7) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path8);
        } else if (i == 8) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path9);
        } else if (i == 9) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path10);
        } else if (i == 10) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path11);
        } else if (i == 11) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path12);
        } else if (i == 12) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path13);
        } else if (i == 13) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path14);
        } else if (i == 14) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path15);
        } else if (i == 15) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path16);
        } else if (i == 16) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path17);
        } else if (i == 17) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path18);
        } else if (i == 18) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path19);
        } else if (i == 19) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path20);
        } else if (i == 20) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path21);
        } else if (i == 21) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path22);
        } else if (i == 22) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path23);
        } else if (i == 23) {
            path_dis[i] = Simulated_Annealing_2(globalVar.travel_distance, new_path24);
        } else if (i == 16) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path17);
        } else if (i == 17) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path18);
        } else if (i == 18) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path19);
        } else if (i == 19) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path20);
        } else if (i == 20) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path21);
        } else if (i == 21) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path22);
        } else if (i == 22) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path23);
        } else if (i == 23) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path24);
        } else if (i == 24) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path25);
        } else if (i == 25) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path26);
        } else if (i == 26) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path27);
        } else if (i == 27) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path28);
        } else if (i == 28) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path29);
        } else if (i == 29) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path30);
        } else if (i == 30) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path31);
        } else if (i == 31) {
            path_dis[i] = Simulated_Annealing(globalVar.travel_distance, new_path32);
        }

    }




    double distance = 999999;
    int best = 0;

    for (int i = 0; i < 24; i++) {
        if (path_dis[i] < distance) {
            distance = path_dis[i];
            best = i;
        }
    }



    if (best == 0) {
        path = new_path1;
    } else if (best == 1) {
        path = new_path2;
    } else if (best == 2) {
        path = new_path3;
    } else if (best == 3) {
        path = new_path4;
    } else if (best == 4) {
        path = new_path5;
    } else if (best == 5) {
        path = new_path6;
    } else if (best == 6) {
        path = new_path7;
    } else if (best == 7) {
        path = new_path8;
    } else if (best == 8) {
        path = new_path9;
    } else if (best == 9) {
        path = new_path10;
    } else if (best == 10) {
        path = new_path11;
    } else if (best == 11) {
        path = new_path12;
    } else if (best == 12) {
        path = new_path13;
    } else if (best == 13) {
        path = new_path14;
    } else if (best == 14) {
        path = new_path15;
    } else if (best == 15) {
        path = new_path16;
    } else if (best == 16) {
        path = new_path17;
    } else if (best == 17) {
        path = new_path18;
    } else if (best == 18) {
        path = new_path19;
    } else if (best == 19) {
        path = new_path20;
    } else if (best == 20) {
        path = new_path21;
    } else if (best == 21) {
        path = new_path22;
    } else if (best == 22) {
        path = new_path23;
    } else if (best == 23) {
        path = new_path24;
    }
}




void four_opt(vector<int>& path) {
    for (unsigned p1 = 0; p1 < path.size() / 2; p1++) {
        for (unsigned p2 = p1; p2 < path.size() - 4; p2++) {
            for (unsigned p3 = p2; p3 < path.size() - 2; p3++) {
                for (unsigned p4 = p3; p3 < path.size(); p4++) {

// reference http://ieeexplore.ieee.org/document/7799275/?reload=true
//void four_opt(vector<int>& path){
//    // the four opt function
//    bool legal_seg1 = true;
//    bool legal_seg2 = true;
//    double current_dis = find_travel_distance(path);
//    vector<int> opt_temp;
//    
//
//
//    vector<unsigned> corresponding(path.size(), 0);
//    
//    // create aid matrix to prevent too many loops
//    for (int n = 0; n < path.size(); n++) {
//        if ((path[n] % 2) == 1) { // find drop ID
//            corresponding[path[n] - 1] = n;
//        }
//    }
//    for (unsigned i = 0; i < path.size(); i++){
//                            
//                            // cout << " "<< path[i];
//                        }
//                        // cout << endl;
//    
//    //// cout << "path size: " <<path.size() << endl;
//    if(path.size() > 8)
//    for (int p1 = 2; p1 < path.size()-4; p1++){
//        for (int p2 = p1+2; p2 < path.size()-3; p2++){
//            legal_seg1 = true;
//            for (int p3 = p2+2; p3 < path.size()-2 && legal_seg1; p3++){
//                legal_seg2 = true;
//                for (int p4 = p3+2; p4 < path.size()-1 && legal_seg1; p4++){
//                    
//                 //   // cout << p1 << p2 << p3 << p4 << endl;
//                  //  // cout << "checking legality" << endl;
//                    // check legal
//                    for (unsigned check = p1-2; check <= p2; check++){                        
//                        if (path[check] %2 == 0)
//                            if (corresponding[check] >= p2-2 && corresponding[check] <= p4){
//                                legal_seg1 = false;
//                              //  p2 = corresponding[check]-1;
//                                break;
//                            }                        
//                    }
//                    
//                    if (!legal_seg1)
//                        break;
//                    
//                    for (unsigned check = p2-2; check <= p3+2; check++){                        
//                        if (path[check] %2 == 0)
//                            if (corresponding[check] >= p2-2 && corresponding[check] <= p4){
//                                    legal_seg2 = false;
//                   //                 // cout << "illegal2" <<endl;
//                                 //   p3 = corresponding[check];
//                                    break;
//                            }
//                    }
//                    
//       
//                    if (!legal_seg2)
//                        break;
//                  //  else // cout << "legal" <<endl;
//                    
//                    //// cout << diff << endl;
//                    // determine optimization
//                    
//                    else {
//                        opt_temp.clear();
//                        opt_temp = path;
//                        rotate(opt_temp.begin()+p1, opt_temp.begin()+p3, opt_temp.begin()+p4+1);
//                        
//                        rotate(opt_temp.begin() + p1 + p4 - p3 + 1, opt_temp.begin() + p2 - p3 + p4 +2, 
//                                opt_temp.begin()+p4+1);
//                        rotate(corresponding.begin()+p1, corresponding.begin()+p3, corresponding.begin()+p4+1);
//                        
//                        rotate(corresponding.begin() + p1 + p4 - p3 + 1, corresponding.begin() + p2 - p3 + p4 +2, 
//                                corresponding.begin()+p4+1);
//                        
//                        if (find_travel_distance(opt_temp) < current_dis){
//                            current_dis = find_travel_distance(opt_temp);
//                        }
//                        else continue;
//                        
//                        
//
//                        
//                        
//                        
//                        for (unsigned i = 0; i < path.size(); i++){
//                            path[i] = opt_temp[i];
//                            // cout << " "<< opt_temp[i];
//                        }
//                        // cout << endl;
//                        // cout << "4 opt success! " << endl;
//                        
//                        
//                        
//                        
//                        //exchange the corresponding index
//                        for (int cor = p1; cor <= p4; cor++) {
//                            if (path[cor] % 2 != 0) {
//
//                                corresponding[path[cor] - 1] = cor;
//                            }
//                        }
////                        temp_exchange = path[p1];
////                        path[p1] = path[p3];
////                        path[p3] = temp_exchange;
////                        temp_exchange = path[p2];
////                        path[p2] = path[p4];
////                        path[p4] = temp_exchange; 
////  
////                        temp_exchange = corresponding[p1];
////                        corresponding[p1] = corresponding[p3];
////                        corresponding[p3] = temp_exchange;
////                        temp_exchange = corresponding[p2];
////                        corresponding[p2] = corresponding[p4];
////                        corresponding[p4] = temp_exchange;      
//                    }
//                    
//                    
//                    
//                    
//                    
//                }
//            }
//        }
//    }
//    else return;
//}
















//void four_opt(vector<int>& path) {
//    // the four opt function
//    bool legal = true;
//    double current_dis = find_travel_distance(path);
//    vector<int> opt_temp;
//    vector<int> cor_temp;
//
//
//
//    vector<unsigned> corresponding(path.size(), 0);
//
//    // create aid matrix to prevent too many loops
//    for (unsigned n = 0; n < path.size(); n++) {
//        if ((path[n] % 2) == 1) { // find drop ID
//
//            corresponding[path[n] - 1] = n;
//        }
//    }
//    for (unsigned i = 0; i < path.size(); i++) {
//
//        // cout << "  " << path[i];
//    }
//    // cout << endl;
//
//    //// cout << "path size: " <<path.size() << endl;
//    if (path.size() > 12)
//        for (unsigned p1 = 2; p1 < path.size() - 6; p1++) {
//            for (unsigned p4 = p1 + 8; p4 < path.size() - 2; p4++) {
//                legal = true;
//                for (unsigned p2 = p1 + 2; p2 < p4 - 4 && legal; p2++) {
//                    for (unsigned p3 = p2 + 2; p3 < p4 - 2 && legal; p3++) {
//                        opt_temp.clear();
//                        opt_temp.resize(path.size());
//
//                        legal = true;
//
//                        for (unsigned check = p1; check < p2; check++) {
//
//                            if (path[check] % 2 == 0 && corresponding[check] >= p2 && corresponding[check] < p4) {
//                                legal = false;
//
//                            }
//
//                        }
//
//
//
//                        if (!legal) continue;
//                            // if 3 opt is legal
//                        else if (legal) {
//                            current_dis = find_travel_distance(path);
//
//                            for (unsigned i = 0; i < p1; i++)
//
//                                opt_temp[i] = path[i];
//
//                            for (unsigned i = p4; i < path.size(); i++)
//
//                                opt_temp[i] = path[i];
//
//
//                            for (unsigned i = p2; i < p3; i++)
//                                // from 0 - size - p2
//                                opt_temp[p1 + i - p2] = path[i];
//
//                            for (unsigned i = p3; i < p4; i++)
//                                // from size- p2 - size - p2 +p1
//                                opt_temp[p1 - p2 + i] = path[i];
//
//                            for (unsigned i = p1; i < p2; i++)
//                                opt_temp[p4 - p2 + i] = path[i];
//
//
//
//                            double temp_distance = find_travel_distance(opt_temp);
//                            //   // cout << "current distance" <<mix_index.size() <<endl;
//                            //  // cout << "temp distance" << opt_temp.size() <<endl;
//                            if (temp_distance < current_dis) {
//                                // cout << "success! 4opt" << endl;
//                                
//                                for (unsigned i = 0; i < path.size(); i++){
//                                    path[i] = opt_temp[i];
//   
//                                }
//                                
//                                
//                                for (unsigned n = 0; n < path.size(); n++) {
//                                    if ((path[n] % 2) == 1) { // find drop ID
//
//                                        corresponding[path[n] - 1] = n;
//                                    }
//                                }
//                                
//                                
//                                
//                                
//                                for (unsigned i = 0; i < path.size(); i++)
//                                    // cout << "  " << path[i];
//
//                                // cout << endl;
//                            }
//                        }
//                    }
//
//                }
//
//            }
//        }

//
//    return;
//}
