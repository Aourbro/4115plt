#ifndef PLT_RATIONAL_H
#define PLT_RATIONAL_H

class Rational {
private:
    int numer;
    int denom;
public:
    Rational(int n, int d) : numer(n), denom(d) {};
};

#endif