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
    std::cout << "\n\n\n\t\t\tВЫБОР ПОСТАВЩИКА\n\n\n";
    int index = 1;
    for (const auto& supplier : suppliers) {
        std::cout << index << ". " << supplier.first << "\n";
        names.push_back(supplier.first);
        index++;
    }
    std::cout << "0. Свой ввод\n";
    int choice;
    while (true) {
        std::cout << "Выбор: ";
        Getline(choice);

        if (choice == 0) {
            std::string custom;
            std::cout << "Введите поставщика: ";
            Getline(custom);
            if (!custom.empty()) return custom;
            std::cerr << "Поставщик не может быть пустым.\n";
            continue;
        }

        if (choice >= 1 && choice <= static_cast<int>(names.size())) {
            return names[choice - 1];
        }

        std::cerr << "Неверный выбор. Попробуйте снова.\n";
    }
}

std::string SupplierManager::choose_category(const std::string& supplier) {
    auto suppliers = load_suppliers();
    auto it = suppliers.find(supplier);

    if (it == suppliers.end() || it->second.empty()) {
        std::cout << "Введите категорию вручную: ";
        std::string custom;
        Getline(custom);
        return custom;
    }

    const auto& categories = it->second;
    std::cout << "\n\n\n\t\t\tКАТЕГОРИИ ПОСТАВЩИКА " << supplier << "\n\n\n";
    for (std::size_t i = 0; i < categories.size(); ++i) {
        std::cout << i + 1 << ". " << categories[i] << "\n";
    }
    std::cout << "0. Свой ввод\n";
    int choice;
    while (true) {
        std::cout << "Выбор: ";
        Getline(choice);

        if (choice == 0) {
            std::string custom;
            std::cout << "Введите категорию: ";
            Getline(custom);
            if (!custom.empty()) return custom;
            std::cerr << "Категория не может быть пустой.\n";
            continue;
        }

        if (choice >= 1 && choice <= static_cast<int>(categories.size())) {
            return categories[choice - 1];
        }

        std::cerr << "Неверный выбор. Попробуйте снова.\n";
    }
}
