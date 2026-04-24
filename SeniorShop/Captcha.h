#ifndef _CAPTCHA_H
#define _CAPTCHA_H
#include "Libraries.h"

class Captcha {
    std::string code;
    void generate();
public:
    bool verify();
};

#endif
