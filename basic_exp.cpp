#include "basic_exp.h"
#include "utils.h"
#include <cstdio>

Rational operator+(const Rational& ratA, const Rational& ratB)
{
    if (ratA.denom == 1 && ratB.denom == 1) {
        return Rational(ratA.numer + ratB.numer, 1);
    }
    int numer = ratA.numer * ratB.denom + ratA.denom * ratB.numer;
    if (numer == 0) {
        return Rational(0, 1);
    }
    int denom = ratA.denom * ratB.denom;
    int gcd = Gcd({numer, denom});
    return Rational(numer / gcd, denom / gcd);
}

Rational operator-(const Rational& ratA, const Rational& ratB)
{
    if (ratA.denom == 1 && ratB.denom == 1) {
        return Rational(ratA.numer - ratB.numer, 1);
    }
    int numer = ratA.numer * ratB.denom - ratA.denom * ratB.numer;
    if (numer == 0) {
        return Rational(0, 1);
    }
    int denom = ratA.denom * ratB.denom;
    int gcd = Gcd({numer, denom});
    return Rational(numer / gcd, denom / gcd);
}

Rational operator*(const Rational& ratA, const Rational& ratB)
{
    if (ratA.denom == 1 && ratB.denom == 1) {
        return Rational(ratA.numer * ratB.numer, 1);
    }
    if (ratA.denom == 0 || ratB.denom == 0) {
        return Rational(0, 1);
    }
    int numer = ratA.numer * ratB.numer;    // may have overflow problem, could be optimized
    int denom = ratA.denom * ratB.denom;
    int gcd = Gcd({numer, denom});
    return Rational(numer / gcd, denom / gcd);
}

Rational operator/(const Rational& ratA, const Rational& ratB)
{
    if (ratA.denom == 0) {
        return Rational(0, 1);
    }
    int numer = ratA.numer * ratB.denom;    // may have overflow problem, could be optimized
    int denom = ratA.denom * ratB.numer;
    int gcd = Gcd({numer, denom});
    return Rational(numer / gcd, denom / gcd);
}

BasicTerm operator-(const BasicTerm& term)
{
    return BasicTerm(Rational(-term.rational.numer, term.rational.denom), term.symbolTable);
}

BasicTerm operator*(const BasicTerm& termA, const BasicTerm& termB)
{
    // same symbols are not allowed to multiply, e.g. a*a
    assert("no same symbols allowed in multiply" && (termA.symbolTable & termB.symbolTable) == 0);

    return BasicTerm(termA.rational * termB.rational, termA.symbolTable | termB.symbolTable);
}

BasicExp operator-(const BasicExp& exp)
{
    BasicExp res;
    int len = exp.numer.size();

    for (int i = 0; i < len; ++i) {
        res.numer.push_back(-exp.numer[i]);
    }
    return res;
}

BasicExp operator+(const BasicExp& expA, const BasicExp& expB)
{
    BasicExp res = expA;
    int lenA = expA.numer.size();
    int lenB = expB.numer.size();

    for (int j = 0; j < lenB; ++j) {
        int i = 0;
        for ( ; i < lenA; ++i) {
            if (expA.numer[i].symbolTable == expB.numer[j].symbolTable) {
                res.numer[i].rational = expA.numer[i].rational + expB.numer[j].rational;
                break;
            }
        }
        if (i == lenA) {
            res.numer.push_back(expB.numer[j]);
        }
    }
    return res;
}

BasicExp operator-(const BasicExp& expA, const BasicExp& expB)
{
    BasicExp res = expA;
    int lenA = expA.numer.size();
    int lenB = expB.numer.size();

    for (int j = 0; j < lenB; ++j) {
        int i = 0;
        for ( ; i < lenA; ++i) {
            if (expA.numer[i].symbolTable == expB.numer[j].symbolTable) {
                res.numer[i].rational = expA.numer[i].rational - expB.numer[j].rational;
                break;
            }
        }
        if (i == lenA) {
            res.numer.push_back(-expB.numer[j]);
        }
    }
    return res;
}

BasicExp operator*(const BasicExp& expA, const BasicExp& expB)
{
    BasicExp res;
    int lenA = expA.numer.size();
    int lenB = expB.numer.size();

    for (int i = 0; i < lenA; ++i) {
        for (int j = 0; j < lenB; ++j) {
            res.numer.push_back(expA.numer[i] * expB.numer[j]);
        }
    }
    return res;
}

BasicExp operator/(const BasicExp& expA, const BasicExp& expB)
{
    BasicExp res = expA;
    int lenA = expA.numer.size();
    int lenB = expB.numer.size();

    // we only support devision when no symbol in expB
    assert ("no symbols allowed in devisor" && lenB == 1 && expB.numer[0].symbolTable == 0);

    for (int i = 0; i < lenA; ++i) {
        res.numer[i].rational = expA.numer[i].rational / expB.numer[0].rational;
    }
    return res;
}
