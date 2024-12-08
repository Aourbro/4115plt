#ifndef PLT_PARSER_H
#define PLT_PARSER_H

#include <memory>
#include "AST.h"
#include "lexer.h"

/* LL(1) Grammar for parser:
 *
 * Expr  -> Term Exprs
 * Exprs -> ("+" | "-") Term Exprs | e
 * Term  -> UExpr Terms
 * Terms -> ("*" | "/") UExpr Terms | e
 * UExpr -> ("+" | "-") Fact | Fact
 * Fact  -> "(" Expr ")" | Num Symb0 | Symbs
 * Num   -> Frac | number
 * Symbs -> symbol Symb0
 * Symb0 -> Symbs | e
 * Frac  -> "\frac" "{" Expr "}" "{" Expr "}"
 * 
 * Parsing table:
 *         Symbol        Number        "+" | "-"                "*" | "/"                 "\frac"                             "}"   "("            ")"   $
 * Expr  | Term Exprs  | Term Exprs  | Term Exprs             |                         | Term Exprs                        |     | Term Exprs   |     |   |
 * Exprs |             |             | ("+" | "-") Term Exprs |                         |                                   |  e  |              |  e  | e |
 * Term  | UExpr Terms | UExpr Terms | UExpr Terms            |                         | UExpr Terms                       |     | UExpr Terms  |     |   |
 * Terms |             |             | e                      | ("*" | "/") UExpr Terms |                                   |  e  |              |  e  | e |
 * UExpr | Fact        | Fact        | ("+" | "-") Fact       |                         | Fact                              |     | Fact         |     |   |
 * Fact  | Symbs       | Num Symb0   |                        |                         | Num Symb0                         |     | "(" Expr ")" |     |   |
 * Num   |             | number      |                        |                         | Frac                              |     |              |     |   |
 * Symbs | Symb Symb0  |             |                        |                         |                                   |     |              |     |   |
 * Symb0 | Symbs       |             | e                      | e                       |                                   |  e  |              |  e  | e |
 * Frac  |             |             |                        |                         | "\frac" "{" Expr "}" "{" Expr "}" |     |              |     |   |
 */

class Parser {
public:
    bool getSuccess() {return _success;};
    std::unique_ptr<BaseAST> parse(std::vector<std::pair<TokenClass, std::string> > stream);
private:
    std::unique_ptr<BaseAST> _ast {nullptr};
    std::vector<std::pair<TokenClass, std::string> > _stream;
    uint32_t _pos {0};
    bool _success {true};
    std::unique_ptr<ExprAST> parseExpr();
    std::unique_ptr<ExprsAST> parseExprs();
    std::unique_ptr<TermAST> parseTerm();
    std::unique_ptr<TermsAST> parseTerms();
    std::unique_ptr<UExprAST> parseUExpr();
    std::unique_ptr<FactAST> parseFact();
    std::unique_ptr<NumAST> parseNum();
    std::unique_ptr<SymbsAST> parseSymbs();
    std::unique_ptr<Symb0AST> parseSymb0();
    std::unique_ptr<FracAST> parseFrac();
};

#endif