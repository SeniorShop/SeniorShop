#include <fstream>
#include <vector>
#include <string>
#include "Product.hpp"

class Storage {
private:
    std::vector<Product> goods;

public:
    // Загрузка всех товаров из файла (вызывать один раз в начале)
    void loadFromFile(const std::string& ProductBD) {
        std::ifstream in(ProductBD);
        if (!in.is_open()) {
            std::cout << "Файл " << ProductBD << " не найден. Склад пуст.\n";
            return;
        }
        goods.clear();
        Product _prodEat;
        while (in >> _prodEat.name >> _prodEat.category >> _prodEat.price >> _prodEat.article >> _prodEat.beginDate >> _prodEat.endDate
            >> _prodEat.count >> _prodEat.manufacturer >> _prodEat.country) {
            goods.push_back(_prodEat);
        }
        in.close();
        std::cout << "Загружено " << goods.size() << " товаров.\n";
    }

    // Сохранение всех товаров в файл
    void saveToFile(const std::string& ProductBD) const {
        std::ofstream out(ProductBD);
        if (!out.is_open()) {
            std::cout << "Ошибка открытия файла для записи!\n";
            return;
        }
        for (const auto& _prodEat : goods) {
            out << _prodEat.name << ' ' << _prodEat.category << ' ' << _prodEat.price << ' '
                << _prodEat.article << ' ' << _prodEat.beginDate << ' ' << _prodEat.endDate << ' '
                << _prodEat.count << ' ' << _prodEat.manufacturer << ' ' << _prodEat.country << '\n';
        }
        out.close();
        std::cout << "Сохранено " << goods.size() << " товаров.\n";
    }

    
    void addProduct() {
        Product newProd;
        std::cin.ignore();

        std::cout << "Добавление товара\n";
        std::cout << "Введите название товара: ";
        Getline(newProd.name);
        std::cout << "Введите категорию товара (пример: Выпечка): ";
        Getline(newProd.category);
        std::cout << "Введите цену товара: ";
        std::cin >> newProd.price;
        std::cout << "Введите артикул: ";
        std::cin >> newProd.article;
        std::cin.ignore();
        std::cout << "Введите срок годности (от): ";
        Getline(newProd.beginDate);
        std::cout << "Введите срок годности (до): ";
        Getline(newProd.endDate);
        std::cout << "Введите количество: ";
        std::cin >> newProd.count;
        std::cin.ignore();
        std::cout << "Введите производителя: ";
        Getline(newProd.manufacturer);
        std::cout << "Введите страну производства: ";
        Getline(newProd.country);

      
        goods.push_back(newProd);
        std::cout << "Товар добавлен!\n";

     
        saveToFile("Product.txt");
    }

    // Показать все товары
    void showAll() const {
        if (goods.empty()) {
            std::cout << "Склад пуст.\n";
            return;
        }
        for (size_t i = 0; i < goods.size(); ++i) {
            const auto& _prodEat = goods[i];
            std::cout << i + 1 << ". " << _prodEat.name << " | " << _prodEat.category
                << " | цена: " << _prodEat.price << " | артикул: " << _prodEat.article
                << " | годен: " << _prodEat.beginDate << " - " << _prodEat.endDate
                << " | кол-во: " << _prodEat.count << " | произв.: " << _prodEat.manufacturer
                << " | страна: " << _prodEat.country << '\n';
        }
    }
};