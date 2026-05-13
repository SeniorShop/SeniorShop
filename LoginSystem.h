#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H
#include "AuthSystemUser.h"
#include "Storage.h"

class LoginSystem {
    AuthSystemUser asu;
    Storage get_start;
    void exit_button();
public:
    void launch();
};

#endif // LOGINSYSTEM_H
