#include "../include/Storage.h"
#include "../include/Check.h"
#include "../include/SupplierManager.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include "../include/AuthSystemUser.h"
#include "../include/MethodSuperAdmin.h"
#include <algorithm>
#include <thread>
#ifdef _WIN32
#include "Windows.h"
#else
#include "stdlib.h"
#endif

void Storage::add_supply_products(const Supply& supply) {
    auto it = std::find_if(goods.begin(), goods.end(),
                           [&](const Product& p) { return p.article == supply.product_name.article; });

    if (it != goods.end()) {
        if (it->count + supply.product_name.count > 199) {
            std::cerr << "Ошибка: превышение максимального количества (199) для товара '"
                      << it->name << "'. Текущее количество: " << it->count
                      << ", попытка добавить: " << supply.product_name.count
                      << ". Операция отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            return;
        }
        it->count += supply.product_name.count;
        std::cout << "Товар обновлён: " << it->name
                  << " + " << supply.product_name.count << " шт.\n"
                  << "Теперь на складе: " << it->count << " шт.\n";
    }
    else {
        if (supply.product_name.count > 199) {
            std::cerr << "Ошибка: количество товара '" << supply.product_name.name
                      << "' превышает максимальное значение (199). Операция отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            return;
        }
        goods.push_back(supply.product_name);
        std::cout << "Добавлен новый товар: " << supply.product_name.name
                  << " (" << supply.product_name.count << " шт.)\n";
    }
    save_to_file("Product.txt");
}

void Storage::set_auth_system(AuthSystemUser* auth) {
    authSystem = auth;
}

void Storage::load_from_file(const std::string& product_database) {
    std::ifstream input(product_database);
    if (!input.is_open()) return;
    goods.clear();
    Product p;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::getline(ss, p.name, '|');
        std::getline(ss, p.category, '|');
        ss >> p.price; ss.ignore();
        ss >> p.article; ss.ignore();
        std::getline(ss, p.begin_date, '|');
        std::getline(ss, p.end_date, '|');
        ss >> p.count; ss.ignore();
        std::getline(ss, p.manufacturer, '|');
        std::getline(ss, p.supplier, '|');
        std::getline(ss, p.country);
        goods.push_back(p);
    }
    input.close();
}

void Storage::save_to_file(const std::string& product_database) {
    std::ofstream output(product_database);
    if (!output.is_open()) return;
    for (const auto& p : goods) {
        output << p.name << '|' << p.category << '|' << p.price << '|'
               << p.article << '|' << p.begin_date << '|' << p.end_date << '|'
               << p.count << '|' << p.manufacturer << '|' << p.supplier << '|'
               << p.country << '\n';
    }
    output.close();
    std::cout << "Записано " << goods.size() << " товаров\n";
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
        std::cout << "3) Редактировать склад\n";
        std::cout << "4) Редактировать персонал\n";
        std::cout << "5) Отчет\n";
        std::cout << "6) Поставки\n";
        std::cout << "0) Выход\n";
        std::cout << "Выберите действие: ";
        std::string choice;
        Getline(choice, true);

        if(choice.size() > 2) {
            std::cerr << "Ошибка: введите номер действия\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choice.empty()) {
            std::cerr << "Ошибка: введите номер действия\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        bool is_valid = true;
        for (char c : choice) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) {
            std::cerr << "Ошибка: введите только цифры от 0 до 6\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        int num_choice = std::stoi(choice);

        if (num_choice < 0 || num_choice > 6) {
            std::cerr << "Ошибка! Введите цифру от 0 до 6.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        switch (num_choice) {
        case 0:
            is_exit = true;
            break;
        case 1:
            start_sales.start();
            break;
        case 2:
            show_all();
            break;
        case 3:
            edit_storage_menu();
            break;
        case 4:
            storage_user_method(authSystem);
            break;
        case 5:
            get_check.show_financial_report("Checks.txt");
            break;
        case 6:
            supply_menu();
            break;
        default:
            std::cerr << "Неверное действие\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        }
        if (is_exit) break;
    }
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
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
        std::cout << "3) Редактировать склад\n";
        std::cout << "4) Отчет\n";
        std::cout << "5) Поставки\n";
        std::cout << "0) Выход\n";
        std::cout << "Выберите действие: ";
        std::string choice;
        Getline(choice, true);

        if(choice.size() > 2) {
            std::cerr << "Ошибка: введите номер действия\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choice.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        bool is_valid = true;
        for (char c : choice) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) {
            std::cerr << "Ошибка: введите только цифры от 0 до 6\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (std::stoi(choice) < 0 || std::stoi(choice) > 9) {
            std::cerr << "Ошибка! Введите цифру от 0 до 9.\n";
#ifdef _WIN32
            system("pause");
#else
            std::cout << "Нажмите Enter для продолжения: ";
            std::cin.get();
#endif
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        switch (std::stoi(choice)) {
        case 0:
            is_exit = true;
            break;
        case 1:
            start_sales.start();
            break;
        case 2:
            show_all();
            break;
        case 3:
            edit_storage_menu("admin");
            break;
        case 4:
            get_check.show_financial_report("Checks.txt");
            break;
        case 5:
            supply_menu();
            break;
        default:
            std::cerr << "Неверное действие\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        }
        if (is_exit) break;
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

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
        Getline(choice, true);

        if(choice.size() > 2) {
            std::cerr << "Ошибка: введите номер действия\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choice.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        bool is_valid = true;
        for (char c : choice) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) {
            std::cerr << "Ошибка: введите только цифры от 0 до 6\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (std::stoi(choice) < 0 || std::stoi(choice) > 4) {
            std::cerr << "Ошибка! Введите цифру от 0 до 4.\n";
#ifdef _WIN32
            system("pause");
#else
            std::cout << "Нажмите Enter для продолжения: ";
            std::cin.get();
#endif
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if(std::isalpha(choice[0])) {
            std::cerr << "Неверное действие\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        switch (std::stoi(choice)) {
        case 0:
            is_exit = true;
            break;
        case 1:
            start_sales.start();
            break;
        case 2:
            show_all();
            break;
        case 3:
            get_check.show_financial_report("Checks.txt");
            break;
        case 4:
            supply_menu();
            break;
        default:
            std::cerr << "Неверное действие\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        }
        if (is_exit) break;
    }
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Storage::start(const std::string& user_status) {
    auto_writeoff_expired();
    load_from_file("Product.txt");
    if (user_status == "superadmin") {
        super_admin_menu();
    }
    else if (user_status == "admin") {
        admin_menu();
    }
    else {
        user_menu();
    }
}

void Storage::show_all_for_actions() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::vector<Product> display_goods;
    for (const auto& p : goods) {
        if (p.count > 0) {
            display_goods.push_back(p);
        }
    }

    if (display_goods.empty()) {
        std::cout << "Склад пуст.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    const int w_id = 4;
    const int w_name = 18;
    const int w_cat = 14;
    const int w_price = 10;
    const int w_count = 8;
    const int w_art = 10;
    const int w_expiry = 12;

    int total_width = w_id + w_name + w_cat + w_price + w_count + w_art + w_expiry + 13;

    std::cout << "\n" << std::setfill('=') << std::setw(total_width) << "" << std::setfill(' ') << "\n";
    std::cout << std::left
              << "| " << std::setw(w_id) << "ID"
              << "| " << std::setw(w_name) << "Название"
              << "| " << std::setw(w_cat) << "Категория"
              << "| " << std::setw(w_price) << "Цена"
              << "| " << std::setw(w_count) << "Кол-во"
              << "| " << std::setw(w_art) << "Артикль"
              << "| " << std::setw(w_expiry) << "Годен до"
              << "|\n";
    std::cout << std::setfill('-') << std::setw(total_width) << "" << std::setfill(' ') << "\n";

    for (size_t i = 0; i < display_goods.size(); ++i) {
        const Product& p = display_goods[i];
        std::cout << std::left
                  << "| " << std::setw(w_id) << i + 1
                  << "| " << std::setw(w_name) << format_field(p.name, w_name - 1)
                  << "| " << std::setw(w_cat) << format_field(p.category, w_cat - 1)
                  << "| " << std::setw(w_price) << std::fixed << std::setprecision(2) << p.price
                  << "| " << std::setw(w_count) << p.count
                  << "| " << std::setw(w_art) << p.article
                  << "| " << std::setw(w_expiry) << p.end_date
                  << "|\n";
    }
    std::cout << std::setfill('=') << std::setw(total_width) << "" << std::setfill(' ') << "\n\n";
}

void Storage::add_product() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    Product new_product;

    new_product.supplier = SupplierManager::choose_supplier();
    if (new_product.supplier.empty()) {
        std::cerr << "Операция добавления отменена (нет поставщика).\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    while (true) {
        std::cout << "Название товара (пустая строка или 'exit' для отмены): ";
        Getline(new_product.name,false);
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
            continue;
        }
        break;
    }

    new_product.category = SupplierManager::choose_category(new_product.supplier);
    if (new_product.category.empty()) {
        std::cerr << "Ошибка: категория не может быть пустой.\n";
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
        return;
    }

    while (true) {
        std::cout << "Цена (0 – отмена): ";
        Getline(new_product.price);
        if (new_product.price < 0) {
            std::cerr << "Некорректная цена. Введите положительное число.\n";
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
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Артикль (целое число > 0, 0 - отмена): ";
        Getline(new_product.article);
        if (new_product.article < 0) {
            std::cerr << "Некорректный артикль\n";
            continue;
        }
        if (new_product.article == 0) {
            std::cerr << "Операция добавления отменена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            return;
        }

        auto it = std::find_if(goods.begin(), goods.end(),
                               [&](const Product& p) { return p.article == new_product.article; });
        if (it != goods.end()) {
            std::cerr << "Товар с таким артиклем уже существует. Введите другой артикль.\n";
            continue;
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
            continue;
        }
        dates_valid = true;
    }

    while (true) {
        std::cout << "Количество (максимум 199, 0 – отмена): ";
        Getline(new_product.count);
        if (new_product.count < 0) {
            std::cerr << "Некорректное количество.\n";
            continue;
        }
        if (new_product.count == 0) {
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

void Storage::writeoff_product() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    if (goods.empty()) {
        std::cerr << "Склад пуст. Нет товаров для списания.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    show_all_for_actions();

    int article;
    std::cout << "Введите артикль товара для списания: ";
    Getline(article);

    auto it = std::find_if(goods.begin(), goods.end(),
                           [article](const Product& p) { return p.article == article; });

    if (it == goods.end()) {
        std::cerr << "Товар с артиклем " << article << " не найден.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "\n\n\n\t\t\tИНФОРМАЦИЯ О ТОВАРЕ\n\n\n";
    std::cout << "Название: " << it->name << "\n";
    std::cout << "Категория: " << it->category << "\n";
    std::cout << "Цена: " << it->price << " руб.\n";
    std::cout << "Доступно на складе: " << it->count << " шт.\n";
    std::cout << "Артикль: " << it->article << "\n";
    std::cout << "Срок годности до: " << it->end_date << "\n";

    unsigned int count;
    std::cout << "\nВведите количество для списания: ";
    Getline(count);

    if (count == 0) {
        std::cerr << "Ошибка: количество должно быть больше 0.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    if (it->count < count) {
        std::cerr << "Ошибка: на складе только " << it->count << " шт. товара.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "\n\n\n\t\t\tПРИЧИНА СПИСАНИЯ\n\n\n";
    std::cout << "1) Брак\n";
    std::cout << "2) Порча\n";
    std::cout << "3) Истечение срока годности\n";
    std::cout << "4) Другое\n";
    std::cout << "Выберите причину: ";

    std::string reason;
    Getline(reason);

    std::string reason_str;
    switch(std::stoi(reason)) {
    case 1:
        reason_str = "Брак";
        break;
    case 2:
        reason_str = "Порча";
        break;
    case 3:
        reason_str = "Истек срок годности";
        break;
    default:
        reason_str = "Ручное списание";
        break;
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "\n\n\n\t\t\tПОДТВЕРЖДЕНИЕ\n\n\n";
    std::cout << "Вы уверены, что хотите списать:\n";
    std::cout << "Товар: " << it->name << "\n";
    std::cout << "Количество: " << count << " шт.\n";
    std::cout << "Причина: " << reason_str << "\n";
    std::cout << "Подтвердить? (y/n): ";

    std::string confirm;
    Getline(confirm);

    if (confirm != "y" && confirm != "Y") {
        std::cerr << "Списание отменено.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    it->count -= count;

    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::floor<std::chrono::days>(now);
    std::string date_str = date_to_string(today);

    std::string user_name = "Администратор";
    if (authSystem && authSystem->getCurrentUser()) {
        user_name = authSystem->getCurrentUser()->username;
    }

    Check writeoff(TransactionType::WriteOff, it->name, it->price,
                   count, user_name + " (" + reason_str + ")", date_str);
    writeoff.save_check("Checks.txt");

    save_to_file("Product.txt");

    std::cout << "\nСписание прошло успешно!\n";
    std::cout << "Остаток на складе: " << it->count << " шт.\n";

    if (it->count == 0) {
        goods.erase(it);
        save_to_file("Product.txt");
        std::cout << "Товар полностью списан и удалён из базы.\n";
    }

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

    std::vector<Product> display_goods;
    for (const auto& p : goods) {
        if (p.count > 0) {
            display_goods.push_back(p);
        }
    }

    if (display_goods.empty()) {
        std::cout << "Склад пуст.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    const int w_id = 4;
    const int w_name = 18;
    const int w_cat = 14;
    const int w_price = 10;
    const int w_count = 8;
    const int w_art = 10;
    const int w_expiry = 12;

    int total_width = w_id + w_name + w_cat + w_price + w_count + w_art + w_expiry + 13;

    std::cout << "\n" << std::setfill('=') << std::setw(total_width) << "" << std::setfill(' ') << "\n";
    std::cout << std::left
              << "| " << std::setw(w_id) << "ID"
              << "| " << std::setw(w_name) << "Название"
              << "| " << std::setw(w_cat) << "Категория"
              << "| " << std::setw(w_price) << "Цена"
              << "| " << std::setw(w_count) << "Кол-во"
              << "| " << std::setw(w_art) << "Артикль"
              << "| " << std::setw(w_expiry) << "Годен до"
              << "|\n";
    std::cout << std::setfill('-') << std::setw(total_width) << "" << std::setfill(' ') << "\n";

    for (size_t i = 0; i < display_goods.size(); ++i) {
        const Product& p = display_goods[i];
        std::cout << std::left
                  << "| " << std::setw(w_id) << i + 1
                  << "| " << std::setw(w_name) << format_field(p.name, w_name - 1)
                  << "| " << std::setw(w_cat) << format_field(p.category, w_cat - 1)
                  << "| " << std::setw(w_price) << std::fixed << std::setprecision(2) << p.price
                  << "| " << std::setw(w_count) << p.count
                  << "| " << std::setw(w_art) << p.article
                  << "| " << std::setw(w_expiry) << p.end_date
                  << "|\n";
    }
    std::cout << std::setfill('=') << std::setw(total_width) << "" << std::setfill(' ') << "\n\n";

    std::cout << "Нажмите Enter для продолжения: ";
    std::cin.get();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
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
    show_all_for_actions();
    int article;
    std::cout << "Введите артикль товара: ";
    Getline(article);
    auto it = std::find_if(goods.begin(), goods.end(),
                           [&](const Product& p) { return p.article == article; });
    if (it == goods.end()) {
        std::cerr << "Товар не найден\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    std::cout << "Текущая цена '" << it->name << "': " << it->price << " руб.\n";
    double new_price;
    std::cout << "Введите новую цену: ";
    Getline(new_price);
    if (new_price <= 0.0 || new_price > 10000.0) {
        std::cerr << "Ошибка: цена должна быть от 1 до 10000 руб.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    it->price = new_price;
    std::cout << "Цена изменена на " << it->price << " руб.\n";
    save_to_file("Product.txt");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
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
    for (const auto& p : goods) {
        if (is_expired(p.end_date)) {
            std::cout << "Товар просрочен: " << p.name
                      << " (Артикль: " << p.article
                      << ", Годен до: " << p.end_date << ")\n";
            expired_count++;
        }
    }
    if (expired_count == 0) {
        std::cout << "Просроченных товаров нет\n";
    }
    else {
        std::cout << "Всего просроченных товаров: " << expired_count << "\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Storage::show_valid_products() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    if (goods.empty()) {
        std::cerr << "Список товаров пустой" << std::endl;
        return;
    }
    int valid_count = 0;
    for (const auto& p : goods) {
        if (!is_expired(p.end_date)) {
            std::cout << "Название: " << p.name
                      << " | Категория: " << p.category
                      << " | Цена: " << p.price
                      << " | Артикль: " << p.article
                      << " | Годен до: " << p.end_date
                      << " | Количество: " << p.count << "\n";
            valid_count++;
        }
    }
    if (valid_count == 0) {
        std::cerr << "Нет годных товаров на складе\n";
    }
    else {
        std::cout << "Всего годных товаров: " << valid_count << "\n";
    }
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
        Getline(choice, true);

        if(choice.size() > 2) {
            std::cerr << "Ошибка: введите номер действия\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choice.empty()) {
            std::cerr << "Ошибка: введите номер действия\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (std::stoi(choice) < 0 || std::stoi(choice) > 5) {
            std::cerr << "Ошибка! Введите цифру от 0 до 5.\n";
            continue;
        }

        bool is_valid = true;
        for (char c : choice) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) {
            std::cerr << "Ошибка: введите только цифры от 0 до 6\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        switch (std::stoi(choice)) {
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
            is_exit = true;
            break;
        default:
            std::cout << "Неверное действие\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        }
        if (is_exit) break;

    }
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Storage::edit_storage_menu(std::string status) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    bool is_exit = false;

    if(status == "superadmin") {
        while (true) {
            std::cout << "\n\n\n\t\t\tРЕДАКТИРОВАНИЕ СКЛАДА\n\n\n";
            std::cout << "1) Добавить новый товар\n";
            std::cout << "2) Удалить товар по артикулу\n";
            std::cout << "3) Изменить название товара по артикулу\n";
            std::cout << "4) Пополнить количество товара по артикулу\n";
            std::cout << "5) Изменить цену товара\n";
            std::cout << "6) Списать товар со склада\n";
            std::cout << "0) Вернуться в главное меню\n";
            std::cout << "Выбор: ";
            std::string choice;
            Getline(choice, true);

            if (choice.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }

            bool is_valid = true;
            for (char c : choice) {
                if (!std::isdigit(static_cast<unsigned char>(c))) {
                    is_valid = false;
                    break;
                }
            }

            if (!is_valid) {
                std::cerr << "Ошибка: введите только цифры от 0 до 6\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }

            if (std::stoi(choice) < 0 || std::stoi(choice) > 6) {
                std::cerr << "Ошибка! Введите цифру от 0 до 6.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }
            switch (std::stoi(choice)) {
            case 1:
                add_product();
                break;
            case 2:
                delete_product();
                break;
            case 3:
                change_product_name();
                break;
            case 4:
                replenish_product();
                break;
            case 5:
                change_product_price();
                break;
            case 6:
                writeoff_product();
                break;
            case 0:
                is_exit = true;
                break;
            default:
                std::cerr << "Неверное действие\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                break;
            }

            if(is_exit) break;

        }
    } else {
        while (true) {
            std::cout << "\n\n\n\t\t\tРЕДАКТИРОВАНИЕ СКЛАДА\n\n\n";
            std::cout << "1) Добавить новый товар\n";
            std::cout << "2) Изменить цену товара\n";
            std::cout << "3) Списать товар со склада\n";
            std::cout << "0) Вернуться в главное меню\n";
            std::cout << "Выбор: ";
            std::string choice;
            Getline(choice, true);

            if(choice.size() > 2) {
                std::cerr << "Ошибка: введите номер действия\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }

            if (choice.empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }

            bool is_valid = true;
            for (char c : choice) {
                if (!std::isdigit(static_cast<unsigned char>(c))) {
                    is_valid = false;
                    break;
                }
            }

            if (!is_valid) {
                std::cerr << "Ошибка: введите только цифры от 0 до 6\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }

            if (std::stoi(choice) < 0 || std::stoi(choice) > 3) {
                std::cerr << "Ошибка! Введите цифру от 0 до 3.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }
            switch (std::stoi(choice)) {
            case 1:
                add_product();
                break;
            case 2:
                change_product_price();
                break;
            case 3:
                writeoff_product();
                break;
            case 0:
                is_exit = true;
                break;
            default:
                std::cerr << "Неверное действие\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                break;
            }

            if(is_exit) break;
        }
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Storage::delete_product() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    load_from_file("Product.txt");
    show_all_for_actions();

    if (goods.empty()) {
        std::cerr << "Склад пуст.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    int article;
    std::cout << "Введите артикль товара для удаления: ";
    Getline(article);
    auto it = std::find_if(goods.begin(), goods.end(),
                           [article](const Product& p) { return p.article == article; });
    if (it == goods.end()) {
        std::cerr << "Товар с артиклем " << article << " не найден.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    std::cout << "Удалить товар '" << it->name << "' (арт. " << it->article << ")? (y/n): ";
    bool confirm;
    Getline(confirm);
    if (confirm) {
        auto now = std::chrono::system_clock::now();
        auto today = std::chrono::floor<std::chrono::days>(now);
        std::string date_str = date_to_string(today);
        Check writeoff(TransactionType::WriteOff, it->name, it->price, it->count, "Администратор", date_str);
        writeoff.save_check("Checks.txt");

        goods.erase(it);
        save_to_file("Product.txt");
        std::cout << "Товар удалён.\n";
    }
    else {
        std::cout << "Удаление отменено.\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Storage::change_product_name() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    load_from_file("Product.txt");
    show_all_for_actions();

    if (goods.empty()) {
        std::cerr << "Склад пуст.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    int article;
    std::cout << "Введите артикль товара для изменения названия: ";
    Getline(article);
    auto it = std::find_if(goods.begin(), goods.end(),
                           [article](const Product& p) { return p.article == article; });
    if (it == goods.end()) {
        std::cerr << "Товар не найден.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    std::cout << "Текущее название: " << it->name << "\n";
    std::string new_name;
    std::cout << "Введите новое название (не более 60 символов): ";
    Getline(new_name);
    if (new_name.empty() || new_name.size() > 60) {
        std::cerr << "Некорректное название.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    it->name = new_name;
    save_to_file("Product.txt");
    std::cout << "Название изменено.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Storage::replenish_product() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    load_from_file("Product.txt");
    show_all_for_actions();

    if (goods.empty()) {
        std::cerr << "Склад пуст.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    int article;
    std::cout << "Введите артикль товара для пополнения: ";
    Getline(article);
    auto it = std::find_if(goods.begin(), goods.end(),
                           [article](const Product& p) { return p.article == article; });
    if (it == goods.end()) {
        std::cerr << "Товар не найден.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    unsigned int add_count;
    std::cout << "Текущее количество: " << it->count << "\nВведите количество для добавления от 1 до 199: ";
    Getline(add_count);
    if (add_count < 1 || add_count > 199) {
        std::cerr << "Некорректное количество.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    if (it->count + add_count > 199) {
        std::cerr << "Превышение максимального количества (199). Операция отменена.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    it->count += add_count;
    save_to_file("Product.txt");
    std::cout << "Количество увеличено. Теперь: " << it->count << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Storage::auto_writeoff_expired() {
    bool any_expired = false;
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::floor<std::chrono::days>(now);
    for (auto it = goods.begin(); it != goods.end();) {
        if (is_expired(it->end_date)) {
            std::cout << "Списание просроченного товара: " << it->name
                      << " (арт. " << it->article << "), кол-во: " << it->count << "\n";
            std::string date_str = date_to_string(today);
            Check writeoff(TransactionType::WriteOff, it->name, it->price, it->count, "Система", date_str);
            writeoff.save_check("Checks.txt");
            it = goods.erase(it);
            any_expired = true;
        }
        else {
            ++it;
        }
    }
    if (any_expired) {
        save_to_file("Product.txt");
    }
}

std::chrono::sys_days Storage::parse_date(const std::string& date_str) {
    int day, month, year;
    char dot1, dot2;
    std::istringstream ss(date_str);
    ss >> day >> dot1 >> month >> dot2 >> year;
    if (year < 100) year += 2000;
    return std::chrono::sys_days{
        std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day)
    };
}

std::string Storage::format_field(std::string str, std::size_t width) const {
    if (str.length() > width) return str.substr(0, width - 2) + "..";
    return str;
}

bool Storage::is_expired(const std::string& end_date_str) {
    try {
        auto end_date = parse_date(end_date_str);
        auto now = std::chrono::system_clock::now();
        auto today = std::chrono::floor<std::chrono::days>(now);
        return end_date < today;
    }
    catch (...) {
        return true;
    }
}

bool Storage::is_date_range_valid(const std::string& begin_date_str, const std::string& end_date_str) {
    try {
        auto begin = parse_date(begin_date_str);
        auto end = parse_date(end_date_str);
        return begin <= end;
    }
    catch (...) {
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
    if (!is_date_range_valid(begin_date, end_date)) return false;

    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::floor<std::chrono::days>(now);
    auto begin = parse_date(begin_date);
    auto end = parse_date(end_date);

    if (begin > today) {
        std::cerr << "Ошибка: дата начала не может быть в будущем.\n";
        return false;
    }
    auto min_date = parse_date("01.01.2026");
    if (begin < min_date) {
        std::cerr << "Ошибка: дата начала слишком старая (должна быть не ранее 01.01.2026).\n";
        return false;
    }
    if (end < today) {
        auto days_ago = std::chrono::duration_cast<std::chrono::days>(today - end).count();
        std::cerr << "Ошибка: товар уже просрочен на " << days_ago << " дней.\n";
        return false;
    }
    auto days_left = std::chrono::duration_cast<std::chrono::days>(end - today).count();
    if (days_left <= 30 && days_left > 0) {
        std::cout << "Внимание: срок годности истекает через " << days_left << " дней!\n";
    }
    return true;
}

std::string Storage::date_to_string(std::chrono::sys_days tp) const {
    auto ymd = std::chrono::year_month_day(tp);
    if (!ymd.ok())
        return "01.01.2026";

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << static_cast<unsigned>(ymd.day()) << "."
       << std::setw(2) << std::setfill('0') << static_cast<unsigned>(ymd.month()) << "."
       << static_cast<int>(ymd.year());
    return ss.str();
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
