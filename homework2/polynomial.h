
#ifndef C___POLYNOMIAL_H
#define C___POLYNOMIAL_H

#include <iostream>



using namespace std;

class Polynomial {

public:
    Polynomial();

    Polynomial(int, int, int *);

    Polynomial(const Polynomial &);

    Polynomial &operator=(const Polynomial &);

    ~Polynomial();

private:
    int power_min_;
    int power_max_;
    int *chain_;


public:
    int operator[](int) const;

    int &operator[](int);

    double get(double) const;

    int getN() const;


    int getMaxPower() const;

    int &getChainN(int i) const;

    static void init(Polynomial &);

    //todo +=, =... should return Polynomial&
    Polynomial operator+=(const Polynomial &);

    Polynomial operator-=(const Polynomial &);

    Polynomial& operator/=(int);

    Polynomial& operator*=(const Polynomial &);


};

bool operator==(const Polynomial &, const Polynomial &);

Polynomial operator-(const Polynomial &);

Polynomial operator*(const Polynomial &, const Polynomial &);

Polynomial operator*(const Polynomial &, int);

Polynomial operator*(int, const Polynomial &);



Polynomial operator/(Polynomial &, int);



Polynomial operator+(const Polynomial &, const Polynomial &);

Polynomial operator-(const Polynomial &, const Polynomial &);


bool operator!=(const Polynomial &, const Polynomial &);


ostream &operator<<(ostream &, const Polynomial &);


istream &operator>>(istream &, Polynomial &);


#endif //C___POLYNOMIAL_H




