#ifndef SELL_ACTIV_H
#define SELL_ACTIV_H

#include "Product.h"
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

class PromoManager {
private:
    bool discountUsed; // Использована ли акция в текущем чеке

    void applyMathQuiz(double& totalSum);
    void applyMeatPromotion(double& totalSum, const std::vector<std::pair<std::string, double>>& cart);
    void applyWheelOfFortune(double& totalSum);  

public:
    PromoManager();

    // Главное меню акций
    void showPromoMenu(double& totalSum, const std::vector<std::pair<std::string, double>>& cart);

    void reset() { discountUsed = false; }
};

#endif // SELL_ACTIV_H