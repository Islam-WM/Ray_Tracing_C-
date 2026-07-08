/**
 * @file camera.cpp
 * @brief This file contains the implementation of the camera class.
 *
 * Detailed description of what this source file contains.
 */

#include "scene.h"

Camera::Camera():Scene_Element({0,0,0},{0,0,0},{1.92,1.08,1}), dpi(1000){}
Camera::Camera(Vector3d location, Vector3d rotation, double depth, double dpi):Scene_Element(location, rotation, {1.92, 1.08, depth}),dpi(dpi){}  
Camera::Camera(Vector3d location, Vector3d rotation, Vector3d scale, double dpi):Scene_Element(location, rotation, scale),dpi(dpi){}  
// The camera has a location and a rotation. The scale consists of different factors: the camera has a depth ( distance from screen to the vanishing point),
// and the screen size (x,y), since we don't want to stretch the image we can assume that the scale is (1,1) for now and addapt it later with the screen/image resolution



// with a screen scale of 1 the width and height of one pixel is 0.01=> Camera with 1920*1080 is 19.2cm wide
vector<Vector3d> Camera::send_rays(vector<Primitive*> objects, vector<Light*> lights){

    //Rays start at origin point of camera (for now (0,0,0))
    // Point locationp = location;
    vector<Vector3d> v;

    Vector3d pix_color;

    // send rays through the pixels of the screen;
    for(Vector3d p : screen_points){
        //-------------------Could write a constructor that does this --------
        Ray r(location, p-location, 1);
        //----------------------------------

        pix_color = r.trace_ray(objects, lights, 0);

        v.push_back(pix_color); 
    }

    return v;
}


void Camera::compute_screen_points(){
    screen_points.clear(); // empty the vector
    //to create the ray we also need it's direction:
    Vector3d p;   // every pixel has dimension 1cm*1cm
    // for(double j=screen_res[1]; j>0; j--){  // starting from the top
    //     for(double i=0; i<screen_res[0]; i++){  // starting from the left

    //         p[2] = scale[2]; // distance to the screen;


    //         p[0] = (i / 1000 - screen_res[0]/2000.0 + 0.0005); // position on the x axis
    //         p[1] = j / 1000 - screen_res[1]/2000.0 - 0.0005; // position on the y axis


            
    //         p = global(p);
    //         screen_points.push_back(p);
    //     }
    // }

    double first_x = - scale[0]/2 + 1/(dpi*2);
    double first_y = - scale[1]/2 - 1/(dpi*2);

    for(uint j=uint(scale[1]*dpi); j>0; j--){  // starting from the top
        for(uint i=0; i<uint(scale[0]*dpi); i++){  // starting from the left

            p[2] = scale[2]; // distance to the screen;

            p[0] = first_x + (i/dpi); // position on the x axis
            p[1] = first_y + (j/dpi); // position on the y axis

            p = global(p, false); 
            screen_points.push_back(p);
        }
    }

}