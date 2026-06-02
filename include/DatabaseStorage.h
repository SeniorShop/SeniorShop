#ifndef DATABASESTORAGE_H
#define DATABASESTORAGE_H

#include "IStorage.h"
#include "sqlite3.h"
#include <string>
#include <memory>

class DatabaseStorage : public IStorage {
private:
    sqlite3* db;
    std::string dbPath;

    bool execute_SQL(const std::string& sql);
    bool create_tables();
    int get_last_insert_ID();

    Product row_to_product(sqlite3_stmt* stmt);
    Supply row_to_supply(sqlite3_stmt* stmt);
    Check row_to_check(sqlite3_stmt* stmt);

public:
    DatabaseStorage(const std::string& databasePath = "database/shop.db");
    ~DatabaseStorage();

    bool initialize() override;
    void close() override;

    // для товаров
    bool load_product(int article, Product& outProduct) override;
    bool save_product(const Product& product) override;
    bool update_product_count(int article, int newCount) override;
    bool update_product_price(int article, double newPrice) override;
    bool update_product_name(int article, const std::string& newName) override;
    bool delete_product(int article) override;
    std::vector<Product> get_all_products() override;
    bool product_exists(int article) override;


    // для поставок
    bool save_supply(const Supply& supply) override;
    bool update_supply_status(int supplyNumber, bool isActually, const std::string& status) override;
    bool delete_supply(int supplyNumber) override;
    Supply get_supply_by_number(int supplyNumber) override;
    std::vector<Supply> get_all_supplies() override;

    // для чеков
    bool save_check(const Check& check) override;
    std::vector<Check> get_sales_checks() override;
    std::vector<Check> get_writeoff_checks() override;
    double get_total_sales() override;
    double get_total_writeoffs() override;
    std::map<std::string, double> get_employee_sales_stats() override;

    // для пользователей
    bool save_user(const std::string& username, const std::string& password, const std::string& role) override;
    bool validate_user(const std::string& username, const std::string& password) override;
    bool update_user_password(const std::string& username, const std::string& newPassword) override;
    bool update_user_role(const std::string& username, const std::string& newRole) override;
    bool delete_user(const std::string& username) override;
    bool user_exists(const std::string& username) override;
    std::vector<std::pair<std::string, std::string>> get_all_users() override;
};

#endif // DATABASESTORAGE_H
