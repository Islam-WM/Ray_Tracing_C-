/**
 * @file scene.cpp
 * @brief This file contains the implementation of the scene class and the global functions.
 *
 * Detailed description of what this source file contains.
 */

#include "scene.h"


double medium_index = 1;
Vector3d medium_ambient = {0.1,0.1,0.1};
int medium_recursion = 1;

vector<Vector3d> normalize_color(vector<Vector3d> img_data){
    for(size_t i = 0; i<img_data.size(); i++){
        img_data[i][0] = min(img_data[i][0],1.0)*255;
        img_data[i][1] = min(img_data[i][1],1.0)*255;
        img_data[i][2] = min(img_data[i][2],1.0)*255;
    }
    return img_data;
}


void write_img_open_cv(vector<Vector3d> img_data ,int width, int height){

    cv::Mat image(height, width, CV_8UC3, cv::Scalar(0,0,0));

    img_data = normalize_color(img_data);

    for(size_t i= 0; i< img_data.size(); i++){
        int y = i/width;
        int x = i%width;

        image.at<cv::Vec3b>(y,x) = cv::Vec3b(static_cast<uchar>(img_data[i][2]),
                                             static_cast<uchar>(img_data[i][1]),
                                             static_cast<uchar>(img_data[i][0])
                                             );
    }

    cv::imwrite("output_image.png", image);

}

double findRoots(double a, double b, double c){

    if (a == 0) {
        cout << "Error: Expected quadratic equation, got linear";
        return a;
    }
 
    double d = b * b - 4 * a * c;


    // double sqrt_val = std::sqrt(std::abs(d));
 
    double root = 0;
    if (d > 0) {
        double root1 = (-b + std::sqrt(d)) / (2 * a);
        double root2 = (-b - std::sqrt(d)) / (2 * a);
        if(root1<root2){
            root = root1; 
        }else{
            root = root2;
        }
    }
    else if (d == 0) {
        root = -b / (2 * a);
    }
    return root;
}


Vector3d add(Vector3d a, Vector3d b){
    return {1 - (1-a[0]) * (1-b[0]), 1 - (1-a[1]) * (1-b[1]), 1 - (1-a[2]) * (1-b[2])};
}

Scene::Scene(){}

void Scene::load_settings(){

    std::ifstream settings_file("settings.json", std::ifstream::binary);
    
    Json::Value settings;
    settings_file >> settings;

    //-------------------Screen--------------------------------

    Json::Value screen = settings["screen"];

    double dpi = screen["dpi"].asDouble();

    vector<double> dimensions;
    for ( uint index = 0; index < screen["dimensions"].size(); index++ )
        dimensions.push_back(screen["dimensions"][index].asDouble());

    // vector<int> pixels;
    // for ( uint index = 0; index < screen["pixels"].size(); index++ )
    //     pixels.push_back(screen["pixels"][index].asInt());


    Vector3d position;
    for ( uint index = 0; index < screen["position"].size(); index++ )
        position[index] = (screen["position"][index].asDouble());
    
    Vector3d rotation;
    if(screen.isMember("rotation")){
        for ( uint index = 0; index < screen["rotation"].size(); index++ )
            rotation[index] = (screen["rotation"][index].asDouble());
    }else{
        rotation =  {0,0,0};
    }

    double depth;
    if(screen.isMember("observer")){
        // Vector3d observer;
        // for ( uint index = 0; index < screen["observer"].size(); index++ )
        //     observer[index] = (screen["observer"][index].asDouble());
        depth = - screen["observer"][2].asDouble();

    }else{
        depth = screen["depth"].asDouble();
    }

    camera = new Camera(position,rotation,{double(dimensions[0]),double(dimensions[1]),depth}, dpi);

    //-------------------Medium------------------

    Json::Value medium = settings["medium"];

    Vector3d ambient;
    for ( uint index = 0; index < medium["ambient"].size(); index++ )
        ambient[index] = (medium["ambient"][index].asDouble());

    double Ref_index = medium["index"].asDouble();

    int recursion = medium["recursion"].asInt();

    medium_ambient = ambient; // store in global variables
    medium_index = Ref_index;
    medium_recursion = recursion;

    //-------------------Light-sources------------------------
    Json::Value sources = settings["sources"];

    // vector<Light*> lights;

    for ( uint index = 0; index < sources.size(); index++ ){

        Json::Value Light_i = sources[index];

        Vector3d position;
        for ( uint i = 0; i < Light_i["position"].size(); i++ )
            position[i] = Light_i["position"][i].asDouble();

        Vector3d intensity;
        for ( uint i = 0; i < Light_i["intensity"].size(); i++ )
            intensity[i] = Light_i["intensity"][i].asDouble();
        
        lights.push_back(new Light(position,intensity));
    }

    //-------------------Objects--------------------------------

    Json::Value Objects = settings["objects"];

    // vector<Object*> objects;

    for ( uint index = 0; index < Objects.size(); index++ ){
        
        Json::Value Object_i = Objects[index];

        if(Object_i.isMember("sphere")){

            Json::Value Sphere_i = Object_i["sphere"];

            Vector3d position;
            for ( uint i = 0; i < Sphere_i["position"].size(); i++ )
                position[i] = Sphere_i["position"][i].asDouble();

            double radius = Sphere_i["radius"].asDouble();

            Json::Value color = Sphere_i["color"];

                Vector3d ambient;
                for ( uint i = 0; i < color["ambient"].size(); i++ )
                    ambient[i] = color["ambient"][i].asDouble();

                Vector3d diffuse;
                for ( uint i = 0; i < color["diffuse"].size(); i++ )
                    diffuse[i] = color["diffuse"][i].asDouble();

                Vector3d specular;
                for ( uint i = 0; i < color["specular"].size(); i++ )
                    specular[i] = color["specular"][i].asDouble();

                Vector3d reflected;
                for ( uint i = 0; i < color["reflected"].size(); i++ )
                    reflected[i] = color["reflected"][i].asDouble();

                Vector3d refracted;
                for ( uint i = 0; i < color["refracted"].size(); i++ )
                    refracted[i] = color["refracted"][i].asDouble();

                double shininess = color["shininess"].asDouble();

            double Ref_index = Sphere_i["index"].asDouble();

            objects.push_back(new Sphere(position, radius, ambient, diffuse, specular, reflected, refracted, shininess, Ref_index));
        }


        if(Object_i.isMember("halfspace")){

            Json::Value Plane_i = Object_i["halfspace"];

            Vector3d position;
            for ( uint i = 0; i < Plane_i["position"].size(); i++ )
                position[i] = Plane_i["position"][i].asDouble();

            Vector3d normal;
            for ( uint i = 0; i < Plane_i["normal"].size(); i++ )
                normal[i] = Plane_i["normal"][i].asDouble();

            Json::Value color = Plane_i["color"];

                Vector3d ambient;
                for ( uint i = 0; i < color["ambient"].size(); i++ )
                    ambient[i] = color["ambient"][i].asDouble();

                Vector3d diffuse;
                for ( uint i = 0; i < color["diffuse"].size(); i++ )
                    diffuse[i] = color["diffuse"][i].asDouble();

                Vector3d specular;
                for ( uint i = 0; i < color["specular"].size(); i++ )
                    specular[i] = color["specular"][i].asDouble();

                Vector3d reflected;
                for ( uint i = 0; i < color["reflected"].size(); i++ )
                    reflected[i] = color["reflected"][i].asDouble();

                Vector3d refracted;
                for ( uint i = 0; i < color["refracted"].size(); i++ )
                    refracted[i] = color["refracted"][i].asDouble();

                double shininess = color["shininess"].asDouble();

            double Ref_index = Plane_i["index"].asDouble();

            objects.push_back(new Plane(position, normal, ambient, diffuse, specular, reflected, refracted, shininess, Ref_index));
        }

        if(Object_i.isMember("cube")){

            Json::Value Cube_i = Object_i["cube"];

            Vector3d position;
            for ( uint i = 0; i < Cube_i["position"].size(); i++ )
                position[i] = Cube_i["position"][i].asDouble();

            Vector3d rotation;
            for ( uint i = 0; i < Cube_i["rotation"].size(); i++ )
                rotation[i] = Cube_i["rotation"][i].asDouble();

            Vector3d scale;
            for ( uint i = 0; i < Cube_i["scale"].size(); i++ )
                position[i] = Cube_i["scale"][i].asDouble();


            Json::Value color = Cube_i["color"];

                Vector3d ambient;
                for ( uint i = 0; i < color["ambient"].size(); i++ )
                    ambient[i] = color["ambient"][i].asDouble();

                Vector3d diffuse;
                for ( uint i = 0; i < color["diffuse"].size(); i++ )
                    diffuse[i] = color["diffuse"][i].asDouble();

                Vector3d specular;
                for ( uint i = 0; i < color["specular"].size(); i++ )
                    specular[i] = color["specular"][i].asDouble();

                Vector3d reflected;
                for ( uint i = 0; i < color["reflected"].size(); i++ )
                    reflected[i] = color["reflected"][i].asDouble();

                Vector3d refracted;
                for ( uint i = 0; i < color["refracted"].size(); i++ )
                    refracted[i] = color["refracted"][i].asDouble();

                double shininess = color["shininess"].asDouble();

            double Ref_index = Cube_i["index"].asDouble();

            objects.push_back(new Cube(position, rotation, scale, ambient, diffuse, specular, reflected, refracted, shininess, Ref_index));
        }

    }
}

void Scene::render_imag(){

    camera->compute_screen_points();
    vector<Vector3d> im_vec = camera->send_rays(objects, lights);

    write_img_open_cv(im_vec, camera->get_scale()[0]*camera->dpi, camera->get_scale()[1]*camera->dpi);


}
