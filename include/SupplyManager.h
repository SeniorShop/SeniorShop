#ifndef SUPPLY_MANAGER_H
#define SUPPLY_MANAGER_H

#include "Supply.h"
#include <vector>
#include <string>

class Storage;

class SupplyManager {
    std::vector<Supply> supplies;
    std::string filename = "Supplies.txt";
public:
    SupplyManager();
    void add_to_file(const Supply& supply);
    void create_supplies();
    void show_all_supplies();
    void change_supply_from_file();
    void delete_supply_from_file();
    void apply_supply_to_storage(Storage& storage);
    void show_all_supplies_for_actions();

    void load_from_file();
    std::vector<Supply>& get_all_supplies() { return supplies; }
    bool add_supply(const Supply& supply);
    bool delete_supply_by_number(unsigned int number);

private:
    bool validate_supply_dates(const std::string& begin_date, const std::string& end_date);
};

#endif
