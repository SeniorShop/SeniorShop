#include "../include/Cart.h"

void Cart::add(const Product& product, unsigned int count) {
    items.push_back({product, count});
}
double Cart::total() const {
    double sum = 0;
    for (const auto& item : items) {
        sum += item.product.price * item.count;
    }
    return sum;
}
bool Cart::empty() const {
    return items.empty();
}

void Cart::clear() {
    items.clear();
}
const std::vector<CartItem>& Cart::get_items() const {
    return items;
}
