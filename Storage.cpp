#include "Storage.h"
#include "SupplyManager.h"
#include "AuthSystemUser.h"
#include <iomanip>
#include <algorithm>
#include <unordered_set>
#include <fstream>

void Storage::load_from_file(const std::string& filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Ошибка открытия файла " << filename << std::endl;
        return;
    }
    goods.clear();
    Product product;
    while (input >> product.name >> product.category >> product.price
           >> product.article >> product.begin_date >> product.end_date
           >> product.count >> product.manufacturer >> product.country) {
        goods.push_back(product);
    }
    input.close();
    std::cout << "Загружено товаров: " << goods.size() << std::endl;
}

void Storage::save_to_file(const std::string& filename) {
    std::ofstream output(filename);
    if (!output.is_open()) {
        std::cerr << "Ошибка сохранения в файл " << filename << std::endl;
        return;
    }
    for (const auto& p : goods) {
        output << p.name << " " << p.category << " " << p.price << " "
               << p.article << " " << p.begin_date << " " << p.end_date << " "
               << p.count << " " << p.manufacturer << " " << p.country << "\n";
    }
    output.close();
    std::cout << "Сохранено товаров: " << goods.size() << std::endl;
}

void Storage::add_product() {
    Product p;
    std::cout << "Название: ";
    Getline(p.name, false);
    std::cout << "Категория: ";
    Getline(p.category, false);
    std::cout << "Цена: ";
    Getline(p.price);
    std::cout << "Артикль: ";
    Getline(p.article);

    bool dates_valid = false;
    while (!dates_valid) {
        std::cout << "Начало срока годности (день.месяц.год): ";
        Getline(p.begin_date);
        std::cout << "Конец срока годности (день.месяц.год): ";
        Getline(p.end_date);
        if (!validate_dates(p.begin_date, p.end_date)) {
            std::cerr << "Повторите ввод дат\n";
        } else {
            dates_valid = true;
        }
    }

    std::cout << "Количество: ";
    Getline(p.count);
    std::cout << "Производитель: ";
    Getline(p.manufacturer, false);
    std::cout << "Страна: ";
    Getline(p.country, false);

    if (!check_characteristics(p)) return;

    goods.push_back(p);
    std::cout << "Товар " << p.name << " добавлен" << std::endl;
    save_to_file("Product.txt");
}

void Storage::show_all() {
    if (goods.empty()) {
        std::cout << "Склад пуст" << std::endl;
        return;
    }

    const int w_id = 4, w_name = 16, w_cat = 14, w_price = 10, w_count = 8, w_art = 12;

    std::cout << "\n" << std::setfill('=') << std::setw(80) << "" << std::setfill(' ') << "\n";
    std::cout << std::left
              << std::setw(w_id) << "ID"
              << std::setw(w_name) << "Название"
              << std::setw(w_cat) << "Категория"
              << std::setw(w_price) << "Цена"
              << std::setw(w_count) << "Кол-во"
              << std::setw(w_art) << "Артикль" << "\n";
    std::cout << std::setfill('-') << std::setw(80) << "" << std::setfill(' ') << "\n";

    for (size_t i = 0; i < goods.size(); ++i) {
        Product& p = goods[i];
        std::cout << std::left
                  << std::setw(w_id) << i + 1
                  << std::setw(w_name) << format_field(p.name, w_name - 1)
                  << std::setw(w_cat) << format_field(p.category, w_cat - 1)
                  << std::setw(w_price) << std::fixed << std::setprecision(2) << p.price
                  << std::setw(w_count) << p.count
                  << p.article << "\n";
    }
    std::cout << std::setfill('=') << std::setw(80) << "" << std::setfill(' ') << "\n\n";
}

void Storage::show_valid_products() {
    if (goods.empty()) {
        std::cout << "Склад пуст" << std::endl;
        return;
    }

    int valid_count = 0;
    for (size_t i = 0; i < goods.size(); ++i) {
        if (!is_expired(goods[i].end_date)) {
            std::cout << i + 1 << ". " << goods[i].name
                      << " (арт. " << goods[i].article
                      << ", годен до: " << goods[i].end_date
                      << ", кол-во: " << goods[i].count << ")\n";
            valid_count++;
        }
    }

    if (valid_count == 0) {
        std::cout << "Нет годных товаров" << std::endl;
    } else {
        std::cout << "Всего годных товаров: " << valid_count << std::endl;
    }
}

void Storage::check_expired_products() {
    if (goods.empty()) {
        std::cout << "Склад пуст" << std::endl;
        return;
    }

    int expired_count = 0;
    for (const auto& p : goods) {
        if (is_expired(p.end_date)) {
            std::cout << "Просрочен: " << p.name
                      << " (арт. " << p.article
                      << ", годен до: " << p.end_date
                      << ", кол-во: " << p.count << ")\n";
            expired_count++;
        }
    }

    if (expired_count == 0) {
        std::cout << "Просроченных товаров нет" << std::endl;
    } else {
        std::cout << "Всего просроченных: " << expired_count << std::endl;
    }
}

void Storage::supply_menu() {
    int choice;
    while (true) {
        std::cout << "\n\n\n\t\t\tУПРАВЛЕНИЕ ПОСТАВКАМИ\n\n\n";
        std::cout << "1) Создать новые поставки\n";
        std::cout << "2) Показать все поставки\n";
        std::cout << "3) Изменить поставку\n";
        std::cout << "4) Удалить поставку\n";
        std::cout << "5) Применить поставку (пополнить склад)\n";
        std::cout << "0) Выход\n";
        std::cout << "Выбор: ";
        Getline(choice);

        switch(choice) {
        case 1:
            supply_manager.create_supplies();
            break;
        case 2:
            supply_manager.show_all_supplies();
            break;
        case 3:
            supply_manager.change_supply_from_file();
            break;
        case 4:
            supply_manager.delete_supply_from_file();
            break;
        case 5:
            supply_manager.apply_supply_to_storage(*this);
            break;
        case 0:
            return;
        default:
            std::cerr << "Неверный выбор\n";
        }
    }
}

void Storage::add_supply_products(const Supply& supply) {
    for (auto& p : goods) {
        if (p.article == supply.product_name.article) {
            p.count += supply.product_name.count;
            std::cout << "Товар обновлён: " << p.name
                      << " +" << supply.product_name.count << " шт.\n"
                      << "Теперь на складе: " << p.count << " шт.\n";
            save_to_file("Product.txt");
            return;
        }
    }
    goods.push_back(supply.product_name);
    std::cout << "Добавлен новый товар: " << supply.product_name.name
              << " (" << supply.product_name.count << " шт.)\n";
    save_to_file("Product.txt");
}

void Storage::super_admin_menu(AuthSystemUser& authSystem) {
    int choice;
    while (true) {
        std::cout << "\n\n\n\t\t\tМЕНЮ СУПЕР-АДМИНИСТРАТОРА\n\n\n";
        std::cout << "1) Добавить товар\n";
        std::cout << "2) Показать все товары\n";
        std::cout << "3) Показать годные товары\n";
        std::cout << "4) Проверить просроченные товары\n";
        std::cout << "5) Управление поставками\n";
        std::cout << "6) Регистрация нового пользователя\n";
        std::cout << "7) Показать всех пользователей\n";
        std::cout << "8) Изменить пользователя\n";
        std::cout << "9) Удалить пользователя\n";
        std::cout << "10) Отчет\n";
        std::cout << "0) Выход\n";
        std::cout << "Выбор: ";
        Getline(choice);

        switch(choice) {
        case 1:
            add_product();
            break;
        case 2:
            show_all();
            break;
        case 3:
            show_valid_products();
            break;
        case 4:
            check_expired_products();
            break;
        case 5:
            supply_menu();
            break;
        case 6:
            authSystem.register_user();
            break;
        case 7:
            authSystem.show_all_users();
            break;
        case 8:
            authSystem.change_user();
            break;
        case 9:
            authSystem.remove_user();
            break;
        case 10:
            // реализация позже
            break;
        case 0:
            return;
        default:
            std::cerr << "Неверный выбор\n";
        }
    }
}

void Storage::admin_menu() {
    int choice;
    while (true) {
        std::cout << "\n\n\n\t\t\tМЕНЮ АДМИНИСТРАТОРА\n\n\n";
        std::cout << "1) Добавить товар\n";
        std::cout << "2) Показать все товары\n";
        std::cout << "3) Показать годные товары\n";
        std::cout << "4) Проверить просроченные товары\n";
        std::cout << "5) Показать все поставки\n";
        std::cout << "6) Изменить цену\n";
        std::cout << "7) Списать товар\n";
        std::cout << "8) Отчет\n";
        std::cout << "0) Выход\n";
        std::cout << "Выбор: ";
        Getline(choice);

        switch(choice) {
        case 1:
            add_product();
            break;
        case 2:
            show_all();
            break;
        case 3:
            show_valid_products();
            break;
        case 4:
            check_expired_products();
            break;
        case 5:
            supply_manager.show_all_supplies();
            break;
        case 6:
            // реализация позже
            break;
        case 7:
            // реализация позже
            break;
        case 8:
            // реализация позже
            break;
        case 0:
            return;
        default:
            std::cerr << "Неверный выбор\n";
        }
    }
}

void Storage::user_menu() {
    int choice;
    while (true) {
        std::cout << "\n\n\n\t\t\tМЕНЮ ПОЛЬЗОВАТЕЛЯ\n\n\n";
        std::cout << "1) Показать все товары\n";
        std::cout << "2) Показать годные товары\n";
        std::cout << "0) Выход\n";
        std::cout << "Выбор: ";
        Getline(choice);

        switch(choice) {
        case 1:
            show_all();
            break;
        case 2:
            show_valid_products();
            break;
        case 0:
            return;
        default:
            std::cerr << "Неверный выбор\n";
        }
    }
}

void Storage::start(const std::string& userStatus, AuthSystemUser& authSystem) {
    load_from_file("Product.txt");
    if (userStatus == "superadmin") {
        super_admin_menu(authSystem);
    } else if (userStatus == "admin") {
        admin_menu();
    } else {
        user_menu();
    }
}

std::chrono::sys_days Storage::parse_date(const std::string& date_str) {
    int day, month, year;
    char dot1, dot2;
    std::stringstream ss(date_str);
    ss >> day >> dot1 >> month >> dot2 >> year;
    if (year < 100) year += 2000;
    return std::chrono::sys_days{
        std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day)
    };
}

std::string Storage::format_field(std::string str, std::size_t width) const {
    if (str.length() > width) {
        return str.substr(0, width - 2) + "..";
    }
    return str;
}

bool Storage::is_expired(const std::string& date_str) {
    try {
        auto end_date = parse_date(date_str);
        auto now = std::chrono::system_clock::now();
        return end_date < now;
    } catch (...) {
        return true;
    }
}

bool Storage::is_date_range_valid(const std::string& begin_str, const std::string& end_str) {
    try {
        auto begin = parse_date(begin_str);
        auto end = parse_date(end_str);
        return begin <= end;
    } catch (...) {
        return false;
    }
}

bool Storage::validate_dates(const std::string& begin, const std::string& end) {
    if (!is_date_range_valid(begin, end)) {
        std::cerr << "Дата начала не может быть позже даты окончания\n";
        return false;
    }
    if (is_expired(end)) {
        std::cerr << "Товар уже просрочен\n";
        return false;
    }
    try {
        auto end_date = parse_date(end);
        auto now = std::chrono::system_clock::now();
        auto days_left = std::chrono::duration_cast<std::chrono::days>(end_date - now).count();
        if (days_left <= 30 && days_left > 0) {
            std::cout << "Срок годности истекает через " << days_left << " дней\n";
        }
    } catch (...) {}
    return true;
}

bool Storage::check_characteristics(const Product& p) {
    auto it = std::find(goods.begin(), goods.end(), p);
    if (it != goods.end()) {
        std::cerr << "Товар с артиклем " << p.article << " уже существует\n";
        return false;
    }
    if (p.price <= 0) {
        std::cerr << "Цена должна быть положительной\n";
        return false;
    }
    if (p.count == 0) {
        std::cerr << "Количество должно быть больше 0\n";
        return false;
    }
    if (p.article <= 0) {
        std::cerr << "Артикль должен быть положительным числом\n";
        return false;
    }
    return true;
}
