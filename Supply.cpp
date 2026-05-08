#include "Supply.h"
#include <format>
#include <sstream>

Supply::Supply()
    : number_supply(0)
    , is_actually(true)
    , status("Ожидается") {}

std::string Supply::date_to_string(std::chrono::sys_days tp) {
    return std::format("{:%d.%m.%Y}", tp);
}

std::chrono::sys_days Supply::string_to_date(const std::string& str) {
    int day, month, year;
    char dot1, dot2;
    std::stringstream ss(str);
    ss >> day >> dot1 >> month >> dot2 >> year;

    if (ss.fail()) {
        return std::chrono::sys_days{};
    }

    return std::chrono::sys_days{
        std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day)
    };
}

void Supply::print() const {
    std::cout << "Поставка под номером: " << number_supply << "\n";
    std::cout << "От кого: " << name_user << "\n";
    std::cout << "Дата выгрузки: " << date_to_string(date) << "\n";
    std::cout << "Дата принятия: " << date_to_string(date_acception) << "\n";
    std::cout << "Дата обработки: " << date_to_string(date_processing) << "\n";
    std::cout << "Кто взял: " << object_user.get_name()
              << " (" << object_user.get_status() << ")\n";
    std::cout << "Товар: " << product_name.name
              << " (арт. " << product_name.article << ")\n";
    std::cout << "Актуальна: " << (is_actually ? "true" : "false") << "\n";
    std::cout << "Статус: " << status << "\n";
}
