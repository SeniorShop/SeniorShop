#include "Check.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <chrono>
#include <iomanip>

Check::Check(TransactionType type, std::string name, double price, int count, std::string emp_name, std::string date)
    : type(static_cast<int>(type)), product_name(name), price(price), count(count), employee_name(emp_name), date(date) {}

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
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    bool is_exit = false;
    while (true) {
        std::cout << "\n\n\n\t\t\tОТЧЁТЫ И ДОКУМЕНТЫ\n\n\n";
        std::cout << "1) Продажи (чеки)\n";
        std::cout << "2) Логи авторизации\n";
        std::cout << "3) Списания товаров\n";
        std::cout << "4) Пополнения (поставки)\n";
        std::cout << "5) Полный финансовый отчёт\n";
        std::cout << "0) Назад\n";
        std::cout << "Выбор: ";

        std::string choice;
        Getline(choice,true);

        if(choice.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if(choice[0] < '0' || choice[0] > '5') {
            std::cerr << "Ошибка: Выберите пункты с 0 по 5\n";
        } else {
            switch(choice[0]) {
            case '1':
                show_sales_documents(filename);
                break;
            case '2':
                show_logs();
                break;
            case '3':
                show_writeoffs(filename);
                break;
            case '4':
                show_supplies();
                break;
            case '5':
                show_full_financial_report(filename);
                break;
            case '0':
                is_exit = true;
                break;
            default:
                std::cerr << "Неверный выбор\n";
                break;
            }

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
        std::cout << "Нет данных о продажах\n";
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

    std::cout << "\n\n\n\t\t\tЧЕКИ С ПРОДАЖ\n\n\n";
    std::cout << std::left
              << std::setw(25) << "Товар"
              << std::setw(10) << "Цена"
              << std::setw(8) << "Кол-во"
              << std::setw(15) << "Сотрудник"
              << std::setw(20) << "Дата"
              << "\n";
    std::cout << std::string(80, '-') << "\n";

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

        if (t == 0) {
            std::cout << std::left
                      << std::setw(25) << (p_name.length() > 23 ? p_name.substr(0, 21) + ".." : p_name)
                      << std::setw(12) << std::fixed << std::setprecision(2) << pr
                      << std::setw(8) << cnt
                      << std::setw(15) << emp
                      << std::setw(20) << dt
                      << "\n";
        }
    }
    in.close();
}

void Check::show_logs() {
    std::ifstream log_file("Logs.txt");
    if (!log_file.is_open()) {
        std::cout << "Логи не найдены\n";
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

    std::cout << "\n\n\n\t\t\tЛОГИ АВТОРИЗАЦИИ\n\n\n";
    std::string line;
    while (std::getline(log_file, line)) {
        std::cout << line << "\n";
    }
    log_file.close();
}

void Check::show_writeoffs(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Нет данных о списаниях\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    std::cout << "\n\n\n\t\t\tСПИСАНИЯ ТОВАРОВ\n\n\n";
    std::cout << std::left
              << std::setw(25) << "Товар"
              << std::setw(10) << "Цена"
              << std::setw(10) << "Кол-во"
              << std::setw(15) << "Сотрудник"
              << std::setw(20) << "Дата"
              << "\n";
    std::cout << std::string(80, '-') << "\n";

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

        if (t == 2) {
            std::cout << std::left
                      << std::setw(25) << (p_name.length() > 23 ? p_name.substr(0, 21) + ".." : p_name)
                      << std::setw(10) << pr
                      << std::setw(10) << cnt
                      << std::setw(15) << emp
                      << std::setw(20) << dt
                      << "\n";
        }
    }
    in.close();
}

void Check::show_supplies() {
    std::ifstream supply_file("Supplies.txt");
    if (!supply_file.is_open()) {
        std::cerr << "Нет данных о поставках\n";
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

    std::cout << "\n\n\n\t\t\tПОСТАВКИ\n\n\n";
    std::cout << std::left
              << std::setw(10) << "Номер"
              << std::setw(25) << "Поставщик"
              << std::setw(20) << "Ответственный"
              << std::setw(15) << "Товар"
              << std::setw(10) << "Кол-во"
              << std::setw(15) << "Статус"
              << "\n";
    std::cout << std::string(95, '-') << "\n";

    std::string line;
    while (std::getline(supply_file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string number_str, supplier, responsible, date, date_acc, date_proc, status, product_name;
        int act_int;
        double price;
        int article, count;

        std::getline(ss, number_str, '|');
        std::getline(ss, supplier, '|');
        std::getline(ss, date, '|');
        std::getline(ss, date_acc, '|');
        std::getline(ss, date_proc, '|');
        std::getline(ss, responsible, '|');
        std::getline(ss, product_name, '|');
        std::getline(ss, product_name, '|');
        ss >> price;
        ss.ignore();
        ss >> article;
        ss.ignore();
        std::getline(ss, date, '|');
        std::getline(ss, date, '|');
        ss >> count;
        ss.ignore();
        std::getline(ss, date, '|');
        std::getline(ss, date, '|');
        ss >> act_int;
        ss.ignore();
        std::getline(ss, status);

        std::cout << std::left
                  << std::setw(10) << number_str
                  << std::setw(25) << (supplier.length() > 23 ? supplier.substr(0, 21) + ".." : supplier)
                  << std::setw(20) << (responsible.length() > 18 ? responsible.substr(0, 16) + ".." : responsible)
                  << std::setw(15) << (product_name.length() > 13 ? product_name.substr(0, 11) + ".." : product_name)
                  << std::setw(10) << count
                  << std::setw(15) << status
                  << "\n";
    }
    supply_file.close();
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

    std::cout << "\n\n\n\t\t\tФИНАНСОВЫЙ ОТЧЁТ\n\n\n";
    std::cout << "Выручка от продаж: " << std::fixed << std::setprecision(2) << total_sales << " руб.\n";
    std::cout << "Убытки (списания): " << total_losses << " руб.\n";
    std::cout << "Чистая прибыль: " << total_sales - total_losses << " руб.\n";
    std::cout << "Лучший сотрудник: " << winner << "\n";
    if (max_s > 0) {
        std::cout << "Премия лучшему сотруднику: " << max_s * 0.05 << " руб.\n";
    }
}
