/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Street.h
 * Author: chenhu49
 *
 * Created on January 30, 2018, 10:00 PM
 */

#ifndef STREET_H
#define STREET_H

#include <string>
#include <iostream>
#include "StreetsDatabaseAPI.h"
#include "set"
using namespace std;

struct Street {
    string name;
    double length;
    std::vector<unsigned> street_segment_ids;
    std::set<unsigned> intersection_ids;
};


#endif /* STREET_H */

