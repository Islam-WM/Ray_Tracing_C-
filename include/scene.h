/**
 * @file scene.h
 * @brief This file contains the class scene as well as the global functions.
 *
 * Detailed description of what this header file contains.
 */

#pragma once
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES // to get pi as M_PI
#include <cmath>
#include <math.h>
#include <algorithm>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <fstream>
#include <opencv2/opencv.hpp>

using std::cout, std::endl, std::vector, std::string, std::ostream;
using Eigen::Vector3d, Eigen::Matrix3d;
using std::min,std::max;

// global variables
extern double medium_index;
extern Vector3d medium_ambient;
extern int medium_recursion;


class Ray;

#include "json/json.h"
#include "objects.h"
#include "light.h"
#include "camera.h"
#include "ray.h"



// global functions:

/**
 * @brief Takes color of Format (1.0,1.0,1.0) and turns it into (255,255,255)
 * 
 * Takes an input vector with RGB values in double range, limits the range to [0,1] (cuts sets everything bigger than 1 to 1) and multiplies the values with 255.
 * So that the output is a vector of RGB values with range [0,255].
 * 
 * @param imag_data A vector of Vector3d that store the color_data in RGB format with double range
 * @return A vector of of Vector3d with range [0,255].
 * 
 * @note All values of the input data that are bigger than 1.0 are mapped to 1.0 to limit the color range.
*/
vector<Vector3d> normalize_color(vector<Vector3d> img_data);

/**
 * @brief Takes an image in vector form and writes it into a png.
 * 
 * Takes an image in format vector of Vector3d with double range, normalizes it, and converts it into an image that is saved in "output_image.png".
 * @see normalize_color(vector<Vector3d> img_data)
 * 
 * @param imag_data A vector of Vector3d that store the color_data in RGB format with double range.
 * @param width Width of the image.
 * @param height Height of the image.
 * 
*/
void write_img_open_cv(vector<Vector3d> img_data ,int width, int height);

/**
 * @brief Takes color of Format (1.0,1.0,1.0) and turns it into (255,255,255)
 * 
 * The function calculates the real roots of a quadratic equation in the form of "ax^2 + bx + c = 0". 
 * 
 * @param a The coefficient of the quadratic term (x^2).
 * @param b The coefficient of the linear term (x).
 * @param c The constant term.
 * @return The function returns a double value representing the smallest real root of the quadratic equation.
 * 
 * @note Function outputs 0 if a is 0.
*/
double findRoots(double a, double b, double c);

/**
 * @brief add 2 vectors logarithmically
 * 
 * @return 1 - (1-a) * (1-b)
*/
Vector3d add(Vector3d a, Vector3d b);

// SCENE:

/**
 * @brief The Scene class stores the objects and lights of a Scene and takes care of the setup of the render
 *
 * The Scene class provides functions to read the settings and initiates the render process.
 * It stores the objects, lights and camera as members.
 *
 * @todo scene namespace rather than class? 
 * @todo Medium class to store the medium settings
*/
class Scene{
    
    /**
     * @brief objects stores pointers to all renderable objects in the scene.
     * 
     * These include Primitive Objects like Planes, Spheres,... and Unions, Intersection and Exclusions
     * 
    */
    vector<Primitive*> objects;
        
    /**
     * @brief lights stores pointers to all light sources (except for the ambient lighting) in the scene.
     * 
     * For now these only include Point Lights.
     * 
    */
    vector<Light*> lights;

    /**
     * @brief camera stores a pointer to the camera that is used for rendering.
     * 
    */
    Camera* camera;
    
    public:
    /**
     * @brief Default Constructor creates an empty scene with no objects and lights 
     * 
    */
    Scene();

    /**
     * @brief Loads Settings from "settings.json" and builds the creates the objects, lights and camera for the scene.
     * 
     * Settings included in the settings.json should be:
     * 
     * Parameters for the camera
     * @see Camera
     * 
     * General parameters for the scene:
     * under "medium" in the settings.json
     * include ambient light values of the scene, refraction index of the scene medium,...
     * 
     * Parameters for Objects:
     * @see Object
     * @see Sphere
     * @see Plane is under "halfspace" in settings.json
     * 
     * Parameters for the lights:
     * @see Light
     * 
     * @note Currently no support for unions, intersections and exclusions
    */
    void load_settings();

    /**
     * @brief The function sets up to camera, initiates the rendering process and stores the image that the camera returns in the "output_image.png"
     * 
     * The function provides the camera with the neccesarry settings and calls the sends_rays() function to initiate the render.
     * @see Camera::compute_screen_points()
     * @see Camera::send_rays(vector<Object*> objects, vector<Light*> lights)
     * 
     * The function also calls the write_img_open_cv(vector<Vector3d> img_data ,int width, int height) function to save the image in "output_image.png".
     * @see write_img_open_cv(vector<Vector3d> img_data ,int width, int height)
     * 
     * @todo calling compute screen points makes more sense in load_settings or somewhere else.
    */
    void render_imag();
};