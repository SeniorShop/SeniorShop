#include "../include/LoginSystem.h"
#ifdef _WIN32
#include "Windows.h"
#else
#include <stdlib.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <algorithm>

void LoginSystem::launch() {
    std::string choice;

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    bool is_exit = false;

    while (true) {
        std::cout << "\n\n\n\t\t\tАпрельское возвращение\n\n\n";
        std::cout << "1) Вход\n2) Выход\nВыберите действие: ";
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

        std::size_t start = choice.find_first_not_of(" \t\n\r\f\v");
        if (start != std::string::npos) {
            choice = choice.substr(start);
        }
        std::size_t end = choice.find_last_not_of(" \t\n\r\f\v");
        if (end != std::string::npos) {
            choice = choice.substr(0, end + 1);
        }

        if (choice.length() != 1 || (std::stoi(choice) != 1 && std::stoi(choice) != 2)) {
            std::cerr << "Ошибка: введите 1 или 2\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            continue;
        }

        User* logger_user = nullptr;

        switch(std::stoi(choice)) {
        case 1:
            logger_user = asu.login();
            break;
        case 2:
            is_exit = true;
            break;
        default:
            std::cerr << "Неверное действие\n";
            break;
        }

        if (logger_user) {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            get_start.set_auth_system(&asu);
            get_start.start(logger_user->status);
        }

        if (is_exit) break;
    }

    exit_button();
}

void LoginSystem::exit_button() {
    std::cout << "Завершение программы" << std::endl;
    exit(0);
}
