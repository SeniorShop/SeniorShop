#ifndef AUTH_SYSTEM_USER_HPP
#define AUTH_SYSTEM_USER_HPP

#include "Logger.hpp"
#include "Cmd.hpp"
#include <vector>
#include <string>


struct User
{
	std::string _login;
	std::string _password;
	std::string _currentStatus;
};
class AuthSystemUser
{
private:
	std::vector<User> _users;
	std::string _usersFilename;
	bool isValidLogin(const std::string& login) const;
	bool isValidPass(const std::string& pass) const;
	bool isValidCurrentStatus(const std::string& currentStatus) const;
	bool userExists(const std::string&_login) const;
	void loadFromFile();
	void saveToFile() const;

	
public:
	AuthSystemUser(const std::string& filename = "users.txt");
	~AuthSystemUser(); //  = default;

	bool registerUser(const std::string& _login, const std::string _password, const std::string _currentstatus);
	bool login(const std::string& login, const std::string& _password);
	bool changeUser(const std::string& _oldLogin, const std::string& _newLogin, const std::string _newPassword, const std::string _newcurrentStatus);

	void snowAllUsers() const;
	bool isSuperAdminExists() const; 

};


#endif 