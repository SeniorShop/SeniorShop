#include "Storage.h"
#include <chrono>
#include <sstream>
#include <iomanip>

void Storage::load_from_file(const std::string& product_database) {
        std::ifstream input(product_database);
        if (!input.is_open()) {
            std::cerr << "Ошибка открытия файла '" << product_database << "'" << std::endl;
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
void Storage::start() {
    // Реализация будет позже
}
void Storage::add_product() {
    Product new_product;

    std::cout << "Добавление товара\n";
    std::cout << "Название: ";
    Getline(new_product.name);
    std::cout << "Категория: ";
    Getline(new_product.category);
    std::cout << "Цена: ";
    std::cin >> new_product.price;
    std::cout << "Артикль: ";
    std::cin >> new_product.article;
    std::cin.ignore();

    bool dates_valid = false;
    while (!dates_valid) {
        std::cout << "Начало срока годности (ДД.ММ.ГГГГ): ";
        Getline(new_product.begin_date);
        std::cout << "Окончание срока годности (ДД.ММ.ГГГГ): ";
        Getline(new_product.end_date);

        if (!validate_dates(new_product.begin_date, new_product.end_date)) {
            std::cout << "Пожалуйста, введите корректные даты заново.\n";
        } else {
            dates_valid = true;
        }
    }

    std::cout << "Количество: ";
    std::cin >> new_product.count;
    std::cin.ignore();
    std::cout << "Производитель: ";
    Getline(new_product.manufacturer);
    std::cout << "Страна производства: ";
    Getline(new_product.country);

    if(!check_characteristics(new_product)) return;

    goods.push_back(new_product);
    std::cout << "Товар '" << new_product.name << "' добавлен" << std::endl;

    save_to_file("Product.txt");
}

void Storage::show_all() {
    if (goods.empty()) {
        std::cerr << "Список товаров пустой" << std::endl;
        return;
    }
    for (auto i = 0; i < goods.size(); ++i) {
        Product write_product = goods[i];
        std::cout << i + 1 << " | Название: " << write_product.name << " | Категория: " << write_product.category
                      << " | Цена: " << write_product.price << " | Артикль: " << write_product.article
                      << " | Период: " << write_product.begin_date << " - " << write_product.end_date
                      << " | Количество: " << write_product.count << " | Производитель: " << write_product.manufacturer
                      << " | Страна производства: " << write_product.country << '\n';
    }
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
bool Storage::check_characteristics(const Product& analyse_product) {
    auto exist = std::find(goods.begin(), goods.end(), analyse_product);
    if(exist != goods.end()) {
        std::cerr << "Товар уже есть\n";
        return false;
    }

    std::unordered_set<char> available_symbols;
    for(char s = 'A'; s < 'Z'; ++s) available_symbols.insert(s);
    for(char s = '0'; s <= '9'; ++s) available_symbols.insert(s);
    available_symbols.insert('.');
    available_symbols.insert('-');

    for(auto check_name : analyse_product.name) {
        if(!available_symbols.count(check_name)) {
            std::cerr << "Название товара имеет недействительный символ\n";
            return false;
        }
    }
    for(auto& check_category : analyse_product.category) {
        if(!available_symbols.count(check_category)) {
            std::cerr << "Категория товара имеет недействительный символ\n";
            return false;
        }
    }
    for(auto& check_manufacturer : analyse_product.manufacturer) {
        if(!available_symbols.count(check_manufacturer)) {
            std::cerr << "Имя производителя имеет недействительный символ\n";
            return false;
        }
    }
    for(auto& check_begin_date : analyse_product.begin_date) {
        if(!available_symbols.count(check_begin_date)) {
            std::cerr << "Дата начала поставки имеет недействительный символ\n";
            return false;
        }
    }
    for(auto& check_country : analyse_product.country) {
        if(!available_symbols.count(check_country)) {
            std::cerr << "Страна производства поставки имеет недействительный символ\n";
            return false;
        }
    }
    for(auto& check_end_date : analyse_product.end_date) {
        if(!available_symbols.count(check_end_date)) {
            std::cerr << "Дата окончания поставки имеет недействительный символ\n";
            return false;
        }
    }
    if(analyse_product.price <= 0.0 || analyse_product.count <= 0 || analyse_product.article <= 0) {
        std::cerr << "Цена/Количество/Артикль товара не может быть отрицательным или нулевым\n";
        return false;
    }

    return true;
}

