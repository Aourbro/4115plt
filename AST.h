#ifndef PLT_AST_H
#define PLT_AST_H

#include <iostream>
#include <memory>
#include <algorithm>
#include "basic_exp.h"

class BaseAST {
public:
    static BasicExp resultExp;
    virtual ~BaseAST() = default;
    virtual void Dump(std::string indent) const = 0;
    virtual void calc() const = 0;    // TODO: implement this method on every ast nodes
};

class ExprAST : public BaseAST{
public:
    std::unique_ptr<BaseAST> term;
    std::unique_ptr<BaseAST> exprs;

    void Dump(std::string indent) const override{
        std::cout << indent << "ExprAST" << std::endl;
        term->Dump(indent + "  ");
        exprs->Dump(indent + "  ");
    }

    void calc() const override{
        term->calc();
        exprs->calc();
    }
};

class ExprsAST : public BaseAST{
public:
    int type;   // 0="+" 1="-" -1=e
    std::unique_ptr<BaseAST> term;
    std::unique_ptr<BaseAST> exprs;

    void Dump(std::string indent) const override{
        if (type == -1){
            return;
        }
        std::cout << indent << "ExprsAST" << std::endl;
        if (type == 0) {
            std::cout << indent + "  +" << std::endl;
        } else {
            std::cout << indent + "  -" << std::endl;
        }
        term->Dump(indent + "  ");
        exprs->Dump(indent + "  ");
    }

    void calc() const override{
        if (type == -1) {
            return;
        }
        BasicExp tmp = resultExp;
        term->calc();
        if (type == 0) {
            resultExp = tmp + resultExp;
        } else {
            resultExp = tmp - resultExp;
        }
        exprs->calc();
    }
};

class TermAST : public BaseAST{
public:
    std::unique_ptr<BaseAST> uExpr;
    std::unique_ptr<BaseAST> terms;

    void Dump(std::string indent) const override{
        std::cout << indent << "TermAST" << std::endl;
        uExpr->Dump(indent + "  ");
        terms->Dump(indent + "  ");
    }

    void calc() const override{
        uExpr->calc();
        terms->calc();
    }
};

class TermsAST : public BaseAST{
public:
    int type;   // 0="*" 1="/" -1=e
    std::unique_ptr<BaseAST> uExpr;
    std::unique_ptr<BaseAST> terms;

    void Dump(std::string indent) const override{
        if (type == -1){
            return;
        }
        std::cout << indent << "TermsAST" << std::endl;
        if (type == 0) {
            std::cout << indent + "  *" << std::endl;
        } else {
            std::cout << indent + "  /" << std::endl;
        }
        uExpr->Dump(indent + "  ");
        terms->Dump(indent + "  ");
    }

    void calc() const override{
        if (type == -1) {
            return;
        }
        BasicExp tmp = resultExp;
        uExpr->calc();
        if (type == 0) {
            resultExp = tmp * resultExp;
        } else {
            resultExp = tmp / resultExp;
        }
        terms->calc();
    }
};

class UExprAST : public BaseAST{
public:
    int type;   // 0="+" 1="-" 2=no unary op
    std::unique_ptr<BaseAST> fact;

    void Dump(std::string indent) const override{
        std::cout << indent << "UExprAST" << std::endl;
        if (type == 0) {
            std::cout << indent + "  +" << std::endl;
        } else if (type == 1) {
            std::cout << indent + "  -" << std::endl;
        }
        fact->Dump(indent + "  ");
    }

    void calc() const override{
        fact->calc();
        if (type == 1) {
            resultExp = -resultExp;
        }
    }
};

// Fact is an important class, it is the minimal unit for calculation
class FactAST : public BaseAST{
public:
    int type;   // 0=(Epxr) 1=num symb0 2=symbs
    std::unique_ptr<BaseAST> expr;
    std::unique_ptr<BaseAST> num;
    std::unique_ptr<BaseAST> symb0;
    std::unique_ptr<BaseAST> symbs;

    void Dump(std::string indent) const override{
        std::cout << indent << "FactAST" << std::endl;
        if (type == 0) {
            std::cout << indent + "  (" << std::endl;
            expr->Dump(indent + "  ");
            std::cout << indent + "  )";
        } else if (type == 1) {
            num->Dump(indent + "  ");
            symb0->Dump(indent + "  ");
        } else {
            symbs->Dump(indent + "  ");
        }
    }

    void calc() const override{
        if (type == 0) {
            expr->calc();
        } else if (type == 1) {
            num->calc();
            symb0->calc();
        } else {
            symbs->calc();
        }
    }
};

class NumAST : public BaseAST{
public:
    int type;   // 0=frac 1=number
    std::unique_ptr<BaseAST> frac;
    int number;

    void Dump(std::string indent) const override{
        std::cout << indent << "NumAST" << std::endl;
        if (type ==0) {
            frac->Dump(indent + "  ");
        } else {
            std::cout << indent + "  number(" << number << ")" << std::endl;
        }
    }

    void calc() const override{
        if (type == 0) {
            frac->calc();
        } else {
            std::vector<BasicTerm>().swap(resultExp.numer);
            resultExp.numer.push_back(BasicTerm(Rational(number, 1), 0));
        }
    }
};

class SymbsAST : public BaseAST{
private:
    const std::vector<std::string> _symbolPool {
        "\\alpha", "\\beta", "\\gamma", "\\delta", "\\epsilon", "\\zeta", "\\eta", "\\theta", "\\lota", "\\kappa",
        "\\lambda", "\\mu", "\\nu", "\\xi", "\\omicron", "\\pi", "\\rho", "\\sigma", "\\tau", "\\upsilon", "\\phi",
        "\\chi", "\\psi", "\\omega",
    };  // 24

public:
    std::string symbol;
    std::unique_ptr<BaseAST> symb0;

    void Dump(std::string indent) const override{
        std::cout << indent << "SymbsAST" << std::endl;
        std::cout << indent + "  symbol(" << symbol << ")" << std::endl;
        symb0->Dump(indent + "  ");
    }

    void calc() const override{
        uint64_t symbolBit;
        if (symbol.length() == 1) {
            symbolBit = 1ULL << (symbol[0] - 'a');
        } else {
            std::vector<std::string>::const_iterator it = find(_symbolPool.begin(), _symbolPool.end(), symbol);
            symbolBit = 1ULL << (26 + it - _symbolPool.begin());
        }
        std::vector<BasicTerm>().swap(resultExp.numer);
        resultExp.numer.push_back(BasicTerm(Rational(1, 1), symbolBit));
        symb0->calc();
    }
};

class Symb0AST : public BaseAST{
public:
    int type;   // 0=symbs -1=e
    std::unique_ptr<BaseAST> symbs;

    void Dump(std::string indent) const override{
        if (type == -1) {
            return;
        }
        std::cout << indent << "Symb0AST" << std::endl;
        symbs->Dump(indent + "  ");
    }

    void calc() const override{
        if (type == -1) {
            return;
        }
        BasicExp tmp = resultExp;
        symbs->calc();
        resultExp = tmp * resultExp;
    }
};

class FracAST : public BaseAST{
public:
    std::unique_ptr<BaseAST> expr_numer;
    std::unique_ptr<BaseAST> expr_denom;

    void Dump(std::string indent) const override{
        std::cout << indent << "FracAST" << std::endl;
        std::cout << indent + "  \\frac" << std::endl;
        std::cout << indent + "  {" << std::endl;
        expr_numer->Dump(indent + "  ");
        std::cout << indent + "  }" << std::endl;
        std::cout << indent + "  {" << std::endl;
        expr_denom->Dump(indent + "  ");
        std::cout << indent + "  }" << std::endl;
    }

    void calc() const override{
        expr_numer->calc();
        BasicExp tmp = resultExp;
        expr_denom->calc();
        resultExp = tmp / resultExp;
    }
};

#endif