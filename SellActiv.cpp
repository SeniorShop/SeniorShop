#include "SellActiv.h"

PromoManager::PromoManager() : discountUsed(false) {
    srand(static_cast<unsigned>(time(0)));
}

void PromoManager::showPromoMenu(double& totalSum, const std::vector<std::pair<std::string, double>>& cart) {
    if (discountUsed) {
        std::cout << "\nВы уже использовали акцию для этого чека!\n";
        Sleep(1500);
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
        if (totalSum >= 2000) {           
            discount = totalSum * 0.15;
            totalSum -= discount;
            discountUsed = true;
            std::cout << "Скидка 15% применена!\n";
        }
        else {
            std::cout << "Сумма чека меньше 2000 руб.\n";
        }
    }
    else if (choice == "2") {
        applyMathQuiz(totalSum);
    }
    else if (choice == "3") {
        applyMeatPromotion(totalSum, cart);
    }
    else if (choice == "4") {
        applyWheelOfFortune(totalSum);
    }
    Sleep(1500);
}

void PromoManager::applyMathQuiz(double& totalSum) {
    int a = rand() % 50 + 10;
    int b = rand() % 50 + 10;
    int answer;
    std::cout << "Решите пример: " << a << " + " << b << " = ";
    std::cin >> answer;
    std::cin.ignore();
    double discount = 0.0;

    if (answer == (a + b)) {
        discount = totalSum * 0.30;
        totalSum -= discount;
        discountUsed = true;
        std::cout << "Верно! Скидка 30% ваша.\n";
    }
    else {
        std::cout << "Неверно. Акция аннулирована.\n";
    }
}

void PromoManager::applyMeatPromotion(double& totalSum, const std::vector<std::pair<std::string, double>>& cart) {
    double weight = 0;
    for (auto const& item : cart) {
        if (item.first == "Мясо" || item.first == "мясо") weight += item.second;
    }

    if (weight >= 10.0) {
        totalSum -= 200;
        discountUsed = true;
        std::cout << "Акция применена! Списано 200 руб за мясо.\n";
    }
    else {
        std::cout << "Нужно минимум 10 кг мяса (у вас " << weight << ").\n";
    }
}

void PromoManager::applyWheelOfFortune(double& totalSum) {
    std::string choice;

    std::cout << "\nНаш магазин рад, что вы приняли участие в нашей акции\n";
    std::cout << "\t\tКОЛЕСО ФОРТУНЫ \n";
    std::cout << "Стоимость участия: 1000 рублей(добавятся к сумме чека в случаве проигрыша)\n";
    std::cout << "Возможные призы:\n";
    std::cout << "1. Скидка 10% на покупку\n";
    std::cout << "2. Скидка 20% на покупку\n";
    std::cout << "3. Скидка 30% на покупку\n";
    std::cout << "4. Скидка 50% на покупку\n";
    std::cout << "5. Бесплатная покупка (все товары бесплатно!)\n\n\n";


    std::cout << "Хотите участвовать?\n1 - Да\n2 - Нет\nВвод: ";
    Getline(choice);

    if (choice == "2")
    {
        std::cout << "Отказ от участия\n";
        Sleep(1000);
        return;
    }
    else if (choice != "1")
    {
        return;
    }

    double discount = 0.0;

    int prize = rand() % 100 + 1;
    if (prize >= 26 && prize <= 50) {
        discount = totalSum * 0.10;
        std::cout << "ВЫИГРЫШ: Скидка 10%!\n";
    }
    else if (prize >= 51 && prize <= 70) {
        discount = totalSum * 0.20;
        std::cout << "ВЫИГРЫШ: Скидка 20%!\n";
    }
    else if (prize >= 71 && prize <= 85) {
        discount = totalSum * 0.30;
        std::cout << "ВЫИГРЫШ: Скидка 30%!\n";
    }
    else if (prize >= 86 && prize <= 95) {
        discount = totalSum * 0.50;
        std::cout << "ВЫИГРЫШ: Скидка 50%!\n";
    }
    else if (prize >= 96 && prize <= 100) {
        totalSum = 0;
        std::cout << "ДЖЕКПОТ: Покупка бесплатно!\n";
    }    
    else {
        std::cout << "Увы, в этот раз без приза.\n";
        totalSum += 1000;
        std::cout << "Стоимость участия добавлена к чеку. Текущая сумма: " << totalSum << " руб.\n";
        Sleep(1000);
    }    

    totalSum -= discount;
    if (totalSum < 0) totalSum = 0;

    discountUsed = true;
    
}