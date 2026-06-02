#include "../include/DatabaseStorage.h"
#include "../include/Logger.h"
#include <iostream>
#include <filesystem>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/stat.h>
#endif

static bool create_directory(const std::string& path) {
#ifdef _WIN32
    return CreateDirectoryA(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
#else
    return mkdir(path.c_str(), 0755) == 0 || errno == EEXIST;
#endif
}

DatabaseStorage::DatabaseStorage(const std::string& databasePath)
    : db(nullptr), dbPath(databasePath) {}

DatabaseStorage::~DatabaseStorage() {
    close();
}

bool DatabaseStorage::execute_SQL(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseStorage::create_tables() {
    const std::string sqlProducts = R"(
        CREATE TABLE IF NOT EXISTS products (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            category TEXT NOT NULL,
            price REAL NOT NULL CHECK(price > 0 AND price <= 10000),
            article INTEGER UNIQUE NOT NULL,
            begin_date TEXT NOT NULL,
            end_date TEXT NOT NULL,
            count INTEGER NOT NULL CHECK(count >= 0 AND count <= 199),
            manufacturer TEXT,
            country TEXT NOT NULL,
            supplier TEXT NOT NULL
        );
    )";

    const std::string sqlSupplies = R"(
        CREATE TABLE IF NOT EXISTS supplies (
            number_supply INTEGER PRIMARY KEY,
            supplier TEXT NOT NULL,
            date TEXT NOT NULL,
            date_acception TEXT NOT NULL,
            date_processing TEXT NOT NULL,
            responsible_person TEXT,
            product_name TEXT NOT NULL,
            product_category TEXT NOT NULL,
            product_price REAL NOT NULL,
            product_article INTEGER NOT NULL,
            product_begin_date TEXT NOT NULL,
            product_end_date TEXT NOT NULL,
            product_count INTEGER NOT NULL,
            product_manufacturer TEXT,
            product_country TEXT NOT NULL,
            product_supplier TEXT NOT NULL,
            is_actually INTEGER DEFAULT 1,
            status TEXT DEFAULT 'Ожидается'
        );
    )";

    const std::string sqlChecks = R"(
        CREATE TABLE IF NOT EXISTS checks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type INTEGER NOT NULL,
            product_name TEXT NOT NULL,
            price REAL NOT NULL,
            count INTEGER NOT NULL,
            employee_name TEXT NOT NULL,
            date TEXT NOT NULL
        );
    )";

    const std::string sqlUsers = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            role TEXT NOT NULL CHECK(role IN ('superadmin', 'admin', 'user'))
        );
    )";

    const std::string sqlIndexes = R"(
        CREATE INDEX IF NOT EXISTS idx_products_article ON products(article);
        CREATE INDEX IF NOT EXISTS idx_checks_type ON checks(type);
        CREATE INDEX IF NOT EXISTS idx_checks_date ON checks(date);
        CREATE INDEX IF NOT EXISTS idx_checks_employee ON checks(employee_name);
        CREATE INDEX IF NOT EXISTS idx_supplies_number ON supplies(number_supply);
    )";

    return execute_SQL(sqlProducts) &&
           execute_SQL(sqlSupplies) &&
           execute_SQL(sqlChecks) &&
           execute_SQL(sqlUsers) &&
           execute_SQL(sqlIndexes);
}

bool DatabaseStorage::initialize() {
    std::string dir = dbPath.substr(0, dbPath.find_last_of('/'));
    if (!dir.empty()) {
        create_directory(dir);
    }

    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (!create_tables()) {
        std::cerr << "Failed to create tables" << std::endl;
        return false;
    }

    std::cout << "Database initialized: " << dbPath << std::endl;
    return true;
}

void DatabaseStorage::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

int DatabaseStorage::get_last_insert_ID() {
    return sqlite3_last_insert_rowid(db);
}

Product DatabaseStorage::row_to_product(sqlite3_stmt* stmt) {
    Product p;
    p.id = sqlite3_column_int(stmt, 0);
    p.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    p.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    p.price = sqlite3_column_double(stmt, 3);
    p.article = sqlite3_column_int(stmt, 4);
    p.begin_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    p.end_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    p.count = sqlite3_column_int(stmt, 7);
    p.manufacturer = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
    p.country = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
    p.supplier = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
    return p;
}

bool DatabaseStorage::save_product(const Product& product) {
    const char* sql = R"(
        INSERT OR REPLACE INTO products
        (name, category, price, article, begin_date, end_date, count, manufacturer, country, supplier)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, product.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, product.category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, product.price);
    sqlite3_bind_int(stmt, 4, product.article);
    sqlite3_bind_text(stmt, 5, product.begin_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, product.end_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, product.count);
    sqlite3_bind_text(stmt, 8, product.manufacturer.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, product.country.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, product.supplier.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseStorage::load_product(int article, Product& outProduct) {
    const char* sql = "SELECT * FROM products WHERE article = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, article);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        outProduct = row_to_product(stmt);
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
}

bool DatabaseStorage::update_product_count(int article, int newCount) {
    const char* sql = "UPDATE products SET count = ? WHERE article = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, newCount);
    sqlite3_bind_int(stmt, 2, article);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseStorage::update_product_price(int article, double newPrice) {
    const char* sql = "UPDATE products SET price = ? WHERE article = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_double(stmt, 1, newPrice);
    sqlite3_bind_int(stmt, 2, article);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseStorage::update_product_name(int article, const std::string& newName) {
    const char* sql = "UPDATE products SET name = ? WHERE article = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, newName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, article);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseStorage::delete_product(int article) {
    const char* sql = "DELETE FROM products WHERE article = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, article);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

std::vector<Product> DatabaseStorage::get_all_products() {
    std::vector<Product> products;
    const char* sql = "SELECT * FROM products ORDER BY id";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return products;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        products.push_back(row_to_product(stmt));
    }

    sqlite3_finalize(stmt);
    return products;
}

bool DatabaseStorage::product_exists(int article) {
    const char* sql = "SELECT 1 FROM products WHERE article = ? LIMIT 1";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, article);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_ROW;
}

Supply DatabaseStorage::row_to_supply(sqlite3_stmt* stmt) {
    Supply s;
    s.number_supply = sqlite3_column_int(stmt, 0);
    s.name_user = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    s.date = Supply::string_to_date(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    s.date_acception = Supply::string_to_date(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    s.date_processing = Supply::string_to_date(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    s.responsible_person = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    s.product_name.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    s.product_name.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
    s.product_name.price = sqlite3_column_double(stmt, 8);
    s.product_name.article = sqlite3_column_int(stmt, 9);
    s.product_name.begin_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
    s.product_name.end_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
    s.product_name.count = sqlite3_column_int(stmt, 12);
    s.product_name.manufacturer = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
    s.product_name.country = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 14));
    s.product_name.supplier = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));
    s.is_actually = sqlite3_column_int(stmt, 16) == 1;
    s.status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));
    return s;
}

bool DatabaseStorage::save_supply(const Supply& supply) {
    const char* sql = R"(
        INSERT OR REPLACE INTO supplies
        (number_supply, supplier, date, date_acception, date_processing,
         responsible_person, product_name, product_category, product_price,
         product_article, product_begin_date, product_end_date, product_count,
         product_manufacturer, product_country, product_supplier, is_actually, status)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, supply.number_supply);
    sqlite3_bind_text(stmt, 2, supply.name_user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, Supply::date_to_string(supply.date).c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, Supply::date_to_string(supply.date_acception).c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, Supply::date_to_string(supply.date_processing).c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, supply.responsible_person.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, supply.product_name.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, supply.product_name.category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 9, supply.product_name.price);
    sqlite3_bind_int(stmt, 10, supply.product_name.article);
    sqlite3_bind_text(stmt, 11, supply.product_name.begin_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 12, supply.product_name.end_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 13, supply.product_name.count);
    sqlite3_bind_text(stmt, 14, supply.product_name.manufacturer.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 15, supply.product_name.country.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 16, supply.product_name.supplier.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 17, supply.is_actually ? 1 : 0);
    sqlite3_bind_text(stmt, 18, supply.status.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseStorage::update_supply_status(int supplyNumber, bool isActually, const std::string& status) {
    const char* sql = "UPDATE supplies SET is_actually = ?, status = ? WHERE number_supply = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, isActually ? 1 : 0);
    sqlite3_bind_text(stmt, 2, status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, supplyNumber);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseStorage::delete_supply(int supplyNumber) {
    const char* sql = "DELETE FROM supplies WHERE number_supply = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, supplyNumber);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

Supply DatabaseStorage::get_supply_by_number(int supplyNumber) {
    Supply s;
    const char* sql = "SELECT * FROM supplies WHERE number_supply = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return s;

    sqlite3_bind_int(stmt, 1, supplyNumber);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        s = row_to_supply(stmt);
    }

    sqlite3_finalize(stmt);
    return s;
}

std::vector<Supply> DatabaseStorage::get_all_supplies() {
    std::vector<Supply> supplies;
    const char* sql = "SELECT * FROM supplies ORDER BY number_supply";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return supplies;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        supplies.push_back(row_to_supply(stmt));
    }

    sqlite3_finalize(stmt);
    return supplies;
}

Check DatabaseStorage::row_to_check(sqlite3_stmt* stmt) {
    Check c;
    c.type = sqlite3_column_int(stmt, 1);
    c.product_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    c.price = sqlite3_column_double(stmt, 3);
    c.count = sqlite3_column_int(stmt, 4);
    c.employee_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    c.date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    return c;
}

bool DatabaseStorage::save_check(const Check& check) {
    const char* sql = R"(
        INSERT INTO checks (type, product_name, price, count, employee_name, date)
        VALUES (?, ?, ?, ?, ?, ?)
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, check.type);
    sqlite3_bind_text(stmt, 2, check.product_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, check.price);
    sqlite3_bind_int(stmt, 4, check.count);
    sqlite3_bind_text(stmt, 5, check.employee_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, check.date.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

std::vector<Check> DatabaseStorage::get_sales_checks() {
    std::vector<Check> checks;
    const char* sql = "SELECT * FROM checks WHERE type = 0 ORDER BY date DESC";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return checks;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        checks.push_back(row_to_check(stmt));
    }

    sqlite3_finalize(stmt);
    return checks;
}

std::vector<Check> DatabaseStorage::get_writeoff_checks() {
    std::vector<Check> checks;
    const char* sql = "SELECT * FROM checks WHERE type = 2 ORDER BY date DESC";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return checks;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        checks.push_back(row_to_check(stmt));
    }

    sqlite3_finalize(stmt);
    return checks;
}

double DatabaseStorage::get_total_sales() {
    const char* sql = "SELECT SUM(price * count) FROM checks WHERE type = 0";
    sqlite3_stmt* stmt;
    double total = 0.0;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return 0.0;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total;
}

double DatabaseStorage::get_total_writeoffs() {
    const char* sql = "SELECT SUM(price * count) FROM checks WHERE type = 2";
    sqlite3_stmt* stmt;
    double total = 0.0;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return 0.0;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total;
}

std::map<std::string, double> DatabaseStorage::get_employee_sales_stats() {
    std::map<std::string, double> stats;
    const char* sql = "SELECT employee_name, SUM(price * count) FROM checks WHERE type = 0 GROUP BY employee_name";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return stats;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string employee = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        double total = sqlite3_column_double(stmt, 1);
        stats[employee] = total;
    }

    sqlite3_finalize(stmt);
    return stats;
}

bool DatabaseStorage::save_user(const std::string& username, const std::string& password, const std::string& role) {
    const char* sql = "INSERT OR REPLACE INTO users (username, password, role) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseStorage::validate_user(const std::string& username, const std::string& password) {
    const char* sql = "SELECT 1 FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_ROW;
}

bool DatabaseStorage::update_user_password(const std::string& username, const std::string& newPassword) {
    const char* sql = "UPDATE users SET password = ? WHERE username = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseStorage::update_user_role(const std::string& username, const std::string& newRole) {
    const char* sql = "UPDATE users SET role = ? WHERE username = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, newRole.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseStorage::delete_user(const std::string& username) {
    const char* sql = "DELETE FROM users WHERE username = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseStorage::user_exists(const std::string& username) {
    const char* sql = "SELECT 1 FROM users WHERE username = ? LIMIT 1";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_ROW;
}
