#include "MethodSuperAdmin.h"
#include <iostream>
#include "Product.h"
#include "AuthSystemUser.h"
#ifdef _WIN32
#include <windows.h>
#else
#include "stdlib.h"
#include <thread>
#endif

void storage_user_method(AuthSystemUser* auth) {
    std::string choose;
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
        Getline(choose);

        if(choose.empty() || choose[0] < '0' || choose[0] > '5') {
            std::cerr << "Ошибка. Введите цифру от 1 до 5\n";
            continue;
        }

        switch(choose[0]) {
        case '1':
            auth->show_all_users();
            break;
        case '2':
            auth->register_user();
            break;
        case '3':
            auth->change_user();
            break;
        case '4':
            auth->remove_user();
            break;
        case '5':
            is_exit = true;
            break;
        }

        if(is_exit) break;

    }

#ifdef _WIN32
            Sleep(2000);
            system("cls");
            return;
#else
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            system("clear");
            return;
#endif
}
