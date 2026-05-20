#include "SaleControl.h"
#include "SellActiv.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>

void SaleController::start() {
    while (true) {
        service.refresh();
        const std::vector<Product>& products = service.get_products();

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        if (products.empty()) {
            std::cerr << "Нет доступных товаров для продажи\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        }

        std::cout << "\n\n\n\t\t\tДОСТУПНЫЕ ТОВАРЫ\n\n\n";
        std::cout << std::left
                  << std::setw(5) << "ID"
                  << std::setw(25) << "Название"
                  << std::setw(20) << "Категория"
                  << std::setw(10) << "Цена"
                  << std::setw(10) << "В наличии"
                  << std::setw(10) << "Артикль"
                  << "\n";
        std::cout << std::string(85, '-') << "\n";

        for (const auto& p : products) {
            std::string name = p.name.empty() ? "Без названия" : p.name;
            std::string category = p.category.empty() ? "Без категории" : p.category;

            std::cout << std::left
                      << std::setw(5) << p.id
                      << std::setw(25) << (name.length() > 23 ? name.substr(0, 21) + ".." : name)
                      << std::setw(20) << (category.length() > 18 ? category.substr(0, 16) + ".." : category)
                      << std::setw(10) << std::fixed << std::setprecision(2) << p.price
                      << std::setw(10) << p.count
                      << std::setw(10) << p.article
                      << "\n";
        }
        std::cout << std::string(85, '-') << "\n";

        std::string input;
        std::cout << "\nВведите ID или артикль товара (или 'exit' для выхода): ";
        Getline(input);

        if (input.empty()) {
            continue;
        }

        if (input == "exit") {
            break;
        }

        int id;
        try {
            id = std::stoi(input);
        } catch (...) {
            std::cerr << "Ошибка: введите число (ID или артикль)\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        const Product* selected = nullptr;
        for (const auto& p : products) {
            if (p.id == id) {
                selected = &p;
                break;
            }
        }

        if (!selected) {
            for (const auto& p : products) {
                if (p.article == id) {
                    selected = &p;
                    break;
                }
            }
        }

        if (!selected) {
            std::cerr << "Товар не найден\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        unsigned int count;
        std::cout << "Введите количество: ";
        Getline(count);

        if (count == 0) {
            std::cerr << "Количество должно быть больше 0\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        if (selected->count < count) {
            std::cerr << "Недостаточно товара на складе (доступно: " << selected->count << ")\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        std::cout << "\nВы хотите добавить в корзину:\n";
        std::cout << "Товар: " << selected->name << "\n";
        std::cout << "Количество: " << count << "\n";
        std::cout << "Сумма: " << selected->price * count << " руб.\n";
        std::cout << "Подтвердить добавление? (y/n): ";

        std::string confirm;
        Getline(confirm);

        if (confirm == "y" || confirm == "Y") {
            cart.add(*selected, count);
            std::cout << "Товар добавлен в корзину\n";
        } else {
            std::cout << "Добавление отменено\n";
        }

        std::cout << "\nСледующие действия:\n";
        std::cout << "1) Продолжить покупку / добавить ещё товары\n";
        std::cout << "2) Перейти к оплате\n";
        std::cout << "3) Отменить покупку (очистить корзину и выйти)\n";
        std::cout << "Выбор: ";

        int next_choice;
        Getline(next_choice);

        if (next_choice == 2) {
            if (cart.empty()) {
                std::cerr << "Корзина пуста\n";
            } else {
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                payment();
                cart.clear();
                std::cout << "Корзина очищена после оплаты\n";
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                break;
            }
        } else if (next_choice == 3) {
            cart.clear();
            std::cout << "Покупка отменена, корзина очищена\n";
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
}

void SaleController::payment() {
    double total = cart.total();

    std::vector<std::pair<std::string, double>> cart_for_promo;
    for (const auto& item : cart.get_items()) {
        cart_for_promo.push_back({item.product.name, static_cast<double>(item.count)});
    }

    std::cout << "\n\n\n\t\t\tКОРЗИНА\n\n\n";
    for (const auto& item : cart.get_items()) {
        std::cout << item.product.name << " x " << item.count << " = "
                  << item.product.price * item.count << " руб.\n";
    }
    std::cout << "ИТОГО: " << total << " руб.\n";

    std::cout << "\n1) Принять участие в акциях\n";
    std::cout << "2) Оплатить без акций\n";
    std::cout << "Выбор: ";

    int promo_choice;
    Getline(promo_choice);

    double final_total = total;

    if (promo_choice == 1) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        PromoManager promo;
        promo.show_promo_menu(final_total, cart_for_promo);
    }

    std::cout << "\nИтого к оплате: " << final_total << " руб.\n";

    int type;
    while (true) {
        std::cout << "1) Карта\n2) Наличные\n0) Отмена оплаты\nВыбор: ";
        Getline(type);

        if (type == 0) {
            std::cout << "Оплата отменена\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            return;
        } else if (type == 1) {
            std::cout << "Оплата картой...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            std::cout << "Оплачено " << final_total << " руб.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        } else if (type == 2) {
            double money;
            std::cout << "Внесите деньги: ";
            Getline(money);
            if (money < final_total) {
                std::cerr << "Недостаточно денег\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                continue;
            }
            std::cout << "Сдача: " << (money - final_total) << " руб.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        } else {
            std::cerr << "Неверный выбор\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
        }
    }

    service.apply_sale(cart,final_total);
    std::cout << "Продажа завершена. Спасибо за покупку!\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
