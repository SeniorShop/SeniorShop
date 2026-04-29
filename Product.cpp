#include <map>

#include "Product.hpp"
#include <fstream>
std::string category;
double price;
int article;
std::string beginDate;
std::string  endDate;
unsigned int count;
std::string manufacturer;
std::string country;
class StorageProduct
{
private:
	std::vector<Product> goods;
public:
    void addProduct(const Product& prod) // new product
    {
        std::string choose;
        std::ifstream BdProduct("Product.txt");
        Product _prodEat;
        if (!BdProduct.is_open()) { std::cout << "Ошибка!"; return; }
        else
        {
            
            while (BdProduct >> _prodEat.name >> _prodEat.category >> _prodEat.price >> _prodEat.article >> _prodEat.beginDate >> _prodEat.endDate
                >> _prodEat.count >> _prodEat.manufacturer >> _prodEat.country)
            {
                goods.push_back(_prodEat);
            }
            BdProduct.close();
            std::cout << "Все товары загружены из бд.txt\n";
        }

        while (true)
        {
            std::cout << "Добавление товара\n";
            std::cout << "Введите название товара\n";
            std::getline(std::cin, _prodEat.name);
            std::cout << "Введите категорию товара, пример Выпечка\n";
            std::getline(std::cin, _prodEat.category);
            std::cout << "Введите цену товара\n";
            std::cin >> _prodEat.price;
            std::cout << "Введите артикул\n";
            std::cin >> _prodEat.article;
            std::cout << "Введите срок годности, от какого числа\n";
            std::cin >> _prodEat.beginDate;
            std::cout << "Введите срок годности, от какого числа\n";

            
            
        }

    }






    void removeProduct(int id); // delete product
    bool updateProduct(int id, const Product& newData); // update product
    //Product* findById(int id);
    const std::map<int, Product>& getAll() const;
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
    void showAll() const;   // вывод на экран



};