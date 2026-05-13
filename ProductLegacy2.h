#include <string>
#include <iostream>
#include <set>

std::set<std::string> Product::UsedIds;

class Product
{
    static std::set<std::string> UsedIds;
private:
    const std::string Id;
    std::string Name;
    double Price;
    int StockQuantity;
    std::string ExpirationDate;
public:
    Product(const std::string& id, const std::string& name, double price, int stockQuantity, const std::string& expirationDate) : Id(id)
    {
        if (Id.empty() || UsedIds.count(Id)) 
        {
            std::string newId;
            while (true)
            {
                std::cout << "Введите ID: ";
                std::cin >> newId;
                if (newId.empty())
                {
                    std::cout << "ID не может быть пустым\n";
                    continue;
                }
                if (UsedIds.count(newId))
                {
                    std::cout << "ID уже существует, введите другой\n";
                    continue;
                }
                const_cast<std::string&>(Id) = newId;
                break;
            }
        }
        UsedIds.insert(Id);
        SetName(name);
        SetPrice(price);
        SetStock(stockQuantity);
        SetExpirationDate(expirationDate);
    }


    const std::string& GetId() const 
    {
        return Id;
    }

    const std::string& GetName() const
    { 
        return Name;
    }

    double GetPrice() const
    { 
        return Price;
    }

    int GetStockQuantity() const
    { 
        return StockQuantity;
    }

    const std::string& GetExpirationDate(const std::string& date)
    {
        return ExpirationDate;
    }

    void ChangePrice(double newPrice)
    {
        while (true) 
        {
            std::cin >> newPrice;
            if (newPrice < 0) 
            {
                std::cout << "Цена не может быть меньше нуля\n";

                continue;
            }
            Price = newPrice;

            break;
        }
    }


    void IncreaseStock(int amount)
    {
        while (true) 
        {
            std::cin >> amount;
            if (amount <= 0) 
            {
                std::cout << "Количество должно быть больше нуля\n";

                continue;
            }
            StockQuantity += amount;

            break;
        }
    }


    void ReduceStock(int amount)
    {
        while (true) 
        {
            std::cin >> amount;
            if (amount <= 0) 
            {
                std::cout << "Количество должно быть больше нуля\n";

                continue;
            }
            if (amount > StockQuantity) 
            {
                std::cout << "Невозможно снизить количество ниже нуля\n";

                continue;
            }
            StockQuantity -= amount;

            break;
        }
    }


private:
    void SetName(const std::string& name)
    {
        std::string input = name;
        while (true) 
        {
            if (input.empty()) 
            {
                std::cout << "Имя товара не должно быть пустым \nВведите имя: ";

                std::cin >> input;
                continue;
            }
            Name = input;

            break;
        }
    }


    void SetPrice(double price)
    {
        double input = price;
        while (true) 
        {
            if (input < 0) 
            {
                std::cout << "Цена товара не может быть отрицательной \nВведите цену: ";

                std::cin >> input;
                continue;
            }
            Price = input;

            break;
        }
    }


    void SetStock(int stockQuantity)
    {
        int input = stockQuantity;
        while (true) 
        {
            if (input < 0) 
            {
                std::cout << "Количество товара не может быть отрицательным \nВведите количество: ";

                std::cin >> input;
                continue;
            }
            StockQuantity = input;

            break;
        }
    }


    void SetExpirationDate(const std::string& date)
    {
        std::string input = date;
        while (true) 
        {
            if (input.empty()) 
            {
                std::cout << "Срок годности не может быть пустым\nВведите дату: ";

                std::cin >> input;
                continue;
            }
            ExpirationDate = input;

            break;
        }
    }
};

