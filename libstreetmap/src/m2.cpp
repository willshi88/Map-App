#include "m2.h"
#include "graphics.h"
#include "myButton.h"
#include "m1.h"
#include <math.h>
#include "OSMDatabaseAPI.h"
#include "global.h"
#include "Fuzzy_Search.h"
#include "m3.h"
//Draws the map which has been loaded with load_map().
//Your main() program should do the same.
#define SCREEN_LENGTH_MAX_X    1535
#define SCREEN_LENGTH_MAX_Y    935
bool out_of_screen(t_point p);
//global variables
extern global globalVar;
extern double maxlat, minlat, maxlon, minlon;
t_color backgroundColor(249, 243, 234);
t_color white(255, 255, 255);
t_color trans(0, 0, 0, 0);
t_color black(0, 0, 0);
t_color sideBar_color(0, 150, 136, 155);
string search_input = "";
string search_input2 = "";
string dsnb[8];
double zoomTemp;
vector<string> close_names;
vector<string> close_names2;
bool search_bar_clicked = false;
bool search_bar2_clicked = false;
bool show_direction = false;
bool display_auto_complete = false;
bool display_auto_complete2 = false;
bool display_subway = false;
bool display_gas = false;
bool display_food = false;
bool highlight = false;
int process = 0;
unsigned departure, destination;
bool drawNavi = false;
bool draw_help = false;
bool display_path_info = false;
unsigned start = 0;
bool more_info = true;
bool driving = true;
bool draw_poi = false;

/////////////////////////////BUTTONS LOCATIONS/////////////////////////////////

//locations for different features (NOTE: ALL Screen locations)


///////////////////FUNCTIONAILTY BUTTON LOCATIONS//////////////////////////////

//sideBar location
t_point sideBar_bottom_left(0, 0);
t_point sideBar_top_right(15, 1200);
t_point sideBar_icon(-2, 472);

//search_bar location
t_point search_bar_bottom_left(65.5, 65);
t_point search_bar_top_right(345.5, 45);
t_point text_location(65.5, 47);

t_point search_bar2_bottom_left(65.5, 95);
t_point search_bar2_top_right(345.5, 75);
t_point text2_location(65.5, 77);

//search window location
t_point search_win_bottom_left(15, 1100);
t_point search_win_top_right(400, 1);

//direction info starting location
t_point direction_win_top_left(50, 180);
t_point direction_win_bottom_right(350, 260);

//car icon location
t_point car_top_left(100, 140);
t_point car_bottom_right(130, 170);

//man icon location
t_point man_top_left(280, 140);
t_point man_bottom_right(310, 170);

//zoom_in button location
t_point zoom_in_bottom_left(1330, 87);
t_point zoom_in_top_right(1370, 116);

//zoom out button location
t_point zoom_out_bottom_left(1370, 87);
t_point zoom_out_top_right(1410, 116);

//zoom fit button location
t_point zoom_fit_bottom_left(1330, 58);
t_point zoom_fit_top_right(1370, 87);

//exit button location
t_point exit_bottom_left(1370, 58);
t_point exit_top_right(1410, 87);

//Subway button location
t_point subway_top_left(50, 350);
t_point subway_bottom_right(140, 449);

//Gas button location
t_point gas_top_left(160, 350);
t_point gas_bottom_right(250, 449);

//Restaurant button location
t_point food_top_left(270, 350);
t_point food_bottom_right(360, 449);

//Direction button location
t_point direction_top_left(360, 130);
t_point direction_bottom_right(440, 210);

//Help button location
t_point help_top_left(100, 200);
t_point help_bottom_right(150, 250);

//ok button location
t_point ok_top_left(720, 460);
t_point ok_bottom_right(780, 490);

//up_arrow location
t_point up_arrow_top_left(370, 476);
t_point up_arrow_bottom_right(390, 500);

//down_arrow location
t_point down_arrow_top_left(370, 540);
t_point down_arrow_bottom_right(390, 564);

/////////////////////////COUNTRY BUTTON LOCATIONS//////////////////////////////
//Toronto button location
t_point toronto_top_left(60, 550);
t_point toronto_bottom_right(130, 601);

//Beijing button location
t_point beijing_top_left(170, 550);
t_point beijing_bottom_right(240, 601);

//New York button location
t_point new_york_top_left(280, 550);
t_point new_york_bottom_right(350, 601);

//London button location
t_point london_top_left(60, 645);
t_point london_bottom_right(130, 696);

//Sydney button location
t_point sydney_top_left(170, 645);
t_point sydney_bottom_right(240, 696);

//Tokyo button location
t_point tokyo_top_left(280, 645);
t_point tokyo_bottom_right(350, 696);

//Moscow button location
t_point moscow_top_left(60, 740);
t_point moscow_bottom_right(130, 791);

//Interlaken button location
t_point interlaken_top_left(170, 740);
t_point interlaken_bottom_right(240, 791);

//Iceland button location
t_point iceland_top_left(280, 740);
t_point iceland_bottom_right(350, 791);

//Singapore button location
t_point singapore_top_left(60, 835);
t_point singapore_bottom_right(130, 896);

//Rio-De-Janeiro button location
t_point rio_top_left(170, 835);
t_point rio_bottom_right(240, 896);

//Cairo button location
t_point cairo_top_left(280, 835);
t_point cairo_bottom_right(350, 896);


//////////////////////////////////IMAGES////////////////////////////////////////
Surface left_arrow("libstreetmap/resources/left_arrow.png");
Surface right_arrow("libstreetmap/resources/right_arrow.png");
Surface black_arrow("libstreetmap/resources/blackarrow.png");
Surface plus_image("libstreetmap/resources/plus.png");
Surface minus_image("libstreetmap/resources/minus.png");
Surface exit_image("libstreetmap/resources/exit.png");
Surface zoom_fit_image("libstreetmap/resources/zoomfit.png");
Surface toronto_image("libstreetmap/resources/toronto.png");
Surface beijing_image("libstreetmap/resources/beijing.png");
Surface new_york_image("libstreetmap/resources/new_york.png");
Surface london_image("libstreetmap/resources/london.png");
Surface sydney_image("libstreetmap/resources/sydney.png");
Surface tokyo_image("libstreetmap/resources/tokyo.png");
Surface moscow_image("libstreetmap/resources/moscow.png");
Surface interlaken_image("libstreetmap/resources/interlaken.png");
Surface iceland_image("libstreetmap/resources/iceland.png");
Surface singapore_image("libstreetmap/resources/singapore.png");
Surface rio_image("libstreetmap/resources/rio.png");
Surface cairo_image("libstreetmap/resources/cairo.png");
Surface subway_image("libstreetmap/resources/subway.png");
Surface gas_image("libstreetmap/resources/gas.png");
Surface food_image("libstreetmap/resources/food.png");
Surface direction_image("libstreetmap/resources/direction.png");
Surface help_image("libstreetmap/resources/help.png");
Surface loading_image("libstreetmap/resources/loading.png");
Surface straight("libstreetmap/resources/straight.png");
Surface left_turn("libstreetmap/resources/left_turn.png");
Surface right_turn("libstreetmap/resources/right_turn.png");
Surface ok_image("libstreetmap/resources/ok.png");
Surface up_arrow_image("libstreetmap/resources/up_arrow.png");
Surface down_arrow_image("libstreetmap/resources/down_arrow.png");
Surface start_image("libstreetmap/resources/start.png");
Surface end_image("libstreetmap/resources/end.png");
Surface car_image("libstreetmap/resources/car.png");
Surface man_image("libstreetmap/resources/man.png");

/////////////////////////////////BUTTONS////////////////////////////////////////
myButton sideBar(sideBar_bottom_left, sideBar_top_right, t_point(0, 0),
        sideBar_icon, sideBar_color, "", right_arrow, left_arrow, true);

myButton direction(direction_top_left, direction_bottom_right, t_point(0, 0),
        direction_top_left, trans, "", direction_image, direction_image, false);

myButton help(help_top_left, help_bottom_right, t_point(0, 0),
        help_top_left, white, "", help_image, false);

myButton ok_button(ok_top_left, ok_bottom_right, t_point(0, 0),
        ok_top_left, trans, "", ok_image, false);

myButton car_button(car_top_left, car_bottom_right, t_point(0, 0),
        car_top_left, trans, "", car_image, false);

myButton man_button(man_top_left, man_bottom_right, t_point(0, 0),
        man_top_left, trans, "", man_image, false);

myButton up_arrow_button(up_arrow_top_left, up_arrow_bottom_right, t_point(0, 0),
        up_arrow_top_left, trans, "", up_arrow_image, false);

myButton down_arrow_button(down_arrow_top_left, down_arrow_bottom_right, t_point(0, 0),
        down_arrow_top_left, trans, "", down_arrow_image, false);

myButton zoomIn(zoom_in_bottom_left, zoom_in_top_right, t_point(0, 0),
        zoom_in_bottom_left, white, "", plus_image, false);

myButton zoomOut(zoom_out_bottom_left, zoom_out_top_right, t_point(0, 0),
        zoom_out_bottom_left, white, "", minus_image, false);

myButton zoomFit(zoom_fit_bottom_left, zoom_fit_top_right, t_point(0, 0),
        zoom_fit_bottom_left, white, "", zoom_fit_image, false);

myButton exit_button(exit_bottom_left, exit_top_right, t_point(0, 0),
        exit_bottom_left, white, "", exit_image, false);

myButton subway_button(subway_top_left, subway_bottom_right, t_point(0, 0),
        subway_top_left, trans, "", subway_image, false);

myButton gas_button(gas_top_left, gas_bottom_right, t_point(0, 0),
        gas_top_left, trans, "", gas_image, false);

myButton food_button(food_top_left, food_bottom_right, t_point(0, 0),
        food_top_left, trans, "", food_image, false);

//////////////////////////////COUNTRY BUTTONS///////////////////////////////////
myButton toronto_button(toronto_top_left, toronto_bottom_right, t_point(0, 0),
        toronto_top_left, white, "", toronto_image, false);

myButton beijing_button(beijing_top_left, beijing_bottom_right, t_point(0, 0),
        beijing_top_left, white, "", beijing_image, false);

myButton new_york_button(new_york_top_left, new_york_bottom_right, t_point(0, 0),
        new_york_top_left, white, "", new_york_image, false);

myButton london_button(london_top_left, london_bottom_right, t_point(0, 0),
        london_top_left, white, "", london_image, false);

myButton sydney_button(sydney_top_left, sydney_bottom_right, t_point(0, 0),
        sydney_top_left, white, "", sydney_image, false);

myButton tokyo_button(tokyo_top_left, tokyo_bottom_right, t_point(0, 0),
        tokyo_top_left, white, "", tokyo_image, false);

myButton moscow_button(moscow_top_left, moscow_bottom_right, t_point(0, 0),
        moscow_top_left, white, "", moscow_image, false);

myButton interlaken_button(interlaken_top_left, interlaken_bottom_right, t_point(0, 0),
        interlaken_top_left, white, "", interlaken_image, false);

myButton iceland_button(iceland_top_left, iceland_bottom_right, t_point(0, 0),
        iceland_top_left, white, "", iceland_image, false);

myButton singapore_button(singapore_top_left, singapore_bottom_right, t_point(0, 0),
        singapore_top_left, white, "", singapore_image, false);

myButton rio_button(rio_top_left, rio_bottom_right, t_point(0, 0),
        rio_top_left, white, "", rio_image, false);

myButton cairo_button(cairo_top_left, cairo_bottom_right, t_point(0, 0),
        cairo_top_left, white, "", cairo_image, false);

/////////////////////////////Function Prototypes////////////////////////////////

void drawMap();
void setScale();
t_point LatLon_to_screen_XY(LatLon & position);

//functions for each feature
void draw_metro();
void draw_hospital();
void draw_subwaylines();
void draw_restaurant();
void draw_fuelstation();
void draw_bank();
void draw_edu();
void drawIcon(const char* file_path, t_point &initial);
void drawFeatures();
void highlight_interest(unsigned interest_id, float x, float y);
//functions to draw street name
void drawStreetName(string streetName, t_point& from, t_point& to, int textSize);
void drawHighwayName(string streetName, t_point & pt1, t_point & pt2, int textSize);
void draw_highway_link_boundary(int street_width, string road_type);
void drawAllStreetAndNames();
void arrange_street_level(int level);
void arrange_street_name(int level);
void highlight_street(string streetName);
double screen_coordinate_to_window_length(t_point & t1, t_point & t2);
void drawNaviSeg();
unsigned get_segment_array();
void navi_info_reconstruct();

//functions to draw search related windows
void draw_search_bar();
void draw_search_bar2();
void draw_search_window();
void draw_help_win();
void draw_direction_info();
void draw_one_direct_info(unsigned i);
void draw_driving_info(unsigned i);
void draw_auto_complete_window();
void draw_auto_complete_window2();
bool search_bar_clicking(float x, float y);
bool search_bar2_clicking(float x, float y);
bool auto_complete_clicking(int position, float x, float y);
bool auto_complete2_clicking(int position, float x, float y);
void drawNaviRoute(unsigned departure, unsigned destination);
void draw_loading();
void draw_error(string message);

//functions response to clicks and inputs
void findIntersection();
void findDirection();
void highlight_intersection(unsigned intersection_id);
void highlight_start(unsigned id);
void highlight_end(unsigned id);
void highlight_poi_end(unsigned id);
void draw_single_name(int position, vector<string> names);
void draw_single_name2(int position, vector<string> names);
int OSMIDToWayIndex1(OSMID osmid);
void clearData();
vector<unsigned> get_poi_from_name1(string& name);

//functions that handle country button click
void handle_toronto_clicked();
void handle_beijing_clicked();
void handle_new_york_clicked();
void handle_london_clicked();
void handle_sydney_clicked();
void handle_tokyo_clicked();
void handle_moscow_clicked();
void handle_interlaken_clicked();
void handle_iceland_clicked();
void handle_singapore_clicked();
void handle_rio_clicked();
void handle_cairo_clicked();
void handle_hongkong_clicked();

//functions to act with user hardware inputs
void act_on_mouse_button(float x, float y,
        t_event_buttonPressed button_info);
void act_on_keypress(char key_pressed, int keySYM);

//////////////////////////////DRAW MAP FUNCTIONS////////////////////////////////

void act_on_mouse_button(float x, float y,
        t_event_buttonPressed button_info) {
    set_drawing_buffer(ON_SCREEN);
    if (draw_help) {
        draw_help = false;
        drawMap();
        return;
    }
    if (globalVar.error) {
        if (button_info.button == 1 && ok_button.clicking(x, y)) {
            globalVar.error = false;
            drawMap();
            return;
        }
        return;
    }
    //only react when user left click the map
    if (button_info.button == 1) {
        //first, check for the button clicks
        if (sideBar.clicking(x, y)) {
            //if it is a button click, do the button function and
            //return right after (without finding other info)
            if (!sideBar.isClicked()) {
                clearData();
            }
            drawMap();
            return;
        } else if (zoomIn.clicking(x, y)) {
            my_zoom_in(drawMap);
            return;
        } else if (zoomOut.clicking(x, y)) {
            my_zoom_out(drawMap);
            return;
        } else if (zoomFit.clicking(x, y)) {
            my_zoom_fit(drawMap);
            return;
        } else if (exit_button.clicking(x, y)) {
            my_exit(drawMap);
            return;
        }//handle country button clicks
        else if (sideBar.isClicked()) {
            if (!show_direction) {
                if (search_bar_clicking(x, y)) {
                    return;
                } else if (direction.clicking(x, y)) {
                    search_input = "";
                    search_input2 = "";
                    search_bar_clicked = false;
                    search_bar2_clicked = false;
                    show_direction = !show_direction;
                    drawMap();
                    return;
                } else if (toronto_button.clicking(x, y)) {
                    handle_toronto_clicked();
                    return;
                } else if (beijing_button.clicking(x, y)) {
                    handle_beijing_clicked();
                    return;
                } else if (new_york_button.clicking(x, y)) {
                    handle_new_york_clicked();
                    return;
                } else if (london_button.clicking(x, y)) {
                    handle_london_clicked();
                    return;
                } else if (sydney_button.clicking(x, y)) {
                    handle_sydney_clicked();
                    return;
                } else if (tokyo_button.clicking(x, y)) {
                    handle_tokyo_clicked();
                    return;
                } else if (moscow_button.clicking(x, y)) {
                    handle_moscow_clicked();
                    return;
                } else if (interlaken_button.clicking(x, y)) {
                    handle_interlaken_clicked();
                    return;
                } else if (iceland_button.clicking(x, y)) {
                    handle_iceland_clicked();
                    return;
                } else if (singapore_button.clicking(x, y)) {
                    handle_singapore_clicked();
                    return;
                } else if (rio_button.clicking(x, y)) {
                    handle_rio_clicked();
                    return;
                } else if (cairo_button.clicking(x, y)) {
                    handle_cairo_clicked();
                    return;
                } else if (subway_button.clicking(x, y)) {
                    display_subway = !display_subway;
                    drawMap();
                    return;
                } else if (gas_button.clicking(x, y)) {
                    display_gas = !display_gas;
                    drawMap();
                    return;
                } else if (food_button.clicking(x, y)) {
                    display_food = !display_food;
                    drawMap();
                    return;
                } else if (help.clicking(x, y)) {
                    draw_help = true;
                    drawMap();
                    return;
                }
            } else {
                if (search_bar_clicking(x, y)) {
                    return;
                } else if (search_bar2_clicking(x, y)) {
                    return;
                } else if (direction.clicking(x, y)) {
                    search_input = "";
                    search_input2 = "";
                    search_bar_clicked = false;
                    search_bar2_clicked = false;
                    drawNavi = false;
                    display_path_info = false;
                    show_direction = !show_direction;
                    drawMap();
                    return;
                } else if (up_arrow_button.clicking(x, y)) {
                    if (start > 9) {
                        start -= 9;
                    } else {
                        start = 0;
                    }
                    drawMap();
                    return;
                } else if (down_arrow_button.clicking(x, y)) {
                    if (more_info) {
                        start += 9;
                    }
                    drawMap();
                    return;
                } else if (car_button.clicking(x, y)) {
                    start = 0;
                    driving = true;
                    drawMap();
                    return;
                } else if (man_button.clicking(x, y)) {
                    start = 0;
                    driving = false;
                    drawMap();
                    return;
                }

            }
            //find the corresponding map location based on current cursor position
            float map_x = (x / globalVar.x_scale) + globalVar.x_min;
            float map_y = (y / globalVar.y_scale) + globalVar.y_max;
            LatLon current(map_y / DEG_TO_RAD, map_x /
                    (DEG_TO_RAD * cos(DEG_TO_RAD * globalVar.latavg)));
            unsigned intersection_id = find_closest_intersection(current);
            unsigned interest_id = find_closest_point_of_interest(current);
            //if the distance between the nearest intersection and the cursor position
            //is reasonable (i.e. it will not show the info of an intersection on land
            //if the user is clicking in the sea)
            t_point temp(x, y);
            if (find_distance_between_two_points(current, getIntersectionPosition(intersection_id))
                    <= (50 / globalVar.zoomScale + 10)) {

                if (!sideBar.isClicked() || world_to_scrn(temp).x > 420) {
                    highlight_intersection(intersection_id);
                }
            } else if (find_distance_between_two_points(current, getPointOfInterestPosition(interest_id))
                    <= (50 / globalVar.zoomScale + 30) && x >= 420) {
                if (!sideBar.isClicked() || world_to_scrn(temp).x > 420) {
                    highlight_interest(interest_id, x, y);
                }
            } else if (world_to_scrn(temp).x < 400 && sideBar.isClicked()) {
                return;
            } else {

                drawMap();
                return;
            }
        }
    }//user right clicks
    else if (button_info.button == 3) {
        //nothing highlighted yet
        //highlight the first intersection
        if (process == 0) {

            //find the corresponding map location based on current cursor position
            float map_x = (x / globalVar.x_scale) + globalVar.x_min;
            float map_y = (y / globalVar.y_scale) + globalVar.y_max;
            LatLon current(map_y / DEG_TO_RAD, map_x /
                    (DEG_TO_RAD * cos(DEG_TO_RAD * globalVar.latavg)));
            departure = find_closest_intersection(current);
            //cout << departure << endl;
            //unsigned interest_id = find_closest_point_of_interest(current);
            //if the distance between the nearest intersection and the cursor position
            //is reasonable (i.e. it will not show the info of an intersection on land
            //if the user is clicking in the sea)
            t_point temp(x, y);
            if (find_distance_between_two_points(current, getIntersectionPosition(departure))
                    <= (50 / globalVar.zoomScale + 10)) {

                if (!sideBar.isClicked() || world_to_scrn(temp).x > 420) {
                    highlight_start(departure);
                    process += 1;
                }
            }
        }//having one intersection displayed already,
            //display the second one and draw the path
        else if (process == 1) {
            float map_x = (x / globalVar.x_scale) + globalVar.x_min;
            float map_y = (y / globalVar.y_scale) + globalVar.y_max;
            LatLon current(map_y / DEG_TO_RAD, map_x /
                    (DEG_TO_RAD * cos(DEG_TO_RAD * globalVar.latavg)));
            destination = find_closest_intersection(current);
            //cout << destination << endl;
            t_point temp(x, y);
            if (find_distance_between_two_points(current, getIntersectionPosition(destination))
                    <= (50 / globalVar.zoomScale + 10)) {
                //find the path in between
                process += 1;
                find_path_between_intersections(departure, destination, 0);
                if (globalVar.naviRoute_seg.size() <= 0) {
                    draw_error("Sorry, there is no path existing between these two locations");
                    globalVar.error = true;
                    return;
                }
                get_segment_array();
                navi_info_reconstruct();
                drawNavi = true;
                display_path_info = true;
                driving = true;
                start = 0;
                drawMap();
                return;
            }
        } else {
            process = 0;
            drawNavi = false;
            drawMap();
            return;
        }
    }
    return;
}

void act_on_keypress(char key_pressed, int keySYM) {
    if (!sideBar.isClicked()) return;
    set_drawing_buffer(ON_SCREEN);

    if (!show_direction) {
        //if the user input is 'backspace'
        if (key_pressed == 8) {
            //corner case where deleting empty string}
            if (search_input.size() <= 0) return;
            highlight = false;
            display_auto_complete = true;
            search_input.erase(search_input.end() - 1);
            //after deleting, if no input, clear the auto complete window
            if (search_input.size() <= 0) {
                drawMap();
                return;
            }
        }//user input "enter"
        else if (key_pressed == 13) {
            findIntersection();
        }//some other inputs which should not added
            //(i.e. Caps lock, shift, etc.)
        else if (keySYM == 65509
                || keySYM == 65505
                || keySYM == 65506
                || keySYM == 289) {

        } else {
            //only allow adding if the search bar is highlighted
            if (search_bar_clicked) {
                search_input += key_pressed;
                display_auto_complete = true;
            }
        }
        //drawMap();
        draw_search_bar();
    } else {
        if (search_bar_clicked) {
            //if the user input is 'backspace'
            if (key_pressed == 8) {
                //corner case where deleting empty string
                if (search_input.size() <= 0) return;
                highlight = false;
                display_auto_complete = true;
                search_input.erase(search_input.end() - 1);
                //after deleting, if no input, clear the auto complete window
                if (search_input.size() <= 0) {
                    drawMap();
                    return;
                }
            }//user input "enter"
            else if (key_pressed == 13) {
                findDirection();
            }//some other inputs which should not added
                //(i.e. Caps lock, shift, etc.)
            else if (keySYM == 65509
                    || keySYM == 65505
                    || keySYM == 65506
                    || keySYM == 289) {

            } else {
                //only allow adding if the search bar is highlighted
                if (search_bar_clicked) {
                    display_auto_complete = true;
                    search_input += key_pressed;
                }
            }
            //drawMap();
            draw_search_bar();
        } else if (search_bar2_clicked) {

            //if the user input is 'backspace'
            if (key_pressed == 8) {
                //corner case where deleting empty string
                if (search_input2.size() <= 0) return;
                highlight = false;
                display_auto_complete2 = true;
                search_input2.erase(search_input2.end() - 1);
                //after deleting, if no input, clear the auto complete window
                if (search_input2.size() <= 0) {
                    drawMap();
                    return;
                }
            }//user input "enter"
            else if (key_pressed == 13) {
                findDirection();
            }//some other inputs which should not added
                //(i.e. Caps lock, shift, etc.)
            else if (keySYM == 65509
                    || keySYM == 65505
                    || keySYM == 65506
                    || keySYM == 289) {

            } else {
                //only allow adding if the search bar is highlighted
                if (search_bar2_clicked) {
                    search_input2 += key_pressed;
                    display_auto_complete2 = true;
                }
            }
            //drawMap();
            draw_search_bar2();
        }
    }
}

/////////////////////////////////DRAW MAP///////////////////////////////////////

void draw_map() {
    setScale();
    init_graphics("The Polaris", backgroundColor);
    set_visible_world(0, 0, 1535, 935);
    dsnb[0] = "N";
    dsnb[1] = "NE";
    dsnb[2] = "E";
    dsnb[3] = "SE";
    dsnb[4] = "S";
    dsnb[5] = "SW";
    dsnb[6] = "W";
    dsnb[7] = "NW";
    set_mouse_move_input(true);
    set_keypress_input(true);

    event_loop(act_on_mouse_button, nullptr, act_on_keypress, drawMap);

    close_graphics();
}


////////////////////////////////DRAW SCREEN/////////////////////////////////////

void drawMap() {

    set_drawing_buffer(OFF_SCREEN);
    globalVar.zoomScale = globalVar.x_scale * globalVar.x_scale
            / (get_visible_world().right()
            - get_visible_world().left()) / 10000000;
    globalVar.drawScale = globalVar.x_scale * abs(globalVar.x_min
            - globalVar.x_max) / (get_visible_world().right()
            - get_visible_world().left());

    setcolor(RED);
    setlinewidth(1);
    clearscreen();

    globalVar.zoomScale = globalVar.x_scale * globalVar.x_scale
            / (get_visible_world().right() - get_visible_world().left())
            / 10000000;

    //draw street and all of the features

    drawFeatures();

    drawAllStreetAndNames();


    if (globalVar.zoomScale > 100) {
        draw_bank();
        draw_edu();
        draw_hospital();
        draw_fuelstation();
        draw_restaurant();
        draw_metro();
    }


    draw_fuelstation();
    draw_subwaylines();
   highlight_intersection(267776);
    highlight_intersection(39985);

   // find_path_between_intersections(64489, 75020, 15.00000000000000000);

    if (highlight)
        highlight_street(search_input);
        
    if (drawNavi) {
        drawNaviSeg();
        //highlight the intersection at the beginning and the end
        highlight_start(departure);
        if (draw_poi) {
            highlight_poi_end(globalVar.found_interest);
        } else {
            highlight_end(destination);
        }
        if (globalVar.zoomnavi == true) {
            globalVar.zoomnavi = false;
            my_zoom_navi(drawMap);
        }
    }

    // draw four buttons at the top right corner
    zoomIn.display();
    zoomOut.display();
    exit_button.display();
    zoomFit.display();

    // draw the search info
    sideBar.display();
    if (sideBar.isClicked()) {
        draw_search_window();
    }

    setcolor(BLACK);
    setlinewidth(2);
    if (draw_help) {
        draw_help_win();
    }

    update_message(to_string(globalVar.zoomScale));
    copy_off_screen_buffer_to_screen();
    globalVar.alreadyDrewTextPos.clear();
    globalVar.alreadyDrewHighwayTextPos.clear();

}

/******This function is the top-level function to adjust the view of map*******/


void drawAllStreetAndNames() {
    if (globalVar.drawScale < 2) {
        arrange_street_level(1);
        arrange_street_name(1);
    } else if (globalVar.drawScale < 5) {

        arrange_street_level(3);
        arrange_street_name(3);
        arrange_street_level(2);
        arrange_street_name(2);
        arrange_street_level(1);
        arrange_street_name(1);
    } else if (globalVar.drawScale < 10) {
        arrange_street_level(4);
        arrange_street_level(3);
        arrange_street_level(2);
        arrange_street_name(2);
        arrange_street_name(3);
        arrange_street_name(4);
        arrange_street_level(1);
        arrange_street_name(1);
    } else {
        arrange_street_level(5);
        arrange_street_level(4);
        arrange_street_level(3);
        arrange_street_level(2);
        arrange_street_name(5);
        arrange_street_name(4);
        arrange_street_name(3);
        arrange_street_name(2);
        arrange_street_level(1);
        arrange_street_name(1);
    }
}
/********************************************************************************/

/****This function is used to arrange street display level corresponding to zoom scale****/
void arrange_street_level(int level) {
    vector<unsigned> seg = {};
    int street_width;

    //set up the arbitary parameter corresponding to zooming scale
    switch (level) {
        case 1:

            if (globalVar.drawScale < 5)
                street_width = 5;

            else if (globalVar.drawScale > 40)
                street_width = 24;

            else
                street_width = 18 * globalVar.drawScale / 20;

            draw_highway_link_boundary(street_width, "highway");

            setcolor(t_color(255, 222, 59));
            seg = globalVar.platinumSegment;
            break;


        case 2:
            if (globalVar.drawScale > 40)
                street_width = 20;
            else
                street_width = 16 * globalVar.drawScale / 20;

            draw_highway_link_boundary(street_width, "link");

            setcolor(t_color(255, 233, 170));
            seg = globalVar.goldSegment;
            break;

        case 3:

            street_width = 9 * globalVar.drawScale / 20;
            draw_highway_link_boundary(street_width, "avenue");
            setcolor(250, 250, 214);
            seg = globalVar.silverSegment;
            break;


        case 4:
            setcolor(WHITE);
            street_width = 7 * globalVar.drawScale / 20;
            seg = globalVar.regularSegment;
            break;

        case 5:
            setcolor(WHITE);
            street_width = 5 * globalVar.drawScale / 20;
            seg = globalVar.minorSegment;
            break;

        default: break;
    }

    for (unsigned street_seg_index = 0; street_seg_index < seg.size(); street_seg_index++) {
        unsigned seg_id = seg[street_seg_index];

        setlinewidth(street_width);
        StreetSegmentInfo info = getStreetSegmentInfo(seg_id);
        //get the segments two intersections real coordinates
        float x1 = globalVar.siArray[info.from].x - globalVar.x_min;
        float y1 = (globalVar.siArray[info.from].y - globalVar.y_max);
        float x2 = globalVar.siArray[info.to].x - globalVar.x_min;
        float y2 = (globalVar.siArray[info.to].y - globalVar.y_max);
        //get the screen coordinates of the above points
        t_point pt1_screen(x1 * globalVar.x_scale, y1 * globalVar.y_scale);
        t_point pt2_screen(x2 * globalVar.x_scale, y2 * globalVar.y_scale);
        if (out_of_screen(pt1_screen) && out_of_screen(pt2_screen))
            continue;


        //check for curve points to draw streets curves
        unsigned numofCurvePoints = info.curvePointCount;

        if (numofCurvePoints > 1) {
            //get the first curve points
            LatLon curvePoint_LL = getStreetSegmentCurvePoint(seg_id, 0);
            //get the curve points screen coordinates, and draw the from to first curve points
            t_point curvePoint_screen = LatLon_to_screen_XY(curvePoint_LL);
            drawline(pt1_screen.x, pt1_screen.y,
                    curvePoint_screen.x, curvePoint_screen.y);

            //this for loop draws the segments formed by curve points
            for (unsigned curvePointsIndex = 0; curvePointsIndex < numofCurvePoints - 1;
                    curvePointsIndex++) {
                LatLon curvePoint_LL_a =
                        getStreetSegmentCurvePoint(seg_id, curvePointsIndex);
                LatLon curvePoint_LL_b =
                        getStreetSegmentCurvePoint(seg_id, curvePointsIndex + 1);
                t_point curvePoint_screen_a =
                        LatLon_to_screen_XY(curvePoint_LL_a);
                t_point curvePoint_screen_b =
                        LatLon_to_screen_XY(curvePoint_LL_b);
                drawline(curvePoint_screen_a.x, curvePoint_screen_a.y,
                        curvePoint_screen_b.x, curvePoint_screen_b.y);
            }

            //Then draw from last curve point to "to" intersection
            LatLon curvePoint_LL_Last =
                    getStreetSegmentCurvePoint(seg_id, numofCurvePoints - 1);
            t_point curvePoint_screen_Last =
                    LatLon_to_screen_XY(curvePoint_LL_Last);
            drawline(curvePoint_screen_Last.x,
                    curvePoint_screen_Last.y, pt2_screen.x, pt2_screen.y);


        } else if (numofCurvePoints == 1) {
            //get the only curve points
            LatLon curvePoint_LL = getStreetSegmentCurvePoint(seg_id, 0);
            t_point curvePoint_screen = LatLon_to_screen_XY(curvePoint_LL);
            drawline(pt1_screen.x, pt1_screen.y,
                    curvePoint_screen.x, curvePoint_screen.y);
            drawline(curvePoint_screen.x,
                    curvePoint_screen.y, pt2_screen.x, pt2_screen.y);

        } else
            drawline(pt1_screen.x, pt1_screen.y, pt2_screen.x, pt2_screen.y);

    }


}
/*************************************************************************************/

/********arrange the display of street names corresponding to zooming scale*******/
void arrange_street_name(int level) {
    vector<unsigned> seg = {};

    setcolor(BLACK);
    //Classify into 5 different choice of street name printing corresponding to
    //Zooming level
    switch (level) {

        case 1:
            seg = globalVar.platinumSegment;
            break;

        case 2:
            seg = globalVar.goldSegment;
            break;

        case 3:
            seg = globalVar.silverSegment;
            break;

        case 4:
            seg = globalVar.regularSegment;
            break;
        case 5:
            seg = globalVar.minorSegment;
            break;

        default: break;


    }
    //Following will be the requirements for street name printing

    for (unsigned seg_index = 0; seg_index < seg.size(); seg_index++) {
        unsigned seg_id = seg[seg_index];
        //get the segment information by segment id
        StreetSegmentInfo info = getStreetSegmentInfo(seg_id);
        //get the segment's two intersections coordinates
        float x1 = globalVar.siArray[info.from].x - globalVar.x_min;
        float y1 = (globalVar.siArray[info.from].y - globalVar.y_max);
        float x2 = globalVar.siArray[info.to].x - globalVar.x_min;
        float y2 = (globalVar.siArray[info.to].y - globalVar.y_max);

        //get the screen coordinate of these two points
        t_point pt1_screen(x1 * globalVar.x_scale, y1 * globalVar.y_scale);
        t_point pt2_screen(x2 * globalVar.x_scale, y2 * globalVar.y_scale);

        if (out_of_screen(pt1_screen) || out_of_screen(pt1_screen))
            continue;


        // check for curve points
        // if curve point exists, change the pt2 to the curve point position
        //for appropriate angle of display
        if (info.curvePointCount >= 1) {
            LatLon curve_pt_LL = getStreetSegmentCurvePoint(seg_id, 0);
            pt2_screen = LatLon_to_screen_XY(curve_pt_LL);
        }
        string road_name;
        //for highway, only print the box containing the highway code
        if (level == 1) {

            string key, value;
            for (unsigned tag = 0; tag <
                    getTagCount(getWayByIndex(OSMIDToWayIndex1(info.wayOSMID))); tag++) {
                tie(key, value) =
                        getTagPair(getWayByIndex(OSMIDToWayIndex1(info.wayOSMID)), tag);
                if (key == "ref")
                    break;
            }
            road_name = value;
            if (road_name != "yes" && road_name != "no" && road_name != "" &&
                    road_name.size() < 10 && road_name != " " && road_name != "  ")
                drawHighwayName(road_name, pt1_screen, pt2_screen, 6.5);
        }//else, print normal street names
        else {

            //Print normal street name
            road_name = getStreetName(info.streetID);

            //draw one way arrow and check if it is one way
            if (info.oneWay) {
                //if road name is unknown, only print the arrow
                if (road_name == "<unknown>") {
                    if (pt1_screen.x > pt2_screen.x)
                        road_name = "<-";
                    else
                        road_name = "->";

                }//the road name is not known, then append

                else {
                    if (pt1_screen.x > pt2_screen.x) {
                        string arrow = "<- ";
                        arrow.append(road_name);
                        road_name = arrow;
                    } else
                        road_name.append(" ->");
                }
            }
            drawStreetName(road_name, pt1_screen, pt2_screen, 9);
        }
    }
}
/******************************************************************************/

/******Function to change lat&lon to screen coordinate*************************/
t_point LatLon_to_screen_XY(LatLon & position) {
    t_point screen_pt;

    screen_pt.x = (DEG_TO_RAD * position.lon() *
            cos(DEG_TO_RAD * globalVar.latavg) - globalVar.x_min) * globalVar.x_scale;
    screen_pt.y = (DEG_TO_RAD * position.lat() - globalVar.y_max) * globalVar.y_scale;

    return screen_pt;

}
/******************************************************************************/

/*********Draw street name for every level of streets**************************/
void drawStreetName(string streetName, t_point & pt1, t_point & pt2, int textSize) {

    //Do not print street named "unknown"
    if (streetName == "<unknown>") return;
        // Do not print street named nothing
    else if (streetName == "") return;

    //calculating text angle
    double text_angle = atan((pt2.y - pt1.y) / (pt2.x - pt1.x)) / DEG_TO_RAD;
    setcolor(BLACK);
    //Calculating text position
    t_point text_pos((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2);

    t_bound_box bound(pt1, pt2);

    settextattrs(textSize, text_angle);

    setcolor(DARKGREY);
    drawtext(text_pos.x, text_pos.y, streetName, abs(pt1.x - pt2.x), abs(pt1.y - pt2.y));

    return;
}
/******************************************************************************/

/*****This function only draws the code of the highway, and print a box to hold it****/

void drawHighwayName(string streetName, t_point & pt1, t_point & pt2, int textSize) {
    //if street name is not found
    if (streetName == "<unknown") return;

    if (streetName == "") return;

    //get the center of the drawing point
    t_point center((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2);

    //The box only needs to be display horizontally
    settextattrs(textSize, 0);

    //calculate the boundary box sides distance from the center
    double constant = 20 / globalVar.drawScale;

    setlinewidth(1.2);
    //set the visual distance of name printing

    std::vector<t_point>::iterator it;

    for (it = globalVar.alreadyDrewHighwayTextPos.begin();
            it != globalVar.alreadyDrewHighwayTextPos.end(); it++) {

        t_point drew_point = *it;

        // If the point is within certain distance against to already drew point
        // Do not draw this point
        if ((center.x > drew_point.x - 7 * constant)
                && (center.x < drew_point.x + 7 * constant)
                && (center.y > drew_point.y - 7 * constant)
                && (center.y < drew_point.y + 7 * constant)) return;
    }

    //set the color for the background
    setcolor(WHITE);

    //These are the vertices for the bounding box
    t_point vertices[5];
    vertices[0] = t_point(center.x - 0.45 * constant, center.y + 0.38 * constant);
    vertices[1] = t_point(center.x - 0.45 * constant, center.y - 0.38 * constant);
    vertices[2] = t_point(center.x, center.y - 0.45 * constant);
    vertices[3] = t_point(center.x + 0.45 * constant, center.y - 0.38 * constant);
    vertices[4] = t_point(center.x + 0.45 * constant, center.y + 0.38 * constant);

    fillpoly(vertices, 5);

    //set the color for the bounding box
    setcolor(BLACK);

    //Draw the highway code, print in bound box
    drawtext(center.x, center.y, streetName);

    //draw the boundary
    for (unsigned index = 0; index < 4; index++)
        drawline(vertices[index].x, vertices[index].y,
            vertices[index + 1].x, vertices[index + 1].y);

    //connect the last vertices to the first vertices to close the shape
    drawline(vertices[4].x, vertices[4].y, vertices[0].x, vertices[0].y);

    //store the already drew point
    globalVar.alreadyDrewHighwayTextPos.push_back(center);
    return;

}

/********************************************************************************/

/****Draw a wider street with different colors to create the contour of street888*/
void draw_highway_link_boundary(int street_width, string road_type) {
    setlinewidth(street_width + 2.4);
    vector <unsigned> seg;

    if (road_type == "highway") {
        setcolor(t_color(249, 176, 127));
        seg = globalVar.platinumSegment;
    }

    if (road_type == "link") {
        setcolor(t_color(239, 189, 128));
        seg = globalVar.goldSegment;
    }

    if (road_type == "avenue") {
        setcolor(t_color(239, 189, 128));
        seg = globalVar.silverSegment;
    }

    for (unsigned street_seg_index = 0; street_seg_index < seg.size(); street_seg_index++) {
        unsigned seg_id = seg[street_seg_index];

        StreetSegmentInfo info = getStreetSegmentInfo(seg_id);
        //get the segments two intersections real coordinates
        float x1 = globalVar.siArray[info.from].x - globalVar.x_min;
        float y1 = (globalVar.siArray[info.from].y - globalVar.y_max);
        float x2 = globalVar.siArray[info.to].x - globalVar.x_min;
        float y2 = (globalVar.siArray[info.to].y - globalVar.y_max);
        //get the screen coordinates of the above points
        t_point pt1_screen(x1 * globalVar.x_scale, y1 * globalVar.y_scale);
        t_point pt2_screen(x2 * globalVar.x_scale, y2 * globalVar.y_scale);

        if (out_of_screen(pt1_screen) && out_of_screen(pt2_screen))
            continue;

        unsigned numofCurvePoints = info.curvePointCount;

        if (numofCurvePoints > 1) {
            //get the first curve points
            LatLon curvePoint_LL = getStreetSegmentCurvePoint(seg_id, 0);
            //get the curve points real coordinates and then transform it into
            //screen coordinates, and draw the from to first curve points
            t_point curvePoint_screen = LatLon_to_screen_XY(curvePoint_LL);

            drawline(pt1_screen.x, pt1_screen.y, curvePoint_screen.x, curvePoint_screen.y);

            //this for loop draws the segments formed by curve points
            for (unsigned curvePointsIndex = 0; curvePointsIndex < numofCurvePoints - 1; curvePointsIndex++) {
                LatLon curvePoint_LL_a = getStreetSegmentCurvePoint(seg_id, curvePointsIndex);
                LatLon curvePoint_LL_b = getStreetSegmentCurvePoint(seg_id, curvePointsIndex + 1);
                t_point curvePoint_screen_a = LatLon_to_screen_XY(curvePoint_LL_a);
                t_point curvePoint_screen_b = LatLon_to_screen_XY(curvePoint_LL_b);
                drawline(curvePoint_screen_a.x, curvePoint_screen_a.y, curvePoint_screen_b.x, curvePoint_screen_b.y);
            }
            //Then draw from last curve point to "to" intersection

            LatLon curvePoint_LL_Last = getStreetSegmentCurvePoint(seg_id, numofCurvePoints - 1);
            t_point curvePoint_screen_Last = LatLon_to_screen_XY(curvePoint_LL_Last);

            drawline(curvePoint_screen_Last.x, curvePoint_screen_Last.y, pt2_screen.x, pt2_screen.y);


        } else if (numofCurvePoints == 1) {

            //get the only curve points
            LatLon curvePoint_LL = getStreetSegmentCurvePoint(seg_id, 0);

            t_point curvePoint_screen = LatLon_to_screen_XY(curvePoint_LL);

            drawline(pt1_screen.x, pt1_screen.y, curvePoint_screen.x, curvePoint_screen.y);
            drawline(curvePoint_screen.x, curvePoint_screen.y, pt2_screen.x, pt2_screen.y);

        } else
            drawline(pt1_screen.x, pt1_screen.y, pt2_screen.x, pt2_screen.y);

    }
}

/**********************************************************************************************/

/*************The following function is to highlight street according to user's input**********/
void highlight_street(string streetName) {

    setlinewidth(2.4);
    setcolor(RED);
    //get the multiple streetID from a street name
    vector<unsigned> streetID_vec = find_street_ids_from_name(streetName);

    for (unsigned index = 0; index < streetID_vec.size(); index++) {
        //through street ID, get the street segment_ids
        unsigned streetID = streetID_vec[index];

        vector <unsigned> street_segment_ids = globalVar.s_id_array[streetID].street_segment_ids;

        for (unsigned street_seg_index = 0; street_seg_index < street_segment_ids.size(); street_seg_index++) {

            unsigned seg_id = street_segment_ids[street_seg_index];

            StreetSegmentInfo info = getStreetSegmentInfo(seg_id);
            //get the segments two intersections real coordinates
            float x1 = globalVar.siArray[info.from].x - globalVar.x_min;
            float y1 = (globalVar.siArray[info.from].y - globalVar.y_max);
            float x2 = globalVar.siArray[info.to].x - globalVar.x_min;
            float y2 = (globalVar.siArray[info.to].y - globalVar.y_max);
            //get the screen coordinates of the above points
            t_point pt1_screen(x1 * globalVar.x_scale, y1 * globalVar.y_scale);
            t_point pt2_screen(x2 * globalVar.x_scale, y2 * globalVar.y_scale);


            unsigned numofCurvePoints = info.curvePointCount;

            if (numofCurvePoints > 1) {
                //get the first curve points
                LatLon curvePoint_LL = getStreetSegmentCurvePoint(seg_id, 0);
                //get the curve points real coordinates and then transform it into
                //screen coordinates, and draw the from to first curve points
                t_point curvePoint_screen = LatLon_to_screen_XY(curvePoint_LL);

                drawline(pt1_screen.x, pt1_screen.y, curvePoint_screen.x, curvePoint_screen.y);

                //this for loop draws the segments formed by curve points
                for (unsigned curvePointsIndex = 0; curvePointsIndex < numofCurvePoints - 1; curvePointsIndex++) {
                    LatLon curvePoint_LL_a = getStreetSegmentCurvePoint(seg_id, curvePointsIndex);
                    LatLon curvePoint_LL_b = getStreetSegmentCurvePoint(seg_id, curvePointsIndex + 1);
                    t_point curvePoint_screen_a = LatLon_to_screen_XY(curvePoint_LL_a);
                    t_point curvePoint_screen_b = LatLon_to_screen_XY(curvePoint_LL_b);
                    drawline(curvePoint_screen_a.x, curvePoint_screen_a.y, curvePoint_screen_b.x, curvePoint_screen_b.y);
                }
                //Then draw from last curve point to "to" intersection

                LatLon curvePoint_LL_Last = getStreetSegmentCurvePoint(seg_id, numofCurvePoints - 1);
                t_point curvePoint_screen_Last = LatLon_to_screen_XY(curvePoint_LL_Last);

                drawline(curvePoint_screen_Last.x, curvePoint_screen_Last.y, pt2_screen.x, pt2_screen.y);


            } else if (numofCurvePoints == 1) {

                //get the only curve points
                LatLon curvePoint_LL = getStreetSegmentCurvePoint(seg_id, 0);

                t_point curvePoint_screen = LatLon_to_screen_XY(curvePoint_LL);

                drawline(pt1_screen.x, pt1_screen.y, curvePoint_screen.x, curvePoint_screen.y);
                drawline(curvePoint_screen.x, curvePoint_screen.y, pt2_screen.x, pt2_screen.y);

            } else
                drawline(pt1_screen.x, pt1_screen.y, pt2_screen.x, pt2_screen.y);

        }


    }

}

/***********************************************************************************/


void drawIcon(const char* file_path, t_point & initial) {
    Surface surface = load_png_from_file(file_path);
    draw_surface(surface, initial.x, initial.y);
}

void draw_loading() {
    t_point temp(640, 340);
    draw_surface(loading_image, scrn_to_world(temp));
}

void draw_error(string message) {
    t_point temp1(500, 400);
    t_point temp2(1000, 500);
    t_point temp3(750, 410);
    t_point temp4(750, 440);
    t_point top(1000, 425);
    t_point bottom(1000, 500);
    t_point left(500, 425);
    t_point right(1000, 425);

    //define the current bound box
    t_point top_left(scrn_to_world(temp1));
    t_point bottom_right(scrn_to_world(temp2));
    t_point error(scrn_to_world(temp3));
    t_point text(scrn_to_world(temp4));
    t_bound_box bot_b(top_left, scrn_to_world(bottom));
    t_bound_box top_b(top_left, scrn_to_world(top));
    //one for border
    t_point scrn_border(temp1.x - 1, temp1.y - 1);
    t_point border(scrn_to_world(scrn_border));

    //draw the border
    drawrect(border.x, border.y, bottom_right.x, bottom_right.y);

    //draw the background
    t_color light(224, 224, 224);
    setcolor(light);
    fillrect(bot_b);
    t_color dark(160, 160, 160);
    setcolor(dark);
    fillrect(top_b);
    setcolor(BLACK);
    drawline(scrn_to_world(left), scrn_to_world(right));
    //draw the message 
    setcolor(BLACK);
    settextattrs(12, 0);
    drawtext(error.x, error.y, "ERROR", 1535, 935);
    drawtext(text.x, text.y, message, 1535, 935);
    ok_button.display();
}

////////////////////////////////DRAW SEARCH WINDOW//////////////////////////////

void draw_search_bar() {
    if (!sideBar.isClicked()) return;

    if (search_bar_clicked) {
        setcolor(BLUE);
    } else {
        setcolor(BLACK);
    }
    setlinewidth(1);
    //define the current bound box
    t_point bottom_left(scrn_to_world(search_bar_bottom_left));
    t_point top_right(scrn_to_world(search_bar_top_right));
    t_bound_box search_bar(bottom_left, top_right);

    //two special offsets
    //one for border
    t_point scrn_border(search_bar_bottom_left.x - 1, search_bar_top_right.y - 1);
    t_point border(scrn_to_world(scrn_border));
    //one for text position
    t_point text(scrn_to_world(text_location));
    //draw the border
    drawrect(border.x, bottom_left.y, top_right.x, border.y);

    //draw the background
    setcolor(WHITE);
    fillrect(search_bar);

    //update the text inside
    settextattrs(12, 0);
    setcolor(BLACK);
    draw_alined_text(text.x, text.y, search_input, top_right.x, top_right.y);
    if (display_auto_complete) {
        draw_auto_complete_window();
    }
}

void draw_search_bar2() {
    if (!sideBar.isClicked()) return;

    if (search_bar2_clicked) {
        setcolor(BLUE);
    } else {
        setcolor(BLACK);
    }
    setlinewidth(1);
    //define the current bound box
    t_point bottom_left(scrn_to_world(search_bar2_bottom_left));
    t_point top_right(scrn_to_world(search_bar2_top_right));
    t_bound_box search_bar(bottom_left, top_right);

    //two special offsets
    //one for border
    t_point scrn_border(search_bar2_bottom_left.x - 1, search_bar2_top_right.y - 1);
    t_point border(scrn_to_world(scrn_border));
    //one for text position
    t_point text(scrn_to_world(text2_location));
    //draw the border
    drawrect(border.x, bottom_left.y, top_right.x, border.y);

    //draw the background
    setcolor(WHITE);
    fillrect(search_bar);

    //update the text inside
    settextattrs(12, 0);
    setcolor(BLACK);
    draw_alined_text(text.x, text.y, search_input2, top_right.x, top_right.y);

    if (display_auto_complete2) {
        draw_auto_complete_window2();
    }
}

void draw_auto_complete_window() {
    close_names = find_closest_names(search_input);

    //based on how many close names we have
    if (close_names.size() <= 0) return;

    //the closest name
    if (close_names.size() >= 1) {
        draw_single_name(1, close_names);
    }
    //second closest name
    if (close_names.size() >= 2) {
        draw_single_name(2, close_names);
    }
    if (close_names.size() >= 3) {
        draw_single_name(3, close_names);
    }
    if (close_names.size() >= 4) {
        draw_single_name(4, close_names);
    }
    return;
}

void draw_auto_complete_window2() {
    close_names2 = find_closest_names(search_input2);

    //based on how many close names we have
    if (close_names2.size() <= 0) return;

    //the closest name
    if (close_names2.size() >= 1) {
        draw_single_name2(1, close_names2);
    }
    //second closest name
    if (close_names2.size() >= 2) {
        draw_single_name2(2, close_names2);
    }
    if (close_names2.size() >= 3) {
        draw_single_name2(3, close_names2);
    }
    if (close_names2.size() >= 4) {
        draw_single_name2(4, close_names2);
    }
    return;
}

void draw_single_name(int position, vector<string> names) {
    //define the current bound box
    t_point temp1(search_bar_bottom_left.x, search_bar_bottom_left.y + 20 * position);
    t_point temp2(search_bar_top_right.x, search_bar_top_right.y + 20 * position);

    t_point bottom_left(scrn_to_world(temp1));
    t_point top_right(scrn_to_world(temp2));
    t_bound_box new_bar(bottom_left, top_right);

    //two special offsets
    //one for border
    t_point scrn_border(search_bar_bottom_left.x - 1, temp2.y - 1);
    t_point border(scrn_to_world(scrn_border));
    //one for text position
    t_point temp3(text_location.x, text_location.y + 20 * position);
    t_point text(scrn_to_world(temp3));
    //draw the border
    drawrect(border.x, bottom_left.y, top_right.x, border.y);

    //draw the background
    setcolor(WHITE);
    fillrect(new_bar);
    settextattrs(12, 0);
    setcolor(BLACK);
    draw_alined_text(text.x, text.y, names[position - 1], top_right.x, top_right.y);
}

void draw_single_name2(int position, vector<string> names) {
    //define the current bound box
    t_point temp1(search_bar2_bottom_left.x, search_bar2_bottom_left.y + 20 * position);
    t_point temp2(search_bar2_top_right.x, search_bar2_top_right.y + 20 * position);

    t_point bottom_left(scrn_to_world(temp1));
    t_point top_right(scrn_to_world(temp2));
    t_bound_box new_bar(bottom_left, top_right);

    //two special offsets
    //one for border
    t_point scrn_border(search_bar2_bottom_left.x - 1, temp2.y - 1);
    t_point border(scrn_to_world(scrn_border));
    //one for text position
    t_point temp3(text2_location.x, text2_location.y + 20 * position);
    t_point text(scrn_to_world(temp3));
    //draw the border
    drawrect(border.x, bottom_left.y, top_right.x, border.y);

    //draw the background
    setcolor(WHITE);
    fillrect(new_bar);
    settextattrs(12, 0);
    setcolor(BLACK);
    draw_alined_text(text.x, text.y, names[position - 1], top_right.x, top_right.y);
}

void draw_search_window() {
    if (!sideBar.isClicked()) return;
    setcolor(BLACK);
    setlinewidth(1);
    //define the current bound box
    t_point bottom_left(scrn_to_world(search_win_bottom_left));
    t_point top_right(scrn_to_world(search_win_top_right));
    t_bound_box search_win(bottom_left, top_right);

    //special offsets
    t_point scrn_border(search_win_bottom_left.x, search_win_top_right.y - 1);
    t_point border(scrn_to_world(scrn_border));

    //draw the border
    drawrect(border.x, bottom_left.y, top_right.x, border.y);

    //draw the background
    t_color t_white(255, 255, 255, 120);
    setcolor(t_white);

    //draw features on top
    fillrect(search_win);

    //draw the direction button
    //this will be always shown
    direction.display();

    //depending on whether the user is looking for directions
    //however, the first search bar is always displaying
    draw_search_bar();

    if (!show_direction) {
        draw_search_bar();
        t_point country_text(30, 485);
        t_point feature_text(30, 280);
        setcolor(BLACK);
        setfontsize(15);
        draw_alined_text(scrn_to_world(country_text).x,
                scrn_to_world(country_text).y,
                "Other Cities -----", 400, 500);

        draw_alined_text(scrn_to_world(feature_text).x,
                scrn_to_world(feature_text).y,
                "Useful Features -----", 400, 300);
        //buttons
        subway_button.display();
        gas_button.display();
        food_button.display();
        help.display();
        //country buttons
        toronto_button.display();
        beijing_button.display();
        new_york_button.display();
        london_button.display();
        sydney_button.display();
        tokyo_button.display();
        moscow_button.display();
        interlaken_button.display();
        iceland_button.display();
        singapore_button.display();
        rio_button.display();
        cairo_button.display();
    } else {
        if (display_path_info) {
            draw_direction_info();
            car_button.display();
            man_button.display();
            if (start > 0) {
                up_arrow_button.display();
            }
            if (more_info) {
                down_arrow_button.display();
            }
        }
        draw_search_bar2();
        draw_search_bar();
    }
}

//display the information of the path

void draw_direction_info() {

    unsigned end;

    if (!driving) {
        end = globalVar.naviRoute_seg.size() - start + 2;
        if (end > 9) {
            end = 9;
            more_info = true;
        } else {
            more_info = false;
        }
        for (unsigned i = 0; i < end; i++) {
            draw_one_direct_info(i);
        }
    } else {
        end = globalVar.draw_turn.size() - start + 2;
        if (end > 9) {
            end = 9;
            more_info = true;
        } else {
            more_info = false;
        }
        for (unsigned i = 0; i < end; i++) {
            draw_driving_info(i);
        }
    }

}

void draw_driving_info(unsigned i) {
    //draw a box
    setlinewidth(1);
    setcolor(BLACK);

    t_point temp1(direction_win_top_left.x, direction_win_top_left.y + 80 * i);
    t_point temp2(direction_win_bottom_right.x, direction_win_bottom_right.y + 80 * i);
    //define the current bound box
    t_point top_left(scrn_to_world(temp1));
    t_point bottom_right(scrn_to_world(temp2));
    t_bound_box window(top_left, bottom_right);

    //one for border
    t_point scrn_border(temp1.x - 1, temp1.y - 1);
    t_point border(scrn_to_world(scrn_border));

    //draw the border
    drawrect(border.x, border.y, bottom_right.x, bottom_right.y);

    //draw the background
    setcolor(WHITE);
    fillrect(window);

    //define special location for directions
    t_point forward(temp1.x + 20, temp1.y + 5);
    t_point turn(temp1.x, temp1.y + 6);
    t_point end(temp1.x + 10, temp1.y + 3);
    t_point center(temp1.x + 175, temp1.y + 40);
    t_point info(temp1.x + 80, temp1.y + 40);
    t_point name_location(temp1.x + 80, temp1.y + 60);
    t_point length(temp1.x + 80, temp1.y + 10);
    t_point ori(temp1.x + 250, temp1.y + 10);

    string dis;
    string message;
    string name;
    string m = "m";
    setcolor(BLACK);

    //start and end 
    if (i == 0 && start == 0) {
        draw_surface(start_image, scrn_to_world(end));
        settextattrs(15, 0);
        message = "Starting";
        drawtext(scrn_to_world(center).x, scrn_to_world(center).y, message, 1500, 935);
        return;
    }

    if (i + start > globalVar.draw_turn.size()) {
        draw_surface(end_image, scrn_to_world(end));
        settextattrs(15, 0);
        message = "You have arrived";
        drawtext(scrn_to_world(center).x, scrn_to_world(center).y, message, 1500, 935);
        settextattrs(12, 0);
        return;
    }

    double distance = globalVar.move_distance[i + start - 1];
    distance = round(distance);

    dis = to_string(distance);
    size_t pos = dis.find(".");
    dis = dis.substr(0, pos);
    dis.append(m);

    //cout << globalVar.draw_dongnanxibei[i + start - 1] << endl;
    //cout << dsnb[globalVar.draw_dongnanxibei[i + start - 1]] << endl;

    //going straight
    if (globalVar.draw_turn[i + start - 1] == 0) {
        draw_surface(straight, scrn_to_world(forward));
        message = "Moving forward on ";
        name = getStreetName(getStreetSegmentInfo(globalVar.draw_street_seg[i + start - 1]).streetID);
        settextattrs(15, 0);
        draw_alined_text(scrn_to_world(length).x, scrn_to_world(length).y, dis, 1500, 935);
        draw_alined_text(scrn_to_world(ori).x, scrn_to_world(ori).y, dsnb[globalVar.draw_dongnanxibei[i + start - 1]], 1500, 935);
        settextattrs(12, 0);
        draw_alined_text(scrn_to_world(info).x, scrn_to_world(info).y, message, 1500, 935);
        draw_alined_text(scrn_to_world(name_location).x, scrn_to_world(name_location).y, name, 1500, 935);
    }//turning left
    else if (globalVar.draw_turn[i + start - 1] == 1) {
        draw_surface(left_turn, scrn_to_world(temp1));
        message = "Turn left onto ";
        name = getStreetName(getStreetSegmentInfo(globalVar.draw_street_seg[i + start]).streetID);
        settextattrs(15, 0);
        draw_alined_text(scrn_to_world(length).x, scrn_to_world(length).y, dis, 1500, 935);
        draw_alined_text(scrn_to_world(ori).x, scrn_to_world(ori).y, dsnb[globalVar.draw_dongnanxibei[i + start - 1]], 1500, 935);
        settextattrs(12, 0);
        draw_alined_text(scrn_to_world(info).x, scrn_to_world(info).y, message, 1500, 935);
        draw_alined_text(scrn_to_world(name_location).x, scrn_to_world(name_location).y, name, 1500, 935);
    }//turning right
    else if (globalVar.draw_turn[i + start - 1] == 2) {
        draw_surface(right_turn, scrn_to_world(temp1));
        message = "Turn right onto ";
        name = getStreetName(getStreetSegmentInfo(globalVar.draw_street_seg[i + start]).streetID);
        settextattrs(15, 0);
        draw_alined_text(scrn_to_world(length).x, scrn_to_world(length).y, dis, 1500, 935);
        draw_alined_text(scrn_to_world(ori).x, scrn_to_world(ori).y, dsnb[globalVar.draw_dongnanxibei[i + start - 1]], 1500, 935);
        settextattrs(12, 0);
        draw_alined_text(scrn_to_world(info).x, scrn_to_world(info).y, message, 1500, 935);
        draw_alined_text(scrn_to_world(name_location).x, scrn_to_world(name_location).y, name, 1500, 935);
    }
}


//display one single segment information

void draw_one_direct_info(unsigned i) {
    //draw a box
    setlinewidth(1);
    setcolor(BLACK);

    t_point temp1(direction_win_top_left.x, direction_win_top_left.y + 80 * i);
    t_point temp2(direction_win_bottom_right.x, direction_win_bottom_right.y + 80 * i);
    //define the current bound box
    t_point top_left(scrn_to_world(temp1));
    t_point bottom_right(scrn_to_world(temp2));
    t_bound_box window(top_left, bottom_right);

    //one for border
    t_point scrn_border(temp1.x - 1, temp1.y - 1);
    t_point border(scrn_to_world(scrn_border));

    //draw the border
    drawrect(border.x, border.y, bottom_right.x, bottom_right.y);

    //draw the background
    setcolor(WHITE);
    fillrect(window);

    //define special location for directions
    t_point forward(temp1.x + 20, temp1.y + 5);
    t_point turn(temp1.x, temp1.y + 6);
    t_point end(temp1.x + 10, temp1.y + 3);
    t_point center(temp1.x + 175, temp1.y + 40);
    t_point info(temp1.x + 80, temp1.y + 40);
    t_point name_location(temp1.x + 80, temp1.y + 60);
    t_point length(temp1.x + 80, temp1.y + 10);
    t_point ori(temp1.x + 250, temp1.y + 10);

    double distance = globalVar.ssArray[globalVar.naviRoute_seg[i + start - 1]].length;
    distance = round(distance);
    string dis;
    string message;
    string name;
    string m = "m";

    dis = to_string(distance);
    size_t pos = dis.find(".");
    dis = dis.substr(0, pos);
    dis.append(m);
    setcolor(BLACK);

    //start and end 
    if (i == 0 && start == 0) {
        draw_surface(start_image, scrn_to_world(end));
        settextattrs(15, 0);
        message = "Starting";
        drawtext(scrn_to_world(center).x, scrn_to_world(center).y, message, 1500, 935);
        return;
    }

    if (i + start > globalVar.naviRoute_seg.size()) {
        draw_surface(end_image, scrn_to_world(end));
        settextattrs(15, 0);
        message = "You have arrived";
        drawtext(scrn_to_world(center).x, scrn_to_world(center).y, message, 1500, 935);
        settextattrs(12, 0);
        return;
    }

    //going straight
    if (globalVar.turn_by_turn[i + start - 1] == 0) {
        draw_surface(straight, scrn_to_world(forward));
        message = "Moving forward on ";
        name = getStreetName(getStreetSegmentInfo(globalVar.naviRoute_seg[i + start - 1]).streetID);
        settextattrs(15, 0);
        draw_alined_text(scrn_to_world(length).x, scrn_to_world(length).y, dis, 1500, 935);
        draw_alined_text(scrn_to_world(ori).x, scrn_to_world(ori).y, dsnb[globalVar.dongnanxibei[i + start - 1]], 1500, 935);
        settextattrs(12, 0);
        draw_alined_text(scrn_to_world(info).x, scrn_to_world(info).y, message, 1500, 935);
        draw_alined_text(scrn_to_world(name_location).x, scrn_to_world(name_location).y, name, 1500, 935);
    }//turning left
    else if (globalVar.turn_by_turn[i + start - 1] == 1) {
        draw_surface(left_turn, scrn_to_world(temp1));
        message = "Turn left onto ";
        name = getStreetName(getStreetSegmentInfo(globalVar.naviRoute_seg[i + start]).streetID);
        settextattrs(15, 0);
        draw_alined_text(scrn_to_world(length).x, scrn_to_world(length).y, dis, 1500, 935);
        draw_alined_text(scrn_to_world(ori).x, scrn_to_world(ori).y, dsnb[globalVar.dongnanxibei[i + start - 1]], 1500, 935);
        settextattrs(12, 0);
        draw_alined_text(scrn_to_world(info).x, scrn_to_world(info).y, message, 1500, 935);
        draw_alined_text(scrn_to_world(name_location).x, scrn_to_world(name_location).y, name, 1500, 935);
    }//turning right
    else if (globalVar.turn_by_turn[i + start - 1] == 2) {
        draw_surface(right_turn, scrn_to_world(temp1));
        message = "Turn right onto ";
        name = getStreetName(getStreetSegmentInfo(globalVar.naviRoute_seg[i + start]).streetID);
        settextattrs(15, 0);
        draw_alined_text(scrn_to_world(length).x, scrn_to_world(length).y, dis, 1500, 935);
        draw_alined_text(scrn_to_world(ori).x, scrn_to_world(ori).y, dsnb[globalVar.dongnanxibei[i + start - 1]], 1500, 935);
        settextattrs(12, 0);
        draw_alined_text(scrn_to_world(info).x, scrn_to_world(info).y, message, 1500, 935);
        draw_alined_text(scrn_to_world(name_location).x, scrn_to_world(name_location).y, name, 1500, 935);
    }
}

//once the user clicks, it will draw these help messages

void draw_help_win() {
    setcolor(BLACK);
    settextattrs(20, 0);

    //help message for search bar 
    t_point arrow(360, 35);
    draw_surface(black_arrow, scrn_to_world(arrow));
    t_point search(400, 45);
    draw_alined_text(scrn_to_world(search).x, scrn_to_world(search).y, "Search Bar. Search street names or point of interests here", 1500, 935);

    //help message for direction button
    t_point arrow1(450, 145);
    draw_surface(black_arrow, scrn_to_world(arrow1));
    t_point direct(490, 155);
    draw_alined_text(scrn_to_world(direct).x, scrn_to_world(direct).y, "Direction button. Click here and go to search path menu", 1500, 935);

    //help message for show feature buttons
    t_point arrow2(400, 380);
    draw_surface(black_arrow, scrn_to_world(arrow2));
    t_point feature(440, 390);
    draw_alined_text(scrn_to_world(feature).x, scrn_to_world(feature).y, "Feature buttons. Click to draw features on the map", 1500, 935);

    //help message for flag buttons
    t_point arrow3(400, 700);
    draw_surface(black_arrow, scrn_to_world(arrow3));
    t_point flag(440, 710);
    draw_alined_text(scrn_to_world(flag).x, scrn_to_world(flag).y, "Flag buttons. Click to change to another city", 1500, 935);

    //help message for functionalities buttons
    t_point main(1365, 140);
    drawtext(scrn_to_world(main).x, scrn_to_world(main).y, "Major buttons", 1500, 935);
    t_point main1(1365, 170);
    drawtext(scrn_to_world(main1).x, scrn_to_world(main1).y, "Zoom fit. Exit", 1500, 935);
    t_point main2(1365, 200);
    drawtext(scrn_to_world(main2).x, scrn_to_world(main2).y, "Zoom in, Zoom out", 1500, 935);
}

bool search_bar_clicking(float x, float y) {
    if (x > scrn_to_world(search_bar_bottom_left).x
            && x < scrn_to_world(search_bar_top_right).x
            && y > scrn_to_world(search_bar_bottom_left).y
            && y < scrn_to_world(search_bar_top_right).y) {
        if (show_direction) search_bar2_clicking(x, y);
        display_auto_complete = !display_auto_complete;
        if (!display_auto_complete) drawMap();
        search_bar_clicked = true;
        drawMap();
        return true;
    }//user not clicking within the search bar location,
        //and the search bar is not highlighted, which means that
        //the user is clicking somewhere else on the map
    else if (!search_bar_clicked) {
        return false;
    }

    //else if the user is clicking the auto completes
    if (display_auto_complete) {
        int temp;
        if (close_names.size() > 4) {
            temp = 4;
        } else {
            temp = close_names.size();
        }

        for (int i = 1; i <= temp; i++) {
            //if any of the auto completes is being clicked
            if (auto_complete_clicking(i, x, y)) {
                search_bar_clicked = true;
                search_input = close_names[i - 1];
                display_auto_complete = false;
                drawMap();
                return true;
            }
        }
    }

    //none of above, clicking somewhere else
    search_bar_clicked = false;
    display_auto_complete = false;
    drawMap();
    return false;
}

bool search_bar2_clicking(float x, float y) {

    if (x > scrn_to_world(search_bar2_bottom_left).x
            && x < scrn_to_world(search_bar2_top_right).x
            && y > scrn_to_world(search_bar2_bottom_left).y
            && y < scrn_to_world(search_bar2_top_right).y) {
        display_auto_complete2 = !display_auto_complete2;
        if (!display_auto_complete2) drawMap();
        search_bar2_clicked = true;
        drawMap();
        return true;
    }//user not clicking within the search bar location,
        //and the search bar is not highlighted, which means that
        //the user is clicking somewhere else on the map
    else if (!search_bar2_clicked) {
        return false;
    }
    //else if the user is clicking the auto completes
    if (display_auto_complete2) {
        int temp;
        if (close_names2.size() > 4) {
            temp = 4;
        } else {
            temp = close_names2.size();
        }

        for (int i = 1; i <= temp; i++) {
            //if any of the auto completes is being clicked
            if (auto_complete2_clicking(i, x, y)) {
                search_bar2_clicked = true;
                search_input2 = close_names2[i - 1];
                display_auto_complete2 = false;
                drawMap();
                return true;
            }
        }
    }

    //none of above, clicking somewhere else
    search_bar2_clicked = false;
    display_auto_complete2 = false;
    drawMap();
    return false;
}

bool auto_complete_clicking(int position, float x, float y) {
    //define the current bound box based on position
    t_point temp1(search_bar_bottom_left.x, search_bar_bottom_left.y + 20 * position);
    t_point temp2(search_bar_top_right.x, search_bar_top_right.y + 20 * position);

    if (x > scrn_to_world(temp1).x
            && x < scrn_to_world(temp2).x
            && y > scrn_to_world(temp1).y
            && y < scrn_to_world(temp2).y) {
        return true;
    }
    return false;
}

bool auto_complete2_clicking(int position, float x, float y) {
    //define the current bound box based on position
    t_point temp1(search_bar2_bottom_left.x, search_bar2_bottom_left.y + 20 * position);
    t_point temp2(search_bar2_top_right.x, search_bar2_top_right.y + 20 * position);

    if (x > scrn_to_world(temp1).x
            && x < scrn_to_world(temp2).x
            && y > scrn_to_world(temp1).y
            && y < scrn_to_world(temp2).y) {
        return true;
    }
    return false;
}

////////////////////////////////COUNTRY BUTTON CLICKS//////////////////////////

void handle_toronto_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_beijing_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/beijing_china.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_new_york_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/new-york_usa.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_london_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/london_england.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_sydney_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/sydney_australia.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_tokyo_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/tokyo_japan.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_moscow_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/moscow_russia.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_interlaken_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/interlaken_switzerland.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_iceland_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/iceland.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_singapore_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/singapore.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_rio_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_cairo_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/cairo_egypt.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

void handle_hongkong_clicked() {
    close_map();
    load_map("/cad2/ece297s/public/maps/hong-kong_china.streets.bin");
    setScale();
    clearData();
    set_visible_world(0, 0, SCREEN_LENGTH_MAX_X, 935);
    drawMap();
}

//////////////////////////////////FIND FUNCTION/////////////////////////////////

//if the user press "enter" on the keyboard, which means that they
//want to see if there are intersection between two streets

void findIntersection() {
    string street1, street2;
    vector<unsigned> intersection_ids;
    int middlePoint = -1;
    //assuming the input will be "street1&street2"
    //find where the "/" is
    for (unsigned i = 0; i < search_input.size(); i++) {
        //if find where '&' is, record its position
        if (search_input[i] == '&') {
            middlePoint = i;
            break;
        }
    }

    //if there was no '&' symbol, which means user entered wrong
    if (middlePoint == -1) {
        vector<unsigned> temp = find_street_ids_from_name(search_input);
        if (temp.size() == 0) {
            if (search_input == "Beijing") {
                handle_beijing_clicked();
                return;
            } else if (search_input == "Hongkong") {
                handle_hongkong_clicked();
                return;
            }
            draw_error("Sorry the street you entered does not exist");
            globalVar.error = true;
            return;
        } else {
            if (search_input == "<unknown>") {
                draw_error("Sorry, cannot draw street with name <unknown>");
                globalVar.error = true;
                return;
            }
            highlight = true;
            drawMap();
        }
        return;
    }

    //else get the two street name and try to find the intersection in between
    street1 = search_input.substr(0, middlePoint - 1);
    street2 = search_input.substr(middlePoint + 2, search_input.size());

    //reject searching names with <unknowm> (Way to many and pointless)
    if (street1 == "<unknown>" || street2 == "<unknown>") {
        draw_error("Sorry, please enter street names that is now \"unknown\".");
        globalVar.error = true;
        return;
    }

    //find the intersection
    intersection_ids = find_intersection_ids_from_street_names(street1, street2);

    //if no intersections between them
    if (intersection_ids.size() == 0) {
        draw_error("Sorry, the intersection does not exist at the two roads you entered");
        globalVar.error = true;
        return;
    }

    //else highlight the intersections
    for (unsigned i = 0; i < intersection_ids.size(); i++) {
        highlight_intersection(intersection_ids[i]);
    }

    return;
}

///////////////////////////////////DRAW FEATURES////////////////////////////////

void highlight_intersection(unsigned intersection_id) {
    t_point initial(globalVar.x_scale * (globalVar.siArray[intersection_id].x -
            globalVar.x_min) - 13 / globalVar.drawScale, globalVar.y_scale *
            (globalVar.siArray[intersection_id].y -
            globalVar.y_max) + 73 / globalVar.drawScale);
    drawIcon("libstreetmap/resources/pointer.png", initial);
    settextattrs(12, 0);
    setcolor(BLACK);
    drawtext(initial.x, initial.y, getIntersectionName(intersection_id));
}

void highlight_start(unsigned id) {
    t_point initial(globalVar.x_scale * (globalVar.siArray[id].x -
            globalVar.x_min) - 29.5 / globalVar.drawScale, globalVar.y_scale *
            (globalVar.siArray[id].y -
            globalVar.y_max) + 70 / globalVar.drawScale);
    drawIcon("libstreetmap/resources/start.png", initial);
    settextattrs(12, 0);
    setcolor(BLACK);
    drawtext(initial.x, initial.y, getIntersectionName(id));
}

void highlight_end(unsigned id) {
    t_point initial(globalVar.x_scale * (globalVar.siArray[id].x -
            globalVar.x_min) - 29.5 / globalVar.drawScale, globalVar.y_scale *
            (globalVar.siArray[id].y -
            globalVar.y_max) + 70 / globalVar.drawScale);
    drawIcon("libstreetmap/resources/end.png", initial);
    settextattrs(12, 0);
    setcolor(BLACK);
    drawtext(initial.x, initial.y, getIntersectionName(id));
}


// function to draw all subway entrances

void draw_metro() {
    if (globalVar.zoomScale > 60) {
        for (unsigned i = 0; i < globalVar.metroStations.size(); i++) {

            t_point station((DEG_TO_RAD * globalVar.metroStations[i].lon() *
                    cos(DEG_TO_RAD * globalVar.latavg) -
                    globalVar.x_min) * globalVar.x_scale,
                    (DEG_TO_RAD * globalVar.metroStations[i].lat() -
                    globalVar.y_max) * globalVar.y_scale);
            drawIcon("libstreetmap/resources/metro.png", station);
        }
    }
}

// function to draw all the hospitals

void draw_hospital() {

    for (unsigned i = 0; i < globalVar.hospital.size(); i++) {

        t_point hospital((DEG_TO_RAD * globalVar.hospital[i].lon() * cos(DEG_TO_RAD *
                globalVar.latavg) - globalVar.x_min) * globalVar.x_scale,
                (DEG_TO_RAD * globalVar.hospital[i].lat() -
                globalVar.y_max) * globalVar.y_scale);
        drawIcon("libstreetmap/resources/hospital.png", hospital);
        settextattrs(7, 0);
        drawtext(hospital.x + 1 * 10 / globalVar.zoomScale, hospital.y - 3.5 * 10 /
                globalVar.zoomScale, getPointOfInterestName(globalVar.hospitalID[i]));
    }

}

//function to draw all the restaurants

void draw_restaurant() {
    if (!display_food) return;

    for (unsigned i = 0; i < globalVar.restaurant.size(); i++) {

        t_point restaurant((DEG_TO_RAD * globalVar.restaurant[i].lon() *
                cos(DEG_TO_RAD * globalVar.latavg) - globalVar.x_min) *
                globalVar.x_scale, (DEG_TO_RAD * globalVar.restaurant[i].lat() -
                globalVar.y_max) * globalVar.y_scale);
        drawIcon("libstreetmap/resources/dining.png", restaurant);
        settextattrs(7, 0);
        setcolor(251, 153, 91);
        drawtext(restaurant.x + 1 * 10 / globalVar.zoomScale, restaurant.y -
                3.5 * 10 /
                globalVar.zoomScale,
                getPointOfInterestName(globalVar.restaurantID[i]));
    }
}

// function to draw all the gas station

void draw_fuelstation() {
    if (!display_gas) return;
    for (unsigned i = 0; i < globalVar.fuelstation.size(); i++) {

        t_point fuelstation((DEG_TO_RAD * globalVar.fuelstation[i].lon() *
                cos(DEG_TO_RAD * globalVar.latavg) -
                globalVar.x_min) * globalVar.x_scale,
                (DEG_TO_RAD * globalVar.fuelstation[i].lat() -
                globalVar.y_max) * globalVar.y_scale);
        drawIcon("libstreetmap/resources/fuel.png", fuelstation);
    }
}
// function to draw all the banks

void draw_bank() {

    for (unsigned i = 0; i < globalVar.bank.size(); i++) {

        t_point bank((DEG_TO_RAD * globalVar.bank[i].lon() *
                cos(DEG_TO_RAD * globalVar.latavg) - globalVar.x_min) *
                globalVar.x_scale,
                (DEG_TO_RAD * globalVar.bank[i].lat() -
                globalVar.y_max) * globalVar.y_scale);
        drawIcon("libstreetmap/resources/bank.png", bank);
        settextattrs(7, 0);
        setcolor(121, 139, 182);
        drawtext(bank.x + 1 * 10 / globalVar.zoomScale, bank.y -
                3.5 * 10 /
                globalVar.zoomScale,
                getPointOfInterestName(globalVar.bankID[i]));
    }
}

// function to draw all the education places

void draw_edu() {

    for (unsigned i = 0; i < globalVar.edu.size(); i++) {

        t_point edu((DEG_TO_RAD * globalVar.edu[i].lon() *
                cos(DEG_TO_RAD * globalVar.latavg) - globalVar.x_min) *
                globalVar.x_scale,
                (DEG_TO_RAD * globalVar.edu[i].lat() -
                globalVar.y_max) * globalVar.y_scale);
        drawIcon("libstreetmap/resources/edu.png", edu);
        settextattrs(7, 0);
        setcolor(101, 116, 152);
        drawtext(edu.x + 1 * 10 / globalVar.zoomScale, edu.y -
                3.5 * 10 / globalVar.zoomScale,
                getPointOfInterestName(globalVar.eduID[i]));
    }
}


// function to draw all the subwaylines from the OSM relations

void draw_subwaylines() {
    if (!display_subway) return;
    for (unsigned lineIndex = 0; lineIndex < globalVar.metro.size(); lineIndex++) {
        setcolor(globalVar.metroColor[lineIndex]);
        for (unsigned wayID = 0; wayID < globalVar.metro[lineIndex].size(); wayID++) {
            for (unsigned nodeID = 0; nodeID < globalVar.metro[lineIndex][wayID].size() - 1; nodeID++) {
                t_point start2((globalVar.metro[lineIndex][wayID][nodeID].x -
                        globalVar.x_min) * globalVar.x_scale,
                        (globalVar.metro[lineIndex][wayID][nodeID].y -
                        globalVar.y_max) * globalVar.y_scale);
                t_point end((globalVar.metro[lineIndex][wayID][nodeID + 1].x -
                        globalVar.x_min) * globalVar.x_scale,
                        (globalVar.metro[lineIndex][wayID][nodeID + 1].y -
                        globalVar.y_max) * globalVar.y_scale);

                setlinewidth(5);
                drawline(start2.x, start2.y, end.x, end.y);
            }
        }
    }

}

//function to change OSMID to index

int OSMIDToWayIndex1(OSMID osmid) {
    unordered_map<OSMID, unsigned>::iterator it = globalVar.OSMIDToWayIndex.find(osmid);
    if (it == globalVar.OSMIDToWayIndex.end()) {
        return -1;
    }
    return it->second;
}


// function to draw all the features

void drawFeatures() {
    for (unsigned fcount = 0; fcount < globalVar.areaFeatures.size(); fcount++) {

        // for (unsigned i = 0; i < getNumberOfFeatures(); i++) {
        FeatureType t = getFeatureType(globalVar.areaFeatures[fcount].featureIndex);

        switch (t) {
            case Lake:
                setcolor(176, 224, 230, 255);
                fillpoly(globalVar.areaFeatures[fcount].fpoints,
                        getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex));

                break;


            case River:
                // if the river is closed
                if (globalVar.areaFeatures[fcount].fpoints[0].x ==
                        globalVar.areaFeatures[fcount].fpoints[getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex) - 1].x
                        && globalVar.areaFeatures[fcount].fpoints[0].y ==
                        globalVar.areaFeatures[fcount].fpoints[getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex) - 1].y) {
                    setcolor(176, 224, 230, 255);
                    //fillpoly(featurePoints, getFeaturePointCount(i));


                } else {
                    setcolor(RED);

                    // if it is an open river (not closed)
                    for (unsigned j = 0; j <
                            getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex) - 1; j++) {
                        setlinewidth(5);
                        drawline(globalVar.areaFeatures[fcount].fpoints[j].x,
                                globalVar.areaFeatures[fcount].fpoints[j].y,
                                globalVar.areaFeatures[fcount].fpoints[j + 1].x,
                                globalVar.areaFeatures[fcount].fpoints[j + 1].y);
                    }

                }
                break;

            case Island:
                setcolor(255, 235, 205);
                fillpoly(globalVar.areaFeatures[fcount].fpoints,
                        getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex));

                break;



            case Greenspace:
                setcolor(189, 252, 201, 255);
                fillpoly(globalVar.areaFeatures[fcount].fpoints,
                        getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex));


                break;


            case Building:
                if (globalVar.zoomScale > 100) {
                    
                    
                    
                    
                    
                    setcolor(238, 233, 224);
                    fillpoly(globalVar.areaFeatures[fcount].fpoints,
                            getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex));
                    for (unsigned j = 0; j <
                            getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex) - 1; j++) {
                       
                        setlinewidth(1);
                        setcolor(227, 220, 198);
                        drawline(globalVar.areaFeatures[fcount].fpoints[j].x,
                                globalVar.areaFeatures[fcount].fpoints[j].y,
                                globalVar.areaFeatures[fcount].fpoints[j + 1].x,
                                globalVar.areaFeatures[fcount].fpoints[j + 1].y);
                    }
                }

                break;

            case Beach:
                setcolor(KHAKI);
                fillpoly(globalVar.areaFeatures[fcount].fpoints,
                        getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex));


                break;



            case Park:
                setcolor(183, 230, 165, 255);
                fillpoly(globalVar.areaFeatures[fcount].fpoints,
                        getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex));


                break;

            case Shoreline:
                setcolor(BLUE);


                for (unsigned j = 0; j <
                        getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex) - 1; j++) {
                    setlinewidth(5);
                    
                    drawline(globalVar.areaFeatures[fcount].fpoints[j].x,
                            globalVar.areaFeatures[fcount].fpoints[j].y,
                            globalVar.areaFeatures[fcount].fpoints[j + 1].x,
                            globalVar.areaFeatures[fcount].fpoints[j + 1].y);
                }


                break;

            case Golfcourse:
                setcolor(189, 252, 201, 255);
                fillpoly(globalVar.areaFeatures[fcount].fpoints,
                        getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex));
                break;

            case Stream:
                if (globalVar.zoomScale < 60) {

                    break;
                }
                if (globalVar.areaFeatures[fcount].fpoints[0].x ==
                        globalVar.areaFeatures[fcount].fpoints[getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex) - 1].x
                        && globalVar.areaFeatures[fcount].fpoints[0].y ==
                        globalVar.areaFeatures[fcount].fpoints[getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex) - 1].y) {
                    setcolor(152, 216, 236, 255);
                    //fillpoly(featurePoints, getFeaturePointCount(i));


                } else {
                    setcolor(152, 216, 236, 255);

                    // if it is an open river (not closed)
                    for (unsigned j = 0; j < getFeaturePointCount(globalVar.areaFeatures[fcount].featureIndex) - 1; j++) {
                        setlinewidth(5);
                       
                        drawline(globalVar.areaFeatures[fcount].fpoints[j].x,
                                globalVar.areaFeatures[fcount].fpoints[j].y,
                                globalVar.areaFeatures[fcount].fpoints[j + 1].x,
                                globalVar.areaFeatures[fcount].fpoints[j + 1].y);
                    }

                }
                break;

            case Unknown:
                break;
            default:
                break;
        }

    }
}

void clearData() {
    search_input = "";
    search_input2 = "";
    highlight = false;
    search_bar_clicked = false;
    display_auto_complete = false;
    display_subway = false;
    display_gas = false;
    display_food = false;
    show_direction = false;
    drawNavi = false;
    draw_help = false;
    display_path_info = false;
    start = 0;
    more_info = true;
    globalVar.error = false;
    globalVar.naviRoute.clear();
    globalVar.naviRoute_seg.clear();
}

void highlight_interest(unsigned interest_id, float x, float y) {
    t_point initial(x - 13 / globalVar.drawScale, y + 73 / globalVar.drawScale);
    drawIcon("libstreetmap/resources/pointer.png", initial);
    settextattrs(12, 0);
    drawtext(initial.x, initial.y, getPointOfInterestName(interest_id));
}

void findDirection() {
    string street1, street2, street3, street4;
    vector<unsigned> id1, id2;
    int middlePoint1 = -1;
    int middlePoint2 = -1;
    //assuming the input will be "street1&street2"
    //find where the "&" is
    for (unsigned i = 0; i < search_input.size(); i++) {
        //if find where '&' is, record its position
        if (search_input[i] == '&') {
            middlePoint1 = i;
            break;
        }
    }

    //assuming the input will be "street1&street2"
    //find where the "&" is
    for (unsigned i = 0; i < search_input2.size(); i++) {
        //if find where '&' is, record its position
        if (search_input2[i] == '&') {
            middlePoint2 = i;
            break;
        }
    }

    //user if looking for direction from two intersections
    if (middlePoint1 != -1 && middlePoint2 != -1) {
        //else get the two street name and try to find the intersection in between
        street1 = search_input.substr(0, middlePoint1 - 1);
        street2 = search_input.substr(middlePoint1 + 2, search_input.size());
        street3 = search_input2.substr(0, middlePoint2 - 1);
        street4 = search_input2.substr(middlePoint2 + 2, search_input2.size());

        //reject searching names with <unknowm> (Way to many and pointless)
        if (street1 == "<unknown>" || street2 == "<unknown>"
                || street3 == "<unknown>" || street4 == "<unknown>") {
            draw_error("Sorry, please enter street name that is not \"unknown\".");
            globalVar.error = true;
            return;
        }

        //find the intersection
        id1 = find_intersection_ids_from_street_names(street1, street2);
        id2 = find_intersection_ids_from_street_names(street3, street4);

        if (id1.size() <= 0 || id2.size() <= 0) {
            draw_error("Sorry, the intersection you entered does not exist");
            globalVar.error = true;
            return;
        } else {
            cout << "loading" << endl;
            find_path_between_intersections(id1[0], id2[0], 0);
            if (globalVar.naviRoute_seg.size() <= 0) {
                draw_error("Sorry, there is no path existing between these two locations");
                globalVar.error = true;
                return;
            }
            get_segment_array();
            navi_info_reconstruct();
            departure = id1[0];
            destination = id2[0];
            drawNavi = true;
            draw_poi = false;
            display_path_info = true;
            globalVar.zoomnavi = true;
            start = 0;
            drawMap();
        }
    } else if (middlePoint1 == -1 && middlePoint2 == -1) {
        //user entered no intersections
        draw_error("Sorry, please enter at least one intersection to find path");
        globalVar.error = true;
        return;
    } else if (middlePoint1 == -1) {
        vector<unsigned> poi_id = get_poi_from_name1(search_input);

        //poi entered does not exist
        if (poi_id.size() == 0) {
            draw_error("Sorry, the point of interest you entered does not exit");
            globalVar.error = true;
            return;
        }

        //get two street names
        street3 = search_input2.substr(0, middlePoint2 - 1);
        street4 = search_input2.substr(middlePoint2 + 2, search_input2.size());

        //reject searching names with <unknowm> (Way to many and pointless)
        if (street3 == "<unknown>" || street4 == "<unknown>") {
            draw_error("Sorry, please enter street name that is not \"unknown\".");
            globalVar.error = true;
            return;
        }

        id2 = find_intersection_ids_from_street_names(street3, street4);

        //intersection entered does not exist
        if (id2.size() <= 0) {
            draw_error("Sorry, the intersection you entered does not exist");
            globalVar.error = true;
            return;
        }

        //find path 
        cout << "loading" << endl;
        find_path_to_point_of_interest(id2[0], search_input, 0);
        if (globalVar.naviRoute_seg.size() <= 0) {
            draw_error("Sorry, there is no path existing between these two locations");
            globalVar.error = true;
            return;
        }
        get_segment_array();
        navi_info_reconstruct();
        departure = id2[0];
        //destination = id2[0];
        drawNavi = true;
        driving = true;
        display_path_info = true;
        globalVar.zoomnavi = true;
        draw_poi = true;
        start = 0;
        drawMap();
    } else if (middlePoint2 == -1) {
        vector<unsigned> poi_id = get_poi_from_name1(search_input2);

        //poi entered does not exist
        if (poi_id.size() == 0) {
            draw_error("Sorry, the point of interest you entered does not exit");
            globalVar.error = true;
            return;
        }

        //get two street names
        street1 = search_input.substr(0, middlePoint1 - 1);
        street2 = search_input.substr(middlePoint1 + 2, search_input.size());

        //reject searching names with <unknowm> (Way to many and pointless)
        if (street1 == "<unknown>" || street2 == "<unknown>") {
            draw_error("Sorry, please enter street name that is not \"unknown\".");
            globalVar.error = true;
            return;
        }

        id1 = find_intersection_ids_from_street_names(street1, street2);

        //intersection entered does not exist
        if (id1.size() <= 0) {
            draw_error("Sorry, the intersection you entered does not exist");
            globalVar.error = true;
            return;
        }

        //find path 
        cout << "loading" << endl;
        find_path_to_point_of_interest(id1[0], search_input2, 0);
        if (globalVar.naviRoute_seg.size() <= 0) {
            draw_error("Sorry, there is no path existing between these two locations");
            globalVar.error = true;
            return;
        }
        get_segment_array();
        navi_info_reconstruct();
        departure = id1[0];
        //destination = id2[0];
        drawNavi = true;
        driving = true;
        draw_poi = true;
        display_path_info = true;
        globalVar.zoomnavi = true;
        start = 0;
        drawMap();
    }

}

void drawNaviSeg() {
    setlinewidth(4);
    setcolor(BLUE);

    for (unsigned street_seg_index = 0; street_seg_index < globalVar.naviRoute_seg.size(); street_seg_index++) {
        unsigned seg_id = globalVar.naviRoute_seg[street_seg_index];

        StreetSegmentInfo info = getStreetSegmentInfo(seg_id);
        //get the segments two intersections real coordinates
        float x1 = globalVar.siArray[info.from].x - globalVar.x_min;
        float y1 = (globalVar.siArray[info.from].y - globalVar.y_max);
        float x2 = globalVar.siArray[info.to].x - globalVar.x_min;
        float y2 = (globalVar.siArray[info.to].y - globalVar.y_max);
        //get the screen coordinates of the above points
        t_point pt1_screen(x1 * globalVar.x_scale, y1 * globalVar.y_scale);
        t_point pt2_screen(x2 * globalVar.x_scale, y2 * globalVar.y_scale);


        unsigned numofCurvePoints = info.curvePointCount;

        if (numofCurvePoints > 1) {
            //get the first curve points
            LatLon curvePoint_LL = getStreetSegmentCurvePoint(seg_id, 0);
            //get the curve points real coordinates and then transform it into
            //screen coordinates, and draw the from to first curve points
            t_point curvePoint_screen = LatLon_to_screen_XY(curvePoint_LL);

            drawline(pt1_screen.x, pt1_screen.y, curvePoint_screen.x, curvePoint_screen.y);

            //this for loop draws the segments formed by curve points
            for (unsigned curvePointsIndex = 0; curvePointsIndex < numofCurvePoints - 1; curvePointsIndex++) {
                LatLon curvePoint_LL_a = getStreetSegmentCurvePoint(seg_id, curvePointsIndex);
                LatLon curvePoint_LL_b = getStreetSegmentCurvePoint(seg_id, curvePointsIndex + 1);
                t_point curvePoint_screen_a = LatLon_to_screen_XY(curvePoint_LL_a);
                t_point curvePoint_screen_b = LatLon_to_screen_XY(curvePoint_LL_b);
                drawline(curvePoint_screen_a.x, curvePoint_screen_a.y, curvePoint_screen_b.x, curvePoint_screen_b.y);
            }
            //Then draw from last curve point to "to" intersection

            LatLon curvePoint_LL_Last = getStreetSegmentCurvePoint(seg_id, numofCurvePoints - 1);
            t_point curvePoint_screen_Last = LatLon_to_screen_XY(curvePoint_LL_Last);

            drawline(curvePoint_screen_Last.x, curvePoint_screen_Last.y, pt2_screen.x, pt2_screen.y);


        } else if (numofCurvePoints == 1) {

            //get the only curve points
            LatLon curvePoint_LL = getStreetSegmentCurvePoint(seg_id, 0);

            t_point curvePoint_screen = LatLon_to_screen_XY(curvePoint_LL);

            drawline(pt1_screen.x, pt1_screen.y, curvePoint_screen.x, curvePoint_screen.y);
            drawline(curvePoint_screen.x, curvePoint_screen.y, pt2_screen.x, pt2_screen.y);

        } else
            drawline(pt1_screen.x, pt1_screen.y, pt2_screen.x, pt2_screen.y);
    }


}

vector<unsigned> get_poi_from_name1(string& name) {

    vector<unsigned> tempID;
    multimap<string, vector<unsigned>>::iterator start1, end, it;
    start1 = globalVar.POIName.lower_bound(name);
    end = globalVar.POIName.upper_bound(name);
    for (it = start1; it != end; it++)
        tempID.push_back(it->second[1]);

    return tempID;

}

bool out_of_screen(t_point p) {
    return (p.x < get_visible_world().left() || p.y > get_visible_world().top() ||
            p.y < get_visible_world().bottom() || p.x > get_visible_world().right());
}

void highlight_poi_end(unsigned id) {
    LatLon temp = getPointOfInterestPosition(id);
    t_point temp2 = globalVar.LatLon_to_real_XY2(temp);
    t_point initial(globalVar.x_scale * (temp2.x -
            globalVar.x_min) - 29.5 / globalVar.drawScale, globalVar.y_scale *
            (temp2.y -
            globalVar.y_max) + 70 / globalVar.drawScale);
    drawIcon("libstreetmap/resources/end.png", initial);
    settextattrs(12, 0);
    setcolor(BLACK);
    drawtext(initial.x, initial.y, getPointOfInterestName(id));
}
