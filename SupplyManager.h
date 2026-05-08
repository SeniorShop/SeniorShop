#ifndef SUPPLY_MANAGER_H
#define SUPPLY_MANAGER_H
#include "Supply.h"
#include <vector>
#include <string>

class Storage;

class SupplyManager {
private:
    std::vector<Supply> supplies;
    std::string filename = "Supplies.txt";

    void add_to_file(const Supply& supply);
    void load_from_file();

public:
    SupplyManager();

    void create_supplies();
    void show_all_supplies();
    void change_supply_from_file();
    void delete_supply_from_file();
    void apply_supply_to_storage(Storage& storage);
};

#endif
