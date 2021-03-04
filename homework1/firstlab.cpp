#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const double PI = acos(-1.0);

class Point {
public:
    Point(int x = 0, int y = 0)
            : x_(x)
            , y_(y)
    {};

    //copy
    Point(const Point& other)
            : x_(other.x_)
            , y_(other.y_)
    {};

    //=
    Point& operator=(const Point&  other){
        if(&other == this){
            return *this;
        }
            x_ = other.x_;
            y_ = other.y_;

        return *this;
    }

    ~Point() = default;


    int getX() const{
        return x_;
    }

    int getY() const{
        return y_;
    }



private:
    int x_;
    int y_;
};

class PolygonalChain{

public:
    PolygonalChain(unsigned int n = 0, Point *p = {}){
        N_=n;
        for(unsigned int i=0; i<n; i++){
            Points_.push_back(p[i]);
        };
    };

    //copy
    PolygonalChain(const PolygonalChain& other)
            : N_(other.N_)
            , Points_(other.Points_)
    {};

    //=
    PolygonalChain& operator=(const PolygonalChain&  other){
        if(&other == this){
            return *this;
        }else {
            N_ = other.N_;
            Points_ = other.Points_;

            return *this;
        }
    };

    virtual ~PolygonalChain(){
        N_=0;
        Points_.clear();
    }


    const Point getPoint(unsigned int k) const{
        return Points_[k];
    }

    int getN() const{
        return N_;
    }

    virtual double perimeter() const{
        int x0=Points_[0].getX();
        int y0=Points_[0].getY();
        int x1,y1;
        double ans;
        for (unsigned int i=1;i<N_;i++){
            x1=Points_[i].getX();
            y1=Points_[i].getY();
            ans+=sqrt(pow(x1-x0,2)+pow(y1-y0,2));
            x0=x1;
            y0=y1;

        }
        return ans;
    }

private:
    unsigned int N_;
    vector<Point> Points_;

};


class ClosedPolygonalChain: public PolygonalChain{

    using PolygonalChain::PolygonalChain;
public:
    virtual double perimeter() const{
        int x0=getPoint(0).getX();
        int y0=getPoint(0).getY();
        int x1=getPoint(getN()-1).getX();
        int y1=getPoint(getN()-1).getY();
        double ans=PolygonalChain::perimeter()+sqrt(pow(x1-x0,2)+pow(y1-y0,2));;
        return ans;
    }

};

class Polygon: public ClosedPolygonalChain{

    using ClosedPolygonalChain::ClosedPolygonalChain;
public:
    virtual double area() const{
        int n = getN()-1;
        int sum1=0,sum2=0;

        for(int i=0; i<n;i++){
            sum1+=getPoint(i).getX()*getPoint(i+1).getY();
            sum2+=getPoint(i).getY()*getPoint(i+1).getX();

        }
        sum1+=getPoint(n-1).getX()*getPoint(0).getY();
        sum2+=getPoint(n-1).getY()*getPoint(0).getX();
        return abs(sum2-sum1)/2;
    }

};

class Triangle: public Polygon{

    using Polygon::Polygon;
public:
    bool hasRightAngle() const{
        int sc1,sc2,sc3;
        sc1=(getPoint(1).getX()-getPoint(0).getX())*((getPoint(2).getX()-getPoint(1).getX()))+
            ((getPoint(1).getY()-getPoint(0).getY()))*((getPoint(2).getY()-getPoint(1).getY()));
        sc2=(getPoint(1).getX()-getPoint(0).getX())*((getPoint(2).getX()-getPoint(0).getX()))+
            ((getPoint(1).getY()-getPoint(0).getY()))*((getPoint(2).getY()-getPoint(0).getY()));
        sc3=(getPoint(2).getX()-getPoint(1).getX())*((getPoint(2).getX()-getPoint(0).getX()))+
            ((getPoint(2).getY()-getPoint(1).getY()))*((getPoint(2).getY()-getPoint(0).getY()));
        if(sc1==0 || sc2==0 || sc3==0){
            return true;
        }else return false;
    }

};

class Trapezoid: public Polygon{
    using Polygon::Polygon;
public:
    double height() const{
        double a = sqrt(pow(getPoint(1).getX()-getPoint(2).getX(),2)+pow(getPoint(1).getY()-getPoint(2).getY(),2));
        double b = sqrt(pow(getPoint(0).getX()-getPoint(3).getX(),2)+pow(getPoint(0).getY()-getPoint(3).getY(),2));

        double ans = (2*area())/(a+b);
        return ans;
    }

};

class RegularPolygon: public Polygon{
    using Polygon::Polygon;
public:

    double area() const{
        double ans = (getN()*a*a)/(4* tan(PI/getN()));
        return ans;

    }

    double perimeter() const{
        return getN()*a;
    }
private:
    double a = sqrt(pow(getPoint(0).getX()-getPoint(1).getX(),2)+pow(getPoint(0).getY()-getPoint(1).getY(),2));
};

int main(){
    




    return 0;
}
