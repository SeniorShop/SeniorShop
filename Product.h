#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <chrono>
#include <type_traits>

inline bool is_valid_int(const std::string& str) {
    if (str.empty()) return false;
    size_t start = (str[0] == '-') ? 1 : 0;
    if (start >= str.length()) return false;
    for (size_t i = start; i < str.length(); ++i) {
        if (!std::isdigit(str[i])) return false;
    }
    return true;
}

inline bool is_valid_unsigned_int(const std::string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

inline bool is_valid_double(const std::string& str) {
    if (str.empty()) return false;
    bool has_dot = false;
    size_t start = (str[0] == '-') ? 1 : 0;
    if (start >= str.length()) return false;
    for (size_t i = start; i < str.length(); ++i) {
        if (str[i] == '.') {
            if (has_dot) return false;
            has_dot = true;
        } else if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

inline bool is_valid_date(const std::string& str) {
    if (str.length() != 10) return false;
    if (str[2] != '.' || str[5] != '.') return false;

    int day, month, year;
    char dot1, dot2;
    std::stringstream ss(str);
    ss >> day >> dot1 >> month >> dot2 >> year;

    if (ss.fail()) return false;
    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year < 1900 || year > 2099) return false;

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }

    if (month == 2) {
        bool is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (is_leap ? 29 : 28)) return false;
    }

    return true;
}

inline bool is_valid_bool(const std::string& str) {
    return str == "y" || str == "Y" || str == "n" || str == "N" ||
           str == "yes" || str == "Yes" || str == "no" || str == "No" ||
           str == "1" || str == "0";
}

template<typename T>
void Getline(T& value, bool allow_empty = false) {
    std::string input;
    while (true) {
        std::getline(std::cin, input, '\n');
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(365000, '\n');
            continue;
        }

        if (!allow_empty && input.empty()) {
            std::cerr << "Ошибка: поле не может быть пустым\n";
            continue;
        }

        if constexpr (std::is_same_v<T, int>) {
            if (!is_valid_int(input)) {
                std::cerr << "Ошибка: введите целое число\n";
                continue;
            }
            std::stringstream ss(input);
            ss >> value;
            return;
        }
        else if constexpr (std::is_same_v<T, unsigned int>) {
            if (!is_valid_unsigned_int(input)) {
                std::cerr << "Ошибка: введите неотрицательное целое число\n";
                continue;
            }
            std::stringstream ss(input);
            ss >> value;
            return;
        }
        else if constexpr (std::is_same_v<T, double>) {
            if (!is_valid_double(input)) {
                std::cerr << "Ошибка: введите число\n";
                continue;
            }
            std::stringstream ss(input);
            ss >> value;
            return;
        }
        else if constexpr (std::is_same_v<T, bool>) {
            if (!is_valid_bool(input)) {
                std::cerr << "Ошибка: введите y/n\n";
                continue;
            }
            value = (input == "y" || input == "Y" || input == "yes" || input == "Yes" || input == "1");
            return;
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            value = input;
            return;
        }
        else if constexpr (std::is_same_v<T, std::chrono::sys_days>) {
            if (!is_valid_date(input)) {
                std::cerr << "Ошибка: введите дату в формате (дата.месяц.год)\n";
                continue;
            }
            int day, month, year;
            char dot1, dot2;
            std::stringstream ss(input);
            ss >> day >> dot1 >> month >> dot2 >> year;
            value = std::chrono::sys_days{
                std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day)
            };
            return;
        }
        else {
            static_assert(sizeof(T) == 0, "Getline: неподдерживаемый тип");
        }
    }
}

struct Product {
    std::string name;
    std::string category;
    double price;
    int article;
    std::string begin_date;
    std::string end_date;
    unsigned int count;
    std::string manufacturer;
    std::string country;

    Product();
    Product(const Product& other);
    bool operator==(const Product& other) const;
};

#endif
