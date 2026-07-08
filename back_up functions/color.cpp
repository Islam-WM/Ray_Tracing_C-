#include "scene.h"

Color::Color():red(0),green(0),blue(0){}
Color::Color(vector<int> rgb): red(rgb[0]),green(rgb[1]),blue(rgb[2]){}

void Color::operator=(const vector<int>& rgb){
    red = rgb[0];
    green = rgb[1];
    blue = rgb[2];
} // does not work yet; should work now

// void Color::operator=(const Color c){
//     red = c.red;
//     green = c.green;
//     blue = c.blue;
// }

Color Color::operator/(double t){
    Color temp;
    temp.red = this->red/t;
    temp.blue = this->blue/t;
    temp.green = this->green/t;
    return temp;
}

Color Color::operator*(double t){
    Color temp;
    temp.red = this->red*t;
    temp.blue = this->blue*t;
    temp.green = this->green*t;
    return temp;
}

Color Color::operator+(Color other){
    Color temp;
    temp.red = this->red + other.red;
    temp.blue = this->blue + other.blue;
    temp.green = this->green + other.green;
    return temp;
}