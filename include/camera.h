/**
 * @file camera.h
 * @brief This file contains the class camera.
 *
 */

#pragma once
#include "scene.h"

/**
 * @brief The Class Camera provides the functionality of the screen.
 * 
 * The Camera consists of a screen and a vanishing point. The screen is a set of pixels that have some location in the scene.
 * The Camera sends Rays trough every pixel. The vanishing point is the point that all of these rays originate from.
 * 
 * Since Camera is a child class of Object, it has a location, rotation and scale.
 * The vanishing point is stored in the location parameter. The rotation is the rotation of the camera and 
 * scale consists of the x and y scale of the screen in the scene 
 * and the z-value of scale correndponds to the depth of the camera (the 
 * distance between screen and vanishing point).
 * @see Scene_Element
 * 
 * @warning One needs to be careful with how he chooses the x and y scale and resolution of the camera, 
 * because otherwise one will and up with a stretched image. 
 * It is advised to follow this rule: scale[0]/screen_res[0] = scale[1]/screen_res[1]
 * @note Another way to store the resolution would be to store a dpi and the scale of the screen
 * or to store the dpi and the resolution of the image. We decided to use the resolution and the scale of the screen, 
 * as we felt this gives us more control and is easier to work with since both the scale and dpi are values, that don't 
 * provide very good intuition for the outcome of the render.
 * 
*/
class Camera: public Scene_Element{
    public:
    // /**
    //  * @brief The screen resolution in the format {x_resolution, y_resolution}
    //  * 
    //  * The screen resolution refers to the number of pixels on the x and the y axis.
    //  * 
    //  * @note The default value is {1920, 1080}. 
    //  * @warning One needs to be careful with how he chooses the x and y scale and resolution of the camera, 
    //  * because otherwise one will end up with a stretched image. 
    //  * It is advised to follow this rule: scale[0]/screen_res[0] = scale[1]/screen_res[1]
    //  * @note Another way to store the resolution would be to store a dpi and the scale of the screen
    //  * or to store the dpi and the resolution of the image. We decided to use the resolution and the scale of the screen, 
    //  * as we felt this gives us more control and is easier to work with since both the scale and dpi are values, that don't 
    //  * provide very good intuition for the outcome of the render.
    //  * 
    //  * @todo store dpi and resolution of the image instead of scale of the camera.
    //  * @todo add check res/size to read_settings()
    // */
    // vector<int> screen_res = {1920,1080};

    /**
     * @dpi (Dots per "inch")
     * 
     * used to determine the density of pixels
     * 
     * @note The resolution can be computed by x_resolution = sale[0]*dpi, y_resolution scale[1]*dpi
    */
    double dpi;
    /**
     * @brief The vector stores the locations of the pixels in the global space.
     *  
     * We need these points to compute the direction of the rays that are send from the vanishing point of the camera.
     * 
     * The location of the pixels depends on the resolution and the scale of the camera and is computed by the compute_screen_points() function.
     * 
     * @see Camera::screen_res
     * @see Scene_Element::scale the x and y elements of the scale describe the scale of the screen and the z element describes the depth of the camera i.e. the distance between vanishing point and screen.
     * @see Camera::compute_screen_points()
    */
    vector<Vector3d> screen_points;

    public:
    
    /**
     * @brief Base Constructor:
     * 
     * @param location @see Scene_Element::location
     * @param rotation @see Scene_Element::rotation 
     * @param scale @see Scene_Element::scale
     * @param dpi @see dpi 
     * 
     * @note scale consists of 3 doubles {x,y,z} x and y describe the scale of the screen in the global space. z describes the depth of the camera or the distance between the screen and the vanishing point.
     * @see screen_res
     * @note this constructor only considers the depth and uses 1.92 and 1.08 as default arguments for scale[0] and scale[1]
    */
    Camera(Vector3d location, Vector3d rotation, double depth, double dpi); // Base constructor
    
    /**
     * @brief Base Constructor with screen size:
     * 
     * @param location @see Scene_Element::location
     * @param rotation @see Scene_Element::rotation 
     * @param scale @see Scene_Element::scale  
     * @param dpi @see dpi
     * 
     * @note scale consists of 3 doubles {x,y,z} x and y describe the scale of the screen in the global space. z describes the depth of the camera or the distance between the screen and the vanishing point.
     * @see screen_res
     * 
    */
    Camera(Vector3d location, Vector3d rotation, Vector3d scale, double dpi); // Base constructor
    
    /**
     * @brief Default constructor:
     * 
     * Default parameters:
     * 
     * location : {0,0,0}  
     * rotation : {0,0,0}  
     * scale : {1.92,1.08,1}
     * dpi : 1000
     * 
     * @note conresponds to 1920 by 1080 image
     * 
     * @see Camera(Vector3d location, Vector3d rotation, double depth, int dpi)
     * 
    */
    Camera(); // Default constructor

    /**
     * @brief The function computes a vector with the locations of all pixels in the global space.
     * 
     * compute_screen_points() computes the location of all the pixels of the screen in the scene and stores them in screen_pixels.
     * @see screen_pixels
    */
    void compute_screen_points();

    /**
     * @brief The function creates and sends rays through all the pixels of the screen.
     * 
     * send_rays() creates rays starting at the vanishing point(Camera::location) and going through all the locations stored in screen_pixels and calls trace_ray() on all of them.
     * @see Ray
     * @see Ray::trace_ray(vector<Primitive*> objects, vector<Light*> lights, int bounces)
     * 
     * @return image as vector<Vector3d>
    */
    vector<Vector3d> send_rays(vector<Primitive*> objects, vector<Light*> lights);

};

