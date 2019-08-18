/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   genetic.h
 * Author: chenhu49
 *
 * Created on March 31, 2018, 4:23 PM
 */

#ifndef GENETIC_H
#define GENETIC_H

#include "global.h"

#include <cassert>
#include <ctime>
#include <cstdlib>
#include <random>
#include <fstream>

class RanGen2{
public:   
    std::mt19937 generator;
   
    std::uniform_int_distribution<unsigned> distribution;   
        RanGen2(unsigned num_of_cities):
    generator({std::random_device{}()}), distribution(0, num_of_cities - 1){
        
    }
    
    unsigned generate(){
        return distribution(generator);
    }
    
};


void init();
void compute(vector<vector<double>>& path, vector<vector<int>>& city, vector<double>& depot_distance);
int select();
int getMinDis();
int getMaxDis();
double getdis();
void save(vector<vector<int>>& city);
bool isExist(int value, int* array, int len);
void makeLegal(int* src);
void makeLegal_vec(vector<int>& src);
void evolve(int p, int* src, int* dst);
void convert(int p1, int p2, int* src, int* dst);
void cross(vector<vector<int>>& city);
void morphis(vector<vector<int>>& city);
void dispose();
vector<int> genetic(int _num, int pop,
        vector<vector<double>>& path, 
        vector<vector<int>>& city,
        vector<double>& depot_distance
) ;

#endif /* GENETIC_H */

