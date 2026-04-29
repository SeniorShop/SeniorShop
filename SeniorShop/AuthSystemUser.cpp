#include "Logger.hpp"
#include "Cmd.hpp"
#include <vector>
#include <string>
#include <iostream>
#include "AuthSystemUser.hpp"
#include <sstream> 
#include <fstream>

AuthSystemUser::AuthSystemUser(const std::string& filename) : _usersFilename(filename)
{
	loadFromFile();
	if (_users.empty())
	{
		_users.push_back({ "SuperAdmin", "admin***123", "superadmin" });
		saveToFile();
		std::cout << "const superadmin, áóäåò ïðîâåðêà ïîòîì (ëîãèí SuperAdmin, ïàðîëü admin***123)\n";

	}
}

AuthSystemUser::~AuthSystemUser()
{
	saveToFile();
}

bool AuthSystemUser::CheckLogin(const std::string& str)
{
    // äîïîëíèòü: 
    // ïðîâåðêó, ñóùåñòâóåò ëè ïîëüçîâàòåëü

    if (str.size() < 5 || str.size() >= 20)
    {
        std::cout << "Íåäîïóñòèìàÿ äëèíà ëîãèíà. Îò 5 äî 20 ñèìâîëîâ\n";
        return false;

    }
    std::unordered_set<char> specialSymbols;
    for (char i = 'A'; i <= 'Z'; i++)
    {
        specialSymbols.insert(i);
    }
    for (char i = 'a'; i <= 'z'; i++)
    {
        specialSymbols.insert(i);
    }
    for (char i = '0'; i <= '9'; i++)
    {
        specialSymbols.insert(i);
    }
    for (char symb : str)
    {
        if (!specialSymbols.count(symb))
        {
            std::cout << "Íåêîððåêòíûå ñèìâîëû â ëîãèíå\n\n";
            return false;
        }
    }
   
    return true;
}

bool AuthSystemUser::CheckPass(const std::string& str)
{
    if (str.size() < 5 || str.size() >= 64)
    {
        std::cout << "Íåäîïóñòèìàÿ äëèíà ïàðîëÿ. Îò 5 äî 64 ñèìâîëîâ\n";
        return false;

    }
    int symbolsCount = 0;
    std::unordered_set<char> specialSymbols;
    std::unordered_set<char> passSymbols{ '!', '@', '#', '%', '^', '&', '*', '(',')', '-', '_', '=', '+', '/', '?', '|', '\\', '\"', '\'', ',', '.', '>', '<', '~', '`', ':', ';', '{','}', '[', ']' };

    for (char i = '!'; i <= '~'; i++)
    {
        specialSymbols.insert(i);
    }

    for (char symb : str)
    {
        if (!specialSymbols.count(symb))
        {
            
            std::cout << "Íåêîððåêòíûå ñèìâîëû â ïàðîëå\n\n";
            return false;
        }
    }

    for (char symb : str)
    {
            
        if (passSymbols.count(symb))
        {
            symbolsCount++;
            
        }

    }
    if (symbolsCount >= 3)
    {
        return true;
    }
  
    std::cout << "Òðåáóåòñÿ ìèíèìóì 3 ñïåöèàëüíûõ ñèìâîëà\n\n";
    return false;
}

bool AuthSystemUser::userExists(const std::string& _login) const
{
	for (const auto& user : _users)
	{
		if (user._login == _login) return true;
	}
	return false;
}

bool AuthSystemUser::isValidCurrentStatus(const std::string& currentStatus) const
{
	return (currentStatus == "superadmin" || currentStatus == "admin" || currentStatus == "user");
}

//ðàáîòà ñ ôàéëàìè

void AuthSystemUser::loadFromFile()
{
	_users.clear();
	std::ifstream in(_usersFilename);
	if (!in.is_open()) return;

	std::string choose;
	User _u;
	while (std::getline(in, choose))
	{
		if (choose.empty()) continue; //ïðîâåðêà íà null
		std::istringstream iss(choose);

		if (std::getline(iss, _u._login, '|')
			&& (std::getline(iss, _u._password, '|'))
			&& (std::getline(iss, _u._currentStatus))) {
			_users.push_back(_u);
		}
	}


}

void AuthSystemUser::saveToFile() const
{
	std::ofstream out(_usersFilename);
	if (!out.is_open()) return;
	for (const auto& u : _users)
	{
		out << u._login << '|' << u._password << '|' << u._currentStatus << '\n';
	}
}
