/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "genetic.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <array>

using namespace std;


#define PC 0.9              //the chance of crossing over
#define PM 0.1             //the chance of mutation
#define COUNT 30          //max generations

int num; //global definition of number of cities
//vector<vector<double>> path; //global definition of weight between cities
//int** city; //the order of visiting for every individual
double* dis; //the weight for every visiting of an individual
double* fitness; //how well can one individual fits in the environment

//double path[10][10] = {
//    {0, 23, 93, 18, 40, 34, 13, 75, 50, 35},
//    {23, 0, 75, 4, 72, 74, 36, 57, 36, 22},
//    {93, 75, 0, 64, 21, 73, 51, 25, 74, 89},
//    {18, 4, 64, 0, 55, 52, 8, 10, 67, 1},
//    {40, 72, 21, 55, 0, 43, 64, 6, 99, 74},
//    {34, 74, 73, 52, 43, 0, 43, 66, 52, 39},
//    {13, 36, 51, 8, 64, 43, 0, 16, 57, 94},
//    {75, 57, 25, 10, 6, 66, 16, 0, 23, 11},
//    {50, 36, 74, 67, 99, 52, 57, 23, 0, 42},
//    {35, 22, 89, 1, 74, 39, 94, 11, 42, 0}
//};
int POPULATION;
double min_dis = 999999;
vector<int> min_path;

//initialization of the group

void init() {

    //    int* a = new int[num];
    //    for (int i = 0; i < num; i++) {
    //        a[i] = i;
    //    }
    //
    //    city = new int*[POPULATION];
    //    for (int i = 0; i < POPULATION; i++) {
    //        city[i] = new int[num];
    //    }
    //
    //    for (int i = 0; i < POPULATION; i++) {
    //        for (int j = num - 1; j >= 0; j--) {
    //            int n = rand() % (j + 1); //the output is between 0 ~ j
    //            swap(a[j], a[n]); //the number in a is between 0 ~ (num-1)
    //            city[i][j] = a[j];
    //            //cout << city[i][j];
    //        }
    //        //cout << endl;
    //        makeLegal(city[i]);
    //    }
    //    
    //
    //    delete[] a;
    min_dis = 999999;
    dis = new double[POPULATION];
    fitness = new double[POPULATION];
    min_path.clear();
    min_path.resize(num);
}



//compute fitness

void compute(vector<vector<double>>&path, vector<vector<int>>&city, vector<double>& depot_distance) {
    // cout << "do compute now" << endl;
    double total = 0;
    for (int i = 0; i < POPULATION; i++) {
        //compute the weight of the every case
        dis[i] = 0;
        int a = city[i][0];
        int b;

        for (int j = 1; j < num; j++) {
            b = city[i][j];
            //            cout << "a: " << a << endl;
            //            cout << i << " " << j << " " << city[i][j] << endl;
            dis[i] += path[a][b];
            a = b;
        }

        dis[i] += depot_distance[city[i][0]];
        dis[i] += depot_distance[city[i][num-1]];      
        fitness[i] = 10000000.0 / dis[i]; //use the inverse of distance as fitness
        total += fitness[i];
        //cout << i << ": " << fitness[i] << endl;
        //cout << dis[i] << endl;
    }
}


//select better individual with (roulette wheel selection)
//note: lun pan du suan fa

int select() {
    double total = 0;
    for (int i = 0; i < POPULATION; i++) {
        total += fitness[i];
    }
    RanGen2 temp(num);
    
    double size = double(temp.generate()) / double(num)*total*10; //make sure to not have a 0 size
    //cout << "size" << size << endl;

    double sum = 0;
    int i = 0;

    while (sum <= size && i < POPULATION) {
        sum += fitness[i];
        i += 1;
    }

    return --i; //return the selected one
}



//some simple functions for general use purpose

int getMinDis() {
    double result = dis[0];
    int index = 0;
    for (int i = 1; i < POPULATION; i++) {
        if (result > dis[i]) {
            result = dis[i];
            index = i;
        }
    }
    return index;
}

int getMaxDis() {
    double result = dis[0];
    int index = 0;
    for (int i = 1; i < POPULATION; i++) {
        if (result < dis[i]) {
            result = dis[i];
            index = i;
        }
    }
    return index;
}

double getdis() {
    double result = dis[0];
    //int index = 0;
    for (int i = 1; i < POPULATION; i++) {
        if (result > dis[i]) {
            result = dis[i];
            //index = i;
        }
    }
    return result;
}

void save(vector<vector<int>>&city) {
    int current_min_index = getMinDis();
    int current_max_index = getMaxDis();
    //cout << current_min_index << endl;
    if (dis[current_min_index] < min_dis) {
        min_dis = dis[current_min_index];

        for (int i = 0; i < num; i++) {
            min_path[i] = city[current_min_index][i];
            //cout << min_path[i] << endl;
        }
<<<<<<< .mine
        //cout << "current min dis is: " << min_dis << endl;
=======
       // cout << "current min dis is: " << min_dis << endl;
>>>>>>> .r227
    } else {
        for (int i = 0; i < num; i++) {
            if (min_path[i] < 0 || min_path[i] > 9) {
                //                cout << i << endl;
                //                cout << min_path[i] << endl;
            }
            city[current_max_index][i] = min_path[i];
            //            cout << endl;
            //            cout << city[current_max_index][i] << endl;
        }

        dis[current_max_index] = min_dis;
        fitness[current_max_index] = 1.0 / min_dis;
    }
}

bool isExist(int value, int* array, int len) {
    for (int i = 0; i < len; i++) {
        if (value == array[i]) {
            return true;
        }
    }

    return false;
}

void convert(int p1, int p2, int* src, int* dst) {
    int len = p2 - p1 + 1;
    int* temp = new int[len];

    for (int i = p1; i <= p2; i++) {
        temp[i - p1] = src[i];
    }

    int j = (p2 + 1) % num;
    for (int i = 1; i <= num; i++) {
        int index = (i + p2) % num;

        if (!isExist(dst[index], temp, len)) {
            dst[j] = dst[index];
            j = (j + 1) % num;
        }
    }

    for (int i = p1; i <= p2; i++) {
        dst[i] = src[i];
    }

    delete[] temp;

}

void evolve(int p, int* src, int* dst) {
    //cout << "P is " << p << endl;
    int* temp;
    temp = new int[num];

    //first, inherit from parent 1
    for (int i = 0; i < p; i++) {
        temp[i] = src[i];
    }

    //    for (int i =0; i < num; i++){
    //        cout << i << ": " << temp[i] << " ";
    //    }
    //cout << endl;
    int current = p;
    for (int i = 0; i < num; i++) {
        for (int j = p; j < num; j++) {
            //if found it 
            if (src[j] == dst[i]) {
                temp[current] = dst[i];
                current++;
            }
        }
    }

    //    for (int i =0; i < num; i++){
    //        dst[i] = temp[i];
    //        cout << i << ": " << dst[i] << " ";
    //    }
    //    cout << endl;

    delete[] temp;
}

void makeLegal(int* src) {

    vector<int> pick(num, 0);
    //    for (int i = 0; i < num; i++) {
    //        cout << src[i];
    //    }
    //    cout << endl;

    for (int i = 0; i < num - 1; i++) {
        //only check for drop off locations
        if (src[i] % 2 != 0 && pick[src[i] - 1] != 1) {
            //find where the pick up is
            int j = i + 1;
            //cout << j << endl;
            //cout << src[j] << endl;
            while (src[j] != (src[i] - 1)) {
                j++;
            }

            //when find it, do a swap
            int temp = src[j];
            //  cout << "temp: " << temp << endl;
            src[j] = src[i];
            src[i] = temp;
            pick[temp] = 1;
        } else {
            pick[src[i]] = 1;
        }
    }
    //pick.clear();

    //    cout << "new: " << endl;
    //    for (int i = 0; i < num; i++) {
    //        cout << src[i];
    //    }
    //cout << endl;
}

void makeLegal_vec(vector<int>& src) {

    vector<int> pick(num, 0);
    //    for (int i = 0; i < num; i++) {
    //        cout << src[i];
    //    }
    //    cout << endl;

    for (int i = 0; i < num - 1; i++) {
        //only check for drop off locations
        if (src[i] % 2 != 0 && pick[src[i] - 1] != 1) {
            //find where the pick up is
            int j = i + 1;
            //cout << j << endl;
            //cout << src[j] << endl;
            while (src[j] != (src[i] - 1)) {
                j++;
            }

            //when find it, do a swap
            int temp = src[j];
            //  cout << "temp: " << temp << endl;
            src[j] = src[i];
            src[i] = temp;
            pick[temp] = 1;
        } else {
            pick[src[i]] = 1;
        }
    }
    //pick.clear();

    //    cout << "new: " << endl;
    //    for (int i = 0; i < num; i++) {
    //        cout << src[i];
    //    }
    //cout << endl;
}


//cross in order

void cross(vector<vector<int>>&city) {
    //cout << "do cross now. " << endl;
    for (int k = 0; k < POPULATION; k += 2) {
        int a = select();
        int b = select();

        while (a == b) {
            b = select(); //make sure not to select the same individual
            //cout << "same" << b << endl;
        }
        
        RanGen2 temp(num);
        //cout << "choose population" << a << " " << b << endl;
        double p = double (temp.generate()) / double (num);

        // cout << "cross rate is " << p << endl;

        int* a1 = new int[num];
        int* a2 = new int[num];
        int* b1 = new int[num];
        int* b2 = new int[num];

        for (int i = 0; i < num; i++) {
            a1[i] = city[a][i];
            a2[i] = city[b][i];
            b1[i] = a2[i];
            b2[i] = a1[i];
        }

        if (p < PC) { //it meets the condition
            //            int p1 = -1;
            //            int p2 = -1;
            //
            //            while (p1 == p2) {
            //                p1 = rand() % num;
            //                p2 = rand() % num;
            //
            //                if (p1 > p2) { //make sure p1 < p2
            //                    swap(p1, p2);
            //                }
            //            }
            int p1 = temp.generate();
            //cout << "choose pos " << p1 << " " << p2 << endl; //start to cross

            //            for (int i = 0; i < num; i++){
            //                cout << b1[i] << "   " << b2[i] <<endl;               
            //            }

            //cout << endl << endl;
            //            convert(p1, p2, a1, b1);
            //            convert(p1, p2, a2, b2);
            //            makeLegal(b1);
            //            makeLegal(b2);
            evolve(p1, a1, b1);
            evolve(p1, a2, b2);
            //                        for (int i = 0; i < num; i++){
            //                            cout << b1[i] << "   " << b2[i] <<endl;               
            //                        }
            //           
            for (int i = 0; i < num; i++) {
                city[k][i] = b1[i];
                city[k + 1][i] = b2[i];
                //                cout << "too big??? " << k + 1 << endl;
            }

        } else {
            for (int i = 0; i < num; i++) {
                city[k][i] = a1[i];
                city[k + 1][i] = a2[i];
                //                cout << "too big??? " << k + 1 << endl;
            }
        }

        delete[] a1;
        delete[] a2;
        delete[] b1;
        delete[] b2;
    }
}


//mutation, using exchange method

void morphis(vector<vector<int>>&city) {
    //cout << "do morphis now. " << endl;
    RanGen2 temp(num);
    for (int i = 0; i < POPULATION; i++) {
        
        double p = double(temp.generate()) / double(num);
        //cout << "morphis rate is " << p << endl;
        
        if (p < PM) { //start mutation
            int a = -1;
            int b = -1;

            while (a == b) {
                
                a = temp.generate();
                b = temp.generate();
            }
            //            cout << "a: " << a << endl;
            //            cout << "b: " << b << endl;
            swap(city[i][a], city[i][b]);
            //            swap(city[i][a+1], city[i][b+1]);
            //            swap(city[i][a+2], city[i][b+2]);
            makeLegal_vec(city[i]);
            //            cout << "mutated!" << endl;
        }
    }
}


//clear the memory

void dispose() {
    //    for (int i = 0; i < POPULATION; i++) {
    //        delete[] city[i];
    //    }
    //
    //    delete[] city;
    delete[] dis;
    delete[] fitness;
}



// main function

vector<int> genetic(int _num, int pop, vector<vector<double>>&path, vector<vector<int>>&city, vector<double>& depot_distance) {

    num = _num;
    POPULATION = pop;
    //path = _path;
    init();

    compute(path, city, depot_distance);
    int i = 0;

    while (i < COUNT) {
        cross(city);
        morphis(city);
        compute(path, city, depot_distance);
        save(city); //save the best individual



        //        if (++i % 10 == 0) {
        //            cout << endl;
        //        }
        //        for (int k = 0; k < POPULATION; k++) {
        //            for (int j = 0; j < num; j++) {
        //                // cout << city[k][j];
        //            }
        //            //  cout << endl;
        //        }

//                cout << "count " << i << endl;
//                cout << getdis() << " ";
        i++;
    }
    //#pragma omp parallel for
    //    for (int i = 0; i < COUNT; i += 8) {
    //        cross(city);
    //        morphis(city);
    //        compute(path, city, depot_distance);
    //        save(city); //save the best individual
    //    }
    //
    //#pragma omp parallel for
    //    for (int i = 1; i < COUNT; i += 8) {
    //        cross(city);
    //        morphis(city);
    //        compute(path, city, depot_distance);
    //        save(city); //save the best individual
    //    }
    //
    //#pragma omp parallel for
    //    for (int i = 2; i < COUNT; i += 8) {
    //        cross(city);
    //        morphis(city);
    //        compute(path, city, depot_distance);
    //        save(city); //save the best individual
    //    }
    //
    //#pragma omp parallel for
    //    for (int i = 3; i < COUNT; i += 8) {
    //        cross(city);
    //        morphis(city);
    //        compute(path, city, depot_distance);
    //        save(city); //save the best individual
    //    }
    //
    //#pragma omp parallel for
    //    for (int i = 4; i < COUNT; i += 8) {
    //        cross(city);
    //        morphis(city);
    //        compute(path, city, depot_distance);
    //        save(city); //save the best individual
    //    }
    //
    //#pragma omp parallel for
    //    for (int i = 5; i < COUNT; i += 8) {
    //        cross(city);
    //        morphis(city);
    //        compute(path, city, depot_distance);
    //        save(city); //save the best individual
    //    }
    //
    //#pragma omp parallel for
    //    for (int i = 6; i < COUNT; i += 8) {
    //        cross(city);
    //        morphis(city);
    //        compute(path, city, depot_distance);
    //        save(city); //save the best individual
    //    }
    //
    //#pragma omp parallel for
    //    for (int i = 7; i < COUNT; i += 8) {
    //        cross(city);
    //        morphis(city);
    //        compute(path, city, depot_distance);
    //        save(city); //save the best individual
    //    }


//    compute(path, city, depot_distance);
//    cout << "min distance is: " << min_dis << endl;

    //    for (int j = 0; j < num; j++) {
    //        cout << min_path[j] << " ";
    //        cout << endl;
    //    }

    dispose();

    return min_path;
}