#include "Captcha.h"
#include "Product.h"
 

void Captcha::generate() {

    std::string charset =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<std::size_t> dist(0, charset.size() - 1);

    code.clear();
    for (int i = 0; i < 5; ++i)
        code.push_back(charset[dist(gen)]);

}

bool Captcha::verify() {
 
    std::string chooseCapha;
    for (int attemp = 0; attemp < 5; ++attemp)
    {
        generate();
        std::cout << "Введите капчу: " << code << "\n";
        Getline(chooseCapha);
        if (chooseCapha == code) return true;
        std::cerr << "Ошибка! Неверный код. Осталось попыток: " << (4 - attemp) << "\n";


    }
    return false;
}



