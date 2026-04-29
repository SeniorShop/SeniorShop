#pragma once
#include <iostream>
#include <string>
#ifndef PRODUCT_HPP
#define PRODUCT_HPP

inline void Getline(std::string& str)
{
	std::getline(std::cin, str, '\n');
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(365000, '\n');
	}
}

struct Product
{
	std::string name;
	std::string category;
	double price;
	int article;
	std::string beginDate;
	std::string  endDate;
	unsigned int count;
	std::string manufacturer;
	std::string country;

};


#endif // !PRODUCT_HPP

