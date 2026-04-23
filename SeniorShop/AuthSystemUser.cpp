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
		std::cout << "const superadmin, будет проверка потом (логин SuperAdmin, пароль admin***123)\n";

	}
}

AuthSystemUser::~AuthSystemUser()
{
	saveToFile();
}

bool AuthSystemUser::isValidLogin(const std::string& login) const
{
	if (login.size() < 5 || login.size() > 20) return false;
	for (char ch : login)
	{
		unsigned char uc = static_cast<unsigned char>(ch);
		if (!(std::isalnum(uc)))
			return false;
	}
	return true;
}

bool AuthSystemUser::isValidPass(const std::string& pass) const
{
	return pass.size() >= 8;
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

//работа с файлами

void AuthSystemUser::loadFromFile()
{
	_users.clear();
	std::ifstream in(_usersFilename);
	if (!in.is_open()) return;

	std::string choose;
	User _u;
	while (std::getline(in, choose))
	{
		if (choose.empty()) continue; //проверка на null
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