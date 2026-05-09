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
    for (int attempt = 0; attempt < 5; ++attempt) {
        generate();
        std::cout << "Введите капчу: " << code << "\n";
        std::string user_input;
        Getline(user_input);
        if (user_input == code) return true;
        std::cerr << "Неверный код. Осталось попыток: " << (4 - attempt) << "\n";
    }
    return false;
}
