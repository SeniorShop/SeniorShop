#ifndef ISTORAGE_H
#define ISTORAGE_H

#include "Product.h"
#include "Supply.h"
#include "Check.h"
#include <vector>
#include <string>
#include <map>

class IStorage {
public:
    virtual ~IStorage() = default;

    virtual bool load_product(int article, Product& outProduct) = 0;
    virtual bool save_product(const Product& product) = 0;
    virtual bool update_product_count(int article, int newCount) = 0;
    virtual bool update_product_price(int article, double newPrice) = 0;
    virtual bool update_product_name(int article, const std::string& newName) = 0;
    virtual bool delete_product(int article) = 0;
    virtual std::vector<Product> get_all_products() = 0;
    virtual bool product_exists(int article) = 0;

    virtual bool save_supply(const Supply& supply) = 0;
    virtual bool update_supply_status(int supplyNumber, bool isActually, const std::string& status) = 0;
    virtual bool delete_supply(int supplyNumber) = 0;
    virtual Supply get_supply_by_number(int supplyNumber) = 0;
    virtual std::vector<Supply> get_all_supplies() = 0;

    virtual bool save_check(const Check& check) = 0;
    virtual std::vector<Check> get_sales_checks() = 0;
    virtual std::vector<Check> get_writeoff_checks() = 0;
    virtual double get_total_sales() = 0;
    virtual double get_total_writeoffs() = 0;
    virtual std::map<std::string, double> get_employee_sales_stats() = 0;

    virtual bool save_user(const std::string& username, const std::string& password, const std::string& role) = 0;
    virtual bool validate_user(const std::string& username, const std::string& password) = 0;
    virtual bool update_user_password(const std::string& username, const std::string& newPassword) = 0;
    virtual bool update_user_role(const std::string& username, const std::string& newRole) = 0;
    virtual bool delete_user(const std::string& username) = 0;
    virtual bool user_exists(const std::string& username) = 0;
    virtual std::vector<std::pair<std::string, std::string>> get_all_users() = 0;

    virtual bool initialize() = 0;
    virtual void close() = 0;
};

#endif // ISTORAGE_H
