#include <cassert>
#include "lexer.h"
#include "error.h"

int Lexer::tokenize(const std::string &iString)
{
    uint32_t len = iString.size();
    State curState = State::Init;

    for (; _pos < len; ++_pos) {
        char c = iString[_pos];
        curState = readChar(c, curState);
    }

    return postTokenize(curState);
}

int Lexer::tokenize(std::ifstream &iFile)
{
    assert("Error: file not open" && iFile.is_open());

    char c;
    State curState = State::Init;

    while (!iFile.eof()) {
        iFile.get(c);
        if (iFile.fail()) {
            break;
        }
        curState = readChar(c, curState);
        _pos++;
    }

    return postTokenize(curState);
}

State Lexer::readChar(char c, State curState)
{
    CharacterType charType = classifyChar(c);
    if (charType == CharacterType::IllegalChar) {
        printf("illegal char at position %d: %c\n", _pos, c);
        pushToken(curState);
        return State::Init;
    }
    auto it =  _stateTrans.find(PAIR(curState, charType));
    assert("Error: no state transform info" && it != _stateTrans.end());
    State nxtState = State(PFIRST(it->second));
    Action action = Action(PSECOND(it->second));
    switch (action) {
    case Action::DoNothing:
        break;
    case Action::AddToken:
        _curToken.push_back(c);
        break;
    case Action::PushToken:
        pushToken(curState);
        break;
    case Action::PushTokenAndThis:
        pushToken(curState);
        [[fallthrough]];
    case Action::PushThis:
        pushThis(c);
        break;
    case Action::ErrorHandle:
        printf("Error: %s at position %d: \'%c\'\n", dumpError(Error(nxtState)).c_str(), _pos, c);
        return State::Init;
    default:
        assert("Error: this branch is unavailable" && false);
    }
    return nxtState;
}

int Lexer::postTokenize(State curState)
{
    if (_pos == 0) {
        return Error::EmptyString;
    }

    if (!_badSymbol.empty()) {
        printf("Bad Symbols:\n");
        int len = _badSymbol.size();
        for (int i = 0; i < len; ++i) {
            printf("  %s at position %d\n", _badSymbol[i].first.c_str(), _badSymbol[i].second);
        }
        return Error::BadSymbol;
    }

    // in case there is no whitespace at the end of the string
    if (curState == State::Number || curState == State::Symbol) {
        pushToken(curState);
    }

    return 0;
}

inline void Lexer::pushToken(State curState)
{
    if (curState == State::Number) {
        _tokenStream.push_back(std::make_pair(TokenClass::Number, _curToken));
    } else if (curState == State::Symbol) {
        if (_keywordPool.find(_curToken) != _keywordPool.end()) {
            _tokenStream.push_back(std::make_pair(TokenClass::Keyword, _curToken));
        } else if (_symbolPool.find(_curToken) != _symbolPool.end()) {
            _tokenStream.push_back(std::make_pair(TokenClass::Symbol, _curToken));
        } else {
            _badSymbol.push_back(std::make_pair(_curToken, _pos - _curToken.length()));
        }
    } else {
        assert("Error: this branch is unavailable" && false);
    }
    _curToken.clear();
}

inline void Lexer::pushThis(char c)
{
    _curToken.push_back(c);
    _tokenStream.push_back(std::make_pair(tokenizeChar(c), _curToken));
    _curToken.clear();
}

void Lexer::printTokens()
{
    int size = _tokenStream.size();
    printf("%% total %d tokens:\n", size);

    for (int i = 0; i < size; ++i) {
        printf("%%  <%s, %s>\n", token2String(_tokenStream[i].first).c_str(), _tokenStream[i].second.c_str());
    }
}