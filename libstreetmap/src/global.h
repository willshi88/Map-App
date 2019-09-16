#pragma once
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   global.h
 * Author: zhan3114
 *
 * Created on February 5, 2018, 12:49 PM
 */

#ifndef GLOBAL_H
#define GLOBAL_H
#include "string"
#include "Street.h"
#include "Street_Seg.h"
#include "vector"
#include "set"
#include "intersection.h"
#include "map"
#include "unordered_map"
#include "unordered_set"
#include "graphics.h"
#include "list"
#include <math.h>
#include "queue"

#include <thread>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <random>
#include <fstream>
#include <chrono>
#include "algorithm"
#include <assert.h>
#include "genetic.h"
#include <chrono>
#define LANDMARKS_NUM 5
using namespace std;
#define TIME_LIMIT   30

class featureP{
public:
    t_point *fpoints;
    int size;
    unsigned featureIndex;
    featureP(){
        
    }
    ~featureP(){
        if(fpoints != NULL)
        delete [] fpoints;
    }
    featureP(const featureP & temp){
        size = temp.size;
        fpoints = new t_point[size];
        featureIndex = temp.featureIndex;
        for (int i = 0; i < size; i++){
            fpoints[i] = temp.fpoints[i];
        }
    }

};



/*************used for definition of priority queue*************************/
class Node {
public:
    IntersectionIndex intersection_ID;
    double G;
    
    Node(IntersectionIndex _intersection_ID, double _G){
        intersection_ID = _intersection_ID; 
        G = _G;

    }
    
};

struct cmp{
    bool operator () (const Node & n1, const Node & n2 ) const{
        
        return n1.G > n2.G;
    }
   
};
/******************************************************************************/
class global {
public:
    
    double x_min, x_max, y_min, y_max;
    double x_scale, y_scale;
   double zoomScale;
   double drawScale;
   double max_speed = 0;
   std::string OSMpath; 
   bool intersectionSelected = false;
   bool zoomnavi = false;
   bool error = false;
   unsigned start_int;
   unsigned end_point;
   unsigned poi_dest;
   std::vector<Street_Seg> ssArray;
   std::vector<t_point> alreadyDrewTextPos;
   std::vector<t_point> highwaySegmentsCenterPos;
   std::vector<unsigned> platinumSegment;
   std::vector<unsigned> goldSegment;
   std::vector<unsigned> silverSegment;
   std::vector<unsigned> regularSegment;
   std::vector<unsigned> minorSegment;
   std::vector<LatLon> metroStations;
   std::vector<LatLon> hospital;
   std::vector<unsigned> hospitalID;
   
   
   // tear apart intersection
   std::vector<vector<unsigned>> int_ssid;  // THE STREET SEG ID CONNECTED WITH INT
   std::vector<vector<unsigned>> int_intids; // aCCESSIBLE INT IDS, FROM INT
   std::vector<vector<unsigned>> int_valid_seg; // accessible segments from int
   std::vector<vector<unsigned>> int_seg_street_ID;
   std::vector<vector<double>> int_weight; // int seg weight cor to seg
   
   
   std::vector<unsigned> seg_street_ID;
   
   //Special structure used to store "invalid" segments, intersections information
   std::vector<vector<unsigned>> int_intids_rev; 
   std::vector<vector<unsigned>> int_valid_seg_rev; 
   std::vector<vector<unsigned>> int_seg_street_ID_rev;
   std::vector<vector<double>> int_weight_rev; 
   
   std::vector<vector<unsigned> >int_intids_nodup;
   std::vector<vector<unsigned> >int_ssid_nodup;
   std::vector<vector<unsigned> >int_seg_street_ID_nodup;
   std::vector<vector<double> >int_weight_nodup;  
   
   std::vector<vector <int> > int_shortest_segment;//intersection_ID to shortest segment
   std::vector<vector <int> > int_second_shortest_segment;//second shortest segments
   
   std::vector<vector <int> > int_shortest_segment_rev;

   std::vector<std::vector <double> > landmarks_to_int;
   std::vector<std::vector <double> > int_to_landmarks;

   
   // tear apart street segment
   std::vector<double> seg_weight; //segments weight (all)
   
   //m4 structure
   vector<vector<double>> travel_distance;
   vector<double> depot_distance;
   //This is the Tabu table to tabu certain solutions, remember to initialize
   //in the procedure of optimize, according to number of cities            
   vector< vector<int> > Tabu_List; 
   /****************These two structure should be renewed together*/
   //Used to cooperate with aspiration rule, saving the corresponding 
   //cost energy of the solution in Tabu_List
   vector<double> Tabu_List_Corresponding_Value;
   chrono::high_resolution_clock::time_point STARTING_TIME;

   std::vector<LatLon> fuelstation;
   std::vector<std::vector<std::vector<t_point>>> metro;
   std::vector<t_color> metroColor;
   
   std::set<std::string> street_pointOfInterest_names_fuzzy_only;
   std::set<std::string> pointOfInterest_intersection_name_fuzzy_only;
   std::vector<unsigned> featureID;
   
   std::vector<Street> s_id_array;
   std::vector<Intersection> siArray;
   
   std::set<std::string> intersectionName_fuzzy;
   std::multimap<double, unsigned> areaID;
   std::multimap<string, vector<unsigned>> POIName;
   std::unordered_map<std::string, std::set<unsigned> > streetName_to_intersectionsID;
   std::unordered_map<std::string, std::vector<unsigned> > streetName_to_streetID;
   std::unordered_map<OSMID, unsigned>  OSMIDToNodeIndex;
   std::unordered_map<OSMID, unsigned>  OSMIDToWayIndex;
   std::unordered_map<OSMID, unsigned>  OSMIDToRelationIndex;
   std::vector<std::vector<unsigned>> subwayWayIndex;
   std::vector<std::vector<t_point>> subwayStations;
   std::vector<t_point> drawTextPos;
   std::vector<t_point> alreadyDrewHighwayTextPos;
   std::vector<LatLon> restaurant;
   std::vector<unsigned> restaurantID; 
   std::vector<unsigned> shortestDij;
   std::vector<unsigned> naviRoute;
   std::vector<unsigned> naviRoute_seg;
   std::vector<featureP> areaFeatures;
   std::vector<unsigned> turn_by_turn;
   std::vector<unsigned> dongnanxibei;
   std::vector<unsigned> navi_direction;
   std::vector<unsigned> deliveryRoute;
   
   std::vector<unsigned> draw_turn;
   std::vector<unsigned> draw_dongnanxibei;
   std::vector<unsigned> draw_street_seg;
   std::vector<double> move_distance;
   std::vector<unsigned> find_interest;
   unsigned found_interest;
   
   std::vector<LatLon> bank;
   std::vector<unsigned> bankID;
   std::vector<LatLon> edu;
   std::vector<unsigned> eduID;
   
   double** weightedMatrix;
   
   bool load_successful = false;
   double latavg;
   double find_Length(unsigned id);

   global();
   global(const global& orig);
   virtual ~global();
   t_point LatLon_to_real_XY2(LatLon position) {
    t_point pt;
    pt.x = DEG_TO_RAD * position.lon() * cos(DEG_TO_RAD * latavg);
    pt.y = DEG_TO_RAD * position.lat();
    return pt;
}
private:

};




#endif /* GLOBAL_H */

