#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H
#include "AuthSystemUser.h"

class LoginSystem {
    AuthSystemUser asu;
public:
    void launch();
private:
    void registration_button();
    void authorization_button();
    void change_user_button();
    void show_all_users_button();
    void exit_button();
};

#endif // LOGINSYSTEM_H
