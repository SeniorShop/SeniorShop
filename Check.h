#ifndef CHECK_H
#define CHECK_H

#include <string>
#include <vector>

enum class TransactionType {
    Sale = 0,
    Supply = 1,
    WriteOff = 2
};

struct Check {
    int type;
    std::string product_name;
    double price;
    int count;
    std::string employee_name;
    std::string date;

    Check() = default;
    Check(TransactionType type, std::string name, double price, int count, std::string emp_name, std::string date);

    void save_check(const std::string& filename) const;
    static void show_financial_report(const std::string& filename);
};

#endif