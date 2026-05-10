#include "Storage.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include "AuthSystemUser.h"
#include "methodsSuperAdmin.h"
#include <cctype>

void Storage::setAuthSystem(AuthSystemUser* auth)
{
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
        std::cout << "Было считано товаров с базы данных: " << goods.size() << std::endl;
    }

void Storage::save_to_file(const std::string& product_database) {
    std::ofstream output(product_database);
    if (!output.is_open()) {
         std::cerr << "Ошибка открытия файла" << std::endl;
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
    while (true)
    {
        std::cout << "Список действий для супер админа:\n";
        std::cout << "1) Добавить продажу\n";
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
        Getline(choose);
        if (choose.size() != 1 || choose[0] < '0' || choose[0] > '9') {
            std::cerr << "Ошибка! Введите одну цифру от 0 до 9.\n";
            system("pause");
            continue;
        }
    
        if (choose == "0") break;   // выход
        else if (choose == "1") { /* продажа */ }
        else if (choose == "2") show_all_from_file("Product.txt");
        else if (choose == "3") add_product();
        else if (choose == "4") { /* списание */ }
        else if (choose == "5") { /* изменение цены */ }
        else if (choose == "6") { /* редактирование склада – пока пусто */ }
        else if (choose == "7") StorageUserMethod(authSystem);
        else if (choose == "8") { /* отчёт */ }
        else if (choose == "9") { /* поставки */ }
        else std::cerr << "Ошибка ввода\n";
    }
}
void Storage::admin_menu() {
    while (true)
    {
        std::cout << "Список действий для админа:\n";
        std::cout << "1) Начать продажу\n";
        std::cout << "2) Показать склад\n";
        std::cout << "3) Пополнить склад\n";
        std::cout << "4) Пополнить товар\n";
        std::cout << "5) Списать товар\n";
        std::cout << "6) Отчет\n";
        std::cout << "7) Поставки\n";
        std::cout << "0) Выход\n";;
        std::cout << "Выберите действие: ";
        std::string choose;
        Getline(choose);
        if (choose.size() != 1 || choose[0] < '0' || choose[0] > '7') {
            std::cerr << "Ошибка! Введите одну цифру от 0 до 7.\n";
            system("pause");
            continue;
        }
        if (choose == "0") break;
        else if (choose == "1") { /* продажа */ }
        else if (choose == "2") show_all_from_file("Product.txt");
        else if (choose == "3") add_product();
        else if (choose == "4") { /* списание */ }
        else if (choose == "5") { /* изменение цены */ }
        else if (choose == "6") { /* отчёт */ }
        else if (choose == "7") { /* поставки */ }
        else std::cerr << "Ошибка ввода\n";
    }
}
void Storage::user_menu() {
    while (true)
    {
        std::cout << "Список действий для пользователя:\n";
        std::cout << "1) Начать продажу\n";
        std::cout << "2) Показать склад\n";
        std::cout << "3) Отчет\n";
        std::cout << "4) Поставки\n";
        std::cout << "0) Выход\n";
        std::cout << "Выберите действие: ";
        std::string choose;
        Getline(choose);
        if (choose.size() != 1 || choose[0] < '0' || choose[0] > '4') {
            std::cerr << "Ошибка! Введите одну цифру от 0 до 4.\n";
            system("pause");
            continue;
        }
        if (choose == "0") break;
        else if (choose == "1") { /* продажа */ }
        else if (choose == "2") show_all_from_file("Product.txt");
        else if (choose == "3") { /* отчёт */ }
        else if (choose == "4") { /* поставки */ }
        else std::cerr << "Ошибка ввода\n";
    }
}
void Storage::start(const std::string& userStatus)
{
    load_from_file("Product.txt");
    while (true)
    {
  
   
        if (userStatus == "superadmin")
        {
            super_admin_menu();
        }
        else if (userStatus == "admin")
        {
            admin_menu();
        }
        else
        {
            user_menu();
        }
        std::cout << "Выбор действия: ";
        std::string choose;
        Getline(choose);
          
    }
}



void Storage::show_all() 
{
  
if (goods.empty()) {
    std::cout << "Склад пуст." << std::endl;
    return;
}
    // Константы ширины колонок
    const int w_ID = 4, w_name = 16, w_cat = 14, w_price = 10, w_count = 8, w_art = 12;

    std::cout << "\n" << std::setfill('=') << std::setw(80) << "" << std::setfill(' ') << "\n";
    std::cout << std::left
        << std::setw(w_ID) << "ID"
        << std::setw(w_name) << "Название"
        << std::setw(w_cat) << "Категория"
        << std::setw(w_price) << "Цена"
        << std::setw(w_count) << "Кол-во"
        << std::setw(w_art) << "Артикль" << "\n";
    std::cout << std::setfill('-') << std::setw(80) << "" << std::setfill(' ') << "\n";

    for (std::size_t i = 0; i < goods.size(); ++i) {
        Product& p = goods[i];

        std::cout << std::left
            << std::setw(w_ID) << i + 1
            << std::setw(w_name) << format_field(p.name, w_name - 1)
            << std::setw(w_cat) << format_field(p.category, w_cat - 1)
            << std::setw(w_price) << std::fixed << std::setprecision(2) << p.price
            << std::setw(w_count) << p.count
            << p.article << "\n";
    }
    std::cout << std::setfill('=') << std::setw(80) << "" << std::setfill(' ') << "\n\n";
}

void Storage::show_all_from_file(const std::string& filename)
{
    load_from_file(filename);
    show_all();
}


void Storage::show_valid_products() {
    if (goods.empty()) {
        std::cerr << "Список товаров пустой" << std::endl;
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
        std::cout << "Нет годных товаров на складе\n";
    } else {
        std::cout << "Всего годных товаров: " << valid_count << "\n";
    }
}
void Storage::check_expired_products() {
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
    } else {
        std::cout << "Всего просроченных товаров: " << expired_count << "\n";
    }
}

std::chrono::sys_days Storage::parse_date(const std::string& date_str) {
    int day, month, year;
    char delim1, delim2;

    std::istringstream ss(date_str);
    ss >> day >> delim1 >> month >> delim2 >> year;

    if (year < 100) {
        year += 2000;
    }

    return std::chrono::sys_days(
        std::chrono::year(year) / std::chrono::month(month) / std::chrono::day(day)
        );
}
std::string Storage::format_field(std::string str, std::size_t width) const {
    if (str.length() > width) {
        return str.substr(0, width - 2) + "..";
    }
    return str;
}
bool Storage::is_expired(const std::string& end_date_str) {
    try {
        auto now = std::chrono::system_clock::now();
        auto today = now;
        auto end_date = parse_date(end_date_str);

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

        if (begin_date > end_date) {
            std::cerr << "Дата начала не может быть позже даты окончания!\n";
            return false;
        }

        return true;
    } catch (...) {
        std::cerr << "Неверный формат даты! Используйте ДД.ММ.ГГГГ\n";
        return false;
    }
}
bool Storage::validate_dates(const std::string& begin_date, const std::string& end_date) {
    if (!is_date_range_valid(begin_date, end_date)) {
        return false;
    }
    if (is_expired(end_date)) {
        auto today = std::chrono::system_clock::now();
        auto get_end_date = parse_date(end_date);
        auto days_ago = std::chrono::duration_cast<std::chrono::days>(today - get_end_date).count();
        std::cerr << "Ошибка: Товар просрочен на " << days_ago
                    << " дней (срок годности истек " << end_date << ")\n";
        return false;
    }
    auto get_end_date = parse_date(end_date);
    auto today = std::chrono::system_clock::now();
    auto days_left = duration_cast<std::chrono::days>(get_end_date - today).count();
    if (days_left <= 30 && days_left > 0) {
        std::cout << "Срок годности истекает через " << days_left << " дней!\n";
    }
    return true;
}
void Storage::add_product() {
    Product new_product;

    
    while (true) {
        std::cout << "Название товара (пустая строка или 'exit' для отмены): ";
        Getline(new_product.name);
        if (new_product.name.empty() || new_product.name == "exit") {
            std::cerr << "Операция добавления отменена.\n";
            return;
        }
        if (new_product.name.size() > 60) {
            std::cerr << "Название слишком длинное (максимум 60 символов).\n";
            continue;
        }
        break;
    }

   
    while (true) {
        std::cout << "Категория (пустая строка или 'exit' для отмены): ";
        Getline(new_product.category);
        if (new_product.category.empty() || new_product.category == "exit") {
            std::cerr << "Операция добавления отменена.\n";
            return;
        }
        if (new_product.category.size() > 50) {
            std::cerr << "Категория слишком длинная (максимум 50 символов).\n";
            continue;
        }
        break;
    }

    // ---- ЦЕНА ----
    while (true) {
        std::cout << "Цена (0 – отмена): ";
        std::cin >> new_product.price;
        std::cin.ignore();
        if (std::cin.fail() || new_product.price < 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "Некорректная цена. Введите положительное число.\n";
            continue;
        }
        if (new_product.price == 0) {
            std::cerr << "Операция добавления отменена.\n";
            return;
        }
        if (new_product.price > 10000) {
            std::cerr << "Цена не может превышать 10000.\n";
            continue;
        }
        break;
    }

    // ---- АРТИКУЛ ----
    while (true) {
        std::cout << "Артикул (целое число, 0 - отмена): ";
        std::cin >> new_product.article;
        std::cin.ignore();
        if (std::cin.fail() || new_product.article < 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "Некорректный артикул.\n";
            continue;
        }
        if (new_product.article == 0) {
            std::cerr << "Операция добавления отменена.\n";
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
            return;
        }

        std::cout << "Окончание срока годности (ДД.ММ.ГГГГ): ";
        Getline(new_product.end_date);
        if (new_product.end_date.empty() || new_product.end_date == "exit") {
            std::cerr << "Операция добавления отменена.\n";
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
        std::cin >> new_product.count;
        std::cin.ignore();
        if (std::cin.fail() || new_product.count < 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "Некорректное количество.\n";
            continue;
        }
        if (new_product.count == 0) {
            std::cerr << "Операция добавления отменена.\n";
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
            return;
        }
        if (new_product.manufacturer.size() > 60) {
            std::cerr << "Название производителя слишком длинное (максимум 60 символов).\n";
            continue;
        }
        break;
    }

 
    while (true) {
        std::cout << "Страна производства (пустая строка или 'exit' для отмены): ";
        Getline(new_product.country);
        if (new_product.country.empty() || new_product.country == "exit") {
            std::cerr << "Операция добавления отменена.\n";
            return;
        }
        if (new_product.country.size() > 60) {
            std::cerr << "Название страны слишком длинное (максимум 60 символов).\n";
            continue;
        }
        break;
    }
    

    goods.push_back(new_product);
    std::cout << "Товар '" << new_product.name << "' успешно добавлен.\n";

    save_to_file("Product.txt");
}