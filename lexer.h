#ifndef PLT_LEXER_H
#define PLT_LEXER_H

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include "error.h"

// fsm for lexer
enum class State:uint32_t {
    Init,
    Number,
    Letter,
    Escape,
    Symbol,
};

enum class CharacterType:uint32_t {
    Digit,
    Letter,
    EscapeChar,
    WhiteSpace,
    Operator,
    Bracket,

    IllegalChar,
};

enum class Action:uint32_t {
    DoNothing,
    AddToken,
    PushToken,
    PushTokenAndThis,
    PushThis,
    ErrorHandle,
};
//fsm end

enum class TokenClass:uint32_t {
    Symbol,             // a b ... z alpha beta ... zeta
    Number,
    Keyword,            // 'frac'
    Operator,           // + - * /
    LeftParenthesis,    // (
    RightParenthesis,   // )
    LeftBrace,          // {
    RightBrace,         // }
    EOS,                // $, end of stream

    UnknownClass,
};

class Lexer {
public:
    int tokenize(const std::string &iString);
    int tokenize(std::ifstream &iFile);
    std::vector<std::pair<TokenClass, std::string> > getStream () {return _tokenStream;}
    void printTokens();
private:
    uint32_t _pos {0};
    std::string _curToken {""};
#define PAIR(x, y) (uint32_t(x) << 16 | uint32_t(y))
#define PFIRST(x) ((uint32_t(x) >> 16) & 0x0000ffff)
#define PSECOND(x) (uint32_t(x) & 0x0000ffff)
    const std::unordered_map<uint32_t, uint32_t> _stateTrans {
        {PAIR(State::Init, CharacterType::Digit), PAIR(State::Number, Action::AddToken)},
        {PAIR(State::Init, CharacterType::Letter), PAIR(State::Letter, Action::PushThis)},
        {PAIR(State::Init, CharacterType::EscapeChar), PAIR(State::Escape, Action::AddToken)},
        {PAIR(State::Init, CharacterType::WhiteSpace), PAIR(State::Init, Action::DoNothing)},
        {PAIR(State::Init, CharacterType::Operator), PAIR(State::Init, Action::PushThis)},
        {PAIR(State::Init, CharacterType::Bracket), PAIR(State::Init, Action::PushThis)},

        {PAIR(State::Number, CharacterType::Digit), PAIR(State::Number, Action::AddToken)},
        {PAIR(State::Number, CharacterType::Letter), PAIR(State::Letter, Action::PushToken)},
        {PAIR(State::Number, CharacterType::EscapeChar), PAIR(State::Escape, Action::PushToken)},
        {PAIR(State::Number, CharacterType::WhiteSpace), PAIR(State::Init, Action::PushToken)},
        {PAIR(State::Number, CharacterType::Operator), PAIR(State::Init, Action::PushTokenAndThis)},
        {PAIR(State::Number, CharacterType::Bracket), PAIR(State::Init, Action::PushTokenAndThis)},

        {PAIR(State::Letter, CharacterType::Digit), PAIR(Error::DigitAfterLetter, Action::ErrorHandle)},
        {PAIR(State::Letter, CharacterType::Letter), PAIR(State::Letter, Action::PushThis)},
        {PAIR(State::Letter, CharacterType::EscapeChar), PAIR(State::Escape, Action::AddToken)},
        {PAIR(State::Letter, CharacterType::WhiteSpace), PAIR(State::Init, Action::DoNothing)},
        {PAIR(State::Letter, CharacterType::Operator), PAIR(State::Init, Action::PushThis)},
        {PAIR(State::Letter, CharacterType::Bracket), PAIR(State::Init, Action::PushThis)},

        {PAIR(State::Escape, CharacterType::Digit), PAIR(Error::IllegalCharAfterEscape, Action::ErrorHandle)},
        {PAIR(State::Escape, CharacterType::Letter), PAIR(State::Symbol, Action::AddToken)},
        {PAIR(State::Escape, CharacterType::EscapeChar), PAIR(Error::IllegalCharAfterEscape, Action::ErrorHandle)},
        {PAIR(State::Escape, CharacterType::WhiteSpace), PAIR(Error::IllegalCharAfterEscape, Action::ErrorHandle)},
        {PAIR(State::Escape, CharacterType::Operator), PAIR(Error::IllegalCharAfterEscape, Action::ErrorHandle)},
        {PAIR(State::Escape, CharacterType::Bracket), PAIR(Error::IllegalCharAfterEscape, Action::ErrorHandle)},

        {PAIR(State::Symbol, CharacterType::Digit), PAIR(Error::DigitAfterLetter, Action::ErrorHandle)},
        {PAIR(State::Symbol, CharacterType::Letter), PAIR(State::Symbol, Action::AddToken)},
        {PAIR(State::Symbol, CharacterType::EscapeChar), PAIR(State::Escape, Action::PushToken)},
        {PAIR(State::Symbol, CharacterType::WhiteSpace), PAIR(State::Init, Action::PushToken)},
        {PAIR(State::Symbol, CharacterType::Operator), PAIR(State::Init, Action::PushTokenAndThis)},
        {PAIR(State::Symbol, CharacterType::Bracket), PAIR(State::Init, Action::PushTokenAndThis)},
    };

    const std::unordered_set<std::string> _keywordPool {
        "\\frac",
    };

    const std::unordered_set<std::string> _symbolPool {
        "\\alpha", "\\beta", "\\gamma", "\\delta", "\\epsilon", "\\zeta", "\\eta", "\\theta", "\\lota", "\\kappa",
        "\\lambda", "\\mu", "\\nu", "\\xi", "\\omicron", "\\pi", "\\rho", "\\sigma", "\\tau", "\\upsilon", "\\phi",
        "\\chi", "\\psi", "\\omega",
    };  // 24

    std::vector<std::pair<TokenClass, std::string> > _tokenStream {};
    std::vector<std::pair<std::string, uint32_t> > _badSymbol {};

    const CharacterType classifyChar(char c)
    {
        if (c >= '0' && c <= '9') {
            return CharacterType::Digit;
        }
        if (c >= 'a' && c <= 'z') {
            return CharacterType::Letter;
        }
        if (c == '\\') {
            return CharacterType::EscapeChar;
        }
        if (c == ' ' || c == '\0' || c == '\t' || c == '\n') {
            return CharacterType::WhiteSpace;
        }
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            return CharacterType::Operator;
        }
        if (c == '{' || c == '}' || c == '(' || c == ')') {
            return CharacterType::Bracket;
        }
        return CharacterType::IllegalChar;
    }

    const TokenClass tokenizeChar(char c)
    {
        if (c >= 'a' && c <= 'z') {
            return TokenClass::Symbol;
        }
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            return TokenClass::Operator;
        }
        switch (c) {
        case '{':
            return TokenClass::LeftBrace;
        case '(':
            return TokenClass::LeftParenthesis;
        case '}':
            return TokenClass::RightBrace;
        case ')':
            return TokenClass::RightParenthesis;
        default:
            return TokenClass::UnknownClass;
        }
    }

    State readChar(char c, State curState);
    inline void pushToken(State curState);
    inline void pushThis(char c);
    int postTokenize(State curState);

    inline std::string token2String(TokenClass token)
    {
        switch (token){
        case TokenClass::Symbol:
            return "Symbol";
        case TokenClass::Number:
            return "Number";
        case TokenClass::Keyword:
            return "Keyword";
        case TokenClass::Operator:
            return "Operator";
        case TokenClass::LeftParenthesis:
            return "Left Parenthesis";
        case TokenClass::RightParenthesis:
            return "Right Parenthesis";
        case TokenClass::LeftBrace:
            return "Left Brace";
        case TokenClass::RightBrace:
            return "Right Brace";
        default:
            return "Unkown Token Class";
        }
    }
};

#endif