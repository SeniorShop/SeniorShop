#include "Product.h";

Product::Product(const Product& other) {
    this->name = other.name;
    this->category = other.category;
    this->price = other.price;
    this->article = other.article;
    this->begin_date = other.begin_date;
    this->end_date = other.end_date;
    this->count = other.count;
    this->manufacturer = other.manufacturer;
    this->country = other.country;
}
Product::Product() = default;
bool Product::operator==(const Product& other) const {
    return name == other.name && category == other.category && article == other.article;
}

