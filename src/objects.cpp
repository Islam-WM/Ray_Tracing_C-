/**
 * @file objects.cpp
 * @brief This file contains the implementation of the object class as well as the sphere and plane classes.
 *
 * 
 * Detailed description of what this source file contains.
*/

#include "scene.h"

    
// Base Constructor
Scene_Element::Scene_Element(Vector3d location, Vector3d rotation, Vector3d scale):location(location), rotation(rotation), scale(scale){}

// Default Constructor
Scene_Element::Scene_Element():Scene_Element({0,0,0},{0,0,0}, {1,1,1}){}

/**
 * @brief returns the location of the object
*/
Vector3d Scene_Element::get_location(){
    return location;
};

Vector3d Scene_Element::get_rotation(){
    return rotation;
};

Vector3d Scene_Element::get_scale(){
    return scale;
};



Vector3d Scene_Element::global(const Vector3d& local_point, bool scale){
    Vector3d global_point = local_point;
    Matrix3d m;
    m = Eigen::AngleAxisd(rotation[0]*M_PI/180, Vector3d::UnitX())
    * Eigen::AngleAxisd(rotation[1]*M_PI/180, Vector3d::UnitY())
    * Eigen::AngleAxisd(rotation[2]*M_PI/180, Vector3d::UnitZ());
    
    // // 1. multiply with Matrix S for scaling
    if(scale)global_point = local_point.cwiseProduct(this->scale);

    // 2. multiply with Matrix R for rotating
    global_point = m * global_point;

    // 3. add global position of Object for transposing
    global_point = global_point + location;

    return global_point;
}

// Every Object has a point of origin with a location rotation and scale in the global space.


// All other points of the object can be described in a local space. They only need a location relative to the origin. 
// This local space always has the point of origin of the object as its (0,0,0).
// The location of the additional points in the global space is encoded in their location relative to the origin and in the location rotaion and scale of the origin.

// For example: We have a point of origin which is located at (1,2,1) and another point A which is always one meter above the origin, then its location in the local space
// can be described by (0,0,1) while its respective location in the global space would be (1,2,2) because it's one meter above the origin.
// If we now move the entire object one meter to the right the location of the origin in the local space is (1,3,1). 
// But the relative location of the other point A is still (0,0,1), because it is still one meter above the origin. 
// It's location in the global space however has changed because we moved the entire object to the right. Therefore its new position in the global space is (1,3,2).

// Base Constructor
Primitive::Primitive(Vector3d location, Vector3d rotation, Vector3d scale, Vector3d ambient, Vector3d diffuse, Vector3d specular, Vector3d reflected, Vector3d refracted, double shininess, double refraction_index):Scene_Element(location,rotation,scale), ambient(ambient), diffuse(diffuse), specular(specular), reflected(reflected), refracted(refracted), shininess(shininess), ref_index(refraction_index){}

//Default Constructor
Primitive::Primitive():Scene_Element({0,0,0},{0,0,0}, {1,1,1}), ambient({0.5,0.5,0.5}), diffuse({0.5,0.5,0.5}), ref_index(1){}


Vector3d Primitive::get_color(){
    return ambient;
}

double Primitive::get_refraction_index(){
    return ref_index;
}


Ray Primitive::reflect_ray(Ray r, Vector3d intersection){
        Vector3d normal_vector = normal(intersection);

        // mirror ray in relation to the normal vector <=> einfallswinkel=ausfallswinkel
        Vector3d axes_to_scale = (r.get_direction().dot(normal_vector) /
                                    normal_vector.dot(normal_vector)) * normal_vector;
        Vector3d new_ray_direction = r.get_direction() - 2*axes_to_scale;
        
        //2. create new Ray and trace the ray
        return Ray(intersection,new_ray_direction, get_refraction_index());

}

Ray Primitive::refract_ray(Ray r, Vector3d intersection){
        Vector3d normal_vector = normal(intersection);

        double IN = r.get_direction().dot(normal_vector);
        double ref_in_relative = r.get_ref_index() / this->ref_index;

        // 1. Apply formula to compute refraction vector
        Vector3d new_ray_direction= ref_in_relative * 
                                    (r.get_direction() - 
                                    (IN + 
                                    std::sqrt(std::pow(ref_in_relative, 2) + std::pow(IN, 2) - 1 )
                                    ) 
                                    * normal_vector);
        
        //2. create new Ray and trace the ray
        return Ray(intersection,new_ray_direction, get_refraction_index());
}

//--------------------------------Primitives:--------------------------------------
//--------------------------------Sphere-------------------------------------------

Sphere::Sphere(Vector3d location, double scale, Vector3d ambient, Vector3d diffuse, Vector3d specular, Vector3d reflected, Vector3d refracted, double shininess, double refraction_index):Primitive(location, {0,0,0}, {scale,scale,scale}, ambient, diffuse, specular, reflected, refracted, shininess, refraction_index){}

double Sphere::is_hit(Ray* r){ // returns the distance, if hit, else -1
    double radius = scale[0]; // scale is {radius,radius,radius} for sphere, therefore we can just take the first element [0]

    Vector3d Q = r->get_location() - location;

    double a = (r->get_direction()).dot(r->get_direction());      // should be = 1
    double b = 2* Q.dot(r->get_direction());
    double c = Q.dot(Q) - radius*radius;
    double d = b*b - 4*a*c;  // discriminant of quadratic


    if(d >= 0){
        //this means that there are intersections with the sphere

        double dist = findRoots(a, b, c); // returns the smallest root ie. the length of the ray to the sphere

        if(dist >= 0){
            return dist; // store new closest distance
        }
    }
    return -1;
} 
// For a true sphere the rotation does not matter and the should affect all axes the same. In other words if we give a sphere a scale of 4 we want it to have a radius of two in all directions.

bool Sphere::inside(Vector3d point){
    if((point-location).norm() <= scale[0]+1e-10f){
        return true;
    }
    return false;
}

Vector3d Sphere::normal(Vector3d intersection){ // returns a normal vector of the object in the intersetion point
    return (intersection - location).normalized();
}


//--------------------------------Plane-------------------------------------------

Plane::Plane(Vector3d location, Vector3d rotation, Vector3d ambient, Vector3d diffuse, Vector3d specular, Vector3d reflected, Vector3d refracted, double shininess, double ref_index):Primitive(
    {compute_theta(location),0,0},  // planes only need one location value because they span across the whole space
    rotation, {0,0,0}, ambient, diffuse, specular, reflected, refracted, shininess, ref_index){
        this->location[0] = compute_theta(location);
    } // we can get a location for the plane by multiplying location[0]*rotation

double Plane::compute_theta(Vector3d location){

    double normal_comp = location.dot(rotation)/rotation.dot(rotation);
    double norm = (normal_comp * rotation).norm();
    if(normal_comp>=0){
        return -norm;
    }
    return norm;
}

double Plane::is_hit(Ray* ray){ // returns the distance, if hit, else -1
    //----------------<rotation_of_plane,x> - location_in_relation_to_rotation(location[0]) = 0 => x is on the plane-------------
    //----------------x = r.get_location() + a * r.get_rotation()

    double denom = rotation.dot(ray->get_direction());

    if(denom <= 1e-4f and denom >= -1e-4f){ // avoid division by 0
        return -1;
    }

    double a = -(location[0] + rotation.dot(ray->get_location())) / denom;
    if(a>0){
        return a;
    }
    return -1;
} 

bool Plane::inside(Vector3d point){
    if(point.dot(rotation)>=location[0]-1e-10f){
        return true;
    }
    return false;
}

Vector3d Plane::normal(Vector3d intersection){ // returns a normal vector of the object in the intersetion point
    if(intersection[0] != -100000){ // only to avoid intersection not used warning
        return rotation;
    }
    return rotation;
}


    //--------------------------------Intersection-------------------------------------------

    Intersection::Intersection(Vector3d location, Vector3d rotation, Vector3d scale, Vector3d ambient, Vector3d diffuse, Vector3d specular, Vector3d reflected, Vector3d refracted, double shininess, double ref_index, vector<Primitive*> components):Primitive(
        location, rotation, scale, ambient, diffuse, specular, reflected, refracted, shininess, ref_index),components(components){}

    double Intersection::is_hit(Ray* ray){ // returns the distance, if hit, else -1
        //-----------------check for all components of the intersection_object-------------------------
        double shortest_dist = 10000;

        for(uint i = 0; i<components.size(); i++){
            double distance = components[i]->is_hit(ray);
            // cout<<distance<<endl;
            Vector3d intersection = ray->get_location() + ray->get_direction()*distance;
            bool inside = true;
            for(uint j = 0; j<components.size(); j++){
                if(i!=j){
                    if(! components[j]->inside(intersection)){
                        inside = false;
                    }
                }
            }
            if(inside && shortest_dist > distance && distance != -1){
                shortest_dist = distance;
            }
        }
        return shortest_dist;
    } 

    bool Intersection::inside(Vector3d point){
        for(Primitive* primitive : components){
            if(!primitive->inside(point)){
                return false;
            }
        }
        return true;
    }

    Vector3d Intersection::normal(Vector3d intersection){ // returns a normal vector of the object in the intersetion point
        int component_index;
        double shortest_dist;
        for(uint i = 0; i<components.size();i++){
        double dist = components[i]->get_rotation().dot(intersection) - components[i]->get_location()[0];
            if(dist<shortest_dist){
                shortest_dist = dist;
                component_index = i;
            }
        }
        return components[component_index]->normal(intersection);
    }


    //--------------------------------Cube-------------------------------------------

    Cube::Cube(Vector3d location, Vector3d rotation, Vector3d scale, Vector3d ambient, Vector3d diffuse, Vector3d specular, Vector3d reflected, Vector3d refracted, double shininess, double ref_index):Primitive(
        location, rotation, scale, ambient, diffuse, specular, reflected, refracted, shininess, ref_index){
            planes.push_back(new Plane(Vector3d({-0.5,0,0}),{-1,0,0},ambient,diffuse,specular,reflected,refracted,shininess,ref_index));
            planes.push_back(new Plane(Vector3d({0.5,0,0}),{1,0,0},ambient,diffuse,specular,reflected,refracted,shininess,ref_index));
            planes.push_back(new Plane(Vector3d({0,-0.5,0}),{0,-1,0},ambient,diffuse,specular,reflected,refracted,shininess,ref_index));
            planes.push_back(new Plane(Vector3d({0,0.5,0}),{0,1,0},ambient,diffuse,specular,reflected,refracted,shininess,ref_index));
            planes.push_back(new Plane(Vector3d({0,0,-0.5}),{0,0,-1},ambient,diffuse,specular,reflected,refracted,shininess,ref_index));
            planes.push_back(new Plane(Vector3d({0,0,0.5}),{0,0,1},ambient,diffuse,specular,reflected,refracted,shininess,ref_index));   
        }

    double Cube::is_hit(Ray* ray){ // returns the distance, if hit, else -1
        //-----------------check for all planes of the cube-------------------------
        double shortest_dist = 10000;
        bool found_intersection = false;
        for(uint i = 0; i<planes.size(); i++){
            double distance = planes[i]->is_hit(ray);
            if(distance >= 0){

                Vector3d intersection = ray->get_location() + ray->get_direction()*distance;
                bool inside = true;
                for(uint j = 0; j<planes.size(); j++){
                    if(i!=j){
                        if(! planes[j]->inside(intersection)){
                            inside = false;
                        }
                    }
                }
                if(inside && shortest_dist > distance && distance != -1){
                    // cout<<distance;
                    found_intersection = true;
                    shortest_dist = distance;
                }
            }
        }
        if(found_intersection){
            return shortest_dist;
        }
        return -1;

    } 

    bool Cube::inside(Vector3d point){
        for(Plane* plane : planes){
            if(!plane->inside(point)){
                return false;
            }
        }
        return true;
    }

    Vector3d Cube::normal(Vector3d intersection){ // returns a normal vector of the object in the intersetion point
        int plane_index;
        double shortest_dist = 10000;
        for(uint i = 0; i<planes.size();i++){
        double dist = planes[i]->get_rotation().dot(intersection) - planes[i]->get_location()[0];
            if(dist<shortest_dist){
                shortest_dist = dist;
                plane_index = i;
            }
        }
        return planes[plane_index]->normal(intersection);
    }