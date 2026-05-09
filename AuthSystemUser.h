#ifndef AUTH_SYSTEM_USER_HPP
#define AUTH_SYSTEM_USER_HPP
#include "User.h"
#include "Logger.h"
#include "Captcha.h"
#include "Storage.h"
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

class AuthSystemUser {
    Storage start_storage;                      //��������� ������ (������������ ������ ��� �������)
    Captcha check_bot;                          //����� ��� �����
    std::vector<std::unique_ptr<User>> users;   // ����������� ���������
    User* currentUser_ = nullptr;               // �������� ������������

    // ��������������� ������
    bool is_valid_username(const std::string& username) const;
    bool is_valid_pass(const std::string& password) const;
    bool user_exists(const std::string& username) const;

    void load_from_file();
    void save_to_file();

    // ���������� ������ �������������� (���������� �� change_user)
    void user_pass_change();
    void user_status_change();

public:
    AuthSystemUser();

    User* login();              // ���������� ��������� �� ��������� ������������ (nullptr ��� ������)
    void register_user();       // �������� ������ ������������  !! superadmin
    void change_user();         // ��������� ������������� ������������ !! superadmin
    void show_all_users();      // �������� ���� ������������� // superadmin ��� admin
    void remove_user();         // �������� ������������ !! superadmin

    User* getCurrentUser() const { return currentUser_; }
};

#endif
