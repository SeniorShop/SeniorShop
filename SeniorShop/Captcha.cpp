#include "Captcha.h"

Captcha::Captcha(int length) : length_(length) {
    generate();
}

void Captcha::generate() {
    std::string charset =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, charset.size() - 1);

    code.clear();
    for (int i = 0; i < length_; ++i)
        code += charset[dist(gen)];
}

bool Captcha::verify(int timeout_sec) {
    generate();

    std::cout << "Введите код: " << code << std::endl;
    std::cout << "Ожидание ввода (" << timeout_sec << " секунд)" << std::endl;

    auto start_time = std::chrono::steady_clock::now();
    std::string user_input;
    bool input_started = false;

    while (true) {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - start_time
        ).count();

        if (elapsed >= timeout_sec) {
            std::cerr << "Капча не введена (время истекло). Генерируется новая" << std::endl;
            generate();
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (user_input == code) {
        std::cout << "Доступ разрешен." << std::endl;
        return true;
    } else {
        std::cout << "Неверный код. Генерируется новая" << std::endl;
        generate();
        return false;
    }
}

bool Captcha::verify_with_retries(int max_attempts, int timeout_sec) {
    for (int attempt = 1; attempt <= max_attempts; ++attempt) {
        std::cout << "Попытка " << attempt << " из " << max_attempts << std::endl;
        if (verify(timeout_sec)) {
            return true;
        }
    }
    std::cerr << "Исчерпаны все попытки. Программа закрывается." << std::endl;
    return false;

}

std::string Captcha::get_code() const {
    return code;
}
