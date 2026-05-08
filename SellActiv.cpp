#include "SellActiv.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

PromoManager::PromoManager() : discount_used(false) {}

void PromoManager::show_promo_menu(double& total_sum, const std::vector<std::pair<std::string, double>>& cart) {
    if (discount_used) {
        std::cout << "Акция уже использована!\n";
        return;
    }

    std::string choice;
    std::cout << "1. Скидка 15% (от 2000 руб)\n";
    std::cout << "2. Математическая задача (30%)\n";
    std::cout << "3. Мясная акция (10кг = -200 руб)\n";
    std::cout << "4. Колесо Фортуны (1000 руб)\n";
    std::cout << "0. Пропустить\n";
    std::cout << "Выберите акцию: ";
    Getline(choice);

    if (choice == "1") {
        if (total_sum >= 2000) {
            double discount = total_sum * 0.15;
            total_sum -= discount;
            discount_used = true;
            std::cout << "Скидка 15% применена!\n";
        } else {
            std::cout << "Сумма менее 2000 руб\n";
        }
    } else if (choice == "2") {
        apply_math_quiz(total_sum);
    } else if (choice == "3") {
        apply_meat_promotion(total_sum, cart);
    } else if (choice == "4") {
        apply_wheel_of_fortune(total_sum);
    }
}

void PromoManager::apply_math_quiz(double& total_sum) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(10, 50);

    int a = dist(gen);
    int b = dist(gen);
    int answer;

    std::cout << "Решите пример: " << a << " + " << b << " = ";
    Getline(answer);

    if (answer == a + b) {
        double discount = total_sum * 0.30;
        total_sum -= discount;
        discount_used = true;
        std::cout << "Верно! Скидка 30%!\n";
    } else {
        std::cout << "Неверно!\n";
    }
}

void PromoManager::apply_meat_promotion(double& total_sum, const std::vector<std::pair<std::string, double>>& cart) {
    double weight = 0;
    for (const auto& item : cart) {
        if (item.first == "Мясо" || item.first == "мясо") {
            weight += item.second;
        }
    }

    if (weight >= 10) {
        total_sum -= 200;
        discount_used = true;
        std::cout << "Мясная акция применена! -200 руб\n";
    } else {
        std::cout << "Нужно минимум 10 кг мяса (у вас " << weight << " кг)\n";
    }
}

void PromoManager::apply_wheel_of_fortune(double& total_sum) {
    std::string choice;
    std::cout << "Участие стоит 1000 руб\n";
    std::cout << "Хотите участвовать? (y/n): ";
    Getline(choice);

    if (choice != "y" && choice != "Y") {
        std::cout << "Отказ от участия\n";
        return;
    }

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(1, 100);

    int prize = dist(gen);

    if (prize >= 1 && prize <= 50) {
        std::cout << "Поздравляем! Скидка 10%!\n";
        total_sum -= total_sum * 0.10;
        discount_used = true;
    } else if (prize >= 51 && prize <= 75) {
        std::cout << "Поздравляем! Скидка 20%!\n";
        total_sum -= total_sum * 0.20;
        discount_used = true;
    } else if (prize >= 76 && prize <= 90) {
        std::cout << "Поздравляем! Скидка 30%!\n";
        total_sum -= total_sum * 0.30;
        discount_used = true;
    } else if (prize >= 91 && prize <= 99) {
        std::cout << "Поздравляем! Скидка 50%!\n";
        total_sum -= total_sum * 0.50;
        discount_used = true;
    } else if (prize == 100) {
        std::cout << "ДЖЕКПОТ! Покупка бесплатно!\n";
        total_sum = 0;
        discount_used = true;
    } else {
        std::cout << "Увы, без приза...\n";
        total_sum += 1000;
    }

    if (total_sum < 0) total_sum = 0;
}
