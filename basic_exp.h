#ifndef PLT_BASIC_EXP_H
#define PLT_BASIC_EXP_H

#include <cstdint>
#include <vector>

class Rational {
public:
    int numer {0};
    int denom {1}; // 1 if this rational is a integer
    
    Rational(int n, int d) : numer(n), denom(d) {};

    friend Rational operator+(const Rational& ratA, const Rational& ratB);
    friend Rational operator-(const Rational& ratA, const Rational& ratB);
    friend Rational operator*(const Rational& ratA, const Rational& ratB);
    friend Rational operator/(const Rational& ratA, const Rational& ratB);
};

class BasicTerm {
public:
    Rational rational;
    uint64_t symbolTable {0};  // bit pattern for symbols, a = 1 << 0, \alpha = 1 << 26

    BasicTerm(Rational rat, uint64_t st) : rational(rat), symbolTable(st) {};

    friend BasicTerm operator-(const BasicTerm& term);
    friend BasicTerm operator*(const BasicTerm& termA, const BasicTerm& termB);
};

class BasicExp {
public:
    std::vector<BasicTerm> numer;   // in case the exp is a frac-style exp, i.e. there are symbols in denom
    // std::vector<BasicTerm> denom;

    friend BasicExp operator+(const BasicExp& expA, const BasicExp& expB);
    friend BasicExp operator-(const BasicExp& expA, const BasicExp& expB);
    friend BasicExp operator*(const BasicExp& expA, const BasicExp& expB);
    friend BasicExp operator/(const BasicExp& expA, const BasicExp& expB);
};

#endif