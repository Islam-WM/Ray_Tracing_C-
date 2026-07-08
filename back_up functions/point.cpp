#include "scene.h"

Point::Point():x(0),y(0),z(0){}
// Point::Point(vector<double> loc):x(loc[0]), y(loc[1]), z(loc[2]){}
Point::Point(Vector3d loc):x(loc[0]), y(loc[1]), z(loc[2]){}

Point Point::operator-(Point other){
    Point temp;
    temp.x = x - other.x;
    temp.y = y - other.y;
    temp.z = z - other.z;

    return temp;
}

Point Point::operator+(Point other){
    Point temp;
    temp.x = x + other.x;
    temp.y = y + other.y;
    temp.z = z + other.z;

    return temp;
}

Point Point::operator*(Point other){
    Point temp;
    temp.x = x*other.x;
    temp.y = y*other.y;
    temp.z = z*other.z;

    return temp;
}

Point Point::operator/(double denom){
    Point temp;
    temp.x = x/denom;
    temp.y = y/denom;
    temp.z = z/denom;

    return temp;
}

// Point Point::operator=(vector<double> loc){
//     //Point temp;
//     this->x = loc[0];
//     this->y = loc[1];
//     this->z = loc[2];
//     return *this;
// }

Point Point::operator=(Vector3d loc){
    //Point temp;
    this->x = loc[0];
    this->y = loc[1];
    this->z = loc[2];
    return *this;
}



Vector3d Point::vect(){
    Vector3d temp(this->x,this->y,this->z);
    return temp;
}

ostream& operator<<(ostream& os, const Point& p)
{
    os <<"("<< p.x <<", " << p.y <<", " << p.z<< ")";
    return os;
}