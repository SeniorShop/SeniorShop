#ifndef CAPTCHA_H
#define CAPTCHA_H

#include <string>

class Captcha {
    std::string code;
    void generate();
public:
    bool verify();
};

#endif
