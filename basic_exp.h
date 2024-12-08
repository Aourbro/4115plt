#ifndef PLT_BASIC_EXP_H
#define PLT_BASIC_EXP_H

#include <cstdint>
#include <vector>
#include <iostream>

class Rational {
public:
    int numer {0};
    int denom {1}; // 1 if this rational is a integer
    
    Rational(int n, int d) : numer(n), denom(d) {};

    friend Rational operator+(const Rational& ratA, const Rational& ratB);
    friend Rational operator-(const Rational& ratA, const Rational& ratB);
    friend Rational operator*(const Rational& ratA, const Rational& ratB);
    friend Rational operator/(const Rational& ratA, const Rational& ratB);

    void CodeGen() {
        if (numer < 0) { // 如果分子为负数
            std::cout << "-";
        }
        std::cout << "\\frac{" << std::abs(numer) << "}{" << denom << "}";
    }
};

class BasicTerm {
private:
    const std::vector<std::string> _symbolPool {
        "\\alpha", "\\beta", "\\gamma", "\\delta", "\\epsilon", "\\zeta", "\\eta", "\\theta", "\\lota", "\\kappa",
        "\\lambda", "\\mu", "\\nu", "\\xi", "\\omicron", "\\pi", "\\rho", "\\sigma", "\\tau", "\\upsilon", "\\phi",
        "\\chi", "\\psi", "\\omega",
    };  // 24

public:
    Rational rational;
    uint64_t symbolTable {0};  // bit pattern for symbols, a = 1 << 0, \alpha = 1 << 26

    BasicTerm(Rational rat, uint64_t st) : rational(rat), symbolTable(st) {};
    BasicTerm& operator=(const BasicTerm& term) {
        this->rational = term.rational;
        this->symbolTable = term.symbolTable;
        return *this;
    }

    friend BasicTerm operator-(const BasicTerm& term);
    friend BasicTerm operator*(const BasicTerm& termA, const BasicTerm& termB);

    void CodeGen() {
        // rational.CodeGen();
        if (rational.denom == 1) {
            if (rational.numer == -1) {
                std::cout << "-"; //
            } else if (rational.numer != 1) {
                std::cout << rational.numer;
            }
        } else {
            rational.CodeGen();
        }
        for (char c = 'a'; c <= 'z'; ++c) {
            if (symbolTable & (1ULL << (c - 'a'))) {
                std::cout << c;
            }
        }
        for (int i = 0; i < 24; ++i) {
            if (symbolTable & (1ULL << (26 + i))) {
                std::cout << _symbolPool[i];
            }
        }
    }
};

class BasicExp {
public:
    std::vector<BasicTerm> numer;   // in case the exp is a frac-style exp, i.e. there are symbols in denom
    // std::vector<BasicTerm> denom;

    BasicExp() {};
    BasicExp& operator=(const BasicExp& exp) {
        this->numer = exp.numer;
        return *this;
    }

    friend BasicExp operator-(const BasicExp& exp);
    friend BasicExp operator+(const BasicExp& expA, const BasicExp& expB);
    friend BasicExp operator-(const BasicExp& expA, const BasicExp& expB);
    friend BasicExp operator*(const BasicExp& expA, const BasicExp& expB);
    friend BasicExp operator/(const BasicExp& expA, const BasicExp& expB);

    void CodeGen() {
        int len = numer.size();
        for (int i = 0; i < len; ++i) {
            // if (i > 0 && numer[i].rational.numer > 0) {  // insert `+` to the subsequent postive value
            //     std::cout << "+";
            // }
            if (i > 0) {
                if (numer[i].rational.numer > 0) {
                    std::cout << "+";
                }
            }
            numer[i].CodeGen();
        }
    }
};

#endif