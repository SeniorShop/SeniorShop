#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
class User
{
	// !!! ����� ������������ � �������� ��� ����� public 
public:
	std::string username;
	std::string password;
	std::string status; // superadmin, admin, user
	User() = default;
	User(const std::string& name, const std::string& pass, const std::string& currentSt) : username(name), password(pass), status(currentSt) {}
	virtual ~User() = default;

	virtual std::string serialize() const
	{
		return username + "|" + password + "|" + status;
	}
	// �������������� �� �����
	static std::unique_ptr<User> deserialize(const std::string& line);

	virtual std::unique_ptr<User> clone() const = 0;

	bool operator==(const User& other) const
	{
		return username == other.username && password == other.password && status == other.status;
	}

};

class SuperAdminUser final : public User {
public:
	SuperAdminUser();
	std::unique_ptr<User> clone() const override;
};

class AdminUser : public User {
public:
	AdminUser(const std::string& name, const std::string& pass);
	std::unique_ptr<User> clone() const override;
};

class RegularUser : public User {
public:
	RegularUser(const std::string& name, const std::string& pass);
	std::unique_ptr<User> clone() const override;
};

#endif // USER_H
