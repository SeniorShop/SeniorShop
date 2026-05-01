#ifndef SELL_ACTIV_H
#define SELL_ACTIV_H
#include "Product.h"
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

class PromoManager {
    bool discount_used; // Использована ли акция в текущем чеке
    void apply_math_quiz(double& total_sum);
    void apply_meat_promotion(double& total_sum, const std::vector<std::pair<std::string, double>>& cart);
    void apply_wheel_of_fortune(double& total_sum);
public:
    PromoManager();
    void show_promo_menu(double& total_sum, const std::vector<std::pair<std::string, double>>& cart);

    void reset() { discount_used = false; }
};

#endif // SELL_ACTIV_H
