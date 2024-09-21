#ifndef PLT_PARSER_H
#define PLT_PARSER_H

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include "error.h"

// fsm for parser
enum class State:uint32_t {
    Init,
    Number,
    Letter,
    Escape,
    Symbol,
};

enum class Character:uint32_t {
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
    PushStream,
    PushStreamAndThis,
    PushThis,
    ErrorHandle,
};
//fsm end

enum class TokenClass:uint32_t {
    Symbol,             // a b ... z alpha beta ... zeta
    Number,
    Keyword,            // 'frac'
    Operator,           // + - * / %
    EscapeChar,         // '\'
    LeftParenthesis,    // (
    RightParenthesis,   // )
    LeftBrace,          // {
    RightBrace,         // }

    UnknownClass,
};

class Parser {
public:
    int tokenize(const std::string &iString);
    int tokenize(std::ifstream &iFile);
    void printTokens();
private:
    std::string _curToken = "";
#define PAIR(x, y) (uint32_t(x) << 16 | uint32_t(y))
#define PFIRST(x) ((uint32_t(x) >> 16) & 0x0000ffff)
#define PSECOND(x) (uint32_t(x) & 0x0000ffff)
    const std::unordered_map<uint32_t, uint32_t> _stateTrans {
        {PAIR(State::Init, Character::Digit), PAIR(State::Number, Action::DoNothing)},
        {PAIR(State::Init, Character::Letter), PAIR(State::Letter, Action::PushThis)},
        {PAIR(State::Init, Character::EscapeChar), PAIR(State::Escape, Action::PushThis)},
        {PAIR(State::Init, Character::WhiteSpace), PAIR(State::Init, Action::DoNothing)},
        {PAIR(State::Init, Character::Operator), PAIR(State::Init, Action::PushThis)},
        {PAIR(State::Init, Character::Bracket), PAIR(State::Init, Action::PushThis)},

        {PAIR(State::Number, Character::Digit), PAIR(State::Number, Action::DoNothing)},
        {PAIR(State::Number, Character::Letter), PAIR(State::Letter, Action::PushStream)},
        {PAIR(State::Number, Character::EscapeChar), PAIR(State::Escape, Action::PushStreamAndThis)},
        {PAIR(State::Number, Character::WhiteSpace), PAIR(State::Init, Action::PushStream)},
        {PAIR(State::Number, Character::Operator), PAIR(State::Init, Action::PushStreamAndThis)},
        {PAIR(State::Number, Character::Bracket), PAIR(State::Init, Action::PushStreamAndThis)},

        {PAIR(State::Letter, Character::Digit), PAIR(Error::DigitAfterLetter, Action::ErrorHandle)},
        {PAIR(State::Letter, Character::Letter), PAIR(State::Letter, Action::PushThis)},
        {PAIR(State::Letter, Character::EscapeChar), PAIR(State::Escape, Action::PushThis)},
        {PAIR(State::Letter, Character::WhiteSpace), PAIR(State::Init, Action::DoNothing)},
        {PAIR(State::Letter, Character::Operator), PAIR(State::Init, Action::PushThis)},
        {PAIR(State::Letter, Character::Bracket), PAIR(State::Init, Action::PushThis)},

        {PAIR(State::Escape, Character::Digit), PAIR(Error::IllegalCharAfterEscape, Action::ErrorHandle)},
        {PAIR(State::Escape, Character::Letter), PAIR(State::Symbol, Action::DoNothing)},
        {PAIR(State::Escape, Character::EscapeChar), PAIR(Error::IllegalCharAfterEscape, Action::ErrorHandle)},
        {PAIR(State::Escape, Character::WhiteSpace), PAIR(Error::IllegalCharAfterEscape, Action::ErrorHandle)},
        {PAIR(State::Escape, Character::Operator), PAIR(Error::IllegalCharAfterEscape, Action::ErrorHandle)},
        {PAIR(State::Escape, Character::Bracket), PAIR(Error::IllegalCharAfterEscape, Action::ErrorHandle)},

        {PAIR(State::Symbol, Character::Digit), PAIR(Error::DigitAfterLetter, Action::ErrorHandle)},
        {PAIR(State::Symbol, Character::Letter), PAIR(State::Symbol, Action::DoNothing)},
        {PAIR(State::Symbol, Character::EscapeChar), PAIR(State::Escape, Action::PushStreamAndThis)},
        {PAIR(State::Symbol, Character::WhiteSpace), PAIR(State::Init, Action::PushStream)},
        {PAIR(State::Symbol, Character::Operator), PAIR(State::Init, Action::PushStreamAndThis)},
        {PAIR(State::Symbol, Character::Bracket), PAIR(State::Init, Action::PushStreamAndThis)},
    };

    const std::unordered_set<std::string> _keywordPool {
        "frac",
    };

    const std::unordered_set<std::string> _symbolPool {
        "alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta", "theta", "lota", "kappa", "lambda", "mu", "nu",
        "xi", "omicron", "pi", "rho", "sigma", "tau", "upsilon", "phi", "chi", "psi", "omega",
    };

    std::vector<std::pair<TokenClass, std::string> > _tokenStream {};
    std::vector<std::string> _badSymbol {};

    const Character classifyChar(char c) {
        if (c >= '0' && c <= '9') {
            return Character::Digit;
        }
        if (c >= 'a' && c <= 'z') {
            return Character::Letter;
        }
        if (c == '\\') {
            return Character::EscapeChar;
        }
        if (c == ' ' || c == '\0' || c == '\t' || c == '\n') {
            return Character::WhiteSpace;
        }
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
            return Character::Operator;
        }
        if (c == '{' || c == '}' || c == '(' || c == ')') {
            return Character::Bracket;
        }
        return Character::IllegalChar;
    }

    const TokenClass tokenizeChar(char c) {
        if (c >= 'a' && c <= 'z') {
            return TokenClass::Symbol;
        }
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
            return TokenClass::Operator;
        }
        switch (c) {
        case '\\':
            return TokenClass::EscapeChar;
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

    void pushStream(State curState);
};

#endif