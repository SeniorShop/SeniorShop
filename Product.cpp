#include "Product.h"

Product::Product()
    : price(0.0), article(0), count(0) {}

Product::Product(const Product& other) {
    name = other.name;
    category = other.category;
    price = other.price;
    article = other.article;
    begin_date = other.begin_date;
    end_date = other.end_date;
    count = other.count;
    manufacturer = other.manufacturer;
    country = other.country;
}

bool Product::operator==(const Product& other) const {
    return article == other.article;
}
