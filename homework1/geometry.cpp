#include <iostream>
#include <cmath>

#include "geometry.h"

using namespace std;


Point::Point() : x_(0), y_(0) {};

Point::Point(int x, int y)
        : x_(x), y_(y) {};

//copy
Point::Point(const Point &other)
        : x_(other.x_), y_(other.y_) {};

//=
Point &Point::operator=(const Point &other) {
    if (&other == this) {
        return *this;
    }
    x_ = other.x_;
    y_ = other.y_;
    return *this;
}

Point::~Point() = default;


int Point::getX() const {
    return x_;
}

int Point::getY() const {
    return y_;
}


PolygonalChain::PolygonalChain(unsigned int n, Point *p = {}) {
    for (unsigned int i = 0; i < n; i++) {
        points_.push_back(p[i]);
    };
};

//copy
PolygonalChain::PolygonalChain(const PolygonalChain &other)
        : points_(other.points_) {};

//=
PolygonalChain &PolygonalChain::operator=(const PolygonalChain &other) {
    if (&other == this) {
        return *this;
    } else {
        points_ = other.points_;

        return *this;
    }
};

PolygonalChain::~PolygonalChain() = default;
//FIXED todo dont do clear

Point PolygonalChain::getPoint(unsigned int k) const {
    return points_[k];
}

int PolygonalChain::getN() const {
    return points_.size();
}

double PolygonalChain::perimeter() const {
    int x0 = points_[0].getX();
    int y0 = points_[0].getY();
    int x1, y1;
    double ans;
    for (unsigned int i = 1; i < points_.size(); i++) {
        x1 = points_[i].getX();
        y1 = points_[i].getY();
        ans += sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
        x0 = x1;
        y0 = y1;

    }
    return ans;
}


double ClosedPolygonalChain::perimeter() const {
    int x0 = getPoint(0).getX();
    int y0 = getPoint(0).getY();
    int x1 = getPoint(getN() - 1).getX();
    int y1 = getPoint(getN() - 1).getY();
    double ans = PolygonalChain::perimeter() + sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));;
    return ans;
}


double Polygon::area() const {
    int n = getN() - 1;
    double sum1 = 0, sum2 = 0;

    for (int i = 0; i < n; i++) {
        sum1 += getPoint(i).getX() * getPoint(i + 1).getY();
        sum2 += getPoint(i).getY() * getPoint(i + 1).getX();

    }
    sum1 += getPoint(n - 1).getX() * getPoint(0).getY();
    sum2 += getPoint(n - 1).getY() * getPoint(0).getX();
    return abs(sum2 - sum1) / 2;
}


bool Triangle::hasRightAngle() const {
    int sc1, sc2, sc3;
    sc1 = (getPoint(1).getX() - getPoint(0).getX()) * ((getPoint(2).getX() - getPoint(1).getX())) +
          ((getPoint(1).getY() - getPoint(0).getY())) * ((getPoint(2).getY() - getPoint(1).getY()));
    sc2 = (getPoint(1).getX() - getPoint(0).getX()) * ((getPoint(2).getX() - getPoint(0).getX())) +
          ((getPoint(1).getY() - getPoint(0).getY())) * ((getPoint(2).getY() - getPoint(0).getY()));
    sc3 = (getPoint(2).getX() - getPoint(1).getX()) * ((getPoint(2).getX() - getPoint(0).getX())) +
          ((getPoint(2).getY() - getPoint(1).getY())) * ((getPoint(2).getY() - getPoint(0).getY()));
    if (sc1 == 0 || sc2 == 0 || sc3 == 0) {
        return true;
    } else return false;
}


double Trapezoid::height() const {
    double a = sqrt(
            pow(getPoint(1).getX() - getPoint(2).getX(), 2) + pow(getPoint(1).getY() - getPoint(2).getY(), 2));
    double b = sqrt(
            pow(getPoint(0).getX() - getPoint(3).getX(), 2) + pow(getPoint(0).getY() - getPoint(3).getY(), 2));

    double ans = (2 * area()) / (a + b);
    return ans;
}


double RegularPolygon::area() const {
    //FIXED: todo a is a bad name
    double length_ = sqrt(
            pow(getPoint(0).getX() - getPoint(1).getX(), 2) + pow(getPoint(0).getY() - getPoint(1).getY(), 2));
    double ans = (getN() * length_ * length_) / (4 * tan(PI / getN()));
    return ans;

}

double RegularPolygon::perimeter() const {
    double length_ = sqrt(
            pow(getPoint(0).getX() - getPoint(1).getX(), 2) + pow(getPoint(0).getY() - getPoint(1).getY(), 2));
    return getN() * length_;
}


