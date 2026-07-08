/**
 * @file light.cpp
 * @brief This file contains the implementation of the light class.
 *
 * Detailed description of what this source file contains.
 */

#include "scene.h"

Light::Light(Vector3d location, Vector3d color):Scene_Element(location, {0,0,0}, {0,0,0}),color(color){}

Light::Light(Vector3d location, double color):Scene_Element(location,{0,0,0},{0,0,0}),color({color,color,color}){}

Vector3d Light::get_color(){
    return color;
}