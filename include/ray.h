/**
 * @file ray.h
 * @brief This file contains the class ray.
 *
 * Detailed description of what this header file contains.
 */

#pragma once
#include "scene.h"

/**
 * @brief The class Ray provides the basic functionalities needed to trace a ray. Like reflection, refraction...
 * 
 * Rays consist of a Point in the global space and a vector. Upon hitting an Object new Rays are created that help compute reflection, refraction, ...
 * 
 * 
*/
class Ray{
    protected:
    /**
     * @brief location stores the location of a ray in the global space
     * 
     * The format of location is {x, y, z} 
    */
    Vector3d location;

    /**
     * @brief direction stores the normalized directional vector of a ray in the global space
     * 
     * The format of direction is {x, y, z}
     * 
     * @note Not to be confused with Object::rotation, where the rotation of the object is determined by Euler angles
     * @see Object::rotation
     * @note direction is normalized => length of direction is equal to 1.
    */
    Vector3d direction;

    /**
     * @brief ref_index stores the refraction index of the object/medium that the ray is contained in
     *  
     * Further explanation of the refraction index can be found here:
     * @see Object::ref_index
     * 
     * @note The ref_index of a Ray refers to the ref_index of the Object/medium that the ray is passing through
    */
    double ref_index;

    public:
    /**
     * @brief Default Constructor:
     * 
     * Default Parameters:  
     *   
     * location : {0,0,0}    
     * direction : {0,0,0}     
     * ref_index : 1  
     * 
    */
    Ray();

    /**
     * @brief Base Constructor:
     * 
     * @param location @see Ray::location 
     * @param direction @see Ray::direction  direction is normalized upon initialization of the object => param direction is not required to be normalized 
     * @param ref_index @see Ray::ref_index
     * 
    */
    Ray(Vector3d loc, Vector3d direct, double refraction_index);

    /**
     * @brief set_location can be used to change the location of a ray
     *
     * location of the ray is set to loc
     *
     * @param loc new location of the ray

     *
     * @see Ray::set_rotation(Vector3d direct)  
     *   
     * @see Ray::get_location()
     * @see Ray::get_direction()
    */
    void set_location(Vector3d loc);
        
    /**
     * @brief set_direction can be used to change the direction of a ray
     *
     * direct is normalized and direction is set to direct.
     *
     * @param direct new direction of the ray

     *
     * @see Ray::set_direction(Vector3d direct)  
     *   
     * @see Ray::get_location()
     * @see Ray::get_direction()
    */
    void set_direction(Vector3d direct);

    /**
     * @brief get_location returns the location of a ray
     *
     *
     * @return location of the ray

     *
     * @see Ray::set_location(Vector3d loc)  
     * @see Ray::set_direction(Vector3d direct) 
     *   
     * @see Ray::get_direction()
    */
    Vector3d get_location();

    /**
     * @brief get_direction returns the direction of a ray
     *
     *
     * @return direction of the ray

     *
     * @see Ray::set_location(Vector3d loc)  
     * @see Ray::set_direction(Vector3d direct) 
     *   
     * @see Ray::get_location()
    */
    Vector3d get_direction();

    /**
     * @brief get_direction returns the refraction index of a ray
     *
     *
     * @return refraction index of the ray
     *
    */
    double get_ref_index();


    /**
     * @brief trace_ray() traces a Ray through the scene
     *
     * trace_ray() is the function that checks if a ray hits an object.
     * If it does it computes a return_color for the ray which consists of ambient light, direct light through light sources, reflection, refraction,...
     * Inorder to compute reflection, refraction,... new rays are send recursively.
     * 
     *
     * @param objects A vector of pointers to all renderable objects in the scene
     * @param lights A vector of pointers to all light sources in the scene
     * @param bounces The number of times the light has bounced including this ray. (If a ray hits an object, this as a bounce and rays are send out.) The number of bounces is relevant in order to limit the render time.
     * @return The "color of the ray". The color of the point the object that the ray hits times <angle of incidence, normal of the object>, or in case no object is hit the color of the backgound 
     *
     * @note This function does not handle overflow or other error conditions.
     * @see Ray::compute_closest_object(vector<Object*> objects, double& distance)  Which is used to compute the object a ray hits.
     * @see Ray::compute_light(vector<Object*> objects, double& distance, Vector3d intersection)  Which is used to compute the sum of all lights that direcly hit the point on an object.
     * 
    */
    Vector3d trace_ray(vector<Primitive*> objects, vector<Light*> lights, int bounces); // returns color of the ray
    
    /**
     * @brief Computes the Object the Ray hits first and the distance
     *
     * Computes whether a ray hits an Object, if not the function returns -1, else distance is set to the distance to the object from the location of the ray.
     * See Ray::location
     * 
     * And the function returns the index of the object in the objects vector that is hit first. 
     * 
     *
     * @param objects A vector of pointers to all renderable objects in the scene
     * @param lights A vector of pointers to all light sources in the scene
     * @return The index of the Object that is hit in the vector of objects. Or if no object is hit -1.
     *
     * @note The intersection of the ray with the object, can be computed with: location(of the ray) + direction(of the ray) * distance because direction is normalized
     * @see Ray::direction
     * @see Ray::trace_ray(vector<Object*> objects, vector<Light*> lights, int bounces) 
     * @see Ray::compute_light(vector<Object*> objects, double& distance, Vector3d intersection)
     * 
    */
    int compute_closest_object(vector<Primitive*> objects, double& distance); // returns the index of the closest and stores the distance in distance, if no object is hit the function returns -1
    
    /**
     * @brief Computes the sum of all lights that hit a point.
     *
     * Checks for each light if it can hit the intersection point and sum up the lights multiplied with <(intersection - light.location, normal of the object> (Lambert's Rule)
     * 
     * 
     *
     * @param objects A vector of pointers to all renderable objects in the scene
     * @see Scene::objects
     * @param lights A vector of pointers to all light sources in the scene
     * @see Scene:: lights
     * @param intersection The point for which the light value is computed
     * @return A Vector3d that contains RGB values for the light intensity
     *
     * @see Ray::trace_ray(vector<Object*> objects, vector<Light*> lights, int bounces) 
     * @see Ray::compute_closest_object(vector<Object*> objects, double& distance, double& distance)
     * 
    */
    Vector3d compute_light(vector<Primitive*> objects,vector<Light*> lights, Vector3d intersection); // conpute the light value at the intersection
};
