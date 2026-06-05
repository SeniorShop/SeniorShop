#include <jni.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "../include/Storage.h"
#include "../include/Product.h"
#include "../include/Check.h"
#include "../include/Cart.h"
#include "../include/User.h"
#include "../include/AuthSystemUser.h"
#include "../include/SupplyManager.h"

#ifdef ANDROID
#include <android/log.h>
#define LOG_TAG "NativeStore"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#else
#define LOGD(...) printf(__VA_ARGS__)
#define LOGE(...) fprintf(stderr, __VA_ARGS__)
#define LOGI(...) printf(__VA_ARGS__)
#endif

static Storage* g_storage = nullptr;
static AuthSystemUser* g_auth = nullptr;
static User* g_currentUser = nullptr;
static Cart* g_cart = nullptr;
static SupplyManager* g_supplyManager = nullptr;

static std::string jstringToString(JNIEnv* env, jstring jstr) {
    if (jstr == nullptr) return "";
    const char* chars = env->GetStringUTFChars(jstr, nullptr);
    std::string result(chars);
    env->ReleaseStringUTFChars(jstr, chars);
    return result;
}

static jstring stringToJString(JNIEnv* env, const std::string& str) {
    return env->NewStringUTF(str.c_str());
}

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_init(JNIEnv* env, jobject thiz, jstring dataDir) {
    std::string dir = jstringToString(env, dataDir);

    if (chdir(dir.c_str()) != 0) {
        LOGE("Failed to change directory to %s", dir.c_str());
        return JNI_FALSE;
    }

    LOGI("Initializing Storage in directory: %s", dir.c_str());

    if (g_storage == nullptr) {
        g_storage = new Storage();
        g_storage->load_from_file("Product.txt");
        LOGI("Storage initialized, loaded %zu products",
             g_storage->get_all_products().size());
    }

    if (g_auth == nullptr) {
        g_auth = new AuthSystemUser();
        LOGI("AuthSystem initialized");
    }

    if (g_cart == nullptr) {
        g_cart = new Cart();
        LOGI("Cart initialized");
    }

    if (g_supplyManager == nullptr) {
        g_supplyManager = new SupplyManager();
        LOGI("SupplyManager initialized");
    }

    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_yourcompany_store_NativeLib_cleanup(JNIEnv* env, jobject thiz) {
    delete g_storage;
    delete g_auth;
    delete g_cart;
    delete g_supplyManager;
    g_storage = nullptr;
    g_auth = nullptr;
    g_cart = nullptr;
    g_supplyManager = nullptr;
    g_currentUser = nullptr;
    LOGI("Cleanup completed");
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_login(JNIEnv* env, jobject thiz,
                                           jstring username, jstring password) {
    if (g_auth == nullptr) {
        LOGE("AuthSystem not initialized");
        return JNI_FALSE;
    }

    std::string user = jstringToString(env, username);
    std::string pass = jstringToString(env, password);

    LOGI("Login attempt: %s", user.c_str());

    auto trim = [](std::string& str) {
        std::size_t start = str.find_first_not_of(" \t\n\r\f\v");
        if (start != std::string::npos) {
            str = str.substr(start);
        }
        std::size_t end = str.find_last_not_of(" \t\n\r\f\v");
        if (end != std::string::npos) {
            str = str.substr(0, end + 1);
        }
    };

    trim(user);
    trim(pass);

    if (user.empty() || pass.empty()) {
        LOGE("Username or password is empty");
        return JNI_FALSE;
    }

    for (const auto& u : g_auth->get_users()) {
        if (u->username == user && u->password == pass) {
            Logger::log_attempt(user, true);
            g_currentUser = u.get();
            LOGI("Login successful: %s (%s)", user.c_str(), g_currentUser->status.c_str());
            return JNI_TRUE;
        }
    }

    Logger::log_attempt(user, false);
    LOGE("Login failed: %s", user.c_str());
    return JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_com_yourcompany_store_NativeLib_getCurrentUser(JNIEnv* env, jobject thiz) {
    if (g_currentUser == nullptr) {
        return stringToJString(env, "");
    }
    return stringToJString(env, g_currentUser->username);
}

JNIEXPORT jstring JNICALL
Java_com_yourcompany_store_NativeLib_getUserRole(JNIEnv* env, jobject thiz) {
    if (g_currentUser == nullptr) {
        return stringToJString(env, "guest");
    }
    return stringToJString(env, g_currentUser->status);
}

JNIEXPORT jstring JNICALL
Java_com_yourcompany_store_NativeLib_getAllProducts(JNIEnv* env, jobject thiz) {
    if (g_storage == nullptr)
        return stringToJString(env, "[]");

    g_storage->load_from_file("Product.txt");

    const auto& goods = g_storage->get_all_products();

    std::string json = "[";
    bool first = true;

    for (const auto& p : goods) {
        if (p.count == 0) continue;

        if (!first) json += ",";
        first = false;

        json += "{";
        json += "\"id\":" + std::to_string(p.id) + ",";
        json += "\"name\":\"" + p.name + "\",";
        json += "\"category\":\"" + p.category + "\",";
        json += "\"price\":" + std::to_string(p.price) + ",";
        json += "\"article\":" + std::to_string(p.article) + ",";
        json += "\"beginDate\":\"" + p.begin_date + "\",";
        json += "\"endDate\":\"" + p.end_date + "\",";
        json += "\"count\":" + std::to_string(p.count) + ",";
        json += "\"manufacturer\":\"" + p.manufacturer + "\",";
        json += "\"country\":\"" + p.country + "\",";
        json += "\"supplier\":\"" + p.supplier + "\"";
        json += "}";
    }

    json += "]";

    LOGD("getAllProducts returned %zu products", goods.size());
    return stringToJString(env, json);
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_addProduct(JNIEnv* env, jobject thiz,
                                                jstring name, jstring category,
                                                jdouble price, jint article,
                                                jstring beginDate, jstring endDate,
                                                jint count, jstring country,
                                                jstring supplier) {
    if (g_storage == nullptr) return JNI_FALSE;

    Product p;
    p.name = jstringToString(env, name);
    p.category = jstringToString(env, category);
    p.price = price;
    p.article = article;
    p.begin_date = jstringToString(env, beginDate);
    p.end_date = jstringToString(env, endDate);
    p.count = count;
    p.country = jstringToString(env, country);
    p.supplier = jstringToString(env, supplier);

    const auto& goods = g_storage->get_all_products();
    for (const auto& existing : goods) {
        if (existing.article == article) {
            LOGE("Product with article %d already exists", article);
            return JNI_FALSE;
        }
    }

    g_storage->add_product_from_JNI(p);
    g_storage->save_to_file("Product.txt");

    LOGI("Product added: %s (article: %d)", p.name.c_str(), article);
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_deleteProduct(JNIEnv* env, jobject thiz, jint article) {
    if (g_storage == nullptr) return JNI_FALSE;

    bool result = g_storage->delete_product_by_article(article);
    if (result) {
        g_storage->save_to_file("Product.txt");
        LOGI("Product with article %d deleted", article);
    } else {
        LOGE("Product with article %d not found", article);
    }

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_updateProductCount(JNIEnv* env, jobject thiz,
                                                        jint article, jint new_count) {
    if (g_storage == nullptr) return JNI_FALSE;

    if (new_count < 0 || new_count > 199) {
        LOGE("Invalid count: %d (must be 0-199)", new_count);
        return JNI_FALSE;
    }

    bool result = g_storage->update_product_count(article, new_count);

    if (result) {
        g_storage->save_to_file("Product.txt");
        LOGI("Product count updated: article %d → %d", article, new_count);
    }

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_updateProductPrice(JNIEnv* env, jobject thiz,
                                                        jint article, jdouble new_price) {
    if (g_storage == nullptr) return JNI_FALSE;

    if (new_price <= 0 || new_price > 10000) {
        LOGE("Invalid price: %.2f", new_price);
        return JNI_FALSE;
    }

    bool result = g_storage->update_product_price(article, new_price);
    if (result) {
        g_storage->save_to_file("Product.txt");
        LOGI("Product price updated: article %d → %.2f", article, new_price);
    }

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_addToCart(JNIEnv* env, jobject thiz,
                                               jint article, jint count) {
    if (g_storage == nullptr || g_cart == nullptr) {
        LOGE("Storage or Cart not initialized");
        return JNI_FALSE;
    }

    if (count <= 0) {
        LOGE("Invalid count: %d (must be > 0)", count);
        return JNI_FALSE;
    }

    g_storage->load_from_file("Product.txt");
    const auto& goods = g_storage->get_all_products();

    for (const auto& p : goods) {
        if (p.article == article) {
            if (p.count < static_cast<unsigned int>(count)) {
                LOGE("Not enough stock: requested %d, available %d", count, p.count);
                return JNI_FALSE;
            }

            if (g_storage->is_product_expired(p.end_date)) {
                LOGE("Product expired: %s (end date: %s)", p.name.c_str(), p.end_date.c_str());
                return JNI_FALSE;
            }

            g_cart->add(p, count);

            auto end_date = g_storage->parse_date(p.end_date);
            auto now = std::chrono::system_clock::now();
            auto today = std::chrono::floor<std::chrono::days>(now);
            auto days_left = std::chrono::duration_cast<std::chrono::days>(end_date - today).count();
            if (days_left <= 7 && days_left > 0) {
                LOGI("Warning: Product '%s' expires in %ld days", p.name.c_str(), days_left);
            }

            LOGI("Added to cart: %s x%d (article: %d), stock left: %d",
                 p.name.c_str(), count, article, p.count - count);
            return JNI_TRUE;
        }
    }

    LOGE("Product with article %d not found", article);
    return JNI_FALSE;
}

JNIEXPORT jdouble JNICALL
Java_com_yourcompany_store_NativeLib_getCartTotal(JNIEnv* env, jobject thiz) {
    if (g_cart == nullptr) {
        LOGE("Cart not initialized");
        return 0.0;
    }

    double total = g_cart->total();
    LOGD("Cart total: %.2f", total);
    return total;
}

JNIEXPORT void JNICALL
Java_com_yourcompany_store_NativeLib_clearCart(JNIEnv* env, jobject thiz) {
    if (g_cart == nullptr) {
        LOGE("Cart not initialized");
        return;
    }

    int item_count = g_cart->get_items().size();
    g_cart->clear();
    LOGI("Cart cleared (%d items removed)", item_count);
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_applySale(JNIEnv* env, jobject thiz,
                                               jdouble final_total, jstring employee) {
    if (g_storage == nullptr || g_cart == nullptr) {
        LOGE("Storage or Cart not initialized");
        return JNI_FALSE;
    }

    if (g_cart->empty()) {
        LOGE("Cannot apply sale: cart is empty");
        return JNI_FALSE;
    }

    std::string emp = jstringToString(env, employee);

    std::vector<std::pair<int, unsigned int>> items;
    for (const auto& item : g_cart->get_items()) {
        items.push_back({item.product.article, item.count});
    }

    bool result = g_storage->apply_sale_from_cart(items, final_total, emp);

    if (result) {
        auto cart_items = g_cart->get_items();
        g_cart->clear();

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm* tm = std::localtime(&t);
        std::stringstream ss;
        ss << std::setfill('0')
           << std::setw(2) << tm->tm_mday << "."
           << std::setw(2) << (tm->tm_mon + 1) << "."
           << (tm->tm_year + 1900) << " "
           << std::setw(2) << tm->tm_hour << ":"
           << std::setw(2) << tm->tm_min << ":"
           << std::setw(2) << tm->tm_sec;
        std::string date_str = ss.str();

        for (const auto& item : cart_items) {
            Check check(TransactionType::Sale,
                        item.product.name,
                        item.product.price,
                        item.count,
                        emp,
                        date_str);
            check.save_check("Checks.txt");
        }

        LOGI("Sale applied: total=%.2f, employee=%s, items=%zu",
             final_total, emp.c_str(), items.size());
    } else {
        LOGE("Failed to apply sale");
    }

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_com_yourcompany_store_NativeLib_getFinancialReport(JNIEnv* env, jobject thiz) {
    if (g_storage == nullptr) {
        return stringToJString(env, "Нет данных");
    }

    Check check;
    std::string report = check.get_financial_report_string("Checks.txt");

    return stringToJString(env, report);
}

JNIEXPORT jstring JNICALL
Java_com_yourcompany_store_NativeLib_getSalesDocuments(JNIEnv* env, jobject thiz) {
    Check check;
    std::string sales = check.get_sales_documents_JSON("Checks.txt");
    return stringToJString(env, sales);
}

JNIEXPORT jstring JNICALL
Java_com_yourcompany_store_NativeLib_getWriteoffs(JNIEnv* env, jobject thiz) {
    Check check;
    std::string writeoffs = check.get_write_offs_JSON("Checks.txt");
    return stringToJString(env, writeoffs);
}

JNIEXPORT jstring JNICALL
Java_com_yourcompany_store_NativeLib_getAllUsers(JNIEnv* env, jobject thiz) {
    if (g_auth == nullptr) {
        return stringToJString(env, "[]");
    }

    std::string json = g_auth->get_all_users_JSON();
    return stringToJString(env, json);
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_registerUser(JNIEnv* env, jobject thiz,
                                                  jstring username, jstring password,
                                                  jstring role) {
    if (g_auth == nullptr) return JNI_FALSE;
    if (g_currentUser == nullptr || g_currentUser->status != "superadmin") {
        LOGE("Only superadmin can register new users");
        return JNI_FALSE;
    }

    std::string user = jstringToString(env, username);
    std::string pass = jstringToString(env, password);
    std::string r = jstringToString(env, role);

    bool result = g_auth->register_user_direct(user, pass, r);
    if (result) {
        LOGI("User registered: %s (%s)", user.c_str(), r.c_str());
    }

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_deleteUser(JNIEnv* env, jobject thiz, jstring username) {
    if (g_auth == nullptr) return JNI_FALSE;
    if (g_currentUser == nullptr || g_currentUser->status != "superadmin") {
        LOGE("Only superadmin can delete users");
        return JNI_FALSE;
    }

    std::string user = jstringToString(env, username);
    bool result = g_auth->delete_user_direct(user);

    if (result) {
        LOGI("User deleted: %s", user.c_str());
    }

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_com_yourcompany_store_NativeLib_getAllSupplies(JNIEnv* env, jobject thiz) {
    if (g_supplyManager == nullptr) {
        return stringToJString(env, "[]");
    }

    g_supplyManager->load_from_file();

    const auto& supplies = g_supplyManager->get_all_supplies();

    std::string json = "[";
    bool first = true;

    for (const auto& s : supplies) {
        if (!first) json += ",";
        first = false;

        auto escape = [](const std::string& str) -> std::string {
            std::string result = str;
            size_t pos = 0;
            while ((pos = result.find('"', pos)) != std::string::npos) {
                result.replace(pos, 1, "\\\"");
                pos += 2;
            }
            return result;
        };

        json += "{";
        json += "\"number\":" + std::to_string(s.number_supply) + ",";
        json += "\"supplier\":\"" + escape(s.name_user) + "\",";
        json += "\"responsible\":\"" + escape(s.responsible_person) + "\",";
        json += "\"product\":\"" + escape(s.product_name.name) + "\",";
        json += "\"category\":\"" + escape(s.product_name.category) + "\",";
        json += "\"price\":" + std::to_string(s.product_name.price) + ",";
        json += "\"article\":" + std::to_string(s.product_name.article) + ",";
        json += "\"beginDate\":\"" + s.product_name.begin_date + "\",";
        json += "\"endDate\":\"" + s.product_name.end_date + "\",";
        json += "\"quantity\":" + std::to_string(s.product_name.count) + ",";
        json += "\"country\":\"" + escape(s.product_name.country) + "\",";
        json += "\"status\":\"" + escape(s.status) + "\",";
        json += "\"is_actually\":" + std::string(s.is_actually ? "true" : "false");
        json += "}";
    }

    json += "]";
    return stringToJString(env, json);
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_createSupply(JNIEnv* env, jobject thiz,
                                                  jint supplyNumber, jstring supplier,
                                                  jstring responsible, jstring productName,
                                                  jstring category, jdouble price,
                                                  jint article, jstring beginDate,
                                                  jstring endDate, jint quantity,
                                                  jstring country) {
    if (g_supplyManager == nullptr) {
        LOGE("SupplyManager not initialized");
        return JNI_FALSE;
    }

    std::string supplier_str = jstringToString(env, supplier);
    std::string responsible_str = jstringToString(env, responsible);
    std::string productName_str = jstringToString(env, productName);
    std::string category_str = jstringToString(env, category);
    std::string beginDate_str = jstringToString(env, beginDate);
    std::string endDate_str = jstringToString(env, endDate);
    std::string country_str = jstringToString(env, country);

    if (productName_str.empty()) {
        LOGE("Product name cannot be empty");
        return JNI_FALSE;
    }

    if (quantity <= 0 || quantity > 199) {
        LOGE("Invalid quantity: %d (must be 1-199)", quantity);
        return JNI_FALSE;
    }

    if (price <= 0 || price > 10000) {
        LOGE("Invalid price: %.2f (must be 0.01-10000)", price);
        return JNI_FALSE;
    }

    if (article <= 0) {
        LOGE("Invalid article: %d (must be > 0)", article);
        return JNI_FALSE;
    }

    Supply s;
    s.number_supply = supplyNumber;
    s.name_user = supplier_str;
    s.responsible_person = responsible_str.empty() ? "Не указан" : responsible_str;
    s.product_name.name = productName_str;
    s.product_name.category = category_str;
    s.product_name.price = price;
    s.product_name.article = article;
    s.product_name.begin_date = beginDate_str;
    s.product_name.end_date = endDate_str;
    s.product_name.count = quantity;
    s.product_name.country = country_str;
    s.product_name.supplier = supplier_str;
    s.product_name.manufacturer = "";
    s.is_actually = true;
    s.status = "Ожидается";

    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::floor<std::chrono::days>(now);
    s.date = today;
    s.date_acception = today;
    s.date_processing = today;

    bool result = g_supplyManager->add_supply(s);

    if (result) {
        LOGI("Supply created: #%d, product=%s, quantity=%d, supplier=%s",
             supplyNumber, productName_str.c_str(), quantity, supplier_str.c_str());
    } else {
        LOGE("Failed to create supply #%d", supplyNumber);
    }

    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_deleteSupply(JNIEnv* env, jobject thiz, jint supply_number) {
    if (g_supplyManager == nullptr) {
        LOGE("SupplyManager not initialized");
        return JNI_FALSE;
    }

    LOGI("Deleting supply: #%d", supply_number);

    std::ifstream file("Supplies.txt");
    if (!file.is_open()) {
        LOGE("Cannot open Supplies.txt");
        return JNI_FALSE;
    }

    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        int num;
        ss >> num;

        if (num == supply_number) {
            found = true;
            continue;
        }
        lines.push_back(line);
    }
    file.close();

    if (!found) {
        LOGE("Supply #%d not found", supply_number);
        return JNI_FALSE;
    }

    std::ofstream out("Supplies.txt");
    for (const auto& l : lines) {
        out << l << "\n";
    }
    out.close();

    LOGI("Supply #%d deleted", supply_number);
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_com_yourcompany_store_NativeLib_applySupply(JNIEnv* env, jobject thiz, jint supply_number) {
    if (g_supplyManager == nullptr || g_storage == nullptr) {
        LOGE("SupplyManager or Storage not initialized");
        return JNI_FALSE;
    }

    LOGI("Applying supply: #%d", supply_number);

    g_supplyManager->load_from_file();

    auto& supplies = g_supplyManager->get_all_supplies();
    Supply* foundSupply = nullptr;

    for (auto& s : supplies) {
        if (s.number_supply == static_cast<unsigned int>(supply_number)) {
            foundSupply = &s;
            break;
        }
    }

    if (foundSupply == nullptr) {
        LOGE("Supply #%d not found", supply_number);
        return JNI_FALSE;
    }

    if (!foundSupply->is_actually) {
        LOGE("Supply #%d is not actual", supply_number);
        return JNI_FALSE;
    }

    g_storage->add_supply_products(*foundSupply);

    foundSupply->is_actually = false;
    foundSupply->status = "Выполнена";

    std::ofstream fout("Supplies.txt", std::ios::trunc);
    fout.close();
    for (const auto& s : supplies) {
        g_supplyManager->add_to_file(s);
    }

    LOGI("Supply #%d applied successfully", supply_number);
    return JNI_TRUE;
}

} // extern "C"

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_seniorshop_bot_NativeLib_init(JNIEnv* env, jclass clazz, jstring dataDir) {
    return Java_com_yourcompany_store_NativeLib_init(env, nullptr, dataDir);
}

JNIEXPORT void JNICALL
Java_com_seniorshop_bot_NativeLib_cleanup(JNIEnv* env, jclass clazz) {
    Java_com_yourcompany_store_NativeLib_cleanup(env, nullptr);
}

JNIEXPORT jboolean JNICALL
Java_com_seniorshop_bot_NativeLib_login(JNIEnv* env, jclass clazz, jstring username, jstring password) {
    return Java_com_yourcompany_store_NativeLib_login(env, nullptr, username, password);
}

JNIEXPORT jstring JNICALL
Java_com_seniorshop_bot_NativeLib_getAllProducts(JNIEnv* env, jclass clazz) {
    return Java_com_yourcompany_store_NativeLib_getAllProducts(env, nullptr);
}

JNIEXPORT jstring JNICALL
Java_com_seniorshop_bot_NativeLib_getCurrentUser(JNIEnv* env, jclass clazz) {
    return Java_com_yourcompany_store_NativeLib_getCurrentUser(env, nullptr);
}

JNIEXPORT jstring JNICALL
Java_com_seniorshop_bot_NativeLib_getUserRole(JNIEnv* env, jclass clazz) {
    return Java_com_yourcompany_store_NativeLib_getUserRole(env, nullptr);
}

JNIEXPORT jstring JNICALL
Java_com_seniorshop_bot_NativeLib_getFinancialReport(JNIEnv* env, jclass clazz) {
    return Java_com_yourcompany_store_NativeLib_getFinancialReport(env, nullptr);
}

JNIEXPORT jstring JNICALL
Java_com_seniorshop_bot_NativeLib_getAllSupplies(JNIEnv* env, jclass clazz) {
    return Java_com_yourcompany_store_NativeLib_getAllSupplies(env, nullptr);
}

JNIEXPORT jboolean JNICALL
Java_com_seniorshop_bot_NativeLib_applySupply(JNIEnv* env, jclass clazz, jint supplyNumber) {
    return Java_com_yourcompany_store_NativeLib_applySupply(env, nullptr, supplyNumber);
}

JNIEXPORT jboolean JNICALL
Java_com_seniorshop_bot_NativeLib_addToCart(JNIEnv* env, jclass clazz, jint article, jint count) {
    return Java_com_yourcompany_store_NativeLib_addToCart(env, nullptr, article, count);
}

JNIEXPORT jdouble JNICALL
Java_com_seniorshop_bot_NativeLib_getCartTotal(JNIEnv* env, jclass clazz) {
    return Java_com_yourcompany_store_NativeLib_getCartTotal(env, nullptr);
}

JNIEXPORT void JNICALL
Java_com_seniorshop_bot_NativeLib_clearCart(JNIEnv* env, jclass clazz) {
    Java_com_yourcompany_store_NativeLib_clearCart(env, nullptr);
}

JNIEXPORT jboolean JNICALL
Java_com_seniorshop_bot_NativeLib_applySale(JNIEnv* env, jclass clazz, jdouble finalTotal, jstring employee) {
    return Java_com_yourcompany_store_NativeLib_applySale(env, nullptr, finalTotal, employee);
}

} // extern "C"
