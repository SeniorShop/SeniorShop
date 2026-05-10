#include "Product.h"

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
Product::Product() : name(), category(), price(0.0), article(0), begin_date(), end_date(), count(0), manufacturer(), country() {}
bool Product::operator==(const Product& other) const {
    return name == other.name && category == other.category && article == other.article;
}

//void setColor(int color)
//{
//    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//    SetConsoleTextAttribute(hConsole, color);
//}

//void resetColor()
//{
//    setColor(7);
//}

