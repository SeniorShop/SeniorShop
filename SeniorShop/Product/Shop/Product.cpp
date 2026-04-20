#include "Product.hpp"
#include <iostream>

std::set<int> Product::UsedIds;


Product::Product(int id, const std::string& name, double price, int count, Type type)
{
	ProductType = type;

	SetId(id);
	SetName(name);
	SetPrice(price);
	SetCount(count);
}

int Product::GetId() const { return Id; }
const std::string& Product::GetName() const { return Name; }
double Product::GetPrice() const { return Price; }
int Product::GetCount() const { return Count; }
Product::Type Product::GetType() const { return ProductType; }

std::optional<std::chrono::sys_days> Product::GetExpirationDate() const
{
	return ExpirationDate;
}


void Product::SetId(int id)
{
	while (true)
	{
		if (id <= 0)
		{
			std::cout << "ID должен быть > 0\n";
		}
		else if (UsedIds.count(id))
		{
			std::cout << "ID уже существует\n";
		}
		else
		{
			UsedIds.erase(Id);
			Id = id;
			UsedIds.insert(Id);
			break;
		}

		std::cout << "Введите новый ID: ";
		std::cin >> id;
	}
}

void Product::SetName(const std::string& name)
{
	std::string input = name;

	while (true)
	{
		if (ValidateName(input))
		{
			Name = input;
			break;
		}

		std::cout << "Введите имя: ";
		std::cin >> input;
	}
}

void Product::SetPrice(double price)
{
	while (true)
	{
		if (ValidatePrice(price))
		{
			Price = price;
			break;
		}

		std::cout << "Введите цену: ";
		std::cin >> price;
	}
}

void Product::SetCount(int count)
{
	while (true)
	{
		if (ValidateCount(count))
		{
			Count = count;
			break;
		}

		std::cout << "Введите количество: ";
		std::cin >> count;
	}
}

void Product::SetExpirationDate(int year, int month, int day)
{
	using namespace std::chrono;

	if (ProductType == Type::WithoutExpiration)
	{
		std::cout << "У товара нет срока годности\n";
		return;
	}

	ExpirationDate = sys_days{
		year_month_day
		{
			std::chrono::year{year},
			std::chrono::month{month},
			std::chrono::day{day}
		}
	};
}


bool Product::IsExpired() const
{
	if (!ExpirationDate.has_value())
		return false;

	auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());

	return today > ExpirationDate.value();
}


bool Product::ValidateName(const std::string& name)
{
	if (name.empty())
	{
		std::cout << "Имя не может быть пустым\n";
		return false;
	}
	return true;
}

bool Product::ValidatePrice(double price)
{
	if (price < 0)
	{
		std::cout << "Цена не может быть < 0\n";
		return false;
	}
	return true;
}

bool Product::ValidateCount(int count)
{
	if (count < 0)
	{
		std::cout << "Количество не может быть < 0\n";
		return false;
	}
	return true;
}