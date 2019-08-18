
/* 
 * Copyright 2018 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <limits>
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "graphics.h"
#include "iterator"
#include "m3.h"
#include <thread>
#include "global.h"



const double INF = 99999;

global globalVar;
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

double maxlat;
double minlat;
double maxlon;
double minlon;
double xmin;
double xmax;
double ymin;
double ymax;
t_point LatLon_to_real_XY1(LatLon & position);
vector<LatLon> wayToNodes(unsigned wayID);
std::vector<double> find_landmarks_to_all_ints_cost(const unsigned intersect_id_start,
        const double turn_penalty);
std::vector<double> find_ints_to_all_landmarks_cost(const unsigned intersect_id_start,
        const double turn_penalty);
void setScale();

extern void highlight_intersection(unsigned intersection_id);

// Declaring the structure for the r-tree

// cite from stack-over-flow: 
// https://stackoverflow.com/questions/42182537/how-can-i-use-the-rtree-of-the-boost-library-in-c?rq=1
// author: founder of boost library
// Date: 2011

struct MyLatLon {

    MyLatLon() {
    }

    MyLatLon(double lat_, double lon_) : ll(lat_, lon_) {
    }

    double get_lat() const {
        return ll.lat();
    }

    double get_lon() const {
        return ll.lon();
    }

    void set_lat(float v) {
        ll = LatLon(v, ll.lon());
    }

    void set_lon(float v) {
        ll = LatLon(ll.lat(), v);
    }

    LatLon ll;
};

// Define the data stored in r-tree
BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(MyLatLon, double,
        bg::cs::spherical_equatorial<bg::degree>,
        get_lon, get_lat, set_lon, set_lat)
// Creating 2 r-trees for intersections and points of interests
typedef std::pair<MyLatLon, unsigned> point_pair;
bgi::rtree<point_pair, bgi::quadratic < 16 >> int_tree;
bgi::rtree<point_pair, bgi::quadratic < 16 >> interest_tree;
// pair the data for r-tree

///   End of citation



double polygonArea(unsigned featureID);
int OSMIDToWayIndex(OSMID osmid);
int OSMIDToNodeIndex(OSMID osmid);
int OSMIDToRelationIndex(OSMID osmid);

bool load_map(std::string map_path) {

    //reset max and min 
    xmin = 999;
    xmax = -999;
    ymin = 999;
    ymax = -999;
    maxlat = -999;
    minlat = 999;
    maxlon = -999;
    minlon = 999;

    //Indicates whether the map has loaded 
    globalVar.OSMpath = map_path.substr(0, map_path.size() - 11).append("osm.bin");

    globalVar.load_successful = loadStreetsDatabaseBIN(map_path);
    if (!globalVar.load_successful) return false;
    //successfully
    //Make sure this is updated to reflect whether

    //Load the map database 

    loadOSMDatabaseBIN(globalVar.OSMpath);
    // Getting max and min latitude of a map   

    //Now build the new street segments vector 
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {

        //create a new vector
        Street_Seg newSS;
        if (getStreetSegmentInfo(i).speedLimit > globalVar.max_speed)
            globalVar.max_speed = getStreetSegmentInfo(i).speedLimit / 3.6;
        newSS.length = globalVar.find_Length(i);
        newSS.weight = newSS.length * 3.6 / getStreetSegmentInfo(i).speedLimit;

        globalVar.seg_weight.push_back(newSS.length * 3.6 / getStreetSegmentInfo(i).speedLimit);



        //insert the new street segment structure into vector
        globalVar.ssArray.push_back(newSS);

        globalVar.seg_street_ID.push_back(getStreetSegmentInfo(i).streetID);


    }

    // cout << globalVar.max_speed << endl;

    //build street vector
    //first create a list of empty streets
    for (unsigned i = 0; i < getNumberOfStreets(); i++) {
        Street newS;
        newS.length = 0.0;
        globalVar.s_id_array.push_back(newS);
    }


    for (unsigned wayIndex = 0; wayIndex < getNumberOfWays(); wayIndex++) {
        const OSMWay* temp = getWayByIndex(wayIndex);
        pair<OSMID, unsigned> way(temp->id(), wayIndex);
        globalVar.OSMIDToWayIndex.insert(way);
    }

    for (unsigned nodeIndex = 0; nodeIndex < getNumberOfNodes(); nodeIndex++) {
        const OSMNode* temp = getNodeByIndex(nodeIndex);
        pair<OSMID, unsigned> node(temp->id(), nodeIndex);
        globalVar.OSMIDToNodeIndex.insert(node);
    }

    for (unsigned i = 0; i < getNumberOfIntersections(); i++) {

        if (getIntersectionPosition(i).lat() > maxlat)
            maxlat = getIntersectionPosition(i).lat();

        if (getIntersectionPosition(i).lat() < minlat)
            minlat = getIntersectionPosition(i).lat();

        if (getIntersectionPosition(i).lon() > maxlon)
            maxlon = getIntersectionPosition(i).lon();

        if (getIntersectionPosition(i).lon() < minlon)
            minlon = getIntersectionPosition(i).lon();

    }
    globalVar.latavg = (maxlat + minlat) / 2;
    setScale();

    //for the inner loop to sort the most selected 




    for (unsigned i = 0; i < getNumberOfIntersections(); i++) {

        double x = DEG_TO_RAD * getIntersectionPosition(i).lon() * cos(DEG_TO_RAD * globalVar.latavg);
        double y = DEG_TO_RAD * getIntersectionPosition(i).lat();
        //Store data into the r-tree for intersections
        double pos[2] = {getIntersectionPosition(i).lat(), getIntersectionPosition(i).lon()};
        int_tree.insert(std::make_pair(MyLatLon(pos[0], pos[1]), i));

        xmax = max(xmax, x);
        xmin = min(xmin, x);
        ymin = min(ymin, y);
        ymax = max(ymax, y);

        //Now build the intersections vector
        Intersection newIn;
        unsigned num = getIntersectionStreetSegmentCount(i);
        vector<unsigned> ssids;
        vector<unsigned> intints;
        vector<unsigned> validseg;
        vector<unsigned> seg_street_ID;
        vector<double> weight;

        vector<unsigned> intints_nodup;
        vector<unsigned> validseg_nodup;
        vector<unsigned> seg_street_ID_nodup;
        vector<double> weight_nodup;


        vector<unsigned> validseg_rev;
        vector<unsigned> seg_street_ID_rev;
        vector<double> weight_rev;
        vector<unsigned> intints_rev;


        vector<unsigned>::iterator it;

        for (unsigned j = 0; j < num; j++) {
            unsigned newSS = getIntersectionStreetSegment(i, j);
            newIn.ss_ids.push_back(newSS);
            ssids.push_back(newSS);

            //find which intersection can directly go to
            if (i == getStreetSegmentInfo(newSS).from) {

                newIn.weight.push_back(globalVar.ssArray[newSS].weight);
                newIn.in_ids.push_back(getStreetSegmentInfo(newSS).to);
                newIn.valid_seg.push_back(newSS);


                validseg.push_back(newSS);
                seg_street_ID.push_back(getStreetSegmentInfo(newSS).streetID);
                intints.push_back(getStreetSegmentInfo(newSS).to);
                weight.push_back(globalVar.ssArray[newSS].weight);





                it = find(intints_nodup.begin(), intints_nodup.end(), getStreetSegmentInfo(newSS).to);
                //No duplicated intersection found
                if (it == intints_nodup.end()) {
                    intints_nodup.push_back(getStreetSegmentInfo(newSS).to);
                    validseg_nodup.push_back(newSS);
                    seg_street_ID_nodup.push_back(getStreetSegmentInfo(newSS).streetID);
                    weight_nodup.push_back(globalVar.ssArray[newSS].weight);

                }                    //There is duplicated intersection, so update the one to have the 
                    //fastest street segment
                else {
                    unsigned index = distance(intints_nodup.begin(), it);

                    if (weight_nodup[index] > globalVar.ssArray[newSS].weight) {
                        weight_nodup[index] = globalVar.ssArray[newSS].weight;
                        validseg_nodup[index] = newSS;
                        seg_street_ID_nodup.push_back(getStreetSegmentInfo(newSS).streetID);

                    }
                }






            } else if (i == getStreetSegmentInfo(newSS).to && !getStreetSegmentInfo(newSS).oneWay) {
                newIn.weight.push_back(globalVar.ssArray[newSS].weight);
                newIn.in_ids.push_back(getStreetSegmentInfo(newSS).from);
                newIn.valid_seg.push_back(newSS);

                validseg.push_back(newSS);
                seg_street_ID.push_back(getStreetSegmentInfo(newSS).streetID);
                intints.push_back(getStreetSegmentInfo(newSS).from);
                weight.push_back(globalVar.ssArray[newSS].weight);



                it = find(intints_nodup.begin(), intints_nodup.end(), getStreetSegmentInfo(newSS).from);
                //No duplicated intersection found
                if (it == intints_nodup.end()) {
                    intints_nodup.push_back(getStreetSegmentInfo(newSS).from);
                    validseg_nodup.push_back(newSS);
                    seg_street_ID_nodup.push_back(getStreetSegmentInfo(newSS).streetID);
                    weight_nodup.push_back(globalVar.ssArray[newSS].weight);

                }//There is duplicated intersection, so update the one to have the 
                    //fastest street segment
                else {
                    unsigned index = distance(intints_nodup.begin(), it);

                    if (weight_nodup[index] > globalVar.ssArray[newSS].weight) {
                        weight_nodup[index] = globalVar.ssArray[newSS].weight;
                        seg_street_ID_nodup.push_back(getStreetSegmentInfo(newSS).streetID);
                        validseg_nodup[index] = newSS;
                    }
                }

            }


            if (i == getStreetSegmentInfo(newSS).to) {
                validseg_rev.push_back(newSS);
                seg_street_ID_rev.push_back(getStreetSegmentInfo(newSS).streetID);
                intints_rev.push_back(getStreetSegmentInfo(newSS).from);
                weight_rev.push_back(globalVar.ssArray[newSS].weight);

            } else if (i == getStreetSegmentInfo(newSS).from && !getStreetSegmentInfo(newSS).oneWay) {

                validseg_rev.push_back(newSS);
                seg_street_ID_rev.push_back(getStreetSegmentInfo(newSS).streetID);
                intints_rev.push_back(getStreetSegmentInfo(newSS).to);
                weight_rev.push_back(globalVar.ssArray[newSS].weight);

            }


            unsigned newS = getStreetSegmentInfo(newSS).streetID;
            newIn.s_ids.push_back(newS);
            newIn.s_names.insert(getStreetName(newS));
            newIn.x = x;
            newIn.y = y;
        }
        /****************Store intersections to intersections related information******/
        globalVar.siArray.push_back(newIn);
        globalVar.int_intids.push_back(intints);
        globalVar.int_ssid.push_back(ssids);
        globalVar.int_valid_seg.push_back(validseg);
        globalVar.int_seg_street_ID.push_back(seg_street_ID);
        globalVar.int_weight.push_back(weight);


        /****************Store intersections to intersections, with no duplications****/
        globalVar.int_intids_nodup.push_back(intints_nodup);
        globalVar.int_ssid_nodup.push_back(validseg_nodup);
        globalVar.int_seg_street_ID_nodup.push_back(seg_street_ID_nodup);
        globalVar.int_weight_nodup.push_back(weight_nodup);

        /********Store the intersections to invalid intersections related information**/
        globalVar.int_intids_rev.push_back(intints_rev);
        globalVar.int_valid_seg_rev.push_back(validseg_rev);
        globalVar.int_seg_street_ID_rev.push_back(seg_street_ID_rev);
        globalVar.int_weight_rev.push_back(weight_rev);

        /******************************************************************************/
        //Store related intersections ID array into street ID
        for (unsigned j = 0; j < globalVar.siArray[i].s_ids.size(); j++) {

            globalVar.s_id_array[globalVar.siArray[i].s_ids[j]].intersection_ids.insert(i);

        }



    }




    globalVar.int_shortest_segment.resize(getNumberOfIntersections());


    for (unsigned intFrom = 0; intFrom < getNumberOfIntersections(); intFrom++) {

        vector<unsigned> valid_intersections = globalVar.int_intids[intFrom];
        int num_of_intTo = valid_intersections.size();

        globalVar.int_shortest_segment[intFrom].resize(num_of_intTo);

        vector<unsigned> valid_segments = globalVar.int_valid_seg[intFrom];
        //Traversing "To" and "To" for nested loop, and 

        for (int validTo_index = 0; validTo_index < num_of_intTo; validTo_index++) {

            //intersection ID "TO"
            unsigned intTo = valid_intersections[validTo_index];
            double selected_cost = 100000;
            int shortest_segID = -1;

            for (unsigned validSeg_index = 0; validSeg_index < valid_segments.size(); validSeg_index++) {

                StreetSegmentInfo info = getStreetSegmentInfo(valid_segments[validSeg_index]);

                if (info.oneWay && (info.to != intTo)) {

                    continue;
                } else if (info.to == intTo || info.from == intTo) {

                    //store current segment cost
                    double current_cost = globalVar.int_weight[intFrom][validSeg_index];


                    if (current_cost < selected_cost) {

                        shortest_segID = valid_segments[validSeg_index];
                        selected_cost = current_cost;

                    }

                }

            }
            globalVar.int_shortest_segment[intFrom][validTo_index] = shortest_segID;

        }

    }






    globalVar.int_shortest_segment_rev.resize(getNumberOfIntersections());


    for (unsigned intFrom = 0; intFrom < getNumberOfIntersections(); intFrom++) {

        vector<unsigned> valid_intersections = globalVar.int_intids_rev[intFrom];
        int num_of_intTo = valid_intersections.size();

        globalVar.int_shortest_segment_rev[intFrom].resize(num_of_intTo);




        vector<unsigned> valid_segments = globalVar.int_valid_seg_rev[intFrom];
        //Traversing "To" and "To" for nested loop, and 

        for (int validTo_index = 0; validTo_index < num_of_intTo; validTo_index++) {

            //intersection ID "TO"
            unsigned intTo = valid_intersections[validTo_index];
            double selected_cost = 100000;
            int shortest_segID = -1;

            for (unsigned validSeg_index = 0; validSeg_index < valid_segments.size(); validSeg_index++) {

                StreetSegmentInfo info = getStreetSegmentInfo(valid_segments[validSeg_index]);

                if (info.oneWay && (info.from != intTo)) {

                    continue;
                } else if (info.to == intTo || info.from == intTo) {

                    //store current segment cost
                    double current_cost = globalVar.int_weight_rev[intFrom][validSeg_index];


                    if (current_cost < selected_cost) {

                        shortest_segID = valid_segments[validSeg_index];
                        selected_cost = current_cost;

                    }

                }

            }
            globalVar.int_shortest_segment_rev[intFrom][validTo_index] = shortest_segID;

        }

    }

    vector<unsigned> landmarks_ID(LANDMARKS_NUM, 0);

    //Find the cost from every landmark to each point
    //Store the random points 
    for (int num = 0; num < LANDMARKS_NUM; num++) {
        unsigned random_intersection_index = rand() % (getNumberOfIntersections() - 1);

        // highlight_intersection(random_intersection_index);

        landmarks_ID[num] = random_intersection_index;


        vector<double> G_Value = find_landmarks_to_all_ints_cost(random_intersection_index, 0);


        globalVar.landmarks_to_int.push_back(G_Value);

    }

    for (int num = 0; num < LANDMARKS_NUM; num++) {

        vector<double> G_Value = find_ints_to_all_landmarks_cost(landmarks_ID[num], 0);


        globalVar.int_to_landmarks.push_back(G_Value);

    }


    /**************storing intersections with "&" and without *********************/
    /**************"<unknown>" for fuzzy search************************************/
    for (unsigned num = 0; num < getNumberOfIntersections(); num++) {

        if ((getIntersectionName(num).find(" & ") != string::npos)
                && (getIntersectionName(num).find("<unknown>") == string::npos)) {

            globalVar.intersectionName_fuzzy.insert(getIntersectionName(num));

        }
    }



    /****************structure used to combine points of interests*****************/
    /*********************and street names for fuzzy search************************/

    for (unsigned i = 0; i < getNumberOfStreets(); i++) {

        globalVar.street_pointOfInterest_names_fuzzy_only.insert(getStreetName(i));
    }



    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        globalVar.street_pointOfInterest_names_fuzzy_only.insert(getPointOfInterestName(i));
    }




    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        globalVar.pointOfInterest_intersection_name_fuzzy_only.insert(getPointOfInterestName(i));
    }

    for (unsigned i = 0; i < getNumberOfIntersections(); i++) {
        globalVar.pointOfInterest_intersection_name_fuzzy_only.insert(getIntersectionName(i));
    }


    /********************Now build the street vector.******************************/
    /***************Store street segments ID and street name***********************/

    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {
        unsigned currentS = getStreetSegmentInfo(i).streetID;
        globalVar.s_id_array[currentS].name = getStreetName(currentS);
        //add current street segment into street
        globalVar.s_id_array[currentS].street_segment_ids.push_back(i);

    }
    //        int osmid = OSMIDToWayIndex(getStreetSegmentInfo(i).wayOSMID);
    //
    //
    //
    //        const OSMWay* temp = getWayByIndex(osmid);
    //        for (unsigned tagNum = 0; tagNum < getTagCount(temp); tagNum++) {
    //            string key, value;
    //            tie(key, value) = getTagPair(temp, tagNum);
    //            if (key == "highway" && (value == "motorway" || value == "trunk" || value == "primary")) {
    //                globalVar.platinumSegment.push_back(i);
    //                break;
    //            }
    //            if (key == "highway" && (value == "motorway_link" || value == "trunk_link" || value == "primary_link")) {
    //                globalVar.goldSegment.push_back(i);
    //                break;
    //            }
    //
    //            if (key == "highway" && (value == "secondary" || value == "secondary_link")) {
    //                globalVar.silverSegment.push_back(i);
    //                break;
    //            }
    //
    //            if (key == "highway" && (value == "tertiary_link" || value == "tertiary" || value == "unclassified" || value == "road"
    //                    )) {
    //                globalVar.regularSegment.push_back(i);
    //                break;
    //            } else globalVar.minorSegment.push_back(i);
    //        }
    //    }
    //
    //
    //    for (unsigned i = 0; i < getNumberOfNodes(); i++) {
    //        const OSMNode* node = getNodeByIndex(i);
    //        for (unsigned tagNum = 0; tagNum < getTagCount(node); tagNum++) {
    //            string key, value;
    //            tie(key, value) = getTagPair(node, tagNum);
    //            if (key == "railway" && value == "subway_entrance") {
    //                globalVar.metroStations.push_back(getNodeByIndex(i)->coords());
    //                break;
    //            }
    //
    //
    //
    //
    //            if (key == "amenity" && (value == "fuel")) {
    //                globalVar.fuelstation.push_back(getNodeByIndex(i)->coords());
    //                break;
    //            }
    //
    //        }
    //
    //    }
    //
    //
    //
    //    for (unsigned interestID = 0; interestID < getNumberOfPointsOfInterest(); interestID++) {
    //        string type = getPointOfInterestType(interestID);
    //        if (type == "dentist" || type == "pharmacy" || type == "clinic" || type == "hospital") {
    //            globalVar.hospital.push_back(getPointOfInterestPosition(interestID));
    //            globalVar.hospitalID.push_back(interestID);
    //        }
    //
    //        if (type == "restaurant" || type == "fast_food" || type == "cafe") {
    //            globalVar.restaurant.push_back(getPointOfInterestPosition(interestID));
    //            globalVar.restaurantID.push_back(interestID);
    //
    //
    //        }
    //
    //        if (type == "bank") {
    //            globalVar.bank.push_back(getPointOfInterestPosition(interestID));
    //            globalVar.bankID.push_back(interestID);
    //        }
    //
    //        if (type == "school" || type == "library") {
    //            globalVar.edu.push_back(getPointOfInterestPosition(interestID));
    //            globalVar.eduID.push_back(interestID);
    //        }
    //
    //
    //    }

    //after loading the street id array 
    //we need to calculate its length
    for (unsigned i = 0; i < globalVar.s_id_array.size(); i++) {
        for (unsigned j = 0; j < globalVar.s_id_array[i].street_segment_ids.size();
                j++) {
            globalVar.s_id_array[i].length += globalVar.ssArray[globalVar.s_id_array[i].street_segment_ids[j]].length;
        }
    }

    //Store data into the r-tree for points of interests
    for (unsigned j = 0; j < getNumberOfPointsOfInterest(); j++) {
        double pos[2] = {DEG_TO_RAD * getPointOfInterestPosition(j).
            lon() * cos(DEG_TO_RAD * globalVar.latavg), DEG_TO_RAD * getPointOfInterestPosition(j).lat()};
        interest_tree.insert(std::make_pair(MyLatLon(pos[0], pos[1]), j));
    }



    //Build an unordered map to use street name to access street_ID
    std::unordered_map<std::string, set<unsigned> > ::iterator it;
    std::unordered_map<std::string, vector<unsigned> > ::iterator it1;
    for (unsigned i = 0; i < getNumberOfStreets(); i++) {

        it1 = globalVar.streetName_to_streetID.find(getStreetName(i));
        if (it1 == globalVar.streetName_to_streetID.end()) {

            pair<string, vector<unsigned> >street(getStreetName(i), {
                i
            });
            globalVar.streetName_to_streetID.insert(street);
        } else {
            it1->second.push_back(i);

        }
    }




    //Build an unordered_map to directly store intersections into street name
    for (unsigned i = 0; i < getNumberOfStreets(); i++) {

        it = globalVar.streetName_to_intersectionsID.find(getStreetName(i));
        if (it == globalVar.streetName_to_intersectionsID.end()) {

            pair<string, set<unsigned> >street(getStreetName(i), globalVar.s_id_array[i].intersection_ids);
            globalVar.streetName_to_intersectionsID.insert(street);
        } else {
            it->second.insert(globalVar.s_id_array[i].intersection_ids.begin(), globalVar.s_id_array[i].intersection_ids.end());


        }
    }

    for (unsigned id = 0; id < getNumberOfPointsOfInterest(); id++) {

        string poi = getPointOfInterestName(id);
        vector<unsigned> poiInfo(2);
        poiInfo[0] = id;
        poiInfo[1] = find_closest_intersection(getPointOfInterestPosition(id));
        globalVar.POIName.insert(make_pair(poi, poiInfo));
    }

    //    for (unsigned id = 0; id < getNumberOfFeatures(); id++) {
    //
    //        LatLon begin = getFeaturePoint(id, 0);
    //        LatLon end = getFeaturePoint(id, getFeaturePointCount(id) - 1);
    //        FeatureType tempFeature = getFeatureType(id);
    //        bool closed = false;
    //        if ((begin.lat() == end.lat() && begin.lon() == end.lon()) || ((tempFeature == Lake) || (tempFeature == Island))) {
    //            closed = true;
    //        }
    //
    //
    //
    //        if (closed) {
    //            double area = polygonArea(id);
    //
    //            globalVar.areaID.insert(make_pair(area, id));
    //        } else
    //            globalVar.areaID.insert(make_pair(0, id));
    //    }
    //
    //
    //    multimap<double, unsigned>::iterator it2 = globalVar.areaID.end();
    //    while (it2 != globalVar.areaID.begin()) {
    //        it2--;
    //        unsigned i = it2->second;
    //        // for (unsigned i = 0; i < getNumberOfFeatures(); i++) {
    //
    //
    //        featureP newFeature;
    //        newFeature.fpoints = new t_point[getFeaturePointCount(i)];
    //
    //        for (unsigned j = 0; j < getFeaturePointCount(i); j++) {
    //            LatLon Point = getFeaturePoint(i, j);
    //            double x = (Point.lon() * DEG_TO_RAD) * cos(globalVar.latavg * DEG_TO_RAD);
    //            double y = Point.lat() * DEG_TO_RAD;
    //            newFeature.fpoints[j] = t_point((x - globalVar.x_min) * globalVar.x_scale, (y - globalVar.y_max) * globalVar.y_scale);
    //        }
    //        newFeature.size = getFeaturePointCount(i);
    //        newFeature.featureIndex = i;
    //
    //        globalVar.areaFeatures.push_back(newFeature);
    //
    //
    //        // }
    //    }
    //
    //
    //
    //
    //
    //    for (unsigned relationIndex = 0; relationIndex < getNumberOfRelations(); relationIndex++) {
    //        const OSMRelation* temp = getRelationByIndex(relationIndex);
    //        pair<OSMID, unsigned> relation(temp -> id(), relationIndex);
    //        globalVar.OSMIDToRelationIndex.insert(relation);
    //    }








    //    for (unsigned relationIndex = 0; relationIndex < getNumberOfRelations(); relationIndex++) {
    //        const OSMRelation* tempRelation = getRelationByIndex(relationIndex);
    //        for (unsigned tagIndex = 0; tagIndex < getTagCount(tempRelation); tagIndex++) {
    //            string key, value;
    //            tie(key, value) = getTagPair(tempRelation, tagIndex);
    //            if (key == "route" && value == "subway") {
    //                vector<vector < t_point>> linePoints;
    //                vector <unsigned> subwayWayIndex1;
    //                for (unsigned member = 0; member < tempRelation->members().size(); member++) {
    //                    int tempWayIndex = OSMIDToWayIndex(tempRelation->members()[member].tid);
    //                    if (tempWayIndex != -1)
    //                        subwayWayIndex1.push_back(tempWayIndex);
    //                }
    //                for (unsigned wayID = 0; wayID < subwayWayIndex1.size(); wayID++) {
    //
    //                    vector<LatLon> stations = wayToNodes(subwayWayIndex1[wayID]);
    //                    vector<t_point> t_stations;
    //                    // convert all nodes to xy 
    //                    for (unsigned id = 0; id < stations.size(); id++) {
    //                        LatLon point = stations[id];
    //                        t_point xy = LatLon_to_real_XY1(point);
    //
    //                        t_stations.push_back(xy);
    //                    }
    //
    //                    linePoints.push_back(t_stations);
    //
    //                }
    //
    //
    //
    //                globalVar.subwayWayIndex.push_back(subwayWayIndex1);
    //                globalVar.metro.push_back(linePoints);
    //            }
    //        }
    //
    //    }
    //
    //    globalVar.metroColor.push_back(t_color(227, 23, 13, 255));
    //    globalVar.metroColor.push_back(t_color(50, 205, 50, 255));
    //    globalVar.metroColor.push_back(t_color(255, 97, 0, 255));
    //    globalVar.metroColor.push_back(t_color(30, 144, 255, 255));
    //
    //    globalVar.metroColor.push_back(t_color(153, 51, 250, 255));
    //    globalVar.metroColor.push_back(t_color(255, 255, 0, 255));
    //    globalVar.metroColor.push_back(t_color(56, 94, 15, 255));
    //    globalVar.metroColor.push_back(t_color(56, 94, 15, 255));
    //
    //    globalVar.metroColor.push_back(t_color(176, 48, 96, 255));
    //    globalVar.metroColor.push_back(t_color(0, 199, 140, 255));
    //    globalVar.metroColor.push_back(t_color(115, 74, 18, 255));
    //    globalVar.metroColor.push_back(t_color(218, 112, 214, 255));



    /*  for (unsigned lineIndex = 0; lineIndex < globalVar.subwayWayIndex.size(); lineIndex ++){
          std::vector<unsigned> tempWay1 = globalVar.subwayWayIndex[lineIndex];
          for (unsigned i = 0; i < globalVar.subwayWayIndex[lineIndex].size(); i++){
              unsigned id = tempWay1[i];
            
            
              const OSMWay* tempWay = getWayByIndex(id);
              const OSMNode* tempNode = getNodeByIndex(OSMIDToNodeIndex(tempWay -> ndrefs()[0]));
            
              LatLon tempLatLon = tempNode -> coords();
          
              t_point station(LatLon_to_real_XY1(tempLatLon));
              globalVar.subwayStations.resize(lineIndex+1);
              globalVar.subwayStations[lineIndex].push_back(station); 
          }
      }
      
    
    
     */

    ////////////////////////////////Weighted Direct Matrix///////////////////////////

    //resize the 2d vector
    int n = getNumberOfIntersections();
    globalVar.weightedMatrix = new double*[n];
    for (int i = 0; i < n; i++) {
        globalVar.weightedMatrix[i] = new double[n];
    }

    /*
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) globalVar.weightedMatrix[i][j] = 0;
            else globalVar.weightedMatrix[i][j] = INF;
        }
    } 
     */

    //Make sure this is updated to reflect whether
    //loading the map succeeded or failed

    return globalVar.load_successful;
}

void close_map() {
    //Clean-up your map related data structures here
    if (globalVar.load_successful) {
        globalVar.ssArray.clear();
        globalVar.s_id_array.clear();
        globalVar.siArray.clear();
        globalVar.streetName_to_intersectionsID.clear();
        globalVar.streetName_to_streetID.clear();
        globalVar.OSMIDToWayIndex.clear();
        globalVar.OSMIDToNodeIndex.clear();
        globalVar.OSMIDToRelationIndex.clear();
        globalVar.areaID.clear();
        globalVar.featureID.clear();
        globalVar.hospital.clear();
        globalVar.metroStations.clear();
        globalVar.subwayStations.clear();
        globalVar.subwayWayIndex.clear();
        globalVar.metro.clear();
        globalVar.metroColor.clear();
        globalVar.platinumSegment.clear();
        globalVar.goldSegment.clear();
        globalVar.silverSegment.clear();
        globalVar.restaurant.clear();
        globalVar.restaurantID.clear();
        globalVar.fuelstation.clear();
        globalVar.hospitalID.clear();
        globalVar.POIName.clear();
        globalVar.regularSegment.clear();
        globalVar.minorSegment.clear();
        globalVar.alreadyDrewHighwayTextPos.clear();
        globalVar.alreadyDrewTextPos.clear();
        globalVar.bank.clear();
        globalVar.bankID.clear();
        globalVar.edu.clear();
        globalVar.eduID.clear();
        globalVar.naviRoute_seg.clear();
        globalVar.turn_by_turn.clear();
        globalVar.dongnanxibei.clear();
        globalVar.int_intids.clear();
        globalVar.int_intids_nodup.clear();
        globalVar.int_ssid.clear();
        globalVar.int_ssid_nodup.clear();
        globalVar.int_valid_seg.clear();
        globalVar.int_weight.clear();
        globalVar.int_weight_nodup.clear();
        globalVar.draw_turn.clear();
        globalVar.draw_dongnanxibei.clear();
        globalVar.draw_street_seg.clear();
        globalVar.move_distance.clear();
        globalVar.landmarks_to_int.clear();
        globalVar.int_to_landmarks.clear();
        globalVar.int_intids_rev.clear();
        globalVar.seg_street_ID.clear();
        globalVar.int_seg_street_ID_rev.clear();
        globalVar.int_shortest_segment_rev.clear();
        globalVar.int_seg_street_ID.clear();
        globalVar.int_seg_street_ID_nodup.clear();
        globalVar.int_valid_seg_rev.clear();
        globalVar.int_weight_rev.clear();
     
        
        
        globalVar.areaFeatures.clear();
        closeStreetDatabase();
        closeOSMDatabase();

    }
}


//Returns street id(s) for the given street name
//If no street with this name exists, returns a 0-length vector.

std::vector<unsigned> find_street_ids_from_name(std::string street_name) {
    //declare a street unordered_map iterator
    std::unordered_map <std::string, vector<unsigned> >::iterator itr;

    itr = globalVar.streetName_to_streetID.find(street_name);
    if (itr != globalVar.streetName_to_streetID.end()) {
        return itr -> second;
    } else {
        vector<unsigned> notfind;
        return notfind;
    }
}

//Returns the street segments for the given intersection 

std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {

    return globalVar.siArray[intersection_id].ss_ids;

}

//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)

std::vector<std::string> find_intersection_street_names(unsigned intersection_id) {
    std::vector<std::string> s_names;

    for (unsigned i = 0; i < globalVar.siArray[intersection_id].s_ids.size(); i++) {
        s_names.push_back(getStreetName(globalVar.siArray[intersection_id].s_ids[i]));
    }

    return s_names;
}

//Returns true if you can get from intersection1 to intersection2 using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself

bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2) {

    unsigned num = getIntersectionStreetSegmentCount(intersection_id1);

    // get the segments from first intersection
    for (unsigned i = 0; i < num; i++) {
        StreetSegmentIndex newS = getIntersectionStreetSegment(intersection_id1, i);
        if (getStreetSegmentInfo(newS).oneWay) {
            if (intersection_id2 ==
                    getStreetSegmentInfo(newS).to)
                return true;
        } else {
            if (intersection_id2 ==
                    getStreetSegmentInfo(newS).to
                    || intersection_id2 ==
                    getStreetSegmentInfo(newS).from)

                return true;
        }
    }

    return false;
}

//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections

std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {

    std::vector<unsigned> in_ids;
    unsigned num = getIntersectionStreetSegmentCount(intersection_id);

    for (unsigned i = 0; i < num; i++) {
        StreetSegmentIndex newS = getIntersectionStreetSegment(intersection_id, i);
        if (intersection_id == getStreetSegmentInfo(newS).from) {
            in_ids.push_back(getStreetSegmentInfo(newS).to);

        } else {

            if (!getStreetSegmentInfo(newS).oneWay) {
                in_ids.push_back(getStreetSegmentInfo(newS).from);
            }
        }

    }
    sort(in_ids.begin(), in_ids.end());
    in_ids.erase(unique(in_ids.begin(), in_ids.end()), in_ids.end());

    return in_ids;
}

//Returns all street segments for the given street

std::vector<unsigned> find_street_street_segments(unsigned street_id) {

    return globalVar.s_id_array[street_id].street_segment_ids;
}

//Returns all intersections along the given street

std::vector<unsigned> find_all_street_intersections(unsigned street_id) {

    std::vector<unsigned> in_ids;
    for (unsigned i = 0; i < globalVar.s_id_array[street_id].street_segment_ids.size(); i++) {
        unsigned a = globalVar.s_id_array[street_id].street_segment_ids[i];
        in_ids.push_back(getStreetSegmentInfo(a).from);
        in_ids.push_back(getStreetSegmentInfo(a).to);
    }

    sort(in_ids.begin(), in_ids.end());
    in_ids.erase(unique(in_ids.begin(), in_ids.end()), in_ids.end());

    return in_ids;
}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
//However street names are not guaranteed to be unique, so more than 1 intersection id
//may exist

std::vector<unsigned> find_intersection_ids_from_street_names(std::string street_name1,
        std::string street_name2) {


    std::vector<unsigned> in_ids;
    std::set<unsigned> street1_ids;
    std::set<unsigned> street2_ids;
    std::set<unsigned>::iterator it1;
    std::set<unsigned>::iterator it2;

    //reconstructed, added the case where one of the street name does not exist
    if (globalVar.streetName_to_intersectionsID.find(street_name1) !=
            globalVar.streetName_to_intersectionsID.end()) {
        street1_ids = globalVar.streetName_to_intersectionsID.find(street_name1)->second;
    } else {
        return in_ids;
    }

    if (globalVar.streetName_to_intersectionsID.find(street_name2)
            != globalVar.streetName_to_intersectionsID.end()) {
        street2_ids = globalVar.streetName_to_intersectionsID.find(street_name2)->second;
    } else {
        return in_ids;
    }

    for (it1 = street1_ids.begin(); it1 != street1_ids.end(); it1++) {
        for (it2 = street2_ids.begin(); it2 != street2_ids.end(); it2++) {
            if (*it1 == *it2)
                in_ids.push_back(*it1);

        }

    }

    return in_ids;
}



//Returns the distance between two coordinates in meters

double find_distance_between_two_points(LatLon point1, LatLon point2) {
    double distance = 0.0; // initialization
    // acquire coordinates
    double x_point1 = DEG_TO_RAD * point1.lon() *
            cos(DEG_TO_RAD * (point1.lat() / 2 + point2.lat() / 2));
    double x_point2 = DEG_TO_RAD * point2.lon() *
            cos(DEG_TO_RAD * (point1.lat() / 2 + point2.lat() / 2));
    // calculate distance
    distance = EARTH_RADIUS_IN_METERS * sqrt(DEG_TO_RAD *
            (point1.lat() - point2.lat()) * DEG_TO_RAD *
            (point1.lat() - point2.lat())+ (x_point1 - x_point2)
            *(x_point1 - x_point2));

    return distance;
}

//Returns the length of the given street segment in meters

double find_street_segment_length(unsigned street_segment_id) {

    return globalVar.ssArray[street_segment_id].length;
}


//Returns the length of the specified street in meters

double find_street_length(unsigned street_id) {

    return globalVar.s_id_array[street_id].length;
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)

double find_street_segment_travel_time(unsigned street_segment_id) {

    return globalVar.ssArray[street_segment_id].weight;
}

//Returns the nearest point of interest to the given position

unsigned find_closest_point_of_interest(LatLon my_position) {
    double pos[2] = {DEG_TO_RAD * my_position.lon() * cos(DEG_TO_RAD * globalVar.latavg), DEG_TO_RAD * my_position.lat()};


    std::vector<point_pair> result2;
    interest_tree.query(bgi::nearest(MyLatLon(pos[0], pos[1]), 1), std::back_inserter(result2));
    return result2[0].second;
}

//Returns the the nearest intersection to the given position

unsigned find_closest_intersection(LatLon my_position) {


    double pos[2] = {my_position.lat(), my_position.lon()};


    std::vector<point_pair> result2;
    int_tree.query(bgi::nearest(MyLatLon(pos[0], pos[1]), 1), std::back_inserter(result2));


    return result2[0].second;

}

int OSMIDToWayIndex(OSMID osmid) {
    unordered_map<OSMID, unsigned>::iterator it = globalVar.OSMIDToWayIndex.find(osmid);
    if (it == globalVar.OSMIDToWayIndex.end()) {
        return -1;
    }
    return it->second;
}

int OSMIDToNodeIndex(OSMID osmid) {
    unordered_map<OSMID, unsigned>::iterator it = globalVar.OSMIDToNodeIndex.find(osmid);
    if (it == globalVar.OSMIDToNodeIndex.end()) {
        return -1;
    }
    return it->second;
}

int OSMIDToRelationIndex(OSMID osmid) {
    unordered_map<OSMID, unsigned>::iterator it = globalVar.OSMIDToRelationIndex.find(osmid);
    if (it == globalVar.OSMIDToRelationIndex.end()) {
        return -1;
    }
    return it->second;
}

double polygonArea(unsigned featureID) {
    double area = 0;
    unsigned j = getFeaturePointCount(featureID) - 1;

    for (unsigned i = 0; i < getFeaturePointCount(featureID); i++) {
        area = area + (getFeaturePoint(featureID, j).lon() + getFeaturePoint(featureID, i).lon())*
                (getFeaturePoint(featureID, j).lat() - getFeaturePoint(featureID, i).lat());
        j = i;
    }

    return abs(area);
}

t_point LatLon_to_real_XY1(LatLon & position) {
    t_point pt;
    pt.x = DEG_TO_RAD * position.lon() * cos(DEG_TO_RAD * globalVar.latavg);
    pt.y = DEG_TO_RAD * position.lat();
    return pt;
}

vector<LatLon> wayToNodes(unsigned wayID) {
    const OSMWay* tempWay = getWayByIndex(wayID);
    vector<OSMID> Node_refs;
    vector<OSMID> tempRefs = tempWay->ndrefs();

    vector<LatLon> LatLonVector(tempWay->ndrefs().size());

    for (unsigned i = 0; i < tempWay->ndrefs().size(); ++i) {
        unsigned NodeIndex = OSMIDToNodeIndex(tempRefs[i]);
        const OSMNode* tempNode = getNodeByIndex(NodeIndex);

        LatLonVector[i] = tempNode->coords();
    }
    return LatLonVector;
}

void setScale() {
    globalVar.x_min = DEG_TO_RAD * minlon * cos(DEG_TO_RAD * globalVar.latavg);
    globalVar.y_min = DEG_TO_RAD * maxlat;
    globalVar.x_max = DEG_TO_RAD * maxlon * cos(DEG_TO_RAD * globalVar.latavg);
    globalVar.y_max = DEG_TO_RAD * minlat;

    globalVar.x_scale = 1535 / abs(globalVar.x_min - globalVar.x_max);
    globalVar.y_scale = 935 / abs(globalVar.y_min - globalVar.y_max);
}

std::vector<double> find_landmarks_to_all_ints_cost(const unsigned intersect_id_start,
        const double turn_penalty) {
    //to order the intersection and pick the one with smallest 
    std::priority_queue<Node, vector<Node>, cmp> openQueue;

    //boost::heap::fibonacci_heap< Node, boost::heap::compare <cmp> > openQueue;

    //Vector to store the current intersection's parent intersection
    std::vector<int> parentInt(getNumberOfIntersections(), -1);

    parentInt[intersect_id_start] = -100;

    std::vector<double> G_Vector(getNumberOfIntersections(), -1);
    G_Vector[intersect_id_start] = 0;

    std::vector<int> parentSeg(getNumberOfIntersections(), -1);

    parentSeg[intersect_id_start] = -100;

    std::vector<bool> closeVector(getNumberOfIntersections(), false);

    //put the starting point position into the open list
    openQueue.push(Node(intersect_id_start, 0));

    while (!openQueue.empty()) {

        IntersectionIndex current_intersection_ID = openQueue.top().intersection_ID;


        if (closeVector[current_intersection_ID] == true) {
            openQueue.pop();
            continue;
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

                if (globalVar.seg_street_ID[parentSeg[current_intersection_ID]]
                        != globalVar.int_seg_street_ID[current_intersection_ID][index])
                    candidate_G = candidate_G + turn_penalty;
            } else {

                selected_segment_ID = globalVar.int_shortest_segment[current_intersection_ID][index];

                candidate_G = G_Vector[current_intersection_ID] + globalVar.ssArray[selected_segment_ID].weight;
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

    return G_Vector;




}

vector<double> find_ints_to_all_landmarks_cost(const unsigned intersect_id_start,
        const double turn_penalty) {

    //to order the intersection and pick the one with smallest 
    std::priority_queue<Node, vector<Node>, cmp> openQueue;

    //boost::heap::fibonacci_heap< Node, boost::heap::compare <cmp> > openQueue;

    //Vector to store the current intersection's parent intersection
    std::vector<int> parentInt(getNumberOfIntersections(), -1);

    parentInt[intersect_id_start] = -100;

    std::vector<double> G_Vector(getNumberOfIntersections(), -1);
    G_Vector[intersect_id_start] = 0;


    std::vector<int> parentSeg(getNumberOfIntersections(), -1);

    parentSeg[intersect_id_start] = -100;

    std::vector<bool> closeVector(getNumberOfIntersections(), false);

    //put the starting point position into the open list
    openQueue.push(Node(intersect_id_start, 0));

    while (!openQueue.empty()) {

        IntersectionIndex current_intersection_ID = openQueue.top().intersection_ID;


        if (closeVector[current_intersection_ID] == true) {
            openQueue.pop();
            continue;
        }

        //Delete the intersection ID with lowest F
        openQueue.pop();


        //put the intersection into close Vector
        closeVector[current_intersection_ID] = true;

        //traverse all the valid intersection
        for (unsigned index = 0; index < globalVar.int_valid_seg_rev[current_intersection_ID].size(); ++index) {

            unsigned selected_segment_ID = globalVar.int_valid_seg_rev[current_intersection_ID][index];

            IntersectionIndex potential_intersection_ID = globalVar.int_intids_rev[current_intersection_ID][index];

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

                if (globalVar.seg_street_ID[parentSeg[current_intersection_ID]]
                        != globalVar.int_seg_street_ID_rev[current_intersection_ID][index])
                    candidate_G = candidate_G + turn_penalty;
            } else {

                selected_segment_ID = globalVar.int_shortest_segment_rev[current_intersection_ID][index];

                candidate_G = G_Vector[current_intersection_ID] + globalVar.ssArray[selected_segment_ID].weight;

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

    return G_Vector;
}
