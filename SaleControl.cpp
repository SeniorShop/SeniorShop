#include "SaleControl.h"
#include "SellActiv.h"
#include <iostream>
#include <chrono>
#include <thread>

void SaleController::start() {
    std::vector<Product> products = service.load();

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "\n\n\n\t\t\tДОСТУПНЫЕ ТОВАРЫ\n\n\n";
    std::cout << std::left
              << std::setw(5) << "ID"
              << std::setw(20) << "Название"
              << std::setw(12) << "Цена"
              << std::setw(10) << "В наличии"
              << "\n";
    std::cout << std::string(50, '-') << "\n";

    for (const auto& p : products) {
        std::cout << std::left
                  << std::setw(5) << p.id
                  << std::setw(20) << (p.name.length() > 18 ? p.name.substr(0, 16) + ".." : p.name)
                  << std::setw(12) << std::fixed << std::setprecision(2) << p.price
                  << std::setw(10) << p.count
                  << "\n";
    }
    std::cout << std::string(50, '-') << "\n";

    while (true) {
        std::cout << "\nВведите ID товара или команду:\n";
        std::cout << "  payment - оплата\n";
        std::cout << "  reset   - сброс корзины\n";
        std::cout << "  show    - показать корзину\n";
        std::cout << "  exit    - выход\n";
        std::cout << "> ";

        std::string cmd;
        Getline(cmd);

        if (cmd == "reset") {
            cart.clear();
            std::cout << "Корзина очищена\n";
        }
        else if (cmd == "show") {
            if (cart.empty()) {
                std::cout << "Корзина пуста\n";
            } else {
                show.show_сart(cart);
            }
        }
        else if (cmd == "payment") {
            if (cart.empty()) {
                std::cout << "Корзина пуста. Добавьте товары\n";
            } else {
                payment();
            }
        }
        else if (cmd == "exit") {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        }
        else {
            try {
                int id = std::stoi(cmd);
                unsigned int count;
                std::cout << "Количество: ";
                Getline(count);

                if (count == 0) {
                    std::cout << "Ошибка: количество должно быть больше 0\n";
                    continue;
                }

                if (!service.add_to_cart(cart, id, count)) {
                    std::cout << "Ошибка: товар не найден или недостаточно на складе\n";
                } else {
                    std::cout << "Товар добавлен в корзину\n";
                }
            } catch (...) {
                std::cout << "Неверная команда\n";
            }
        }
    }
}

void SaleController::payment() {
    double total = cart.total();

    std::vector<std::pair<std::string, double>> cart_for_promo;
    for (const auto& item : cart.get_items()) {
        cart_for_promo.push_back({item.product.name, static_cast<double>(item.count)});
    }


    PromoManager promo;
    promo.show_promo_menu(total, cart_for_promo);

    int type;
    while (true) {
        std::cout << "\nИтого к оплате: " << total << " руб.\n";
        std::cout << "1) Карта\n2) Наличные\n3) Выход в меню продаж\nВыберите опцию: ";
        Getline(type);

        if (type == 1) {
            std::cout << "Оплата картой...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            std::cout << "Оплачено " << total << " руб.\n";
            break;
        } else if (type == 2) {
            double money;
            std::cout << "Внесите деньги: ";
            Getline(money);

            if (money < total) {
                std::cerr << "Недостаточно денег\n";
                continue;
            }
            std::cout << "Сдача: " << (money - total) << " руб.\n";
            break;
        } else if (type == 3) {
            std::cout << "Отмена оплаты. Возврат в меню продаж.\n";
            return;
        } else {
            std::cerr << "Неизвестная опция\n";
        }
    }

    service.apply_sale(cart);
    cart.clear();
    std::cout << "Продажа завершена. Спасибо за покупку!\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
