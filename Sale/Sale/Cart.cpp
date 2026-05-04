#include "Cart.hpp"

void Cart::add(const Product& product, int qty) 
{
    items.push_back({ product, qty });
}

double Cart::total() const 
{
    double sum = 0;
    for (const auto& item : items) 
    {
        sum += item.product.GetPrice() * item.quantity;
    }
    return sum;
}

bool Cart::empty() const 
{
    return items.empty();
}

void Cart::clear() 
{
    items.clear();
}

const std::vector<CartItem>& Cart::getItems() const 
{
    return items;
}