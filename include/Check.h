#ifndef CHECK_H
#define CHECK_H

#include "Product.h"
#include <string>
#include <vector>
#include <thread>

enum class TransactionType {
    Sale = 0,
    Supply = 1,
    WriteOff = 2
};

struct Check {
    int type = 0;
    std::string product_name;
    double price = 0.0;
    int count = 0;
    std::string employee_name;
    std::string date;

    Check() = default;
    Check(TransactionType type, std::string name, double price, int count, std::string emp_name, std::string date);
    Check(TransactionType type, double totalAmount, const std::string& emp_name, const std::string& date);

    void save_check(const std::string& filename) const;
    void show_financial_report(const std::string& filename);
    void show_sales_documents(const std::string& filename);
    void show_logs();
    void show_writeoffs(const std::string& filename);
    void show_supplies();
    void show_full_financial_report(const std::string& filename);
};

#endif
