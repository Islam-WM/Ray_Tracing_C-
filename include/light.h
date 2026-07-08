/**
 * @file light.h
 * @brief This file contains the class light.
 *
 * Detailed description of what this header file contains.
 */

#pragma once
#include "scene.h"

/**
 * @brief The Light class implements a simple point light.
 * 
 * Light has a location and a color.
 * 
 * @note The Light class has been implemented as a child of Scene_Element even though it only uses the location, 
 * incase one wants to later create different light sources and not just point lights. 
*/
class Light: public Scene_Element{
    private:

    /**
     * @brief Color of the light source
     * 
     * Format : {red,green,blue}
    */
    Vector3d color; 

    public:

    /**
     * @brief Constructor for colored light source:
     * 
     * @param location @see Scene_Element::location
     * @param color @see Light::color  Here Vector3d (with format {red,green,blue})
    */
    Light(Vector3d location, Vector3d color);

    /**
     * @brief Constructor for grayscale light source:
     * 
     * @param location @see Scene_Element::location
     * @param color @see Light::color  Here double for gray scale color value
     * 
     * @note use Light(Vector3d location, Vector3d color) for colored light source
     * @see Light(Vector3d location, Vector3d color)
    */
    Light(Vector3d location, double color);
    
    /**
     * @brief get_color() returns the color of the Light source
     *
     *
     * @return Color of the Primitive
     * 
     *
     * @note get_color() always returns a Vector3d, even if constructed with gray scale constructor
     * @see Light(Vector3d location, double color)
    */
    Vector3d get_color();
};