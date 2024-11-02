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

}

std::unique_ptr<TermsAST> Parser::parseTerms()
{

}

std::unique_ptr<UExprAST> Parser::parseUExpr()
{

}

std::unique_ptr<FactAST> Parser::parseFact()
{

}

std::unique_ptr<Fact0AST> Parser::parseFact0()
{

}

std::unique_ptr<NumAST> Parser::parseNum()
{

}

std::unique_ptr<SymbsAST> Parser::parseSymbs()
{

}

std::unique_ptr<SymbAST> Parser::parseSymb()
{

}

std::unique_ptr<Symb0AST> Parser::parseSymb0()
{

}

std::unique_ptr<ExprAST> Parser::parseFrac()
{

}
