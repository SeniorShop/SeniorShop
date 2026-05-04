#include "SaleService.hpp"
#include <iostream>

SaleService::SaleService(Storage& s) : storage(s) 
{

}

void SaleService::load() 
{
    storage.load_from_file("Product.txt");

    std::ifstream input("Product.txt");
    Product p(1, "tmp", 1, 1, Product::Type::WithoutExpiration);

    products.clear();

    //while (input >> p.SetName, false) 
    //{
    //   
    //    break;
    //}
}

const std::vector<Product>& SaleService::getProducts() const 
{
    return products;
}

bool SaleService::addToCart(Cart& cart, int id, int qty) 
{
    for (auto& p : products) 
    {
        if (p.GetId() == id) 
        {
            if (p.GetCount() < qty) 
            {
                return false;
            }
            cart.add(p, qty);
            return true;
        }
    }
    return false;
}

void SaleService::applySale(const Cart& cart) 
{
    std::cout << "Продажа завершена\n";
}