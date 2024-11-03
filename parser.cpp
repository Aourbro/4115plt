#include "parser.h"
#include "error.h"

std::unique_ptr<BaseAST> Parser::parse(std::vector<std::pair<TokenClass, std::string> > stream)
{
    this->_stream = stream;
    return parseExpr();
}

    // case TokenClass::Symbol:
    // case TokenClass::Number:
    // case TokenClass::Keyword:
    // case TokenClass::Operator:
    // case TokenClass::EscapeChar:
    // case TokenClass::LeftParenthesis:
    // case TokenClass::RightParenthesis:
    // case TokenClass::LeftBrace:
    // case TokenClass::RightBrace:
    // case TokenClass::EOS;
    // default:
    //     return "Unkown Token Class";

std::unique_ptr<ExprAST> Parser::parseExpr()
{
    std::unique_ptr<ExprAST> expr(new ExprAST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
    case TokenClass::Number:
    case TokenClass::EscapeChar:
    case TokenClass::LeftParenthesis:
        expr->term = parseTerm();
        expr->exprs = parseExprs();
        return expr;
    default:
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
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
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
        return nullptr;
    }
}

std::unique_ptr<TermAST> Parser::parseTerm()
{
    std::unique_ptr<TermAST> term(new TermAST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
    case TokenClass::Number:
    case TokenClass::EscapeChar:
    case TokenClass::LeftBrace:
        term->uExpr = parseUExpr();
        term->terms = parseTerms();
        return term;
    default:
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
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
            [[fallthrough]];
        }
    default:
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
        return nullptr;
    }
}

std::unique_ptr<UExprAST> Parser::parseUExpr()
{
    std::unique_ptr<UExprAST> uexpr(new UExprAST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
    case TokenClass::Number:
    case TokenClass::EscapeChar:
    case TokenClass::LeftBrace:
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
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
        return nullptr;
    }
}


std::unique_ptr<FactAST> Parser::parseFact()
{
    std::unique_ptr<FactAST> fact(new FactAST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
    case TokenClass::Number:
    case TokenClass::EscapeChar:
        fact->type = 1;
        fact->fact0 = parseFact0();
        return fact;
    case TokenClass::LeftBrace:
        _pos++;
        fact->type = 0;
        fact->expr = parseExpr();
        _pos++;
        return fact;
    default:
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
        return nullptr;
    }
}

std::unique_ptr<Fact0AST> Parser::parseFact0()
{
    std::unique_ptr<Fact0AST> fact0(new Fact0AST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
    case TokenClass::EscapeChar:
        fact0->type = 1;
        fact0->symbs = parseSymbs();
        return fact0;
    case TokenClass::Number:
        fact0->type = 0;
        fact0->num = parseNum();
        fact0->symb0 = parseSymb0();
        return fact0;
    default:
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
        return nullptr;
    }
}

std::unique_ptr<NumAST> Parser::parseNum()
{
    std::unique_ptr<NumAST> num(new NumAST());
    switch (_stream[_pos].first) {
    case TokenClass::Number:
        num->frac = parseNum();
        return num;
    default:
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
        return nullptr;
    }
}

std::unique_ptr<SymbsAST> Parser::parseSymbs()
{
    std::unique_ptr<SymbsAST> symbs(new SymbsAST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
    case TokenClass::EscapeChar:
        symbs->symb = parseSymb();
        symbs->symb0 = parseSymb0();
        return symbs;
    default:
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
        return nullptr;
    }
}

std::unique_ptr<SymbAST> Parser::parseSymb()
{
    std::unique_ptr<SymbAST> symb(new SymbAST());
    switch (_stream[_pos].first) {
    case TokenClass::Symbol:
        symb->type = 0;
        symb->symbol = _stream[_pos].second;
        return symb;
    case TokenClass::EscapeChar:
        _pos++;
        symb->type = 1;
        symb->symbol = _stream[_pos].second;
        return symb;
    default:
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
        return nullptr;
    }
}

std::unique_ptr<Symb0AST> Parser::parseSymb0()
{
    std::unique_ptr<Symb0AST> symb0(new Symb0AST());
    switch (_stream[_pos].first) {
    case TokenClass::RightParenthesis:
    case TokenClass::RightBrace:
    case TokenClass::EOS:
        symb0->type = -1;
        return symb0;
    case TokenClass::Symbol:
    case TokenClass::EscapeChar:
        symb0->type = 0;
        symb0->symbs = parseSymbs();
        return symb0;
    default:
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
        return nullptr;
    }
}

std::unique_ptr<FracAST> Parser::parseFrac()
{
    std::unique_ptr<FracAST> frac(new FracAST());
    switch (_stream[_pos].first) {
    case TokenClass::EscapeChar:
        _pos += 3;
        frac->expr_numer = parseExpr();
        _pos += 2;
        frac->expr_denom = parseExpr();
        _pos++;
        return frac;
    default:
        printf("Unexpected token \"%s\" at token %d", _stream[_pos].second.c_str(), _pos);
        return nullptr;
    }
}
