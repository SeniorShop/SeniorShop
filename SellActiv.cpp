#include "SellActiv.h"

PromoManager::PromoManager() : discount_used(false) {
    srand(static_cast<unsigned int>(time(0)));
}

void PromoManager::show_promo_menu(double& total_sum, const std::vector<std::pair<std::string, double>>& cart) {
    if (discount_used) {
        std::cout << "Вы уже использовали акцию для этого чека!\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        return;
    }

    std::string choice;
    std::cout << "\n\tДОСТУПНЫЕ АКЦИИ\n";
    std::cout << "1. Скидка 15% (при покупке от 2000 руб)\n";
    std::cout << "2. Математическая задача (скидка 30%)\n";
    std::cout << "3. Мясная акция (10кг мяса = -200 руб)\n";
    std::cout << "4. Колесо Фортуны (участие 1000 руб)\n";
    std::cout << "0. Пропустить\n";
    std::cout << "Выбор: ";
    Getline(choice);

    if (choice == "1") {
        double discount = 0.0;
        if (total_sum >= 2000) {
            discount = total_sum * 0.15;
            total_sum -= discount;
            discount_used = true;
            std::cout << "Скидка 15% применена!\n";
        }
        else
            std::cout << "Сумма чека меньше 2000 руб.\n";

    }
    else if (choice == "2") {
        apply_math_quiz(total_sum);
    }
    else if (choice == "3") {
        apply_meat_promotion(total_sum, cart);
    }
    else if (choice == "4") {
        apply_wheel_of_fortune(total_sum);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}

void PromoManager::apply_math_quiz(double& total_sum) {
    int a = rand() % 50 + 10;
    int b = rand() % 50 + 10;
    int answer;
    std::cout << "Решите пример: " << a << " + " << b << " = ";
    std::cin >> answer;
    std::cin.ignore();
    double discount = 0.0;

    if (answer == (a + b)) {
        discount = total_sum * 0.30;
        total_sum -= discount;
        discount_used = true;
        std::cout << "Верно! Скидка 30% ваша.\n";
    }
    else
        std::cerr << "Неверно. Акция аннулирована.\n";

}

void PromoManager::apply_meat_promotion(double& total_sum, const std::vector<std::pair<std::string, double>>& cart) {
    double weight = 0;
    for (const auto& item : cart) {
        if (item.first == "Мясо" || item.first == "мясо") weight += item.second;
    }

    if (weight >= 10.0) {
        total_sum -= 200;
        discount_used = true;
        std::cout << "Акция применена! Списано 200 руб за мясо.\n";
    }
    else
        std::cerr << "Нужно минимум 10 кг мяса (у вас " << weight << ").\n";

}

void PromoManager::apply_wheel_of_fortune(double& total_sum) {
    std::string choice;

    std::cout << "\nНаш магазин рад, что вы приняли участие в нашей акции\n";
    std::cout << "\t\tКОЛЕСО ФОРТУНЫ \n";
    std::cout << "Стоимость участия: 1000 рублей(добавятся к сумме чека в случае проигрыша)\n";
    std::cout << "Возможные призы:\n";
    std::cout << "1. Скидка 10% на покупку\n";
    std::cout << "2. Скидка 20% на покупку\n";
    std::cout << "3. Скидка 30% на покупку\n";
    std::cout << "4. Скидка 50% на покупку\n";
    std::cout << "5. Бесплатная покупка (все товары бесплатно!)\n\n\n";

    std::cout << "Хотите участвовать?\n1 - Да\n2 - Нет\nВвод: ";
    Getline(choice);

    if (choice == "2") {
        std::cout << "Отказ от участия\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        return;
    }
    else if (choice != "1")
        return;

    double discount = 0.0;

    int prize = rand() % 100 + 1;
    if (prize >= 26 && prize <= 50) {
        discount = total_sum * 0.10;
        std::cout << "ВЫИГРЫШ: Скидка 10%!" << std::endl;
    }
    else if (prize >= 51 && prize <= 70) {
        discount = total_sum * 0.20;
        std::cout << "ВЫИГРЫШ: Скидка 20%!" << std::endl;
    }
    else if (prize >= 71 && prize <= 85) {
        discount = total_sum * 0.30;
        std::cout << "ВЫИГРЫШ: Скидка 30%!" << std::endl;
    }
    else if (prize >= 86 && prize <= 95) {
        discount = total_sum * 0.50;
        std::cout << "ВЫИГРЫШ: Скидка 50%!" << std::endl;
    }
    else if (prize >= 96 && prize <= 100) {
        total_sum = 0;
        std::cout << "ДЖЕКПОТ: Покупка бесплатно!" << std::endl;
    }    
    else {
        std::cout << "Увы, в этот раз без приза" << std::endl;
        total_sum += 1000;
        std::cout << "Стоимость участия добавлена к чеку. Текущая сумма: " << total_sum << " руб.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }    

    total_sum -= discount;
    if (total_sum < 0) total_sum = 0;

    discount_used = true;
}
