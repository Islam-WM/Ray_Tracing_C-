#pragma once
#include "scene.h"


struct Color{
    int red;
    int green;
    int blue;
    Color();
    Color(vector<int> rgb);
    void operator=(const vector<int>& rgb);
    // void operator=(const Color c);
    Color operator/(double t);
    Color operator*(double t);
    Color operator+(Color other);
};