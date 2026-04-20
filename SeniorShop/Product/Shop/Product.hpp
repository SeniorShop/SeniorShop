#pragma once
#include <string>
#include <set>
#include <chrono>
#include <optional>
#include <iostream>

class Product
{
public:
	enum class Type
	{
		WithExpiration,
		WithoutExpiration
	};

	Product(int id, const std::string& name, double price, int count, Type type);

	
	int GetId() const;
	const std::string& GetName() const;
	double GetPrice() const;
	int GetCount() const;
	Type GetType() const;

	std::optional<std::chrono::sys_days> GetExpirationDate() const;

	
	void SetId(int id);
	void SetName(const std::string& name);
	void SetPrice(double price);
	void SetCount(int count);

	void SetExpirationDate(int year, int month, int day);

	
	bool IsExpired() const;

private:
	static std::set<int> UsedIds;

	int Id;
	std::string Name;
	double Price;
	int Count;

	Type ProductType;

	std::optional<std::chrono::sys_days> ExpirationDate;

	
	bool ValidateName(const std::string& name);
	bool ValidatePrice(double price);
	bool ValidateCount(int count);
};