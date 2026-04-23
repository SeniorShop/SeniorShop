#ifndef CMD_HPP
#define CMD_HPP
#include <iostream>
#include <string>
inline void Getline(std::string& str)
{
	std::getline(std::cin, str, '\n');
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(365000, '\n');
	}
}
void Error()
{
	std::cout << "Ошибка!\n";
}
inline void Err(int a = 1500)
{
	std::cout << "Неккоректный ввод!\n";
	
}

#endif // !CMD_HPP
