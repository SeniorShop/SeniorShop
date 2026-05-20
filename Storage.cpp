#include "Storage.h"
#include "Check.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include "AuthSystemUser.h"
#include "MethodSuperAdmin.h"
#include <algorithm>
#include <thread>
#ifdef _WIN32
#include "Windows.h"
#else
#include "stdlib.h"
#endif

void Storage::set_auth_system(AuthSystemUser* auth) {
    authSystem = auth;
}

void Storage::load_from_file(const std::string& product_database) {
    std::ifstream input(product_database);
    if (!input.is_open()) {
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
}

void Storage::save_to_file(const std::string& product_database) {
    std::ofstream output(product_database);
    if (!output.is_open()) {
        return;
    }
    for (const auto& write_product : goods) {
        output << write_product.name << ' ' << write_product.category << ' ' << write_product.price << ' '
               << write_product.article << ' ' << write_product.begin_date << ' ' << write_product.end_date << ' '
               << write_product.count << ' ' << write_product.manufacturer << ' ' << write_product.country << '\n';
    }
    output.close();
    std::cout << "Было записано " << goods.size() << " товаров в базу данных" << std::endl;
}
void Storage::super_admin_menu() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    bool is_exit = false;
    while (true) {
        std::cout << "\n\n\n\t\t\tСПИСОК ДЕЙСТВИЙ ДЛЯ СУПЕР АДМИНА\n\n\n";
        std::cout << "1) Начать продажу\n";
        std::cout << "2) Показать склад\n";
        std::cout << "3) Пополнить склад\n";
        std::cout << "4) Списать товар со склада\n";
        std::cout << "5) Изменить цену товара\n";
        std::cout << "6) Редактировать склад\n";
        std::cout << "7) Редактировать персонал\n";
        std::cout << "8) Отчет\n";
        std::cout << "9) Поставки\n";
        std::cout << "0) Выход\n";
        std::cout << "Выберите действие: ";
        std::string choose;
        Getline(choose,true);

        if(choose.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choose[0] < '0' || choose[0] > '9') {
            std::cerr << "Ошибка! Введите одну цифру от 0 до 9.\n";
            system("pause");
            continue;
        }

        switch(choose[0]) {
        case '0':
            is_exit = true;
            break;
        case '1':
            start_sales.start();
            break;
        case '2':
            show_all();
            break;
        case '3':
            add_product();
            break;
        case '4':
            // реализация будет позже
            break;
        case '5':
            change_product_price();
            break;
        case '6':
            // реализация будет позже
            break;
        case '7':
            storage_user_method(authSystem);
            break;
        case '8':
            get_check.show_financial_report("Checks.txt");
            break;
        case '9':
            supply_menu();
            break;
        }

        if(is_exit) break;

    }
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    return;
}

void Storage::change_product_price() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    if (goods.empty()) {
        std::cerr << "Список товаров пустой\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    show_all();

    int article = 0;
    std::cout << "Введите артикль товара: ";
    Getline(article);

    auto find = std::find_if(goods.begin(), goods.end(),
                             [&](const Product& p) { return p.article == article; });

    if (find == goods.end()) {
        std::cerr << "Товар не найден\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    std::cout << "Текущая цена '" << find->name << "': " << find->price << " руб.\n";

    double new_price;
    std::cout << "Введите новую цену: ";
    Getline(new_price);

    if (new_price < 0.0 || new_price > 10000.0) {
        std::cerr << "Ошибка: цена должна быть от 0 до 10000 руб.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    find->price = new_price;
    std::cout << "Цена изменена на " << find->price << " руб.\n";
    save_to_file("Product.txt");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Storage::supply_menu() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    bool is_exit = false;
    while (true) {
        std::cout << "\n\n\n\t\t\tУПРАВЛЕНИЕ ПОСТАВКАМИ\n\n\n";
        std::cout << "1) Создать новые поставки\n";
        std::cout << "2) Показать все поставки\n";
        std::cout << "3) Изменить поставку\n";
        std::cout << "4) Удалить поставку\n";
        std::cout << "5) Применить поставку (пополнить склад)\n";
        std::cout << "0) Выход\n";
        std::cout << "Выбор: ";
        std::string choice;
        Getline(choice,true);

        if(choice.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if(choice[0] < '0' || choice[0] > '5') {
            std::cerr << "Ошибка! Введите одну цифру от 0 до 9.\n";
            system("pause");
            continue;
        }

        switch(choice[0]) {
        case '1':
            supply_manager.create_supplies();
            break;
        case '2':
            supply_manager.show_all_supplies();
            break;
        case '3':
            supply_manager.change_supply_from_file();
            break;
        case '4':
            supply_manager.delete_supply_from_file();
            break;
        case '5':
            supply_manager.apply_supply_to_storage(*this);
            break;
        case '0':
            is_exit = true;
            break;
        default:
            std::cerr << "Неверный выбор\n";
        }

        if(is_exit) break;

    }
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    return;
}

void Storage::admin_menu() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    bool is_exit = false;
    while (true) {
        std::cout << "\n\n\n\t\t\tСПИСОК ДЕЙСТВИЙ ДЛЯ АДМИНА\n\n\n";
        std::cout << "1) Начать продажу\n";
        std::cout << "2) Показать склад\n";
        std::cout << "3) Пополнить склад\n";
        std::cout << "4) Пополнить товар\n";
        std::cout << "5) Списать товар\n";
        std::cout << "6) Отчет\n";
        std::cout << "7) Поставки\n";
        std::cout << "0) Выход\n";;
        std::cout << "Выберите действие: ";
        std::string choice;
        Getline(choice,true);

        if(choice.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choice[0] < '0' || choice[0] > '9') {
            std::cerr << "Ошибка! Введите одну цифру от 0 до 9.\n";
            system("pause");
            continue;
        }

        switch(choice[0]) {
        case '1':
            start_sales.start();
            break;
        case '2':
            show_all();
            break;
        case '3':
            add_product();
            break;
        case '4':
            // реализация будет позже
            break;
        case '5':
            // реализация будет позже
            break;
        case '6':
            get_check.show_financial_report("Checks.txt");
            break;
        case '7':
            supply_menu();
            break;
        case '0':
            is_exit = true;
            break;
        }

        if(is_exit) break;
    }
    return;
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

void Storage::user_menu() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    bool is_exit = false;
    while (true) {
        std::cout << "\n\n\n\t\t\tСПИСОК ДЕЙСТВИЙ ДЛЯ ПОЛЬЗОВАТЕЛЯ\n\n\n";
        std::cout << "1) Начать продажу\n";
        std::cout << "2) Показать склад\n";
        std::cout << "3) Отчет\n";
        std::cout << "4) Поставки\n";
        std::cout << "0) Выход\n";
        std::cout << "Выберите действие: ";
        std::string choice;
        Getline(choice,true);

        if(choice.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choice[0] < '0' || choice[0] > '9') {
            std::cerr << "Ошибка! Введите одну цифру от 0 до 9.\n";
            system("pause");
            continue;
        }

        switch(choice[0]) {
        case '0':
            is_exit = true;
            break;
        case '1':
            start_sales.start();
            break;
        case '2':
            show_all();
            break;
        case '3':
            get_check.show_financial_report("Checks.txt");
            break;
        case '4':
            supply_menu();
            break;
        }

        if(is_exit) break;

    }
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    return;
}
void Storage::start(const std::string& user_status) {
    load_from_file("Product.txt");
    if (user_status == "superadmin") {
        super_admin_menu();
    } else if (user_status == "admin") {
        admin_menu();
    } else {
        user_menu();
    }
}

void Storage::add_product() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    Product new_product;

    while (true) {
        std::cout << "Название товара (пустая строка или 'exit' для отмены): ";
        Getline(new_product.name);
        if (new_product.name.empty() || new_product.name == "exit") {
            std::cerr << "Операция добавления отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        }
        if (new_product.name.size() > 60) {
            std::cerr << "Название слишком длинное (максимум 60 символов).\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Категория (пустая строка или 'exit' для отмены): ";
        Getline(new_product.category);
        if (new_product.category.empty() || new_product.category == "exit") {
            std::cerr << "Операция добавления отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
            return;
        }
        if (new_product.category.size() > 50) {
            std::cerr << "Категория слишком длинная (максимум 50 символов).\n";
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Цена (0 – отмена): ";
        Getline(new_product.price);
        if (new_product.price < 0) {
            std::cerr << "Некорректная цена. Введите положительное число.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        if (new_product.price == 0) {
            std::cerr << "Операция добавления отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
            return;
        }
        if (new_product.price > 10000.0) {
            std::cerr << "Цена не может превышать 10000.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Артикль (целое число, 0 - отмена): ";
        Getline(new_product.article);
        if (new_product.article < 0 ) {
            std::cerr << "Некорректный артикль\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        if(new_product.article == 0) {
            std::cerr << "Операция добавления отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        }
        break;
    }

    bool dates_valid = false;
    while (!dates_valid) {
        std::cout << "Начало срока годности (ДД.ММ.ГГГГ): ";
        Getline(new_product.begin_date);
        if (new_product.begin_date.empty() || new_product.begin_date == "exit") {
            std::cerr << "Операция добавления отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        }

        std::cout << "Окончание срока годности (ДД.ММ.ГГГГ): ";
        Getline(new_product.end_date);
        if (new_product.end_date.empty() || new_product.end_date == "exit") {
            std::cerr << "Операция добавления отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        }

        if (!validate_dates(new_product.begin_date, new_product.end_date)) {
            std::cout << "Пожалуйста, введите корректные даты заново.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        dates_valid = true;
    }

    while (true) {
        std::cout << "Количество (максимум 199, 0 – отмена): ";
        Getline(new_product.count);
        if (new_product.count < 0) {
            std::cerr << "Некорректное количество.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
            continue;
        }
        if(new_product.count == 0) {
            std::cerr << "Операция добавления отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
            return;
        }
        if (new_product.count > 199) {
            std::cerr << "Количество не может быть больше 199.\n";
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Производитель (пустая строка или 'exit' для отмены): ";
        Getline(new_product.manufacturer);
        if (new_product.manufacturer.empty() || new_product.manufacturer == "exit") {
            std::cerr << "Операция добавления отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
            return;
        }
        if (new_product.manufacturer.size() > 60) {
            std::cerr << "Название производителя слишком длинное (максимум 60 символов).\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Страна производства (пустая строка или 'exit' для отмены): ";
        Getline(new_product.country);
        if (new_product.country.empty() || new_product.country == "exit") {
            std::cerr << "Операция добавления отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        }
        if (new_product.country.size() > 55) {
            std::cerr << "Название страны слишком длинное (максимум 55 символов).\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        break;
    }


    goods.push_back(new_product);
    std::cout << "Товар '" << new_product.name << "' успешно добавлен.\n";
    save_to_file("Product.txt");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Storage::show_all() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    load_from_file("Product.txt");

    goods.erase(std::remove_if(goods.begin(), goods.end(),
                               [](const Product& p) { return p.count == 0; }), goods.end());

    if (goods.empty()) {
        std::cout << "Склад пуст.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    const int w_id  = 4;
    const int w_name = 18;
    const int w_cat = 14;
    const int w_price = 10;
    const int w_count = 8;
    const int w_art = 10;

    int total_width = w_id + w_name + w_cat + w_price + w_count + w_art + 13;

    std::cout << "\n" << std::setfill('=') << std::setw(total_width) << "" << std::setfill(' ') << "\n";

    std::cout << std::left
              << "| " << std::setw(w_id)   << "ID"
              << "| " << std::setw(w_name) << "Название"
              << "| " << std::setw(w_cat)  << "Категория"
              << "| " << std::setw(w_price) << "Цена"
              << "| " << std::setw(w_count) << "Кол-во"
              << "| " << std::setw(w_art)  << "Артикль" << "|\n";

    std::cout << std::setfill('-') << std::setw(total_width) << "" << std::setfill(' ') << "\n";

    for (size_t i = 0; i < goods.size(); ++i) {
        const Product& p = goods[i];

        std::cout << std::left
                  << "| " << std::setw(w_id)   << i + 1
                  << "| " << std::setw(w_name) << format_field(p.name, w_name - 1)
                  << "| " << std::setw(w_cat)  << format_field(p.category, w_cat - 1)
                  << "| " << std::setw(w_price) << std::fixed << std::setprecision(2) << p.price
                  << "| " << std::setw(w_count) << p.count
                  << "| " << std::setw(w_art)  << p.article << "|\n";
    }

    std::cout << std::setfill('=') << std::setw(total_width) << "" << std::setfill(' ') << "\n\n";
}
void Storage::show_valid_products() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    if (goods.empty()) {
        std::cerr << "Список товаров пустой" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    int valid_count = 0;
    for (size_t i = 0; i < goods.size(); ++i) {
        if (!is_expired(goods[i].end_date)) {
            const auto& p = goods[i];
            std::cout << i + 1 << " | Название: " << p.name
                      << " | Категория: " << p.category
                      << " | Цена: " << p.price
                      << " | Артикль: " << p.article
                      << " | Годен до: " << p.end_date
                      << " | Количество: " << p.count
                      << " | Производитель: " << p.manufacturer
                      << " | Страна: " << p.country << '\n';
            valid_count++;
        }
    }
    if (valid_count == 0) {
        std::cerr << "Нет годных товаров на складе\n";
    }
    else {
        std::cout << "Всего годных товаров: " << valid_count << "\n";
    }
}
void Storage::check_expired_products() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    if (goods.empty()) {
        std::cerr << "Список товаров пустой" << std::endl;
        return;
    }
    int expired_count = 0;

    for (size_t i = 0; i < goods.size(); ++i) {
        if (is_expired(goods[i].end_date)) {
            const auto& p = goods[i];
            std::cout << "Товар просрочен: " << p.name
                      << "(Артикль: " << p.article
                      << ", Годен до: " << p.end_date << ")\n";
            expired_count++;
        }
    }

    if (expired_count == 0) {
        std::cout << "Просроченных товаров нет\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    else {
        std::cout << "Всего просроченных товаров: " << expired_count << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
}

std::chrono::sys_days Storage::parse_date(const std::string& date_str) {
    int day, month, year;
    char dot1, dot2;

    std::istringstream ss(date_str);
    ss >> day >> dot1 >> month >> dot2 >> year;

    if (year < 100) {
        year += 2000;
    }

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
bool Storage::is_expired(const std::string& end_date_str) {
    try {
        auto end_date = parse_date(end_date_str);
        auto now = std::chrono::system_clock::now();

        auto today = std::chrono::floor<std::chrono::days>(now);

        return end_date < today;
    } catch (...) {
        std::cerr << "Ошибка парсинга даты: " << end_date_str << std::endl;
        return true;
    }
}
bool Storage::is_date_range_valid(const std::string& begin_date_str, const std::string& end_date_str) {
    try {
        auto begin_date = parse_date(begin_date_str);
        auto end_date = parse_date(end_date_str);
        return begin_date <= end_date;
    }
    catch (...) {
        std::cerr << "Неверный формат даты! Используйте ДД.ММ.ГГГГ\n";
        return false;
    }
}

bool Storage::validate_dates(const std::string& begin_date, const std::string& end_date) {

    if (begin_date.length() != 10 || end_date.length() != 10) {
        std::cerr << "Ошибка: дата должна быть в формате ДД.ММ.ГГГГ\n";
        return false;
    }
    if (begin_date[2] != '.' || begin_date[5] != '.' ||
        end_date[2] != '.' || end_date[5] != '.') {
        std::cerr << "Ошибка: используйте разделитель '.'\n";
        return false;
    }

    if (!is_date_range_valid(begin_date, end_date)) {
        return false;
    }

    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::floor<std::chrono::days>(now);
    auto begin = parse_date(begin_date);
    auto end = parse_date(end_date);

    if (begin > today) {
        std::cerr << "Ошибка: дата начала срока годности не может быть в будущем\n";
        return false;
    }

    auto min_date = parse_date("01.01.2026");
    if (begin < min_date) {
        std::cerr << "Ошибка: дата начала срока годности слишком старая (должна быть не ранее 01.01.2026)\n";
        return false;
    }

    if (end < today) {
        auto days_ago = std::chrono::duration_cast<std::chrono::days>(today - end).count();
        std::cerr << "Ошибка: Товар просрочен на " << days_ago << " дней\n";
        return false;
    }

    auto days_left = std::chrono::duration_cast<std::chrono::days>(end - today).count();
    if (days_left <= 30 && days_left > 0) {
        std::cout << "Срок годности истекает через " << days_left << " дней!\n";
    }

    return true;
}

bool Storage::check_characteristics(const Product& p) {
    auto exist = std::find(goods.begin(), goods.end(), p);
    if (exist != goods.end()) {
        std::cerr << "Товар уже есть\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return false;
    }

    std::unordered_set<char> allowed_chars;
    for (char c = 'A'; c <= 'Z'; ++c) allowed_chars.insert(c);
    for (char c = 'a'; c <= 'z'; ++c) allowed_chars.insert(c);
    for (char c = '0'; c <= '9'; ++c) allowed_chars.insert(c);
    allowed_chars.insert(' ');
    allowed_chars.insert('.');
    allowed_chars.insert('-');

    for (char c : p.name) {
        if (allowed_chars.find(c) == allowed_chars.end()) {
            std::cerr << "Название содержит недопустимые символы. Разрешены: буквы, цифры, пробел, точка, дефис\n";
            return false;
        }
    }

    for (char c : p.category) {
        if (allowed_chars.find(c) == allowed_chars.end()) {
            std::cerr << "Категория содержит недопустимые символы. Разрешены: буквы, цифры, пробел, точка, дефис\n";
            return false;
        }
    }

    for (char c : p.manufacturer) {
        if (allowed_chars.find(c) == allowed_chars.end()) {
            std::cerr << "Производитель содержит недопустимые символы. Разрешены: буквы, цифры, пробел, точка, дефис\n";
            return false;
        }
    }

    for (char c : p.country) {
        if (allowed_chars.find(c) == allowed_chars.end()) {
            std::cerr << "Страна содержит недопустимые символы. Разрешены: буквы, цифры, пробел, точка, дефис\n";
            return false;
        }
    }

    if (p.name.empty() || p.category.empty() || p.manufacturer.empty() || p.country.empty()) {
        std::cerr << "Все текстовые поля должны быть заполнены\n";
        return false;
    }

    if (p.name.length() > 60 || p.category.length() > 50 ||
        p.manufacturer.length() > 60 || p.country.length() > 55) {
        std::cerr << "Превышена максимальная длина поля\n";
        return false;
    }

    if (p.price <= 0.0 || p.count == 0 || p.article <= 0) {
        std::cerr << "Цена/Количество/Артикль должны быть положительными\n";
        return false;
    }

    return true;
}


