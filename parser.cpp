#include <cassert>
#include "parser.h"
#include "error.h"

int Parser::tokenize(const std::string &iString)
{
    int len = iString.size();
    if (iString.size() == 0) {
        return Error::EmptyString;
    }

    State curState = State::Init;

    for (int i = 0; i < len; ++i) {
        Character ch = classifyChar(iString[i]);
        if (ch == Character::IllegalChar) {
            return Error::BadSymbol;
        }
        auto it =  _stateTrans.find(PAIR(curState, ch));
        assert("Error: no state transform info" && it != _stateTrans.end());
        State nxtState = State(PFIRST(it->second));
        Action action = Action(PSECOND(it->second));
        switch (action) {
        case Action::DoNothing:
            if (ch != Character::WhiteSpace) {
                _curToken.push_back(iString[i]);
            }
            break;
        case Action::PushStream:
            pushStream(curState);
            break;
        case Action::PushStreamAndThis:
            pushStream(curState);
            [[fallthrough]];
        case Action::PushThis:
            _curToken.push_back(iString[i]);
            _tokenStream.push_back(std::make_pair(tokenizeChar(iString[i]), _curToken));
            _curToken.clear();
            break;
        default:
            assert("Error: this branch is unavailable" && false);
            break;
        }
        curState = nxtState;
    }

    if (!_badSymbol.empty()) {
        return Error::BadSymbol;
    }

    return 0;
}

int Parser::tokenize(const std::ifstream &iFile)
{
    return 0;
}

void Parser::pushStream(State curState)
{
    if (curState == State::Number) {
        _tokenStream.push_back(std::make_pair(TokenClass::Number, _curToken));
    } else if (curState == State::Symbol) {
        if (_keywordPool.find(_curToken) != _keywordPool.end()) {
            _tokenStream.push_back(std::make_pair(TokenClass::Keyword, _curToken));
        } else if (_symbolPool.find(_curToken) != _symbolPool.end()) {
            _tokenStream.push_back(std::make_pair(TokenClass::Symbol, _curToken));
        } else {
            _badSymbol.push_back(_curToken);
        }
    } else {
        assert("Error: this branch is unavailable" && false);
    }
    _curToken.clear();
}

void Parser::printTokens()
{
    int size = _tokenStream.size();
    printf("total %d tokens:\n", size);

    for (int i = 0; i < size; ++i) {
        printf("\t<%u, %s>\n", uint32_t(_tokenStream[i].first), _tokenStream[i].second.c_str());
    }
}