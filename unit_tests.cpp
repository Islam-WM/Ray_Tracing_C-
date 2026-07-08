
#include "scene.h"

#include <unistd.h>
#include<ctime>


void test_sphere(){
    Vector3d loc = {0,0,0};
    // vector<float> rot = {0,0,0};
    // vector<float> scale = {0,0,0};

    Sphere sp(loc,1,{50,50,50},{50,50,50}, {50,50,50},{50,50,50}, {50,50,50},1,1);
}

void test_plane(){
    Vector3d loc = {-1,0,0};
    // vector<float> rot = {0,0,0};
    // vector<float> scale = {0,0,0};

    Plane sp(loc,{1,0,0},{50,50,50},{50,50,50}, {50,50,50},{50,50,50}, {50,50,50},1,1);
}


void test_render_imag(){
    double time_before_loop_begins = std::time(NULL);

    Scene scn;
    Camera cam({0,0,-1},{0,0,0},{4,2,1},128);

    vector<Primitive*> objects = {};
    objects.push_back(new Sphere({1,0,3},0.5,{255,0,0}, {255,0,0}, {50,50,50},{50,50,50}, {50,50,50},1,1)); //red Sphere
    objects.push_back(new Sphere({-1,0,3},0.5,{0,255,0}, {0,255,0}, {50,50,50},{50,50,50}, {50,50,50},1,1)); // blue Sphere
    objects.push_back(new Plane({0,0,3},{0,0,1},{0,0,255}, {0,0,255}, {50,50,50},{50,50,50}, {50,50,50},1,1)); // green Plane


    vector<Light*> lights = {};
    lights.push_back(new Light({0,0,2},{6,20,20}));
    lights.push_back(new Light({0,5,2},{30,20,20}));

    cam.compute_screen_points(); // computes all points and stores them in a vector
    scn.render_imag();
    // scn.render_imag_old(cam, objects, lights);


    double time_after_loop_ends = std::time(NULL);

    double time_diff = time_after_loop_ends - time_before_loop_begins;
    cout << "Time taken to run a for loop = " << time_diff << " seconds."<<endl;
}


void test_Color(){
    Vector3d p({0,0.2,0.3});
    cout<< p[0]<< p[1]<<endl;
    p = {2,3,4};
    cout << p[0]<< p[1]<<endl;
}


void test_compute_screen_points(){
    Camera cam;
    cam.compute_screen_points();
    cout<<cam.screen_points[0]<<endl;
    cout<<cam.screen_points[1]<<endl;
}

void test_hit_Sphere(){
        Vector3d object_location;
        object_location = {0,0,2}; // get location of the object that you want to check
        double radius = 1; // scale is {radius,radius,radius} for sphere, therefore we can just take the first element [0]

        Vector3d rotation = {0,0,1};
        Vector3d Q = Vector3d({0,0,0}) - object_location;
        double a = rotation.dot(rotation);      // should be = 1
        double b = 2* Q.dot(rotation);
        double c = Q.dot(Q) - radius*radius;
        double d = b*b - 4*a*c;  // discriminant of quadratic
        //cout<<a<<endl;
 
        if(d >= 0){
            //there are intersections

            double t = findRoots(a, b, c);
            cout<<"Distance to the sphere is :"<< t<<endl;
        }
}

void test_Eigen(){
    Vector3d v(1,2,3);
    cout<<v<<endl;
    Matrix3d m = Matrix3d::Random();
    cout<<m<<endl;
}


double test_send_ray(){
    vector<Primitive*> objects = {};
    objects.push_back(new Sphere({1.5,0,3},1,{255,0,0},{255,0,0}, {50,50,50},{50,50,50}, {50,50,50},1,1)); //red Sphere
    objects.push_back(new Sphere({-1.5,0,3},1,{0,255,0},{0,255,0}, {50,50,50},{50,50,50}, {50,50,50},1,1)); // blue Sphere

    vector<Light*> lights = {};

    Vector3d location = {0,0,0};
    // Vector3d p({0.1,0,1});
    Vector3d pix_color;

    // Ray r;
    // r.set_location(location);
    // r.set_rotation(compute_direction(locationp,p).vect());
    // //----------------------------------
    

    double time_before_loop_begins = std::time(NULL);


    for(size_t i = 0; i<10; i++){
        cout<<"Round: "<<i<<endl;
        Vector3d p({0.01*i,0,1});
        Ray r(location,location-p,1);
        

        r.trace_ray(objects, lights, 1);

    // findRoots(1,rand(),rand());
    }



    double time_after_loop_ends = std::time(NULL);

    double time_diff = time_after_loop_ends - time_before_loop_begins;
    cout << "Time taken to run a for loop = " << time_diff << " seconds."<<endl;
    return time_diff;

}


void test_refraction(){

    double n1 = 1;
    double n2 = 2;
    // 1. compute angle of incidence
    //angle between ray and normal
    Vector3d normal_vector = {0,0,1};
    Vector3d rotation = {-1,0,-1};

    double angle_inc = std::acos(normal_vector.dot(rotation)/ (normal_vector.norm()*rotation.norm())); //cos(a) = v*u/(||v||*||u||), cosine similarity
    cout<<"angle_inc: "<<angle_inc* (180.0 / M_PI)<<endl;
    angle_inc = 10/(180/M_PI);
    double angle_of_change = 0;
    if(angle_inc*180/M_PI>90){
        angle_inc = angle_inc - 90*M_PI/180;
    }
    double angle_ref = std::asin( n1/n2 * std::sin(angle_inc)); // n1sin(a) = n2sin(b)
    cout<<"angle_ref: "<<angle_ref* (180.0 / M_PI)<<endl;
    angle_of_change = angle_inc - angle_ref; // angle by which we need to rotate the vector;
    cout<<"angle_of_change: "<<angle_of_change* (180.0 / M_PI)<<endl;
    

    // 2. compute axis of rotation: crossproduct of normal and rotation of ray
    
    Eigen::AngleAxisd rotation_vec(angle_of_change, normal_vector.cross(rotation));
    // rotate around that axis with angle computed in 
    Vector3d new_ray_rotation = rotation_vec * rotation;
    cout<<"new_ray_rotation: "<<new_ray_rotation<<endl;

}



void test_json(){

    std::ifstream settings_file("settings.json", std::ifstream::binary);
    Json::Value settings;
    settings_file >> settings;

    //-------------------Screen--------------------------------

    Json::Value screen = settings["screen"];

    // int dpi = screen["dpi"].asInt();

    // vector<int> dimensions;
    // for ( int index = 0; index < screen["dimensions"].size(); index++ )
    //     dimensions.push_back(screen["dimensions"][index].asInt());

    vector<int> pixels;
    for ( int index = 0; index < screen["pixels"].size(); index++ )
        pixels.push_back(screen["pixels"][index].asInt());


    Vector3d position;
    for ( int index = 0; index < screen["position"].size(); index++ )
        position[index] = (screen["position"][index].asDouble());

    Vector3d rotation;
    for ( int index = 0; index < screen["rotation"].size(); index++ )
        rotation[index] = (screen["rotation"][index].asDouble());

    // Vector3d observer;
    // for ( int index = 0; index < screen["observer"].size(); index++ )
    //     observer[index] = (screen["observer"][index].asDouble());

    double depth = screen["depth"].asDouble();
    
    Camera Cam(position,rotation,{4,2,depth},128);

    //-------------------medium------------------
    Json::Value medium = settings["medium"];

    Vector3d ambient;
    for ( int index = 0; index < medium["ambient"].size(); index++ )
        ambient[index] = (medium["ambient"][index].asDouble());

    double Ref_index = medium["index"].asDouble();

    int recursion = medium["recursion"].asInt();

    medium_ambient = ambient; // store in global variables
    medium_index = Ref_index;
    medium_recursion = recursion;

    //-------------------Light-sources------------------------
    Json::Value sources = settings["sources"];

    vector<Light*> lights;

    for ( int index = 0; index < sources.size(); index++ ){

        Json::Value Light_i = sources[index];

        Vector3d position;
        for ( int i = 0; i < Light_i["position"].size(); i++ )
            position[i] = Light_i["position"][i].asDouble();

        Vector3d intensity;
        for ( int i = 0; i < Light_i["intensity"].size(); i++ )
            intensity[i] = Light_i["intensity"][i].asDouble();
        
        lights.push_back(new Light(position,intensity));
    }

    //-------------------Objects--------------------------------

    Json::Value Objects = settings["objects"];

    vector<Primitive*> objects;

    for ( int index = 0; index < Objects.size(); index++ ){
        
        Json::Value Object_i = Objects[index];

        if(Object_i.isMember("sphere")){

            Json::Value Sphere_i = Object_i["sphere"];

            Vector3d position;
            for ( int i = 0; i < Sphere_i["position"].size(); i++ )
                position[i] = Sphere_i["position"][i].asDouble();

            double radius = Sphere_i["radius"].asDouble();

            Json::Value color = Sphere_i["color"];

                Vector3d ambient;
                for ( int i = 0; i < color["ambient"].size(); i++ )
                    ambient[i] = color["ambient"][i].asDouble();

                Vector3d diffuse;
                for ( int i = 0; i < color["diffuse"].size(); i++ )
                    diffuse[i] = color["diffuse"][i].asDouble();

                Vector3d specular;
                for ( int i = 0; i < color["specular"].size(); i++ )
                    specular[i] = color["specular"][i].asDouble();

                Vector3d reflected;
                for ( int i = 0; i < color["reflected"].size(); i++ )
                    reflected[i] = color["reflected"][i].asDouble();

                Vector3d refracted;
                for ( int i = 0; i < color["refracted"].size(); i++ )
                    refracted[i] = color["refracted"][i].asDouble();

                double shininess = color["shininess"].asDouble();

            double Ref_index = Sphere_i["index"].asDouble();

            // objects.push_back(new Sphere(position,radius,ambient,Ref_index));
        }

        if(Object_i.isMember("halfspace")){

            Json::Value Plane_i = Object_i["sphere"];

            Vector3d position;
            for ( int i = 0; i < Plane_i["position"].size(); i++ )
                position[i] = Plane_i["position"][i].asDouble();

            Vector3d normal;
            for ( int i = 0; i < Plane_i["normal"].size(); i++ )
                normal[i] = Plane_i["normal"][i].asDouble();

            Json::Value color = Plane_i["color"];

                Vector3d ambient;
                for ( int i = 0; i < color["ambient"].size(); i++ )
                    ambient[i] = color["ambient"][i].asDouble();

                Vector3d diffuse;
                for ( int i = 0; i < color["diffuse"].size(); i++ )
                    diffuse[i] = color["diffuse"][i].asDouble();

                Vector3d specular;
                for ( int i = 0; i < color["specular"].size(); i++ )
                    specular[i] = color["specular"][i].asDouble();

                Vector3d reflected;
                for ( int i = 0; i < color["reflected"].size(); i++ )
                    reflected[i] = color["reflected"][i].asDouble();

                Vector3d refracted;
                for ( int i = 0; i < color["refracted"].size(); i++ )
                    refracted[i] = color["refracted"][i].asDouble();

                double shininess = color["shininess"].asDouble();

            double Ref_index = Plane_i["index"].asDouble();

            // objects.push_back(new Sphere(position,radius));
        }

    }

}

void test_inside(){

    Plane p({0,0,0},{0,0,1},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},0,0);
    Vector3d point({0,0,0});
    if(p.inside(point)){
        cout<<"suui";
    }else{
        cout<<"bruhh";
    }

}

int main(){
    // test_render_imag();
    // test_Color();
    // test_inner_prod();
    // test_compute_direction();
    // test_compute_screen_points();
    // test_hit_Sphere();
    // test_Eigen();
    // test_send_ray();
    // test_refraction();
    // test_json();
    test_inside();
}
