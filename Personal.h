#ifndef _PERSONAL_H_
#define _PERSONAL_H_

#include "Libraries.h"
#include "User.h"

class Personal {
public:
	int ManagePersonal();
	bool IsSupAdmin();
	void AddUser();
	void DeleteUser();
	void UserPassChange();
	void UserRoleChange();
	void PrintUsers();
protected:

private:

};

#endif