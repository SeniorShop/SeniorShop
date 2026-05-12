#include "Check.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>

Check::Check(TransactionType type, std::string name, double price, int count, std::string emp_name, std::string date)
    : type(static_cast<int>(type)), product_name(name), price(price), count(count), employee_name(emp_name), date(date) {
}

void Check::save_check(const std::string& filename) const {
    std::ofstream out(filename, std::ios::app);
    if (out.is_open()) {
        std::string safe_name = product_name;
        std::replace(safe_name.begin(), safe_name.end(), ' ', '_');
        out << type << " " << safe_name << " " << price << " " << count << " " << employee_name << " " << date << "\n";
    }
}

void Check::show_financial_report(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "история операций пуста или файл не найден\n";
        return;
    }

    double total_sales = 0;
    double total_losses = 0;
    std::map<std::string, double> staff_stats;

    int t, cnt;
    double pr;
    std::string p_name, emp, dt;

    while (in >> t >> p_name >> pr >> cnt >> emp >> dt) {
        double sum = pr * cnt;
        if (t == 0) {
            total_sales += sum;
            staff_stats[emp] += sum;
        }
        else if (t == 2) {
            total_losses += sum;
        }
    }

    std::string winner = "нет";
    double max_s = 0;
    for (auto const& [name, val] : staff_stats) {
        if (val > max_s) { max_s = val; winner = name; }
    }

    system("cls");
    std::cout << "финансовый отчет\n";
    std::cout << "выручка: " << total_sales << "\n";
    std::cout << "убытки: " << total_losses << "\n";
    std::cout << "прибыль: " << total_sales - total_losses << "\n";
    std::cout << "лучший сотрудник: " << winner << "\n";
    if (max_s > 0) std::cout << "премия: " << max_s * 0.05 << "\n";
}