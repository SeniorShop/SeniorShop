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
bool Storage::is_existed_name_storage(std::string& check_name_storage) {
    if(!std::filesystem::exists("StorageDatabase.txt"))
        return false;

    std::ifstream check_exist_storage("StorageDatabase.txt");
    if(!check_exist_storage.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return false;
    }

    std::string name_storage, line;
    bool found = false;

    while(std::getline(check_exist_storage, line)) {
        if(line.empty()) continue;

        std::stringstream ss(line);
        std::string id_str;
        if(std::getline(ss, id_str, '|') && std::getline(ss, name_storage)) {
            if(name_storage == check_name_storage) {
                found = true;
                break;
            }
        }
    }

    check_exist_storage.close();
    return found;
}
void Storage::list_superadmin_actions() {
    //storage = storages[select_storage()];
    std::size_t select_action;
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
    std::cout << "Выберите действие: ";
    std::cin >> select_action;

    // реализация будет позже
}
void Storage::list_admin_actions() {
    std::size_t select_action;
    std::cout << "Список действий для админа:\n";
    std::cout << "1) Начать продажу\n";
    std::cout << "2) Показать склад\n";
    std::cout << "3) Пополнить склад\n";
    std::cout << "4) Пополнить товар\n";
    std::cout << "5) Списать товар\n";
    std::cout << "6) Изменить цену\n";
    std::cout << "7) Отчет\n";
    std::cout << "8) Поставки\n";
    std::cout << "9) Выход\n";
    std::cout << "Выберите действие: ";
    std::cin >> select_action;

    // реализация будет позже
}
void Storage::list_similar_user_actions() {
    std::size_t select_action;
    std::cout << "Список действий для пользователя:\n";
    std::cout << "1) Начать продажу\n";
    std::cout << "2) Показать склад\n";
    std::cout << "3) Пополнить склад\n";
    std::cout << "4) Списать товар\n";
    std::cout << "5) Отчет\n";
    std::cout << "6) Выход\n";
    std::cout << "Выберите действие: ";
    std::cin >> select_action;

    // реализация будет позже
}
bool Storage::is_existed_ID_storage(int& check_ID_storage) {
    if(!std::filesystem::exists("StorageDatabase.txt"))
        return false;

    std::ifstream check_exist_ID_storage("StorageDatabase.txt");
    if(!check_exist_ID_storage.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return false;
    }

    std::string line;
    bool found = false;

    while(std::getline(check_exist_ID_storage, line)) {
        if(line.empty()) continue;

        std::stringstream ss(line);
        std::string id_str;

        if(std::getline(ss, id_str, '|')) {
            try {
                int id = std::stoi(id_str);
                if(id == check_ID_storage) {
                    found = true;
                    break;
                }
            } catch(...) {
                continue;
            }
        }
    }

    check_exist_ID_storage.close();
    return found;
}
int Storage::select_storage() {
    std::string name_storage;
    std::size_t type_storages;
    int ID_storage = 0, result = -1;

    std::cout << std::setw(60) << std::right << "ВЫБОР СКЛАДА" << std::endl;
    std::cout << "1) Новый склад (с нуля)" << std::endl;
    std::cout << "2) Существующий склад" << std::endl;
    std::cout << "Выберите тип склада: ";
    std::cin >> type_storages;

    std::cin.ignore(365000, '\n');

    if(type_storages == 1) {
        std::cout << "Введите название нового склада: ";
        Getline(name_storage);

        if(name_storage.empty()) {
            std::cerr << "Название склада не может быть пустым!" << std::endl;
            return -1;
        }

        if(is_existed_name_storage(name_storage)) {
            std::cerr << "Склад с названием '" << name_storage << "' уже существует!" << std::endl;
            return -1;
        }

        std::cout << "Введите ID нового склада в виде положительного числа: ";
        std::cin >> ID_storage;
        std::cin.ignore(365000, '\n');

        if(ID_storage <= 0) {
            std::cerr << " ID склада должен быть положительным числом!" << std::endl;
            return -1;
        }

        if(is_existed_ID_storage(ID_storage)) {
            std::cerr << "Ошибка: склад с ID " << ID_storage << " уже существует!" << std::endl;
            return -1;
        }

        if(save_storage_to_database(name_storage, ID_storage)) {
            std::cout << "Склад '" << name_storage << "' с ID " << ID_storage << " успешно создан!" << std::endl;
            result = ID_storage;
        } else {
            result = -1;
        }
    }
    else if(type_storages == 2) {
        result = load_storage_from_database();
    }
    else {
        std::cerr << "Ошибка: неверный выбор (1 или 2)!" << std::endl;
        return -1;
    }

    return result;
}
bool Storage::save_storage_to_database(const std::string& storage_name, const std::size_t& ID) {
    std::ofstream storage_database("StorageDatabase.txt", std::ios::app);
    if(!storage_database.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return false;
    }

    storage_database << ID << "|" << storage_name << std::endl;
    storage_database.close();
    std::cout << "Склад '" << storage_name << "' с ID '" << ID << "' создан успешно" << std::endl;
    return true;
}

int Storage::load_storage_from_database() {
    if(!std::filesystem::exists("StorageDatabase.txt")) {
        std::cerr << "База данных складов не найдена" << std::endl;
        return -1;
    }

    std::ifstream storage_database("StorageDatabase.txt");
    if(!storage_database.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return -1;
    }

    std::string name_storage, line;
    std::map<int, std::string> get_storages;

    std::cout << "Список складов:\n";

    while(std::getline(storage_database, line)) {
        if(line.empty()) continue;

        std::stringstream ss(line);
        std::string id_str, name;

        if(std::getline(ss, id_str, '|') && std::getline(ss, name)) {
            try {
                int id = std::stoi(id_str);
                get_storages[id] = name;
                std::cout << "ID: " << id << " | Название: " << name << std::endl;
            } catch(...) {
                continue;
            }
        }
    }
    storage_database.close();

    if(get_storages.empty()) {
        std::cerr << "Нет доступных складов" << std::endl;
        return -1;
    }
    std::cout << "Выберите ID склада: ";

    int selected_id;
    std::cin >> selected_id;
    std::cin.ignore(365000, '\n');

    if(get_storages.find(selected_id) != get_storages.end()) {
        std::cout << "Выбран склад: " << get_storages[selected_id] << " (ID: " << selected_id << ")" << std::endl;
        return selected_id;
    } else {
        std::cerr << "Склад с ID " << selected_id << " не существует" << std::endl;
        return -1;
    }
}
void Storage::start(const std::string& get_status) {
    if(get_status == "superadmin") {
        if(select_storage() == -1)
            return;
        list_superadmin_actions();
    } else if(get_status == "admin") {
        list_admin_actions();
    } else {
        list_similar_user_actions();
    }
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
    auto days_left = std::chrono::duration_cast<std::chrono::days>(get_end_date - today).count();
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

