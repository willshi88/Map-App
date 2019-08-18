/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "myButton.h"
//Constructors

myButton::myButton() {

}

//a button with general info

myButton::myButton(t_point _bottom_left, t_point _top_right,
        t_point text_l, t_point image_l, t_color _color,
        string _text, Surface _image, bool _display_border) {
    bottom_left = _bottom_left;
    top_right = _top_right;
    text_location = text_l;
    image_location = image_l;
    color = _color;
    text = _text;
    image = _image;
    clicked_image = _image;
    is_clicked = false;
    display_border = _display_border;
}

//this button changes its icon based on the click state

myButton::myButton(t_point _bottom_left, t_point _top_right,
        t_point text_l, t_point image_l, t_color _color,
        string _text, Surface _image, Surface _clicked_image,
        bool _display_border) {
    bottom_left = _bottom_left;
    top_right = _top_right;
    text_location = text_l;
    image_location = image_l;
    color = _color;
    text = _text;
    image = _image;
    clicked_image = _clicked_image;
    is_clicked = false;
    display_border = _display_border;
}

/*
//a button that only needs a picture
myButton::myButton(t_point _bottom_left, t_point _top_right, t_point image_l, Surface _image){
    bottom_left = _bottom_left;
    top_right = _top_right;
    text_location = t_point(0, 0);
    image_location = image_l;
    color = white;
    text = "";
    image = _image;
    is_clicked = false;
}
 */
myButton::~myButton() {

}


//Functions 

void myButton::display() {

    //draw the button
    setcolor(BLACK);
    setlinewidth(1);
    if (display_border) {
        drawrect(scrn_to_world(bottom_left), scrn_to_world(top_right));
    }
    setcolor(color);

    fillrect(scrn_to_world(bottom_left).x, scrn_to_world(bottom_left).y, scrn_to_world(top_right).x, scrn_to_world(top_right).y);

    //show text
    setcolor(BLACK);
    drawtext(text_location, text);

    //show image 
    if (!is_clicked) {
        draw_surface(image, scrn_to_world(image_location));
    } else {
        draw_surface(clicked_image, scrn_to_world(image_location));
    }
}

bool myButton::isClicked() {
    return is_clicked;
}

bool myButton::clicking(float x, float y) {

    if (x > scrn_to_world(bottom_left).x
            && x < scrn_to_world(top_right).x
            && y < scrn_to_world(bottom_left).y
            && y > scrn_to_world(top_right).y) {
        is_clicked = !is_clicked;
        return true;
    }
    return false;
}