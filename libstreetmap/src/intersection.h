/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   intersection.h
 * Author: zhan3795
 *
 * Created on January 31, 2018, 7:04 PM
 */

#ifndef INTERSECTION_H
#define INTERSECTION_H
#endif /* INTERSECTION_H */
#include "string"
#include <set>
struct Intersection{
    double x;
    double y;
    //array of connected street ids
    std::vector<unsigned> s_ids;
    //array of street segment ids
    std::vector<unsigned> ss_ids;
    std::set<std::string> s_names;
    std::vector<unsigned> valid_seg;   
    //The segment weight between itself and its parent intersection 
    StreetSegmentIndex segment_ID = -1;
    std::vector<unsigned> in_ids;
    std::vector<double> weight;
    
};

