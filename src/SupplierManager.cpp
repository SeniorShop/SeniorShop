#include "../include/SupplierManager.h"
#include "../include/Product.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#ifdef _WIN32
#include "Windows.h"
#else
#include <stdlib.h>
#endif

std::map<std::string, std::vector<std::string>> SupplierManager::load_suppliers() {
    std::map<std::string, std::vector<std::string>> suppliers;
    std::ifstream file("Suppliers.txt");
    if (!file.is_open()) {
        suppliers["Coca-Cola"] = {"Напитки", "Газировка", "Соки"};
        suppliers["Danone"] = {"Молочные продукты", "Йогурты", "Детское питание"};
        suppliers["Mars"] = {"Кондитерские изделия", "Шоколад", "Корм для животных"};
        suppliers["Nestle"] = {"Продукты питания", "Кофе", "Вода"};
        suppliers["PepsiCo"] = {"Напитки", "Снеки", "Соки"};
        suppliers["Procter & Gamble"] = {"Бытовая химия", "Гигиена"};
        suppliers["Unilever"] = {"Бытовая химия", "Продукты питания"};
        suppliers["Мултон"] = {"Напитки", "Соки"};
        suppliers["ЮниморфБилл"] = {"Продукты питания"};
        suppliers["Ойо"] = {"Молочные продукты"};
        return suppliers;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string supplier, categories_str;
        std::getline(ss, supplier, '|');
        std::getline(ss, categories_str);
        std::stringstream cat_stream(categories_str);
        std::string category;
        while (std::getline(cat_stream, category, ',')) {
            suppliers[supplier].push_back(category);
        }
    }
    file.close();
    return suppliers;
}

std::string SupplierManager::choose_supplier() {
    auto suppliers = load_suppliers();
    if (suppliers.empty()) {
        std::cerr << "Список поставщиков пуст. Введите поставщика вручную.\n";
        std::string custom;
        std::cout << "Введите поставщика: ";
        Getline(custom);
        return custom;
    }

    std::vector<std::string> names;

    while (true) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        std::cout << "\n\n\n\t\t\tВЫБОР ПОСТАВЩИКА\n\n\n";
        int index = 1;
        names.clear();
        for (const auto& supplier : suppliers) {
            std::cout << index << ". " << supplier.first << "\n";
            names.push_back(supplier.first);
            index++;
        }
        std::cout << "0. Свой ввод\n";

        std::string choice_str;
        std::cout << "Выбор: ";
        Getline(choice_str);

        if (choice_str.empty()) {
            std::cerr << "Ошибка: введите номер пункта меню\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        bool is_valid = true;
        for (char c : choice_str) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) {
            std::cerr << "Ошибка: введите только цифры (0-" << names.size() << ")\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        int choice = std::stoi(choice_str);

        if (choice == 0) {
            std::string custom;
            std::cout << "Введите поставщика: ";
            Getline(custom);

            std::size_t start = custom.find_first_not_of(" \t\n\r\f\v");
            if (start != std::string::npos) {
                std::size_t end = custom.find_last_not_of(" \t\n\r\f\v");
                custom = custom.substr(start, end - start + 1);
            }

            if (!custom.empty()) {
                return custom;
            }
            std::cerr << "Поставщик не может быть пустым.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choice >= 1 && choice <= static_cast<int>(names.size())) {
            return names[choice - 1];
        }

        std::cerr << "Неверный выбор. Введите число от 0 до " << names.size() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
}

std::string SupplierManager::choose_category(const std::string& supplier) {
    auto suppliers = load_suppliers();
    auto it = suppliers.find(supplier);

    if (it == suppliers.end() || it->second.empty()) {
        std::cout << "Введите категорию вручную: ";
        std::string custom;
        Getline(custom);

        std::size_t start = custom.find_first_not_of(" \t\n\r\f\v");
        if (start != std::string::npos) {
            std::size_t end = custom.find_last_not_of(" \t\n\r\f\v");
            custom = custom.substr(start, end - start + 1);
        }
        return custom;
    }

    const auto& categories = it->second;

    while (true) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        std::cout << "\n\n\n\t\t\tКАТЕГОРИИ ПОСТАВЩИКА " << supplier << "\n\n\n";
        for (std::size_t i = 0; i < categories.size(); ++i) {
            std::cout << i + 1 << ". " << categories[i] << "\n";
        }
        std::cout << "0. Свой ввод\n";

        std::string choice_str;
        std::cout << "Выбор: ";
        Getline(choice_str);

        if (choice_str.empty()) {
            std::cerr << "Ошибка: введите номер категории\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        bool is_valid = true;
        for (char c : choice_str) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) {
            std::cerr << "Ошибка: введите только цифры (0-" << categories.size() << ")\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        int choice = std::stoi(choice_str);

        if (choice == 0) {
            std::string custom;
            std::cout << "Введите категорию: ";
            Getline(custom);

            size_t start = custom.find_first_not_of(" \t\n\r\f\v");
            if (start != std::string::npos) {
                std::size_t end = custom.find_last_not_of(" \t\n\r\f\v");
                custom = custom.substr(start, end - start + 1);
            }

            if (!custom.empty()) {
                return custom;
            }
            std::cerr << "Категория не может быть пустой.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choice >= 1 && choice <= static_cast<int>(categories.size())) {
            return categories[choice - 1];
        }

        std::cerr << "Неверный выбор. Введите число от 0 до " << categories.size() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
}
