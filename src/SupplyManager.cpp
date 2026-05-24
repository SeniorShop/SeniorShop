#include "../include/SupplyManager.h"
#include "../include/Storage.h"
#include "../include/Product.h"
#include "../include/SupplierManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iostream>
#ifdef _WIN32
#include "Windows.h"
#else
#include <stdlib.h>
#endif

SupplyManager::SupplyManager() {
    load_from_file();
}

void SupplyManager::load_from_file() {
    supplies.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;
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
    file.close();
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
    std::cout << "Сколько поставок записать (1-3): ";
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

    for (int i = 0; i < count; ++i) {
        Supply s;
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        std::cout << "\n\n\n\t\t\tПоставка под номером: " << (i + 1) << "\n\n\n";

        while (true) {
            std::cout << "Номер поставки: ";
            Getline(s.number_supply);
            bool exists = false;
            for (const auto& ex : supplies) {
                if (ex.number_supply == s.number_supply) {
                    std::cerr << "Ошибка: поставка с номером " << ex.number_supply << " уже существует\n";
                    exists = true;
                    break;
                }
            }
            if (!exists && s.number_supply > 0) break;
            if (s.number_supply <= 0) std::cerr << "Номер должен быть положительным\n";
        }

        s.name_user = SupplierManager::choose_supplier();
        if (s.name_user.empty()) {
            std::cerr << "Отмена создания поставки\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        }

        std::cout << "Кто взял поставку (ФИО): ";
        Getline(s.responsible_person);
        if (s.responsible_person.empty())
            s.responsible_person = "Не указан";

        auto now = std::chrono::system_clock::now();
        auto today = std::chrono::floor<std::chrono::days>(now);
        s.date = today;
        s.date_acception = today;
        s.date_processing = today;

        Product p;
        bool product_ok = false;
        while (!product_ok) {
            std::cout << "\nНазвание товара (не более 60 символов): ";
            Getline(p.name);
            if (p.name.empty() || p.name.size() > 60) {
                std::cerr << "Ошибка: название не может быть пустым или длиннее 60 символов.\n";
                continue;
            }

            p.category = SupplierManager::choose_category(s.name_user);
            if (p.category.empty()) {
                std::cerr << "Ошибка: категория не может быть пустой.\n";
                continue;
            }
            if (p.category.size() > 50) {
                std::cerr << "Ошибка: категория слишком длинная (макс. 50).\n";
                continue;
            }

            std::cout << "Цена (руб): ";
            Getline(p.price);
            if (p.price <= 0 || p.price > 10000) {
                std::cerr << "Ошибка: цена должна быть от 0.01 до 10000.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }

            std::cout << "Артикль (уникальный >0): ";
            Getline(p.article);
            if (p.article <= 0) {
                std::cerr << "Ошибка: артикль должен быть положительным.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }
            bool art_exists = false;
            for (const auto& ex : supplies) {
                if (ex.product_name.article == p.article) {
                    art_exists = true;
                    break;
                }
            }
            if (art_exists) {
                std::cerr << "Ошибка: товар с артиклем " << p.article << " уже есть в другой поставке.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }

            std::cout << "Начало срока (ДД.ММ.ГГГГ): ";
            Getline(p.begin_date);
            std::cout << "Конец срока (ДД.ММ.ГГГГ): ";
            Getline(p.end_date);

            if (!is_valid_date(p.begin_date) || !is_valid_date(p.end_date)) {
                std::cerr << "Ошибка: неверный формат даты.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }
            auto begin = Supply::string_to_date(p.begin_date);
            auto end = Supply::string_to_date(p.end_date);
            if (begin > end) {
                std::cerr << "Ошибка: дата начала не может быть позже окончания.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }
            if (begin > today) {
                std::cerr << "Ошибка: дата начала не может быть в будущем.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }
            auto min_date = Supply::string_to_date("01.01.2026");
            if (begin < min_date) {
                std::cerr << "Ошибка: дата начала слишком старая (не ранее 01.01.2026).\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }
            if (end < today) {
                auto days_ago = std::chrono::duration_cast<std::chrono::days>(today - end).count();
                std::cerr << "Ошибка: товар уже просрочен на " << days_ago << " дней.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }

            std::cout << "Количество (1-199): ";
            Getline(p.count);
            if (p.count < 1 || p.count > 199) {
                std::cerr << "Ошибка: количество должно быть от 1 до 199.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }

            std::cout << "Страна производства: ";
            Getline(p.country);
            if (p.country.empty() || p.country.size() > 55) {
                std::cerr << "Ошибка: страна не может быть пустой или длиннее 55 символов.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }

            p.supplier = s.name_user;
            product_ok = true;
        }

        s.product_name = p;
        s.is_actually = true;
        s.status = "Ожидается";
        supplies.push_back(s);
        add_to_file(s);
        std::cout << "\nПоставка под номером: " << s.number_supply << " добавлена.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    std::cout << "Добавлено поставок: " << count << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void SupplyManager::show_all_supplies() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    if (supplies.empty()) {
        std::cout << "Нет поставок.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
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
    std::cout << "Нажмите Enter для продолжения: ";
    std::cin.get();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
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
            std::cout << "Актуальна? (y/n): ";
            bool new_actual;
            Getline(new_actual);
            s.is_actually = new_actual;
            std::ofstream fout(filename, std::ios::trunc);
            fout.close();
            for (const auto& item : supplies) add_to_file(item);
            std::cout << "Поставка под номером: " << num << " обновлена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        }
    }
    std::cerr << "Поставка не найдена.\n";
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
        std::cerr << "Поставка не найдена.\n";
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
        std::cout << "Поставка удалена.\n";
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
            std::cout << "Поставка под номером: " << num << " применена.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        }
    }
    std::cerr << "Поставка не найдена или не актуальна.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
