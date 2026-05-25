#include "../include/MethodSuperAdmin.h"
#include <iostream>
#include "../include/Product.h"
#include "../include/AuthSystemUser.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include "stdlib.h"
#include <thread>
#endif

void storage_user_method(AuthSystemUser* auth) {
    std::string choice;
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    bool is_exit = false;
    while (true) {
        std::cout << "Вы выбрали опцию редактирование персонала\n";
        std::cout << "Вам доступны такие опции:\n";
        std::cout << "1) Показ всех пользователей\n";
        std::cout << "2) Добавить нового пользователя\n";
        std::cout << "3) Изменить пароль пользователю или статус\n";
        std::cout << "4) Удалить пользователя\n";
        std::cout << "5) Выход из опции в меню\n";
        std::cout << "Ввод: ";
        Getline(choice);

        if(choice.size() > 2) {
            std::cerr << "Ошибка: введите номер действия\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if (choice.empty()) {
            std::cerr << "Ошибка: введите номер действия\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        bool is_valid = true;
        for (char c : choice) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) {
            std::cerr << "Ошибка: введите только цифры от 0 до 6\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        if(std::stoi(choice) <= 0 || std::stoi(choice) > 5) {
            std::cerr << "Ошибка. Введите цифру от 1 до 5\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        switch(std::stoi(choice)) {
        case 1:
            auth->show_all_users();
            break;
        case 2:
            auth->register_user();
            break;
        case 3:
            auth->change_user();
            break;
        case 4:
            auth->remove_user();
            break;
        case 5:
            is_exit = true;
            break;
        default:
            std::cerr << "Неизвестная опция\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        }

        if(is_exit) break;

    }

#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
    return;
}
