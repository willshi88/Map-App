/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   global.cpp
 * Author: zhan3114
 * 
 * Created on February 5, 2018, 12:49 PM
 */

#include "global.h"
#include "math.h"
global::global() {
}


global::~global() {
}


 


double global::find_Length(unsigned id){
    double distance = 0.0;
    unsigned int numOfCurvePoints = getStreetSegmentInfo(id).curvePointCount;
    
    //no curve points
    if (numOfCurvePoints == 0) {
        return find_distance_between_two_points(
                getIntersectionPosition(getStreetSegmentInfo(id).from),
                getIntersectionPosition(getStreetSegmentInfo(id).to));
    } else {
        //first, get the distance between intersection1 and first curvePoint
        distance += find_distance_between_two_points(
                getIntersectionPosition(getStreetSegmentInfo(id).from),
                getStreetSegmentCurvePoint(id, 0));
        //second, get the distance between intersection2 and the last curve Point
        distance += find_distance_between_two_points(
                getIntersectionPosition(getStreetSegmentInfo(id).to),
                getStreetSegmentCurvePoint(id, numOfCurvePoints - 1));
        //last, add up all of streets in between
        for (unsigned int i = 0; i < numOfCurvePoints - 1; i++) {
            distance += find_distance_between_two_points(
                    getStreetSegmentCurvePoint(id, i),
                    getStreetSegmentCurvePoint(id, i + 1));
        }
    }
    return distance;
}
