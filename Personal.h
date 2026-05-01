#ifndef _PERSONAL_H_
#define _PERSONAL_H_

#include <stdio.h>

#include "Libraries.h"
#include "AuthSystemUser.h"

class Personal : public AuthSystemUser {
public:
	void ManagePersonal();
	void RemoveUser();
	void UserPassChange();
	void UserRoleChange();
protected:

private:

};

#endif