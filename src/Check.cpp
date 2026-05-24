#include "../include/Check.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <thread>
#include <map>
#include <chrono>
#include <iomanip>

Check::Check(TransactionType type, std::string name, double price, int count, std::string emp_name, std::string date)
    : type(static_cast<int>(type)), product_name(name), price(price), count(count), employee_name(emp_name), date(date) {
}

Check::Check(TransactionType type, double totalAmount, const std::string& emp_name, const std::string& date)
    : type(static_cast<int>(type)), product_name("ЧЕК"), price(totalAmount), count(1), employee_name(emp_name), date(date) {
}

void Check::save_check(const std::string& filename) const {
    std::ofstream out(filename, std::ios::app);
    if (!out.is_open()) {
        return;
    }
    std::string safe_name = product_name;
    std::replace(safe_name.begin(), safe_name.end(), ' ', '_');
    out << type << "|" << safe_name << "|" << price << "|" << count << "|" << employee_name << "|" << date << "\n";
    out.close();
}

void Check::show_financial_report(const std::string& filename) {
    bool is_exit = false;
    while (!is_exit) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        std::cout << "\n\n\n\t\t\tОТЧЁТЫ И ДОКУМЕНТЫ\n\n\n";
        std::cout << "1) Продажи (чеки)\n";
        std::cout << "2) Логи авторизации\n";
        std::cout << "3) Списания товаров\n";
        std::cout << "4) Пополнения (поставки)\n";
        std::cout << "5) Полный финансовый отчёт\n";
        std::cout << "0) Назад\n";
        std::cout << "Выбор: ";

        std::string choice;
        Getline(choice, true);

        if (choice.empty()) {
            std::cerr << "Ошибка: введите номер пункта меню\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choice.size() != 1) {
            std::cerr << "Ошибка: введите одну цифру\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        int ch = std::stoi(choice);

        if (ch < 0 || ch > 5) {
            std::cerr << "Ошибка: введите цифру от 0 до 5\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        switch (ch) {
        case 1:
            show_sales_documents(filename);
            break;
        case 2:
            show_logs();
            break;
        case 3:
            show_writeoffs(filename);
            break;
        case 4:
            show_supplies();
            break;
        case 5:
            show_full_financial_report(filename);
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

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Check::show_sales_documents(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        return;
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "\n========== ЧЕКИ С ПРОДАЖ ==========\n";
    std::cout << std::left
              << std::setw(25) << "Товар"
              << std::setw(12) << "Цена"
              << std::setw(8) << "Кол-во"
              << std::setw(15) << "Сотрудник"
              << std::setw(20) << "Дата"
              << "\n";
    std::cout << std::string(80, '-') << "\n";

    std::string line;
    int found = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        int t, cnt;
        double pr;
        std::string p_name, emp, dt;
        char delim;

        ss >> t >> delim;
        std::getline(ss, p_name, '|');
        ss >> pr >> delim >> cnt >> delim;
        std::getline(ss, emp, '|');
        std::getline(ss, dt);

        if (t == 0) {
            std::cout << std::left
                      << std::setw(25) << (p_name.length() > 23 ? p_name.substr(0, 21) + ".." : p_name)
                      << std::setw(12) << std::fixed << std::setprecision(2) << pr
                      << std::setw(8) << cnt
                      << std::setw(15) << emp
                      << std::setw(20) << dt
                      << "\n";
            found++;
        }
    }
    in.close();

    if (found == 0) {
        std::cout << "Нет записей о продажах\n";
    }

    std::cout << std::string(80, '-') << "\n";
    std::cout << "\nНажмите Enter для продолжения: ";
    std::cin.get();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Check::show_logs() {
    std::ifstream log_file("Logs.txt");
    if (!log_file.is_open()) {
        return;
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "\n========== ЛОГИ АВТОРИЗАЦИИ ==========\n\n";

    std::string line;
    int line_count = 0;
    while (std::getline(log_file, line)) {
        if (!line.empty()) {
            std::cout << line << "\n";
            line_count++;
        }
    }
    log_file.close();

    if (line_count == 0) {
        std::cout << "Логи пусты\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    std::cout << "\nНажмите Enter для продолжения: ";
    std::cin.get();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Check::show_writeoffs(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "Нет данных о списаниях\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        return;
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "\n========== СПИСАНИЯ ТОВАРОВ ==========\n";
    std::cout << std::left
              << std::setw(25) << "Товар"
              << std::setw(12) << "Цена"
              << std::setw(8) << "Кол-во"
              << std::setw(20) << "Сотрудник/Причина"
              << std::setw(20) << "Дата"
              << "\n";
    std::cout << std::string(85, '-') << "\n";

    std::string line;
    int count = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        int t, cnt;
        double pr;
        std::string p_name, emp, dt;
        char delim;

        ss >> t >> delim;
        std::getline(ss, p_name, '|');
        ss >> pr >> delim >> cnt >> delim;
        std::getline(ss, emp, '|');
        std::getline(ss, dt);

        if (t == 2) {
            std::cout << std::left
                      << std::setw(25) << (p_name.length() > 23 ? p_name.substr(0, 21) + ".." : p_name)
                      << std::setw(12) << std::fixed << std::setprecision(2) << pr
                      << std::setw(8) << cnt
                      << std::setw(20) << emp
                      << std::setw(20) << dt
                      << "\n";
            count++;
        }
    }
    in.close();

    if (count == 0) {
        std::cout << "Нет записей о списаниях\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    std::cout << std::string(85, '-') << "\n";
    std::cout << "\nНажмите Enter для продолжения: ";
    std::cin.get();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Check::show_supplies() {
    std::ifstream supply_file("Supplies.txt");
    if (!supply_file.is_open()) {
        std::cout << "Нет данных о поставках\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        return;
    }

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "\n========== ПОСТАВКИ ==========\n";
    std::cout << std::left
              << std::setw(10) << "Номер"
              << std::setw(25) << "Поставщик"
              << std::setw(20) << "Ответственный"
              << std::setw(20) << "Товар"
              << std::setw(8) << "Кол-во"
              << std::setw(15) << "Статус"
              << "\n";
    std::cout << std::string(98, '-') << "\n";

    std::string line;
    int count = 0;
    while (std::getline(supply_file, line)) {
        if (line.empty()) continue;
        count++;
        std::cout << line << "\n";
    }
    supply_file.close();

    if (count == 0) {
        std::cout << "Нет записей о поставках\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    std::cout << std::string(98, '-') << "\n";
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.get();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Check::show_full_financial_report(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Нет данных для финансового отчёта\n";
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

    double total_sales = 0;
    double total_losses = 0;
    std::map<std::string, double> staff_stats;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        int t, cnt;
        double pr;
        std::string p_name, emp, dt;
        char delim;

        ss >> t >> delim;
        std::getline(ss, p_name, '|');
        ss >> pr >> delim >> cnt >> delim;
        std::getline(ss, emp, '|');
        std::getline(ss, dt);

        double sum = pr * cnt;
        if (t == 0) {
            total_sales += sum;
            staff_stats[emp] += sum;
        } else if (t == 2) {
            total_losses += sum;
        }
    }
    in.close();

    std::string winner = "нет";
    double max_s = 0;
    for (const auto& pair : staff_stats) {
        if (pair.second > max_s) {
            max_s = pair.second;
            winner = pair.first;
        }
    }

    std::cout << "\n========== ФИНАНСОВЫЙ ОТЧЁТ ==========\n";
    std::cout << "Выручка от продаж: " << std::fixed << std::setprecision(2) << total_sales << " руб.\n";
    std::cout << "Убытки (списания): " << total_losses << " руб.\n";
    std::cout << "Чистая прибыль: " << total_sales - total_losses << " руб.\n";
    std::cout << "Лучший сотрудник: " << winner << "\n";
    if (max_s > 0) {
        std::cout << "Премия лучшему сотруднику: " << max_s * 0.05 << " руб.\n";
    }
    std::cout << "=====================================\n";

    std::cout << "\nНажмите Enter для продолжения: ";
    std::cin.get();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
