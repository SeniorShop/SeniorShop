#include "SupplyManager.h"
#include "Storage.h"
#include "Product.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

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
    int count, iter = 0;
    std::cout << "Сколько поставок записать от 1 до 3: ";
    Getline(count);

    if (count < 1 || count > 3) {
        std::cerr << "Ошибка: количество поставок должно быть от 1 до 3\n";
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

    Supply s;

    for (int i = 0; i < count; ++i) {
        std::cout << "\n\n\n\t\t\tПоставка под номером: " << (i + 1) << "\n\n\n";

        while (true) {
            std::cout << "Номер поставки: ";
            Getline(s.number_supply);

            bool exists = false;
            for (const auto& existing : supplies) {
                if (existing.number_supply == s.number_supply) {
                    std::cerr << "Ошибка: поставка с номером " << existing.number_supply << " уже существует\n";
                    exists = true;
                    break;
                }
            }
            if (!exists) break;
        }
    }

    std::cout << "От кого поставка (поставщик): ";
    Getline(s.name_user);

    // std::string d;
    // std::cout << "Дата выгрузки (ДД.ММ.ГГГГ): ";
    // Getline(d);
    // s.date = Supply::string_to_date(d);
    // std::cout << "Дата принятия (ДД.ММ.ГГГГ): ";
    // Getline(d);
    // s.date_acception = Supply::string_to_date(d);
    // std::cout << "Дата обработки (ДД.ММ.ГГГГ): ";
    // /Getline(d);
    // s.date_processing = Supply::string_to_date(d);
    std::cout << "Кто взял поставку (ФИО пользователя): ";
    Getline(s.responsible_person);

    std::cout << "Добавление товаров в поставку (0 для завершения):\n";
    while (iter < count) {
        Product p;
        std::cout << "Название товара (0 - завершить): ";
        Getline(p.name);
        if (p.name == "0") break;

        if (p.name.empty() || p.name == "exit") {
            std::cerr << "Ошибка: название не может быть пустым\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        if (p.name.size() > 60) {
            std::cerr << "Ошибка: название слишком длинное (макс. 60 символов)\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        std::cout << "Категория: ";
        Getline(p.category);
        if (p.category.empty() || p.category == "exit") {
            std::cerr << "Ошибка: категория не может быть пустой\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        if (p.category.size() > 50) {
            std::cerr << "Ошибка: категория слишком длинная (макс. 50 символов)\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        std::cout << "Цена: ";
        Getline(p.price);
        if (p.price <= 0) {
            std::cerr << "Ошибка: цена должна быть положительной\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }
        if (p.price > 10000) {
            std::cerr << "Ошибка: цена не может превышать 10000\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        std::cout << "Артикль: ";
        Getline(p.article);
        if (p.article <= 0) {
            std::cerr << "Ошибка: артикль должен быть положительным числом\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        bool article_exists = false;
        for (const auto& existing_supply : supplies) {
            if (existing_supply.product_name.article == p.article) {
                article_exists = true;
                break;
            }
        }
        if (article_exists) {
            std::cerr << "Ошибка: товар с артиклем " << p.article << " уже есть в другой поставке\n";
            continue;
        }

        std::cout << "Начало срока (ДД.ММ.ГГГГ): ";
        Getline(p.begin_date);
        std::cout << "Конец срока (ДД.ММ.ГГГГ): ";
        Getline(p.end_date);

        if (!is_valid_date(p.begin_date) || !is_valid_date(p.end_date)) {
            std::cerr << "Ошибка: неверный формат даты\n";
            continue;
        }

        auto begin = Supply::string_to_date(p.begin_date);
        auto end = Supply::string_to_date(p.end_date);
        auto now = std::chrono::system_clock::now();
        auto today = std::chrono::floor<std::chrono::days>(now);

        if (begin > end) {
            std::cerr << "Ошибка: дата начала не может быть позже даты окончания\n";
            continue;
        }

        if (begin > today) {
            std::cerr << "Ошибка: дата начала срока годности не может быть в будущем\n";
            continue;
        }

        auto min_date = Supply::string_to_date("01.01.2026");
        if (begin < min_date) {
            std::cerr << "Ошибка: дата начала срока годности слишком старая (должна быть не ранее 01.01.2026)\n";
            continue;
        }

        if (end < today) {
            auto days_ago = std::chrono::duration_cast<std::chrono::days>(today - end).count();
            std::cerr << "Ошибка: товар просрочен на " << days_ago << " дней\n";
            continue;
        }

        auto days_left = std::chrono::duration_cast<std::chrono::days>(end - today).count();
        if (days_left <= 30 && days_left > 0) {
            std::cout << "Срок годности истекает через " << days_left << " дней!\n";
        }

        std::cout << "Количество: ";
        Getline(p.count);
        if (p.count <= 0) {
            std::cerr << "Ошибка: количество должно быть больше 0\n";
            continue;
        }
        if (p.count > 199) {
            std::cerr << "Ошибка: количество не может превышать 199\n";
            continue;
        }

        std::cout << "Производитель: ";
        Getline(p.manufacturer);
        if (p.manufacturer.empty() || p.manufacturer == "exit") {
            std::cerr << "Ошибка: производитель не может быть пустым\n";
            continue;
        }
        if (p.manufacturer.size() > 60) {
            std::cerr << "Ошибка: название производителя слишком длинное (макс. 60)\n";
            continue;
        }

        std::cout << "Страна: ";
        Getline(p.country);
        if (p.country.empty() || p.country == "exit") {
            std::cerr << "Ошибка: страна не может быть пустой\n";
            continue;
        }
        if (p.country.size() > 55) {
            std::cerr << "Ошибка: название страны слишком длинное (макс. 55)\n";
            continue;
        }

        s.product_name = p;
        supplies.push_back(s);
        add_to_file(s);
        std::cout << "Товар: '" << p.name << "' добавлен в поставку\n";
        iter++;
    }

    s.is_actually = true;
    s.status = "Ожидается";

    std::cout << "Поставка под номером: " << s.number_supply << " добавлена\n";
    std::cout << "Добавлено поставок: " << count << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void SupplyManager::show_all_supplies() {
    if (supplies.empty()) {
        std::cout << "Нет поставок\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }
    for (const auto& s : supplies) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
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
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        }
    }
    std::cerr << "Поставка не найдена\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
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
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
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
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
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
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        }
    }
    std::cerr << "Поставка не найдена или не актуальна\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
