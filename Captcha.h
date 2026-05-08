#ifndef _CAPTCHA_H
#define _CAPTCHA_H
#include <iostream>
#include <string>
#include <random>

class Captcha {
    std::string code;
    void generate();
public:
    bool verify();
};

#endif
