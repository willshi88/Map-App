#include "m3.h"
#include "global.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m1.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "graphics.h"
#include "iterator"
#include "m1.h"
#include <boost/heap/fibonacci_heap.hpp> 
#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)


using namespace std;
const double INF = 99999;
vector<unsigned> reconstruct(unsigned start, unsigned end);
extern global globalVar;

void find_shortest_dij(unsigned start, unsigned end, double penalty);
void get_segment_array();

extern void highlight_intersection(unsigned intersection_id);
unsigned get_turn(unsigned sid1, unsigned sid2);

bool turned(unsigned int1, unsigned int2, unsigned int3);

vector<unsigned> get_poi_from_name(const string name);

unsigned get_dongnanxibei(unsigned sid1, unsigned sid2);

int find_segment_ID_from_intersections(unsigned int1, unsigned int2, int prevStreetID, double turn_penalty);
int find_selected_segment_ID_from_intersections(unsigned int1, unsigned index, int prevStreetID, double turn_penalty);

std::vector<unsigned> Dijkstra(const unsigned intersect_id_start,
        const unsigned intersect_id_end, const double turn_penalty);


double compute_path_travel_time(const std::vector<unsigned>& path,
        const double turn_penalty) {
    // if the route is empty
    if (path.empty() == true)
        return -1;
    double travel_time = 0;
    // loop through all segments and get travel total time
    for (unsigned segment = 0; segment < path.size(); segment++) {
        travel_time += find_street_segment_travel_time(path[segment]);
        if (segment != 0) {
            // add turn penalty
            if (getStreetSegmentInfo(path[segment]).streetID != getStreetSegmentInfo(path[segment - 1]).streetID)
                travel_time += turn_penalty;
        }
    }

    return travel_time;
}

std::vector<unsigned> find_path_between_intersections(const unsigned intersect_id_start,
        const unsigned intersect_id_end, const double turn_penalty) {
    
    globalVar.naviRoute_seg.clear();
    
    //to order the intersection and pick the one with smallest 
    std::priority_queue<Node, vector<Node>, cmp> openQueue;
    
    //boost::heap::fibonacci_heap< Node, boost::heap::compare <cmp> > openQueue;
    
    //Vector to store the current intersection's parent intersection
    std::vector<int> parentInt(getNumberOfIntersections(), -1);
      
    parentInt[intersect_id_start] = -100;
      
    std::vector<double> F_Vector(getNumberOfIntersections(), -1);
    
    std::vector<double> G_Vector(getNumberOfIntersections(), -1);
    
    G_Vector[intersect_id_start] = 0;
    
    std::vector<int> parentSeg(getNumberOfIntersections(), -1);
    
    parentSeg[intersect_id_start] = -100;
    
    std::vector<bool> closeVector(getNumberOfIntersections(), false);
    
    //put the starting point position into the open list
    openQueue.push(Node(intersect_id_start, 0));

    while(!openQueue.empty()){
      
            IntersectionIndex current_intersection_ID = openQueue.top().intersection_ID;
            
            if(closeVector[current_intersection_ID] == true){
                openQueue.pop();
                continue;               
            }
         
        //end condition, if next point to be explored is the end intersection ID
        if (current_intersection_ID == intersect_id_end) {
            
            //If the end intersection is found   
            int result = intersect_id_end;
            
            while (parentSeg[result] >= 0) {                
                globalVar.naviRoute_seg.insert(globalVar.naviRoute_seg.begin(), parentSeg[result]);
                result = parentInt[result];                
            }

            return globalVar.naviRoute_seg;
        } 

        //Delete the intersection ID with lowest F
        openQueue.pop();
       
        //put the intersection into close Vector
        closeVector[current_intersection_ID] = true;
                
        //highlight_intersection(current_intersection_ID);
         
         //traverse all the valid intersection
        for (unsigned index = 0; index < globalVar.int_valid_seg[current_intersection_ID].size(); ++index) {

            unsigned selected_segment_ID = globalVar.int_valid_seg[current_intersection_ID][index];
            
            IntersectionIndex potential_intersection_ID = globalVar.int_intids[current_intersection_ID][index];
            
            if(closeVector[potential_intersection_ID]){
                continue;               
            }

         //value that will be iterated
         
          //If the current intersection's parent is starting point, then choose the shortest segment, 
                //no need to decide turn penalty 
          
          double candidate_G = 0, candidate_F = 0, candidate_H = 0;
            
          /*******Here is to calculate Heuristic value*/  
          
          double H_landmark_to_end = 0;
          double H_landmark_to_potential = 0;
          double H_potential_to_landmark = 0;
          double H_end_to_landmark = 0;
          double temp_H = 100000;
          
          //Loop all landmarks to intersection cost
          for(int index = 0; index < globalVar.landmarks_to_int.size(); index++){

               H_landmark_to_potential = globalVar.landmarks_to_int[index][potential_intersection_ID];              
               
               H_landmark_to_end = globalVar.landmarks_to_int[index][intersect_id_end];                           
                              
               H_potential_to_landmark = globalVar.int_to_landmarks[index][potential_intersection_ID];
               
               H_end_to_landmark = globalVar.int_to_landmarks[index][intersect_id_end];
               
               double H1 = H_potential_to_landmark - H_end_to_landmark;
               
               double H2 = H_landmark_to_end - H_landmark_to_potential;
               
               temp_H = std::max(H1, H2);
              
               if(temp_H > candidate_H)
                   candidate_H = temp_H;                       
          }
      
          //Then it is necessary to calculate turn penalty  
            
          if (parentInt[current_intersection_ID] != -100) {                                                
             
              candidate_G = G_Vector[current_intersection_ID] + globalVar.ssArray[selected_segment_ID].weight;             
                           
              if(globalVar.seg_street_ID[parentSeg[current_intersection_ID]] != 
                      globalVar.int_seg_street_ID[current_intersection_ID][index]) 
                  candidate_G = candidate_G + turn_penalty; 
              
              candidate_F = candidate_G + candidate_H;
            }
          
          else{
          
           selected_segment_ID = globalVar.int_shortest_segment[current_intersection_ID][index]; 
            
          candidate_G = G_Vector[current_intersection_ID] + globalVar.ssArray[selected_segment_ID].weight;
          
          candidate_F = candidate_G + candidate_H;
          }          
          
          if (candidate_F >= F_Vector[potential_intersection_ID] && F_Vector[potential_intersection_ID] != -1)
                    continue;               
 
          openQueue.push(Node(potential_intersection_ID, candidate_F));
          
          //Finally store all the iterative values
          parentInt[potential_intersection_ID] = current_intersection_ID;
          parentSeg[potential_intersection_ID] = selected_segment_ID;
          G_Vector[potential_intersection_ID] = candidate_G;
          F_Vector[potential_intersection_ID] = candidate_F;
                      
        }    
    }   
    
        std::vector<unsigned> no_path;
        return no_path;
}

void get_segment_array() {
    globalVar.dongnanxibei.clear();
    globalVar.turn_by_turn.clear();
    
    if (globalVar.naviRoute_seg.size() == 0) return;
    
    // loop along all the segments to get navigation information
    for (unsigned i = 0; i < globalVar.naviRoute_seg.size() - 1; i++) {
        //To record turn information
        if (getStreetSegmentInfo(globalVar.naviRoute_seg[i]).streetID !=
                getStreetSegmentInfo(globalVar.naviRoute_seg[i + 1]).streetID) {
            globalVar.turn_by_turn.push_back(get_turn(globalVar.naviRoute_seg[i],
                    globalVar.naviRoute_seg[i + 1]));

        } else {
            globalVar.turn_by_turn.push_back(0);
 
        }
        globalVar.dongnanxibei.push_back(get_dongnanxibei(globalVar.naviRoute_seg[i], globalVar.naviRoute_seg[i + 1]));

    }
    globalVar.turn_by_turn.push_back(0);
    if (globalVar.naviRoute_seg.size() == 1)
        globalVar.dongnanxibei.push_back(get_dongnanxibei(globalVar.naviRoute_seg[0], globalVar.naviRoute_seg[0]));
    globalVar.dongnanxibei.push_back(globalVar.dongnanxibei[globalVar.dongnanxibei.size()-1]);
}


// function to calculate the turn direction of a turn

unsigned get_turn(unsigned sid1, unsigned sid2) {
    unsigned id1 = 0, id2 = 0, id3 = 0;
    // conversion from 2 segments to 3 points
    if (getStreetSegmentInfo(sid1).from == getStreetSegmentInfo(sid2).to ||
            getStreetSegmentInfo(sid1).from == getStreetSegmentInfo(sid2).from) {
        id1 = getStreetSegmentInfo(sid1).to;
        id2 = getStreetSegmentInfo(sid1).from;
        if (id2 == getStreetSegmentInfo(sid2).from)
            id3 = getStreetSegmentInfo(sid2).to;
        else id3 = getStreetSegmentInfo(sid2).from;
   // Determine ther second point
    } else if (getStreetSegmentInfo(sid1).to == getStreetSegmentInfo(sid2).to ||
            getStreetSegmentInfo(sid1).to == getStreetSegmentInfo(sid2).from) {
        id1 = getStreetSegmentInfo(sid1).from;
        id2 = getStreetSegmentInfo(sid1).to;
        if (id2 == getStreetSegmentInfo(sid2).from)
            id3 = getStreetSegmentInfo(sid2).to;
        else id3 = getStreetSegmentInfo(sid2).from;

    }
   
    t_point p1, p2, p3;
    double y_calc;
    double slope = 0;
    p1 = globalVar.LatLon_to_real_XY2(getIntersectionPosition(id1));
    p2 = globalVar.LatLon_to_real_XY2(getIntersectionPosition(id2));
    p3 = globalVar.LatLon_to_real_XY2(getIntersectionPosition(id3));

    // use slope and expect growth to calculate turn
    if (p2.x == p1.x)
        slope = (p2.y - p1.y) * 999999;
    else
        slope = (p2.y - p1.y) / (p2.x - p1.x);

    y_calc = p2.y + slope * (p3.x - p2.x);
    if (p2.x >= p1.x) {
        //        if (slope > 0) {
        if (p3.y > y_calc)
            return 1;
        else if (p3.y < y_calc) return 2;
        else return 0;
    // If x1 is > x2
    } else {
        //        if (slope > 0) {
        if (p3.y > y_calc)
            return 2;
        else if (p3.y < y_calc) return 1;
        else return 0;
    }
    return 0;
}

// get heading direction for turn by turn navigation

unsigned get_dongnanxibei(unsigned sid1, unsigned sid2) {
    // conversion from 2 segments to 3 intersections
    
    unsigned id2 = 0;
    unsigned id3 = 0;
    
    if(sid2 != sid1){
    if (getStreetSegmentInfo(sid1).from == getStreetSegmentInfo(sid2).to ||
            getStreetSegmentInfo(sid1).from == getStreetSegmentInfo(sid2).from) {
        
        id2 = getStreetSegmentInfo(sid1).from;
        if (id2 == getStreetSegmentInfo(sid2).from)
            id3 = getStreetSegmentInfo(sid2).to;
        else id3 = getStreetSegmentInfo(sid2).from;

    } else if (getStreetSegmentInfo(sid1).to == getStreetSegmentInfo(sid2).to ||
            getStreetSegmentInfo(sid1).to == getStreetSegmentInfo(sid2).from) {
    
        id2 = getStreetSegmentInfo(sid1).to;
        if (id2 == getStreetSegmentInfo(sid2).from)
            id3 = getStreetSegmentInfo(sid2).to;
        else id3 = getStreetSegmentInfo(sid2).from;

    }
    }
    
    else{
    
        id2 = globalVar.start_int;
        id3 = globalVar.end_point;
        
    }
    
    t_point p2 = globalVar.LatLon_to_real_XY2(getIntersectionPosition(id2));
    t_point p3 = globalVar.LatLon_to_real_XY2(getIntersectionPosition(id3));

    // Use slope to calculate direction
    double slope = 0;
    if (p3.x > p2.x) { // east side
        slope = (p3.y - p2.y) / (p3.x - p2.x);
        if (slope >= 1.732)
            return 0; //north
        else if (slope < 1.732 && slope >= 0.577)
            return 1; // north-east
        else if (slope < 0.577 && slope >= 0)
            return 2; // east
        else if (slope < 0 && slope >= -0.577)
            return 2; // east
        else if (slope < -0.577 && slope >= -1.732)
            return 3; // south-east
        else if (slope < -1.732)
            return 4; //south
        
      // if its going west worlds
    } else if (p3.x < p2.x) { // west side
        slope = (p3.y - p2.y) / (p3.x - p2.x);
        if (slope >= 1.732)
            return 4; //south
        else if (slope < 1.732 && slope >= 0.577)
            return 5; // south-west
        else if (slope < 0.577 && slope >= 0)
            return 6; // west
        else if (slope < 0 && slope >= -0.577)
            return 6; // west
        else if (slope < -0.577 && slope >= -1.732)
            return 7; // north-west
        else if (slope < -1.732)
            return 0; // north
    } else if (p3.y > p2.y)
        return 0;
    else return 4;

    return 0;



}

// Get a POI's nearest nearest intersection from poi id

vector<unsigned> get_poi_from_name(const string name) {

    vector<unsigned> tempID;
    globalVar.find_interest.clear();
    multimap<string, vector<unsigned>>::iterator start, end, it;
    start = globalVar.POIName.lower_bound(name);
    end = globalVar.POIName.upper_bound(name);
    for (it = start; it != end; it++) {
        tempID.push_back(it->second[1]);
        globalVar.find_interest.push_back(it -> second[0]);

    }
    return tempID;
}
        
// reconstruct turn-by-turn navigation information to prevent large quantities of go straight

void navi_info_reconstruct() {
    unsigned i = 0;
    globalVar.draw_turn.clear();
    globalVar.draw_dongnanxibei.clear();
    globalVar.draw_street_seg.clear();
    globalVar.move_distance.clear();
    double length = 0;
    while (i < globalVar.turn_by_turn.size()) {
        unsigned k = 0;
        // put the straight segments together
        if (globalVar.turn_by_turn[i] != 0) {
            globalVar.draw_turn.push_back(globalVar.turn_by_turn[i]);
            globalVar.draw_street_seg.push_back(globalVar.naviRoute_seg[i+1]);
            globalVar.draw_dongnanxibei.push_back(globalVar.dongnanxibei[i]);
            length += (globalVar.ssArray[globalVar.naviRoute_seg[i]]).length;
            globalVar.move_distance.push_back(length);
            //cout << "turn:  " << globalVar.turn_by_turn[i] 
            //      << endl;
            i = i + 1;
            length = 0;
        } else {

            while (globalVar.turn_by_turn[i + k] == 0 && (i+k)< globalVar.turn_by_turn.size()) {
                length = length + (globalVar.ssArray[globalVar.naviRoute_seg[i + k]]).length;
                k = k + 1;
    }
            i = i + k;
}

    }
    globalVar.draw_turn.push_back(0);
    globalVar.move_distance.push_back(length);
    globalVar.draw_dongnanxibei.push_back(globalVar.dongnanxibei[globalVar.dongnanxibei.size() - 1]);
    globalVar.draw_street_seg.push_back(globalVar.naviRoute_seg[globalVar.naviRoute_seg.size()-1]);

}
