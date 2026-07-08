/**
 * @file objects.h
 * @brief This file contains the virtual parent class object and it's child classes Sphere and Plane.
 *
 * @todo provide functionality for unions intersections exclusions
 * 
*/

#pragma once
#include "scene.h"


/**
 * @brief Virtual Scene_Element class that acts as the base for all movable objects in the scene
 *
 * Scene_Elements are all non-temporary elements that have a location in the scene.(primitive Objects, camera, lights, ...)
 * They have a location, rotation(not always) and scale(not always) and some basic funtionality for moving and rotating in 3D Space.
*/
class Scene_Element{
    protected:

    /**
     * @brief location stores the location of a Scene_Element in the global space.
     * 
     * The format of location is {x, y, z}.
    */
    Vector3d location;

    /**
     * @brief rotation stores the rotation of a Scene_Element in the global space using Euler angles.
     * 
     * The format of rotation is {rotation_around_x_axis, rotation_around_y_axis, rotation_around_z_axis}.  
     * The rotation is stored in degrees.
    */
    Vector3d rotation;

    /**
     * @brief scale stores the scale of  Scene_Element in the global space.
     * 
     * The format of scale is {x, y, z} .
    */
    Vector3d scale;
    
    public:
    /**
     * @brief Base constructor:
     * 
     * 
     * @param location @see Scene_Element::location
     * @param rotation @see Scene_Element::rotation
     * @param scale @see Scene_Element::scale
     * 
     * @note The Scene_Element Class only provides the basic functionality to move, rotate and scale an Object. For renderable Objects look at the Primitive Class.
     * For Light objects look at the Light class.
     * @see Primitive
     * @see Light
     * 
    */
    Scene_Element(Vector3d location, Vector3d rotation, Vector3d scale);
    
    /**
     * @brief Default Constructor:
     * 
     * Default Parameter:
     * 
     * location : {0,0,0}
     * rotation : {0,0,0}
     * scale : {1,1,1}
     * 
    */
    Scene_Element();

    /**
     * @brief get_location() returns the location of the Scene_Element
     *
     *
     * @return location of the Scene_Element
     *
     * @see Scene_Element::get_rotation()
     * @see Scene_Element::get_scale()
    */
    Vector3d get_location();

    /**
     * @brief get_rotation() returns the rotation of the Scene_Element
     *
     *
     * @return rotation of the Scene_Element
     *
     * @see Scene_Element::get_location()
     * @see Scene_Element::get_scale()
    */
    Vector3d get_rotation();

    /**
     * @brief get_scale() returns the scale of the Scene_Element
     *
     *
     * @return scale of the Scene_Element
     *
     * @see Scene_Element::get_location()
     * @see Scene_Element::get_rotation()
    */
    Vector3d get_scale();
    
    /**
     * @brief the global() function can compute the location in the global space of a point in the local space.
     * 
     * For objects that consist of several points it can be advangous to describe the object with 
     * the location of one point (origin of the object) in the global space and a object-rotation and -scale. All other 
     * points are described only by their relation to the origin. In other words you only store their location in the local space of the object.  
     *   
     * For example: We have a point of origin which is located at (1,2,1) in the global space and another point A which is always one meter above the origin, then its location in the local space
     * can be described by (0,0,1) while its respective location in the global space would be (1,2,2) because it's one meter above the origin.
     * If we now move the entire object one meter to the right the location of the origin in the global space is (1,3,1).
     * But the relative location of the other point A is still (0,0,1), because it is still just one meter above the origin. 
     * It's location in the global space however has changed because we moved the entire object to the right. Therefore its new location in the global space is (1,3,2).
     * 
     * If one wants the global location of a point, he can call global(A) and the function returns the location in the global space.
     * This accounts for translation, rotation and scaling of the object.
     *  
    */
    Vector3d global(const Vector3d& local_point, bool scale);

};

/**
 * @brief Virtual Primitive class that acts as the base for all renderable objects in the scene
 *
 * The Primitive Class builds up on the Scene_Element Class and adds the functionality needed for an object to be rendered in the scene.
 * @see Scene_Element
 * While Scene_Element is the base for all non-temporary Elements in the scene, the Primitive Class is not a parent of for example the camera that does not need to be included in the rendered image. 
 *
 * Therefore the Primitive Class provides methods to compute e.g. reflection_angles and color values. 
 *
 *  
*/
class Primitive: public Scene_Element{
        public:

    /**
     * @brief color stores the ambient values of the object.  
     * 
     * 
     * The format of ambient is {red, green, blue}
     * 
    */
    Vector3d ambient;
    
    /**
     * @brief diffuse stores the diffuse values of the object.  
     * 
     * 
     * The format of diffuse is {red, green, blue}
     * 
    */
    Vector3d diffuse;

    /**
     * @brief specular stores the specular values of the object.  
     * 
     * 
     * The format of specular is {red, green, blue}
     * 
    */
    Vector3d specular;

    /**
     * @brief reflected stores the reflected values of the object.  
     * 
     * 
     * The format of reflected is {red, green, blue}
     * 
    */
    Vector3d reflected;

    /**
     * @brief refracted stores the refracted values of the object.  
     * 
     * 
     * The format of refracted is {red, green, blue}
     * 
    */
    Vector3d refracted;

    /**
     * @brief shininess stores the shininess value of the object.  
     * 
     * 
     * The shininess value is used to compute the specular reflection.
     * 
    */
    double shininess;



    protected:
    /**
     * @brief ref_index stores the refraction index of the object
     * 
     * The refraction index is used to calculate the refraction angle when going from one object/medium into another.
     * 
     * @note Not every child object needs a refraction index, e.g. Camera.
     * For these objects ref_index is set to a default value
     * @see Object(Vector3d location, Vector3d rotation, Vector3d scale, Vector3d color)
     * 
    */
    double ref_index; // refraction index of the material

    public:
    /**
     * @brief Base constructor:
     * 
     * @param location @see Scene_Element::location
     * @param rotation @see Scene_Element::rotation
     * @param scale @see Scene_Element::scale
     * @param ambient @see Primitive::ambient
     * @param refraction_index @see Primitive::ref_index
     * 
    */
    Primitive(Vector3d location, Vector3d rotation, Vector3d scale, Vector3d ambient, Vector3d diffuse, Vector3d specular, Vector3d reflected, Vector3d refracted, double shininess, double refraction_index);
    
    /**
     * @brief Default constructor:
     * 
     * Default parameters:
     * 
     * location : {0,0,0}  
     * rotation : {0,0,0}  
     * scale : {0,0,0}  
     * color : {0.5,0.5,0.5}  
     * ref_index : 1
     * 
    */
    Primitive();
        
    /**
     * @brief get_color() returns the rotation of the Primitive
     *
     *
     * @return color of the Primitive
     * 
     * 
     * @see get_refraction_index()
     *
     * @note For location, rotation and scale use :
     * @see Scene_Element::get_scale()
     * @see Scene_Element::get_location()
     * @see Scene_Element::get_scale()
    */
    Vector3d get_color();
    
    /**
     * @brief get_refraction_index() returns the rotation of the Primitive
     *
     *
     * @return Refraction index of the Primitive
     * 
     * 
     * @see get_color()
     *
     * @note For location, rotation and scale use :
     * @see Scene_Element::get_scale()
     * @see Scene_Element::get_location()
     * @see Scene_Element::get_scale()
    */
    double get_refraction_index();

    /**
     * @brief Abstract method that checks if a point is inside a Primitive
     * 
     *  @return If the point is inside the Primitive it returns true else false.
     * 
     * @note Abstract method: Must be implemented by subclasses!
    */
    virtual bool inside(Vector3d point) = 0;

    /**
     * @brief Abstract method that returns wether a Primitive is hit
     * 
     *  @return If a Primitive is hit by the Ray it returns the distance to the origin of the ray. If it is not hit the method returns -1.
     * 
     * @note Abstract method: Must be implemented by subclasses!
    */
    virtual double is_hit(Ray* r) = 0;

    /**
     * @brief Abstract method that returns the normal vector of the Primitive.
     * 
     * @return The normal vector of the surface of the Primitive at the intersection point.
     * 
     * @note Abstract method: Must be implemented by subclasses!
    */
    virtual Vector3d normal(Vector3d intersection) = 0;

    /**
     * @brief reflect_ray() takes a Ray r and returns the reflected Ray r'
     * 
     * The method takes a Ray r and an intersection point and computes the reflected Ray r'.
     * r' has the intersection point as it's location.
     * 
     * @return the reflected Ray
    */
    Ray reflect_ray(Ray r, Vector3d intersection);
    
    /**
     * @brief refract_ray() takes a Ray r and returns the refracted Ray r'
     * 
     * The method takes a Ray r and an intersection point and computes the refracted Ray r'.
     * r' has the intersection point as it's location.
     * 
     * @return the refracted Ray
    */
    Ray refract_ray(Ray r, Vector3d intersection);
};


/**
 * @brief The Sphere Class is a child class of Primitive.
 *
 * The Sphere Class provides a sphere that can be rendered. It defines the is_hit(),  inside(), distance() and normal() methods from the Primitive Class.
 * @see Primitive
 * 
 * Also check out:
 * @see Plane
 *
*/
class Sphere: public Primitive{
    public:
    
    /**
     * @brief Base constructor:
     * 
     * @param location @see Scene_Element::location
     * @param scale @see Scene_Element::scale Here implemented as a double since a true Sphere only has a radius.
     * @param color @see Primitive::color
     * @param refraction_index @see Primitive::ref_index
     * 
     * @note rotation not used since a sphere has no rotation
     * @see Scene_Element::rotation
    */
    Sphere(Vector3d location, double scale, Vector3d ambient, Vector3d diffuse, Vector3d specular, Vector3d reflected, Vector3d refracted, double shininess, double refraction_index);

    /**
     * @brief The function implements the is_hit() function from the Primitve class
     * @see Primitive::is_hit()
     * 
     * The function checks if the primitive is hit by the Ray r and returns the distance
     * @returns distance between Object and Ray location. or if not hit -1.
    */
    virtual double is_hit(Ray* r) override; 
    
    /**
     * @brief The function implements the inside() function from the Primitve class
     * @see Primitive::is_hit()
     * 
     * The function checks if a point is inside the primitive primitive
     * @returns true if inside else false
    */
    virtual bool inside(Vector3d point) override; 

    /**
     * @brief The function implements the normal() function from the Primitive class
     * @see Primitive::normal()
     * 
     * The function computes the normal of the surface of the object at the intersection point.
     * @return normal vector of the surface at intersection point
    */
    virtual Vector3d normal(Vector3d intersection) override;
};


/**
 * @brief The Plane Class is a child class of Primitive.
 *
 * The Plane Class provides an infinite plane that can be rendered. It defines the is_hit(), inside(), distance() and normal() methods from the Primitive Class.
 * @see Primitive
 * 
 * Also check out:
 * @see Sphere
 *
 * @todo Turn the Plane class into a functioning Halfspace Class or add Halfspace class.
*/
class Plane: public Primitive{
    public:
    
    /**
     * @brief Base constructor:
     * 
     * @param location @see Scene_Element::location
     * @param rotation @see Scene_Element::rotation
     * @param color @see Primitive::color
     * @param refraction_index @see Primitive::ref_index
     * 
     * @note scale not used for Plane since the plane is infinite => not needed in constructor
     * @see Scene_Element::scale 
     * 
     * @todo Ref_index currently does not change anything, because its a Plane not a halfspace.
    */
    Plane(Vector3d location, Vector3d rotation, Vector3d ambient, Vector3d diffuse, Vector3d specular, Vector3d reflected, Vector3d refracted, double shininess, double ref_index);

    /**
     * @brief reduces the translation of the plane to one value
     * 
     * used in the constructor
     * 
     * @param location Vector3d that is transformed into a double
     * @return the translation of the plane along its normal axis
     * 
    */
    double compute_theta(Vector3d location);

    /**
     * @brief The function implements the is_hit() function from the Primitve class
     * @see Primitive::is_hit()
     * 
     * The function checks if the primitive is hit by the Ray r and returns the distance
     * @returns distance between Object and Ray location. or if not hit -1.
    */
    virtual double is_hit(Ray* r) override; 

    /**
     * @brief The function implements the inside() function from the Primitve class
     * @see Primitive::is_hit()
     * 
     * The function checks if a point is inside the primitive primitive
     * @returns true if inside else false
    */
    virtual bool inside(Vector3d point) override; 

    /**
     * @brief The function implements the normal() function from the Primitive class
     * @see Primitive::normal()
     * 
     * The function computes the normal of the surface of the object at the intersection point.
     * @return normal vector of the surface at intersection point
    */
    virtual Vector3d normal(Vector3d intersection) override;

};


/**
 * @brief The Intersection Class is a child class of Primitive.
 *
 * The Intersection Class provides a composition of several primitives that can be rendered. It defines the is_hit(), inside(), distance() and normal() methods from the Primitive Class.
 * @see Primitive
 * 
 * Also check out:
 * @see Sphere
 * @see Plane
 * 
 * @warning does not work yet
 * @todo change normal such that it considers the distances of points to primitives
 *
*/
class Intersection: public Primitive{
    vector<Primitive*> components;

    public:
    
    /**
     * @brief Base constructor:
     * 
     * @param location @see Scene_Element::location
     * @param rotation @see Scene_Element::rotation
     * @param scale @see Scene_Element::scale
     * @param color @see Primitive::color
     * @param refraction_index @see Primitive::ref_index
     * 
     * @see Scene_Element::scale 
     * 
    */
    Intersection(Vector3d location, Vector3d rotation, Vector3d scale, Vector3d ambient, Vector3d diffuse, Vector3d specular, Vector3d reflected, Vector3d refracted, double shininess, double ref_index, vector<Primitive*> components);

    /**
     * @brief The function implements the is_hit() function from the Primitve class
     * @see Primitive::is_hit()
     * 
     * The function checks if the primitive is hit by the Ray r and returns the distance
     * @returns distance between Object and Ray location. or if not hit -1.
    */
    virtual double is_hit(Ray* r) override; 

    /**
     * @brief The function implements the inside() function from the Primitve class
     * @see Primitive::is_hit()
     * 
     * The function checks if a point is inside the primitive primitive
     * @returns true if inside else false
    */
    virtual bool inside(Vector3d point) override; 
    
    /**
     * @brief The function implements the normal() function from the Primitive class
     * @see Primitive::normal()
     * 
     * The function computes the normal of the surface of the object at the intersection point.
     * @return normal vector of the surface at intersection point
    */
    virtual Vector3d normal(Vector3d intersection) override;

};


/**
 * @brief The Cube Class is a child class of Primitive.
 *
 * The Cube Class provides a composition of six planes that can be rendered. It defines the is_hit(), inside() and normal() methods from the Primitive Class.
 * @see Primitive
 * 
 * Also check out:
 * @see Sphere
 * @see Plane
 * 
 * @todo location, rotation, scale
 *
*/
class Cube: public Primitive{
    vector<Plane*> planes;

    public:
    
    /**
     * @brief Base constructor:
     * 
     * @param location @see Scene_Element::location
     * @param rotation @see Scene_Element::rotation
     * @param scale @see Scene_Element::scale
     * @param color @see Primitive::color
     * @param refraction_index @see Primitive::ref_index
     * 
     * @see Scene_Element::scale 
     * 
    */
    Cube(Vector3d location, Vector3d rotation, Vector3d scale, Vector3d ambient, Vector3d diffuse, Vector3d specular, Vector3d reflected, Vector3d refracted, double shininess, double ref_index);

    /**
     * @brief The function implements the is_hit() function from the Primitve class
     * @see Primitive::is_hit()
     * 
     * The function checks if the primitive is hit by the Ray r and returns the distance
     * @returns distance between Object and Ray location. or if not hit -1.
    */
    virtual double is_hit(Ray* r) override; 

    /**
     * @brief The function implements the inside() function from the Primitve class
     * @see Primitive::is_hit()
     * 
     * The function checks if a point is inside the primitive primitive
     * @returns true if inside else false
    */
    virtual bool inside(Vector3d point) override; 
    
    /**
     * @brief The function implements the normal() function from the Primitive class
     * @see Primitive::normal()
     * 
     * The function computes the normal of the surface of the object at the intersection point.
     * @return normal vector of the surface at intersection point
    */
    virtual Vector3d normal(Vector3d intersection) override;

};