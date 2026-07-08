#pragma once
#include "scene.h"


class Point{
    public:
    double x;
    double y;
    double z;

    public:
    Point();
    // Point(vector<double> loc);
    Point(const Point& p)=default;
    Point(Vector3d loc);

    Point operator-(Point other);
    Point operator+(Point other);
    Point operator*(Point other);
    Point operator/(double denom);

    //Point operator=(vector<double> loc);
    Point operator=(Vector3d loc);

    friend ostream& operator<<(ostream& os, const Point& p);

    Vector3d vect();


};

// A point can be initialized by giving the individuell elements like Point p(1,2,3)
// or by assigning a vector Point p({1,2,3})
// there is also a copy assignment operator for vectors: Point p;    p = {1,2,3};
// if one wants to use the Point as a vector there is a vect() method which returns the Point as a vector: p.vect()=> {1,2,3}

ostream& operator<<(ostream& os, const Point& p);