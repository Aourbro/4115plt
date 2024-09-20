#ifndef PLT_ERROR_H
#define PLT_ERROR_H

enum Error:uint32_t {
    Success = 0,
    EmptyString,
    DigitAfterLetter,
    IllegalCharAfterEscape,
    BadSymbol,
};

#endif