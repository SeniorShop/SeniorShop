#include "SupplyManager.h"
#include "Storage.h"
#include "Product.h"
#include <fstream>
#include <sstream>
#include <algorithm>

SupplyManager::SupplyManager() {
    load_from_file();
}

void SupplyManager::load_from_file() {
    supplies.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        Supply s;
        std::string date_str, date_acc_str, date_proc_str;
        std::string responsible;
        int act_int;

        ss >> s.number_supply; ss.ignore();
        std::getline(ss, s.name_user, '|');
        std::getline(ss, date_str, '|');
        std::getline(ss, date_acc_str, '|');
        std::getline(ss, date_proc_str, '|');
        std::getline(ss, responsible, '|');
        std::getline(ss, s.product_name.name, '|');
        std::getline(ss, s.product_name.category, '|');
        ss >> s.product_name.price; ss.ignore();
        ss >> s.product_name.article; ss.ignore();
        std::getline(ss, s.product_name.begin_date, '|');
        std::getline(ss, s.product_name.end_date, '|');
        ss >> s.product_name.count; ss.ignore();
        std::getline(ss, s.product_name.manufacturer, '|');
        std::getline(ss, s.product_name.country, '|');
        ss >> act_int; ss.ignore();
        std::getline(ss, s.status);

        s.date = Supply::string_to_date(date_str);
        s.date_acception = Supply::string_to_date(date_acc_str);
        s.date_processing = Supply::string_to_date(date_proc_str);

        s.responsible_person = responsible;
        s.is_actually = (act_int == 1);

        supplies.push_back(s);
    }
}

void SupplyManager::add_to_file(const Supply& s) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return;

    file << s.number_supply << "|"
         << s.name_user << "|"
         << Supply::date_to_string(s.date) << "|"
         << Supply::date_to_string(s.date_acception) << "|"
         << Supply::date_to_string(s.date_processing) << "|"
         << s.responsible_person << "|"
         << s.product_name.name << "|"
         << s.product_name.category << "|"
         << s.product_name.price << "|"
         << s.product_name.article << "|"
         << s.product_name.begin_date << "|"
         << s.product_name.end_date << "|"
         << s.product_name.count << "|"
         << s.product_name.manufacturer << "|"
         << s.product_name.country << "|"
         << (s.is_actually ? 1 : 0) << "|"
         << s.status << "\n";

    file.close();
}

void SupplyManager::create_supplies() {
    int count;
    std::cout << "Сколько поставок записать от 1 до 5: ";
    Getline(count);

    if (count < 1 || count > 5) {
        std::cerr << "Ошибка: количество поставок должно быть от 1 до 10\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    for (int i = 0; i < count; ++i) {
        Supply s;
        std::cout << "\n\n\n\t\t\tПоставка под номером: " << (i + 1) << "\n\n\n";

        unsigned int new_number;
        while (true) {
            std::cout << "Номер поставки: ";
            Getline(new_number);

            bool exists = false;
            for (const auto& existing : supplies) {
                if (existing.number_supply == new_number) {
                    exists = true;
                    std::cerr << "Ошибка: поставка с номером " << new_number << " уже существует\n";
                    break;
                }
            }
            if (!exists) break;
        }
        s.number_supply = new_number;

        std::cout << "От кого поставка (поставщик): ";
        Getline(s.name_user, false);

        std::string d;
        std::cout << "Дата выгрузки (ДД.ММ.ГГГГ): ";
        Getline(d);
        s.date = Supply::string_to_date(d);
        std::cout << "Дата принятия (ДД.ММ.ГГГГ): ";
        Getline(d);
        s.date_acception = Supply::string_to_date(d);
        std::cout << "Дата обработки (ДД.ММ.ГГГГ): ";
        Getline(d);
        s.date_processing = Supply::string_to_date(d);
        std::cout << "Кто взял поставку (ФИО пользователя): ";
        Getline(s.responsible_person, false);

        std::cout << "Добавление товаров в поставку (0 для завершения):\n";
        while (true) {
            Product p;
            std::cout << "Название товара (0 - завершить): ";
            Getline(p.name);
            if (p.name == "0") break;

            std::cout << "Категория: ";
            Getline(p.category, false);
            std::cout << "Цена: ";
            Getline(p.price);
            std::cout << "Артикль: ";
            Getline(p.article);
            std::cout << "Начало срока (ДД.ММ.ГГГГ): ";
            Getline(p.begin_date);
            std::cout << "Конец срока (ДД.ММ.ГГГГ): ";
            Getline(p.end_date);
            std::cout << "Количество: ";
            Getline(p.count);
            std::cout << "Производитель: ";
            Getline(p.manufacturer, false);
            std::cout << "Страна: ";
            Getline(p.country, false);

            s.product_name = p;
            supplies.push_back(s);
            add_to_file(s);
        }

        s.is_actually = true;
        s.status = "Ожидается";

        std::cout << "Поставка под номером: " << s.number_supply << " добавлена\n";
    }
    std::cout << "Добавлено поставок: " << count << std::endl;
}

void SupplyManager::show_all_supplies() {
    if (supplies.empty()) {
        std::cout << "Нет поставок\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    for (const auto& s : supplies) {
        s.print();
    }
}

void SupplyManager::change_supply_from_file() {
    unsigned int num;
    std::cout << "Номер поставки для изменения: ";
    Getline(num);

    for (auto& s : supplies) {
        if (s.number_supply == num) {
            s.print();

            std::string new_status;
            std::cout << "Новый статус (текущий: " << s.status << "): ";
            Getline(new_status);
            if (!new_status.empty()) s.status = new_status;

            std::cout << "Актуальна? (y/n, текущий: " << (s.is_actually ? "Да" : "Нет") << "): ";
            Getline(s.is_actually);

            std::ofstream fout(filename, std::ios::trunc);
            fout.close();
            for (const auto& item : supplies) add_to_file(item);

            std::cout << "Поставка под номером: " << num << " обновлена\n";
            return;
        }
    }
    std::cerr << "Поставка не найдена\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void SupplyManager::delete_supply_from_file() {
    unsigned int num;
    std::cout << "Номер поставки для удаления: ";
    Getline(num);

    auto it = std::find_if(supplies.begin(), supplies.end(),
                           [num](const Supply& s) { return s.number_supply == num; });

    if (it == supplies.end()) {
        std::cerr << "Поставка не найдена\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    bool confirm;
    std::cout << "Удалить поставку под номером: " << num << "? (y/n): ";
    Getline(confirm);

    if (confirm) {
        supplies.erase(it);
        std::ofstream fout(filename, std::ios::trunc);
        fout.close();
        for (const auto& item : supplies) add_to_file(item);
        std::cout << "Поставка под номером: " << num << " удалена\n";
    }
}

void SupplyManager::apply_supply_to_storage(Storage& storage) {
    unsigned int num;
    std::cout << "Номер поставки для применения: ";
    Getline(num);

    for (auto& s : supplies) {
        if (s.number_supply == num && s.is_actually) {
            storage.add_supply_products(s);
            s.is_actually = false;
            s.status = "Выполнена";

            std::ofstream fout(filename, std::ios::trunc);
            fout.close();
            for (const auto& item : supplies) add_to_file(item);

            std::cout << "Поставка под номером: " << num << " применена\n";
            return;
        }
    }
    std::cerr << "Поставка не найдена или не актуальна\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
