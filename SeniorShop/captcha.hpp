#ifndef _CAPTCHA_HPP_
#define _CAPTCHA_HPP_

#include <string>
#include <iostream>
#include <future>
#include <chrono>
#include <random>
#include <conio.h>
#include <thread>

class Captcha {
public:
    explicit Captcha(int length = 6);
    bool verify(int timeout_sec = 10);
    bool verifyWithRetries(int maxAttempts = 5, int timeout_sec = 10);

    std::string getCode() const;

private:
    int length_;
    std::string code_;

    void generate_();
};

#endif