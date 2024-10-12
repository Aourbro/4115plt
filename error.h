#ifndef PLT_ERROR_H
#define PLT_ERROR_H

#include <string>

enum Error : uint32_t {
    Success = 0,
    EmptyString,
    DigitAfterLetter,
    IllegalCharAfterEscape,
    BadSymbol,
};

inline std::string dumpError(Error err)
{
    switch (err){
    case Error::Success:
        return "No Error";
    case Error::EmptyString:
        return "Empty String";
    case Error::DigitAfterLetter:
        return "Digit After Letter";
    case Error::IllegalCharAfterEscape:
        return "Illegal Character After \'\\\'";
    case Error::BadSymbol:
        return "Bad Symbol";
    default:
        return "Unkown Error";
    }
}

#endif