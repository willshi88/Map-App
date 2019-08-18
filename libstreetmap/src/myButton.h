/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   myButton.h
 * Author: chenhu49
 *
 * Created on February 15, 2018, 7:26 PM
 */

#ifndef MYBUTTON_H
#define MYBUTTON_H

#include "graphics.h"

using namespace std;

//t_color white(255, 255, 255);

class myButton {
private:
    t_point bottom_left; 
    t_point top_right;
    t_point text_location;
    t_point image_location;
    t_color color;
    Surface image;
    Surface clicked_image;
    string text;
    bool is_clicked;
    bool display_border;
public:
    myButton();
    myButton(t_point _bottom_left, t_point _top_right, 
            t_point text_l, t_point image_l, t_color _color, 
            string _text, Surface _image, bool display_border);
    myButton(t_point _bottom_left, t_point _top_right, 
            t_point text_l, t_point image_l, t_color _color, 
            string _text, Surface _image, Surface _clicked_image,
            bool display_border);
    //myButton(t_point _bottom_left, t_point _top_right, t_point image_l, Surface _image);
    ~myButton();
    
    //function that draws the button on the screen
    void display();
    
    //function that return the current is_clicked state
    bool isClicked();
    //function that response to mouse click
    bool clicking(float x, float y);
    
};

#endif /* MYBUTTON_H */

