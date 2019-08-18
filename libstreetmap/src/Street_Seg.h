/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Street_Seg.h
 * Author: chenhu49
 *
 * Created on January 31, 2018, 3:28 PM
 */

#ifndef STREET_SEG_H
#define STREET_SEG_H

#include <string>
#include "m1.h"
#include "StreetsDatabaseAPI.h"


struct Street_Seg {

    //OSMID ssOSMID; 
    //unsigned from, to;  //the id of its intersections
    //unsigned streetID; //the street it belongs in
    double length;
    double weight; //describes the time that needed to travel down the street 
};


//Functions


#endif /* STREET_SEG_H */

