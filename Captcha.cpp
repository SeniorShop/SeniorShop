#include "Captcha.h"
#include "Product.h"
#include <iostream>
#include <random>

void Captcha::generate() {
    const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, charset.size() - 1);

    code.clear();
    for (int i = 0; i < 5; ++i) {
        code.push_back(charset[dist(gen)]);
    }
}

bool Captcha::verify() {
    for(auto i = 0; i < 5; ++i) {
        if(i >= 5) return false;
        generate();
        std::cout << "Введите код: " << code << std::endl;
        std::string user_input;
        Getline(user_input);

        if(user_input == code)
            break;
    }

    return true;
}
