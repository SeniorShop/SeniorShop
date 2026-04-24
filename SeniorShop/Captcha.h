#ifndef _CAPTCHA_H
#define _CAPTCHA_H
#include "Libraries.h"

class Captcha {
public:
    explicit Captcha(int length = 6);
    bool verify(int timeout_sec = 10);
    bool verify_with_retries(int max_attempts = 5, int timeout_sec = 10);
    std::string get_code() const;

private:
    int length_;
    std::string code;
    void generate();
};

#endif
