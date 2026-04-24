#include "Captcha.h"

void Captcha::generate() {

    std::string charset =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, charset.size() - 1);

    code.clear();
    for (int i = 0; i < charset.size() - 1; ++i)
        code.push_back(charset[dist(gen)]);

}

bool Captcha::verify() {
    std::size_t attempt = 0;
    std::string user_input;

    while(attempt < 5) {
        if(attempt >= 5) return false;
        generate();
        std::cout << "Введите код: " << code << "\n";
        std::cin >> user_input;
        if(user_input != code)
            std::cerr << "Неверный код. Повторите попытку\n";
        else
            break;
        attempt++;
    }

    return true;
}



