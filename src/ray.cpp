/**
 * @file ray.cpp
 * @brief This file contains the implementation of the ray class.
 *
 * Detailed description of what this source file contains.
 */

#include "scene.h"

Ray::Ray():location({0,0,0}), direction({0,0,0}), ref_index(1){}

Ray::Ray(Vector3d loc, Vector3d direct, double refraction_index){
    location = loc;
    direction = direct;
    direction.normalize();
    ref_index = refraction_index;
}

void Ray::set_location(Vector3d loc){
    location = loc;
}
void Ray::set_direction(Vector3d direct){
    direct.normalize();
    direction = direct;
}

Vector3d Ray::get_location(){
    return location;
}
Vector3d Ray::get_direction(){
    return direction;
}
double Ray::get_ref_index(){
    return ref_index;
}



Vector3d Ray::trace_ray(vector<Primitive*> objects, vector<Light*> lights, int bounces){ //compute the color a ray returns
    
    // Vector3d return_color({0.117,0.117,0.117}); // background color
    Vector3d return_color({0.5,0.5,0.5}); // background color

    //--------------------check if RAY HITS an object-----------------

    double distance;
    int closest_obj_index = compute_closest_object(objects, distance); // check if an Object is hit, and stores the distance and closest object;

    //-------------------if NO OBJECT WAS HIT we return the background color----------

    if(closest_obj_index == -1){ // is -1 if no object was hit
        return return_color;
    }

    //------------------else check if LIGHT can hit the intersection----------

    Vector3d intersection = location + distance * direction;
    Vector3d light_value = compute_light(objects, lights, intersection);

    //-----------------check for REFLECTIONS----------------------------------


    Vector3d reflected_color({0,0,0});

    if(bounces < medium_recursion && objects[closest_obj_index]->reflected != Vector3d({0,0,0})){ // send new ray in new direction
        Ray reflected_ray = objects[closest_obj_index]->reflect_ray(*this,intersection);
        reflected_color = reflected_ray.trace_ray(objects, lights, bounces+1);
    }

    //------------------check for Refraction---------------------------------------
    

    Vector3d refracted_color({0,0,0});

    if(bounces < 1 && objects[closest_obj_index]->refracted != Vector3d({0,0,0})){ // send new ray in new direction
        Ray refracted_ray = objects[closest_obj_index]->refract_ray(*this,intersection);
        refracted_color = refracted_ray.trace_ray(objects, lights, bounces+1);
    }

    //------------------add the different components to get the final color of a point on a surface-------

    Vector3d ambient_comp = objects[closest_obj_index]->ambient.cwiseProduct(medium_ambient);
    Vector3d diffuse_comp = light_value;
    Vector3d reflected_comp = reflected_color.cwiseProduct(objects[closest_obj_index]->reflected);
    Vector3d refracted_comp = refracted_color.cwiseProduct(objects[closest_obj_index]->refracted);

    return_color = add(add(add(ambient_comp,diffuse_comp),reflected_comp),refracted_comp);
    // double lambert = abs(objects[closest_obj_index]->normal(intersection).dot(direction));

    return return_color;
}



int Ray::compute_closest_object(vector<Primitive*> objects, double& distance){ // computes closest object and distance
    double distance_to_closest_obj = 10000.0; //maximum length of a ray
    int closest_obj_index = -1; // return -1, if no object is hit

    for(size_t i=0; i<objects.size(); i++){
    
        double dist = objects[i]->is_hit(this); // check for each object if it's hit

        if(dist >= 1e-6f and dist < distance_to_closest_obj){
            closest_obj_index = i;
            distance_to_closest_obj = dist;
        }
    }

    distance = distance_to_closest_obj;
    return closest_obj_index;
}



Vector3d Ray::compute_light(vector<Primitive*> objects, vector<Light*> lights, Vector3d intersection){
    Vector3d light_value = {0.1, 0.1, 0.1}; // 0.1 to have some ambient lighting

    for(Light* light : lights){

        double dist_to_intersection = (intersection - light->get_location()).norm(); // distance to the intersection

        
        //-------create the ray from the light to the intersection-----------

        Vector3d light_direction = intersection - light->get_location();
        Ray r(light->get_location(), light_direction, 1);

        //------compute the distance to the closest object--------------------
        
        double dist_to_clos_obj;
        size_t closest_object_index = r.compute_closest_object(objects, dist_to_clos_obj);

        //--------dist to closest object must be equal to the dist to intersection => intersection is the first cut------

        if(dist_to_clos_obj >= dist_to_intersection - 1e-10f){
            double lambert = abs(objects[closest_object_index]->normal(intersection).dot(light_direction.normalized()));
            Vector3d diffuse_comp = objects[closest_object_index]->diffuse.cwiseProduct(light->get_color()) * lambert;
            Vector3d reflected_light = objects[closest_object_index]->reflect_ray(r, intersection).direction;

            Vector3d specular_comp = objects[closest_object_index]->specular.cwiseProduct(light->get_color()) * std::pow(std::max(reflected_light.dot(-r.direction), 0.0),objects[closest_object_index]->shininess);
            
            light_value = light_value + diffuse_comp + specular_comp;
        }

        //--------else do nothing and check the next light------------

    }

    return light_value;
}
