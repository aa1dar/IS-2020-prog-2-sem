#include <iostream>


#include "polynomial.h"

using namespace std;

Polynomial::Polynomial() : power_min_(0),
                           power_max_(0),
                           chain_(new int[1]{0}) {};

Polynomial::Polynomial(int power_min, int power_max, int *chain) {

    int *tmp = new int[power_max - power_min + 1]{0};
    for (int i = 0; i < power_max - power_min + 1; i++) {
        tmp[i] = chain[i];
    }
    chain_ = tmp;
    power_max_ = power_max;
    power_min_ = power_min;
};

Polynomial::Polynomial(const Polynomial &other){
    int *tmp = new int[other.power_max_ - other.power_min_ + 1]{0};
    for (int i = 0; i < other.power_max_ - other.power_min_ + 1; i++) {
        tmp[i] = other.chain_[i];
    }
    chain_ = tmp;
    power_max_ = other.power_max_;
    power_min_ = other.power_min_;
};

Polynomial &Polynomial::operator=(const Polynomial &other) {
    if (&other == this) {
        return *this;
    }

    int *tmp = new int[other.power_max_ - other.power_min_ + 1]{0};
    for (int i = 0; i < other.power_max_ - other.power_min_ + 1; i++) {
        tmp[i] = other.chain_[i];
    }
    delete[] chain_;
    chain_ = tmp;
    power_max_ = other.power_max_;
    power_min_ = other.power_min_;

    return *this;
}

Polynomial::~Polynomial(){
    delete [] chain_;

};

int Polynomial::operator[](int i) const {
    if (i <= power_max_ && i >= power_min_) {
        return chain_[i - power_min_];
    } else
        return 0;
}

int &Polynomial::operator[](int i) {
    if (i <= power_max_ && i >= power_min_) {
        return chain_[i - power_min_];
    } else if (i > power_max_) {

        int *tmp = new int[i - power_min_ + 1]{0};
        for (int j = 0; j < power_max_ - power_min_ + 1; j++) {
            tmp[j] = chain_[j];
        }

        power_max_ = i;
        delete[] chain_;
        chain_ = tmp;

        return chain_[i - power_min_];

    } else {
        int *tmp = new int[power_max_ - i + 1]{0};

        for (int j = power_max_ - power_min_; j >= 0; j--) {
            tmp[j + power_min_ - i] = chain_[j];
        }

        power_min_ = i;
        delete[] chain_;
        chain_ = tmp;


        return chain_[0];
    }
};

double Polynomial::get(double i) const {
    double ans = 0;
    int pwr = power_max_ + 1;

    for (int j = getN() - 1; j >= 0; j--) {
        if (pwr < 0) {
            if (i == 0) {
                pwr--;
                continue;
            }
            ans = ans * (double) (1 / i) + chain_[j];
        } else
            ans = ans * i + chain_[j];
        pwr--;

    }
    if (pwr < 0 && i != 0)
        ans *= (double) (1 / i);
    return ans;
}

int Polynomial::getN() const {
    return power_max_ - power_min_ + 1;
}


int Polynomial::getMaxPower() const {
    return power_max_;
}

int &Polynomial::getChainN(int i) const {
    return chain_[i];
}

void Polynomial::init(Polynomial &pol) {
    int pmin = pol.power_min_;
    int pmax = pol.power_max_;


    int i = 0;
    int k = pol.getN();
    bool f1 = true;
    bool f2 = true;
    while (i < k) {
        if (pol.chain_[i] == 0 && f1)
            ++pmin;
        else
            f1 = false;

        if (pol.chain_[k - 1 - i] == 0 && f2)
            --pmax;
        else
            f2 = false;


        i++;
    }
    if (pmax < pmin) {

        pol.chain_ = new int[1]{0};
        pol.power_min_ = 0;
        pol.power_max_ = 0;
        return;
    }


    int *tmp = new int[pmax - pmin + 1]{0};

    int pwr = pol.power_min_;
    int counter = 0;
    for (int j = 0; j < k; j++) {
        if (pwr >= pmin && pwr <= pmax) {
            tmp[counter++] = pol.chain_[j];
        }
        pwr++;
    }


    pol.chain_ = tmp;
    pol.power_min_ = pmin;
    pol.power_max_ = pmax;


}

Polynomial Polynomial::operator+=(const Polynomial &other) {

    int pmin = 0;
    int pmax = 0;
    if (power_max_ < other.power_max_)
        pmax = other.power_max_;
    else
        pmax = power_max_;

    if (power_min_ > other.power_min_)
        pmin = other.power_min_;
    else
        pmin = power_min_;

    Polynomial *temp = new Polynomial(pmin, pmax, new int[pmax - pmin + 1]{0});


    int tmp = pmin;
    int i = 0;
    int counter1 = 0;
    int counter2 = 0;

    while (tmp <= pmax) {


        if (tmp >= power_min_ && tmp <= power_max_) {
            temp->chain_[i] += chain_[counter1++];
        }

        if (tmp >= other.power_min_ && tmp <= other.power_max_) {
            temp->chain_[i] += other.chain_[counter2++];
        }

        i++;
        tmp++;
    }


    init(*temp);


    *this = *temp;
    delete temp;

    return *this;
};

//todo without new object
Polynomial Polynomial::operator-=(const Polynomial &other) {

    *this+=(-other);

    return *this;
}

Polynomial operator-(const Polynomial &pol) {
    Polynomial tmp = Polynomial();


    int pwr = pol.getMaxPower();

    for (int i = pol.getN() - 1; i >= 0; i--) {
        tmp[pwr--] = -1 * pol.getChainN(i);
    }


    return tmp;
}


bool operator==(const Polynomial &lhs1, const Polynomial &rhs1) {
    Polynomial lhs(lhs1);
    Polynomial rhs(rhs1);
    Polynomial::init(lhs);
    Polynomial::init(rhs);
    bool f1 = false;
    bool f2 = true;
    f1 = (lhs.getMaxPower() == rhs.getMaxPower() && lhs.getN() == rhs.getN());
    if (f1)
        for (int i = 0; i < lhs.getN(); i++) {
            if (lhs.getChainN(i) != rhs.getChainN(i)) {
                f2 = false;
                break;
            }
        }
    return (f1 && f2);
}


Polynomial operator*(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial* tmp = new Polynomial(lhs);
    *tmp*=rhs;
    return *tmp;

}

Polynomial operator*(const Polynomial &lhs, int num) {

    int pwr1 = lhs.getMaxPower();
    Polynomial tmp = Polynomial();
    for (int i = lhs.getN() - 1; i >= 0; i--) {
        tmp[pwr1] = (int) lhs.getChainN(i) * num;

        pwr1--;
    }
    Polynomial::init(tmp);
    return tmp;
}

Polynomial operator*(int num, const Polynomial &lhs) {
    return lhs * num;
}

Polynomial& Polynomial::operator*=(const Polynomial &rhs) {
    int pwr1 = getMaxPower();
    Polynomial tmp = Polynomial();
    for (int i = getN() - 1; i >= 0; i--) {
        int pwr2 = rhs.getMaxPower();
        for (int j = rhs.getN() - 1; j >= 0; j--) {
            tmp[(pwr1) + (pwr2--)] += rhs.getChainN(j) * getChainN(i);


        }
        pwr1--;
    }
    Polynomial::init(tmp);
    *this = tmp;

    return *this;
}

Polynomial operator/(Polynomial &lhs, int num) {
    Polynomial* temp = new Polynomial(lhs);
    *temp/=num;

    return *temp;
}
                

Polynomial& Polynomial::operator/=(int num) {

    int pwr1 = getMaxPower();
    Polynomial tmp = Polynomial();
    //todo use for_each
    for (int i = getN() - 1; i >= 0; i--) {
        tmp[pwr1] = (int) operator[](pwr1)/ num;

        pwr1--;
    }
    Polynomial::init(tmp);
    *this = tmp;

    return *this;

}

Polynomial operator+(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial tmp(lhs);
    tmp += rhs;

    Polynomial::init(tmp);
    return tmp;
}

Polynomial operator-(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial tmp(lhs);
    tmp -= rhs;

    return tmp;
}


bool operator!=(const Polynomial &lhs, const Polynomial &rhs) {
    return !(operator==(lhs, rhs));
}


ostream &operator<<(ostream &stream, const Polynomial &value) {
    int power = value.getMaxPower();
    int i = value.getN() - 1;

    if (i < 0) {
        stream << 0;
    }

    while (i >= 0) {
        int val = value.getChainN(i);

        if (val == 0) {
            if (power == 0 && value.getN() == 1) {
                stream << 0;
                break;
            }
            i--;
            power--;
            continue;
        }


        if (power == 0) {
            if (val > 0 && i != value.getN() - 1)
                stream << '+' << val;
            else
                stream << val;
        } else if (power == 1) {
            if (val > 0 && i != value.getN() - 1)
                if (val == 1)
                    stream << "+x";
                else
                    stream << '+' << val << "x";
            else {
                if (val == 1)
                    stream << "+x";
                else if (val == -1)
                    stream << "-x";
                else
                    stream << val << "x";
            }
        } else {
            if (val > 0 && i != value.getN() - 1)
                if (val == 1)
                    stream << "+x^" << power;
                else
                    stream << '+' << val << "x^" << power;
            else {
                if (val == 1)
                    stream << "x^" << power;
                else if (val == -1)
                    stream << "-x^" << power;
                else
                    stream << val << "x^" << power;
            }
        }
        i--;
        power--;
    }


    return stream;
}


istream &operator>>(istream &stream, Polynomial &value) {
    int pmin = 0;
    int pmax = 0;
    stream >> pmin;
    stream >> pmax;
    value = Polynomial();
    int p = 0;
    while (pmax >= pmin) {
        stream >> p;
        value[pmin] = p;
        pmin++;
    }

    return stream;
}




