#include "captcha.hpp"


Captcha::Captcha(int length) : length_(length) {
    generate_();
}

void Captcha::generate_() {
    static const std::string charset =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, charset.size() - 1);

    code_.clear();
    for (int i = 0; i < length_; ++i)
        code_ += charset[dist(gen)];
}

bool Captcha::verify(int timeout_sec) {
    generate_();

    std::cout << "Введите код: " << code_ << std::endl;
    std::cout << "Ожидание ввода (" << timeout_sec << " секунд)..." << std::endl;

    auto start_time = std::chrono::steady_clock::now();
    std::string userInput;
    bool input_started = false;

    while (true) {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - start_time
        ).count();

        if (elapsed >= timeout_sec) {
            std::cout << "\nКапча не введена (время истекло). Генерируется новая..." << std::endl;
            generate_();
            return false;
        }

        if (_kbhit()) {
            input_started = true;
            int ch = _getch();
            
            if (ch == '\r' || ch == '\n') {
                std::cout << std::endl;
                break;
            }
            else if (ch == '\b') {
                if (!userInput.empty()) {
                    userInput.pop_back();
                    std::cout << "\b \b";
                }
            }
            else if (ch >= 32 && ch < 127) {
                userInput += ch;
                std::cout << (char)ch;
                std::cout.flush();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (userInput == code_) {
        std::cout << "Доступ разрешен." << std::endl;
        return true;
    } else {
        std::cout << "Неверный код. Генерируется новая..." << std::endl;
        generate_();
        return false;
    }
}

bool Captcha::verifyWithRetries(int maxAttempts, int timeout_sec) {

    for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
        std::cout << "Попытка " << attempt << " из " << maxAttempts << std::endl;
        if (verify(timeout_sec)) {
            return true;
        }
    }
    std::cout << "Исчерпаны все попытки. Программа закрывается." << std::endl;
    return false;

}

std::string Captcha::getCode() const {
    return code_;
}