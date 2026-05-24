#ifndef SUPPLIERMANAGER_H
#define SUPPLIERMANAGER_H
#include <string>
#include <vector>
#include <map>

struct SupplierManager {
    static std::map<std::string, std::vector<std::string>> load_suppliers();
    static std::string choose_supplier();
    static std::string choose_category(const std::string& supplier);
};

#endif
