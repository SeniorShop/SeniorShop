#include "../include/Supply.h"
#include <format>
#include <sstream>

Supply::Supply()
    : number_supply(0)
    , is_actually(true)
    , status("Ожидается") {
}

std::string Supply::date_to_string(std::chrono::sys_days tp) {
    return std::format("{:%d.%m.%Y}", tp);
}

std::chrono::sys_days Supply::string_to_date(const std::string& str) {
    int day, month, year;
    char dot1, dot2;
    std::stringstream ss(str);
    ss >> day >> dot1 >> month >> dot2 >> year;
    if (ss.fail()) return std::chrono::sys_days{};
    return std::chrono::sys_days{
        std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day)
    };
}

void Supply::print() const {
    std::cout << "\n========== ПОСТАВКА #" << number_supply << " ==========\n";
    std::cout << std::left
              << std::setw(20) << "Поставщик: " << name_user << "\n"
              << std::setw(20) << "Ответственный: " << responsible_person << "\n"
              << std::setw(20) << "Товар: " << product_name.name << "\n"
              << std::setw(20) << "Категория: " << product_name.category << "\n"
              << std::setw(20) << "Цена: " << std::fixed << std::setprecision(2) << product_name.price << " руб.\n"
              << std::setw(20) << "Артикль: " << product_name.article << "\n"
              << std::setw(20) << "Количество: " << product_name.count << " шт.\n"
              << std::setw(20) << "Страна: " << product_name.country << "\n"
              << std::setw(20) << "Начало срока: " << product_name.begin_date << "\n"
              << std::setw(20) << "Конец срока: " << product_name.end_date << "\n"
              << std::setw(20) << "Актуальна: " << (is_actually ? "Да" : "Нет") << "\n"
              << std::setw(20) << "Статус: " << status << "\n";
    std::cout << "==================================================\n";
}
