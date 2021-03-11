#ifndef C___GEOMETRY_H
#define C___GEOMETRY_H


#include <vector>
#include <cmath>

using namespace std;

const double PI = acos(-1.0);

//FIXED todo cpp
class Point {
public:
    Point();

    Point(int, int);

    //copy
    Point(const Point &);


    //=
    Point &operator=(const Point &);

    ~Point();


    int getX() const;

    int getY() const;


private:
    int x_;
    int y_;
};

class PolygonalChain {

public:

    PolygonalChain(unsigned int, Point *);

    //copy
    PolygonalChain(const PolygonalChain &);

    //=
    PolygonalChain &operator=(const PolygonalChain &);

    virtual ~PolygonalChain();


    Point getPoint(unsigned int) const;

    int getN() const;

    virtual double perimeter() const;

private:
    //FIXED: todo remove N
    //FIXED: todo var names shouldnt start with capital letter
    vector<Point> points_;

};


class ClosedPolygonalChain : public PolygonalChain {
    using PolygonalChain::PolygonalChain;
public:
    virtual double perimeter() const;

};

class Polygon : public ClosedPolygonalChain {
    using ClosedPolygonalChain::ClosedPolygonalChain;
public:
    virtual double area() const;
};

class Triangle : public Polygon {
    using Polygon::Polygon;
public:
    bool hasRightAngle() const;

};

class Trapezoid : public Polygon {
    using Polygon::Polygon;
public:
    double height() const;

};

class RegularPolygon : public Polygon {
    using Polygon::Polygon;
public:


    double area() const;

    double perimeter() const;

};

#endif //C___GEOMETRY_H