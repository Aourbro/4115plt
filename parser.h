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
 * Fact  -> "(" Expr ")" | Fact0
 * Fact0 -> Num Symb0 | Symbs
 * Num   -> Frac | Number
 * Symbs -> Symb Symb0              symb+
 * Symb  -> Symbol | "\" Symbol
 * Symb0 -> Symbs | e               symb*
 * Frac  -> "\" "frac" "{" Expr "}" "{" Expr "}"
 * 
 * Parsing table:
 *         Symbol        Number        "+" | "-"                "*" | "/"                 "\"                                    "}"   "("            ")"   $
 * Expr  | Term Exprs  | Term Exprs  |                        |                         | Term Exprs                           |     | Term Exprs   |     |   |
 * Exprs |             |             | ("+" | "-") Term Exprs |                         |                                      |  e  |              |  e  | e |
 * Term  | UExpr Terms | UExpr Terms |                        |                         | UExpr Terms                          |     | UExpr Terms  |     |   |
 * Terms |             |             |                        | ("*" | "/") UExpr Terms |                                      |  e  |              |  e  | e |
 * UExpr | Fact        | Fact        | ("+" | "-") Fact       |                         | Fact                                 |     | Fact         |     |   |
 * Fact  | Fact0       | Fact0       |                        |                         | Fact0                                |     | "(" Expr ")" |     |   |
 * Fact0 | Symbs       | Num Symb0   |                        |                         | Symbs                                |     |              |     |   |
 * Num   |             | Number      |                        |                         |                                      |     |              |     |   |
 * Symbs | Symb Symb0  |             |                        |                         | Symb Symb0                           |     |              |     |   |
 * Symb  | Symbol      |             |                        |                         | "\" Symbol                           |     |              |     |   |
 * Symb0 | Symbs       |             |                        |                         | Symbs                                |  e  |              |  e  | e |
 * Frac  |             |             |                        |                         | "\" "frac" "{" Expr "}" "{" Expr "}" |     |              |     |   |
 */

class Parser {
public:
    std::unique_ptr<BaseAST> getAST() {return std::move(_ast);};
    std::unique_ptr<BaseAST> parse(std::vector<std::pair<TokenClass, std::string> > stream);
private:
    std::unique_ptr<BaseAST> _ast {nullptr};
    std::vector<std::pair<TokenClass, std::string> > _stream;
    uint32_t _pos {0};
    std::unique_ptr<ExprAST> parseExpr();
    std::unique_ptr<ExprsAST> parseExprs();
    std::unique_ptr<TermAST> parseTerm();
    std::unique_ptr<TermsAST> parseTerms();
    std::unique_ptr<UExprAST> parseUExpr();
    std::unique_ptr<FactAST> parseFact();
    std::unique_ptr<Fact0AST> parseFact0();
    std::unique_ptr<NumAST> parseNum();
    std::unique_ptr<SymbsAST> parseSymbs();
    std::unique_ptr<SymbAST> parseSymb();
    std::unique_ptr<Symb0AST> parseSymb0();
    std::unique_ptr<FracAST> parseFrac();
};

#endif