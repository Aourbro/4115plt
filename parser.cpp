#include "parser.h"
#include "error.h"

BasicExp BaseAST::resultExp = BasicExp();

std::unique_ptr<BaseAST> Parser::parse(std::vector<std::pair<TokenClass, std::string> > stream)
{
    this->_stream = stream;
    return parseExpr();
}

std::unique_ptr<ExprAST> Parser::parseExpr()
{
    std::unique_ptr<ExprAST> expr(new ExprAST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
    case TokenClass::Number:
    case TokenClass::Keyword:
    case TokenClass::LeftParenthesis:
        expr->term = parseTerm();
        expr->exprs = parseExprs();
        return expr;
    case TokenClass::Operator:
        if (_stream[_pos].second == "+") {
            expr->term = parseTerm();
            expr->exprs = parseExprs();
            return expr;
        } else if (_stream[_pos].second == "-") {
            expr->term = parseTerm();
            expr->exprs = parseExprs();
            return expr;
        } else {
            [[fallthrough]];
        }
    default:
        printf("Unexpected token \"%s\" at token %d\n", _stream[_pos].second.c_str(), _pos);
        _pos++;
        _success = false;
        return nullptr;
    }
}

std::unique_ptr<ExprsAST> Parser::parseExprs()
{
    std::unique_ptr<ExprsAST> exprs(new ExprsAST());
    switch (_stream[_pos].first) {
    case TokenClass::RightParenthesis:
    case TokenClass::RightBrace:
    case TokenClass::EOS:
        exprs->type = -1;
        return exprs;
    case TokenClass::Operator:
        if (_stream[_pos].second == "+") {
            _pos++;
            exprs->type = 0;
            exprs->term = parseTerm();
            exprs->exprs = parseExprs();
            return exprs;
        } else if (_stream[_pos].second == "-") {
            _pos++;
            exprs->type = 1;
            exprs->term = parseTerm();
            exprs->exprs = parseExprs();
            return exprs;
        } else {
            [[fallthrough]];
        }
    default:
        printf("Unexpected token \"%s\" at token %d\n", _stream[_pos].second.c_str(), _pos);
        _pos++;
        _success = false;
        return nullptr;
    }
}

std::unique_ptr<TermAST> Parser::parseTerm()
{
    std::unique_ptr<TermAST> term(new TermAST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
    case TokenClass::Number:
    case TokenClass::Keyword:
    case TokenClass::LeftParenthesis:
        term->uExpr = parseUExpr();
        term->terms = parseTerms();
        return term;
    case TokenClass::Operator:
        if (_stream[_pos].second == "+") {
            term->uExpr = parseUExpr();
            term->terms = parseTerms();
            return term;
        } else if (_stream[_pos].second == "-") {
            term->uExpr = parseUExpr();
            term->terms = parseTerms();
            return term;
        } else {
            [[fallthrough]];
        }
    default:
        printf("Unexpected token \"%s\" at token %d\n", _stream[_pos].second.c_str(), _pos);
        _pos++;
        _success = false;
        return nullptr;
    }
}

std::unique_ptr<TermsAST> Parser::parseTerms()
{
    std::unique_ptr<TermsAST> terms(new TermsAST());
    switch (_stream[_pos].first) {
    case TokenClass::RightParenthesis:
    case TokenClass::RightBrace:
    case TokenClass::EOS:
        terms->type = -1;
        return terms;
    case TokenClass::Operator:
        if (_stream[_pos].second == "*") {
            _pos++;
            terms->type = 0;
            terms->uExpr = parseUExpr();
            terms->terms = parseTerms();
            return terms;
        } else if (_stream[_pos].second == "/") {
            _pos++;
            terms->type = 1;
            terms->uExpr = parseUExpr();
            terms->terms = parseTerms();
            return terms;
        } else {
            terms->type = -1;
            return terms;
        }
    default:
        printf("Unexpected token \"%s\" at token %d\n", _stream[_pos].second.c_str(), _pos);
        _pos++;
        _success = false;
        return nullptr;
    }
}

std::unique_ptr<UExprAST> Parser::parseUExpr()
{
    std::unique_ptr<UExprAST> uexpr(new UExprAST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
    case TokenClass::Number:
    case TokenClass::Keyword:
    case TokenClass::LeftParenthesis:
        uexpr->type = 2;
        uexpr->fact = parseFact();
        return uexpr;
    case TokenClass::Operator:
        if (_stream[_pos].second == "+") {
            _pos++;
            uexpr->type = 0;
            uexpr->fact = parseFact();
            return uexpr;
        } else if (_stream[_pos].second == "-") {
            _pos++;
            uexpr->type = 1;
            uexpr->fact = parseFact();
            return uexpr;
        } else {
            [[fallthrough]];
        }
    default:
        printf("Unexpected token \"%s\" at token %d\n", _stream[_pos].second.c_str(), _pos);
        _pos++;
        _success = false;
        return nullptr;
    }
}


std::unique_ptr<FactAST> Parser::parseFact()
{
    std::unique_ptr<FactAST> fact(new FactAST());
    switch (_stream[_pos].first) {
    case TokenClass::LeftParenthesis:
        _pos++;
        fact->type = 0;
        fact->expr = parseExpr();
        _pos++;
        return fact;
    case TokenClass::Number:
    case TokenClass::Keyword:
        fact->type = 1;
        fact->num = parseNum();
        fact->symb0 = parseSymb0();
        return fact;
    case TokenClass::Symbol:
        fact->type = 2;
        fact->symbs = parseSymbs();
        return fact;
    default:
        printf("Unexpected token \"%s\" at token %d\n", _stream[_pos].second.c_str(), _pos);
        _pos++;
        _success = false;
        return nullptr;
    }
}

std::unique_ptr<NumAST> Parser::parseNum()
{
    std::unique_ptr<NumAST> num(new NumAST());
    switch (_stream[_pos].first) {
    case TokenClass::Keyword:
        num->type = 0;
        num->frac = parseFrac();
        return num;
    case TokenClass::Number:
        num->type = 1;
        num->number = std::stoi(_stream[_pos].second);
        _pos++;
        return num;
    default:
        printf("Unexpected token \"%s\" at token %d\n", _stream[_pos].second.c_str(), _pos);
        _pos++;
        _success = false;
        return nullptr;
    }
}

std::unique_ptr<SymbsAST> Parser::parseSymbs()
{
    std::unique_ptr<SymbsAST> symbs(new SymbsAST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
        symbs->symbol = _stream[_pos].second;
        _pos++;
        symbs->symb0 = parseSymb0();
        return symbs;
    default:
        printf("Unexpected token \"%s\" at token %d\n", _stream[_pos].second.c_str(), _pos);
        _pos++;
        _success = false;
        return nullptr;
    }
}

std::unique_ptr<Symb0AST> Parser::parseSymb0()
{
    std::unique_ptr<Symb0AST> symb0(new Symb0AST());
    switch (_stream[_pos].first) {
    case TokenClass::RightParenthesis:
    case TokenClass::RightBrace:
    case TokenClass::Operator:
    case TokenClass::EOS:
        symb0->type = -1;
        return symb0;
    case TokenClass::Symbol:
        symb0->type = 0;
        symb0->symbs = parseSymbs();
        return symb0;
    default:
        printf("Unexpected token \"%s\" at token %d\n", _stream[_pos].second.c_str(), _pos);
        _pos++;
        _success = false;
        return nullptr;
    }
}

std::unique_ptr<FracAST> Parser::parseFrac()
{
    std::unique_ptr<FracAST> frac(new FracAST());
    switch (_stream[_pos].first) {
    case TokenClass::Keyword:
        _pos += 2;
        frac->expr_numer = parseExpr();
        _pos += 2;
        frac->expr_denom = parseExpr();
        _pos++;
        return frac;
    default:
        printf("Unexpected token \"%s\" at token %d\n", _stream[_pos].second.c_str(), _pos);
        _pos++;
        _success = false;
        return nullptr;
    }
}
