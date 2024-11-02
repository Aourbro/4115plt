#ifndef PLT_AST_H
#define PLT_AST_H

#include <iostream>
#include <memory>
#include "rational.h"

class BaseAST {
public:
    virtual ~BaseAST() = default;
    virtual void Dump(std::string indent) const = 0;
};

class TemplateAST : public BaseAST{
public:
    //TODO

    void Dump(std::string indent) const override{
        std::cout << indent << "xxxAST" << std::endl;
        //TODO
        std::cout << std::endl;
    }
};

class ExprAST : public BaseAST{
public:
    std::unique_ptr<BaseAST> term;
    std::unique_ptr<BaseAST> exprs;

    void Dump(std::string indent) const override{
        std::cout << indent << "ExprAST" << std::endl;
        term->Dump(indent + "  ");
        exprs->Dump(indent + "  ");
        std::cout << std::endl;
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
        std::cout << std::endl;
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
        std::cout << std::endl;
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
        std::cout << std::endl;
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
        std::cout << std::endl;
    }
};

class FactAST : public BaseAST{
public:
    int type;   // 0=(Epxr) 1=fact0
    std::unique_ptr<BaseAST> expr;
    std::unique_ptr<BaseAST> fact0;

    void Dump(std::string indent) const override{
        std::cout << indent << "FactAST" << std::endl;
        if (type ==0) {
            std::cout << indent + "  (" << std::endl;
            expr->Dump(indent + "  ");
            std::cout << indent + "  )";
        } else {
            fact0->Dump(indent + "  ");
        }
        std::cout << std::endl;
    }
};

// Fact0 is an important class, it is the minimal unit for calculation
class Fact0AST : public BaseAST{
public:
    int type;   // 0=num symb0 1=symbs
    std::unique_ptr<BaseAST> num;
    std::unique_ptr<BaseAST> symb0;
    std::unique_ptr<BaseAST> symbs;

    bool readyCalc {false};
    Rational rat;
    uint64_t symbols;   // bitmap, 32 bits for english letters and 32 bits for greek letters

    void Dump(std::string indent) const override{
        std::cout << indent << "Fact0AST" << std::endl;
        if (type ==0) {
            num->Dump(indent + "  ");
            symb0->Dump(indent + "  ");
        } else {
            symbs->Dump(indent + "  ");
        }
        std::cout << std::endl;
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
            std::cout << indent + "  Number(" << number << ")";
        }
        std::cout << std::endl;
    }
};

class SymbsAST : public BaseAST{
public:
    std::unique_ptr<BaseAST> symb;
    std::unique_ptr<BaseAST> symb0;

    void Dump(std::string indent) const override{
        std::cout << indent << "SymbsAST" << std::endl;
        symb->Dump(indent + "  ");
        symb0->Dump(indent + "  ");
        std::cout << std::endl;
    }
};

class SymbAST : public BaseAST{
public:
    int type;   // 0=symbol 1=\symbol
    std::string symbol;

    void Dump(std::string indent) const override{
        std::cout << indent << "SymbAST" << std::endl;
        std::cout << indent + "  Symbol(";
        if (type == 1) {
            std::cout << indent + "  \\";
        }
        std::cout << symbol << ")" << std::endl;
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
        std::cout << std::endl;
    }
};

class FracAST : public BaseAST{
public:
    std::unique_ptr<BaseAST> expr_numer;
    std::unique_ptr<BaseAST> expr_denom;

    void Dump(std::string indent) const override{
        std::cout << indent << "FracAST" << std::endl;
        std::cout << indent + "  \\" << std::endl;
        std::cout << indent + "  frac" << std::endl;
        std::cout << indent + "  {" << std::endl;
        expr_numer->Dump(indent + "  ");
        std::cout << indent + "  }" << std::endl;
        std::cout << indent + "  {" << std::endl;
        expr_denom->Dump(indent + "  ");
        std::cout << indent + "  }" << std::endl;
    }
};

#endif