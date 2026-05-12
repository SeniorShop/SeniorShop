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
    std::cout << "Сколько поставок записать: ";
    Getline(count);

    for (int i = 0; i < count; ++i) {
        Supply s;
        std::cout << "Поставка под номером: " << (i + 1) << "\n";

        std::cout << "Номер поставки: ";
        Getline(s.number_supply);
        std::cout << "От кого поставка (поставщик): ";
        Getline(s.name_user, false);

        std::string d;
        std::cout << "Дата выгрузки (день.месяц.год): ";
        Getline(d);
        s.date = Supply::string_to_date(d);
        std::cout << "Дата принятия (день.месяц.год): ";
        Getline(d);
        s.date_acception = Supply::string_to_date(d);
        std::cout << "Дата обработки (день.месяц.год): ";
        Getline(d);
        s.date_processing = Supply::string_to_date(d);

        std::cout << "Кто взял поставку (ФИО пользователя): ";
        Getline(s.responsible_person, false);

        std::cout << "Товар в поставке:\n";
        std::cout << "  Название: ";
        Getline(s.product_name.name, false);
        std::cout << "  Категория: ";
        Getline(s.product_name.category, false);
        std::cout << "  Цена: ";
        Getline(s.product_name.price);
        std::cout << "  Артикль: ";
        Getline(s.product_name.article);
        std::cout << "  Начало срока (день.месяц.год): ";
        Getline(s.product_name.begin_date);
        std::cout << "  Конец срока (день.месяц.год): ";
        Getline(s.product_name.end_date);
        std::cout << "  Количество: ";
        Getline(s.product_name.count);
        std::cout << "  Производитель: ";
        Getline(s.product_name.manufacturer, false);
        std::cout << "  Страна: ";
        Getline(s.product_name.country, false);

        s.is_actually = true;
        s.status = "Ожидается";

        supplies.push_back(s);
        add_to_file(s);
        std::cout << "Поставка под номером: " << s.number_supply << " добавлена\n";
    }
    std::cout << "Добавлено поставок: " << count << std::endl;
}

void SupplyManager::show_all_supplies() {
    if (supplies.empty()) {
        std::cout << "Нет поставок\n";
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
}

void SupplyManager::delete_supply_from_file() {
    unsigned int num;
    std::cout << "Номер поставки для удаления: ";
    Getline(num);

    auto it = std::find_if(supplies.begin(), supplies.end(),
                           [num](const Supply& s) { return s.number_supply == num; });

    if (it == supplies.end()) {
        std::cerr << "Поставка не найдена\n";
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
}
