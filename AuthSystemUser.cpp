#include "AuthSystemUser.h"

AuthSystemUser::AuthSystemUser() {
    load_from_file();
    if (users.empty()) {
        User supadmin;
        supadmin.set_name("SuperAdmin");
        supadmin.set_password("admin***123");
        supadmin.set_status("superadmin");
        users.push_back(supadmin);
        save_to_file(supadmin);
    }
}

bool AuthSystemUser::is_valid_username(const std::string& username) const {
    if (username.size() < 5 || username.size() > 20) return false;

    std::unordered_set<char> special_symbols;
    for (char i = 'A'; i <= 'Z'; i++) special_symbols.insert(i);
    for (char i = 'a'; i <= 'z'; i++) special_symbols.insert(i);
    for (char i = '0'; i <= '9'; i++) special_symbols.insert(i);

    for (char check_symbol : username) {
        if (!special_symbols.count(check_symbol)) {
            std::cerr << "Имя пользователя содержит недействительный символ" << std::endl;
            return false;
        }
    }
    return true;
}

bool AuthSystemUser::is_valid_pass(const std::string& password) const {
    if(password.size() < 8) return false;

    std::size_t count_unique_symbols = 0;

    std::unordered_set<char> special_symbols;
    std::unordered_set<char> pass_symbols{
        '!', '@', '#', '%', '^',
        '&', '*', '(',')', '-',
        '_', '=', '+', '/', '?',
        '|', '\\', '\"', '\'', ',',
        '.', '>', '<', '~', '`',
        ':', ';', '{','}', '[',
        ']'
    };

    for (char i = '!'; i <= '~'; i++)  special_symbols.insert(i);

    for (char symb : password) {
        if (!special_symbols.count(symb)) {
            std::cout << "Íåêîððåêòíûå ñèìâîëû â ïàðîëå\n\n";
            return false;
        }
    }

    for (char symb : password) if (pass_symbols.count(symb)) count_unique_symbols++;

    if (count_unique_symbols < 3)
        return false;
    return true;
}

bool AuthSystemUser::user_exists(const std::string& username) const {
    for (const auto& user : users) {
        if (user.username == username) return true;
    }
    return false;
}

bool AuthSystemUser::is_valid_current_status(const std::string& current_status) const {
    return current_status == "superadmin" || current_status == "admin" || current_status == "user";
}

void AuthSystemUser::load_from_file() {
    users.clear();
    std::ifstream in_file("Users.txt");
    if (!in_file.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return;
    }

    std::string every_data;
    User user;
    while (std::getline(in_file, every_data)) {
        if (every_data.empty()) continue;
        std::istringstream ss(every_data);

        if (std::getline(ss, user.username, '|')
            && (std::getline(ss, user.password, '|'))
            && (std::getline(ss, user.status))) {
            users.push_back(user);
        }
    }
    in_file.close();
}

void AuthSystemUser::save_to_file(User& add_user) {
    auto exist = std::find(users.begin(),users.end(),add_user);
    if(exist == users.end()) {
        users.push_back(add_user);
    }
    std::ofstream out_file("Users.txt", std::ios::trunc);
    if (!out_file.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return;
    }

    for (const auto& u : users) {
        out_file << u.username << "|" << u.password << "|" << u.status << '\n';  // Используйте | как разделитель
    }
    out_file.close();
}

std::string AuthSystemUser::register_user() {
    std::string username, password;
    std::size_t choice_status, choice_to_continue;

    if(!check_bot.verify()) {
        std::cerr << "Ошибка регистрации пользователя" << std::endl;
        return "";
    }

    std::cout << "Логин: ";
    std::cin >> username;

    if (!is_valid_username(username)) {
        std::cerr << "Неверный логин (должен быть 5-20 символов, только буквы и цифры)" << std::endl;
        return "";
    }

    if (user_exists(username)) {
        std::cerr << "Пользователь с таким логином уже существует" << std::endl;
        return "";
    }

    std::cout << "Пароль: ";
    std::cin >> password;

    if (!is_valid_pass(password)) {
        std::cerr << "Неверный пароль (минимум 8 символов, минимум 3 не буквенно-цифровых символа)" << std::endl;
        return "";
    }

    std::string statuses[]{"user", "admin", "superadmin"};
    std::cout << "1) Обычный пользователь\n2) Администратор\n3) Супер-администратор\nВыберите статус: ";
    std::cin >> choice_status;

    if(choice_status < 1 || choice_status > 3) return "";

    User add_user;
    add_user.username = username;
    add_user.password = password;
    add_user.status = statuses[choice_status - 1];
    users.push_back(add_user);
    save_to_file(add_user);
    Logger::log_attempt(username, true);
    std::cout << "Регистрация прошла успешно" << std::endl;
    return add_user.status;
}

void AuthSystemUser::show_all_users() {
    for (const auto& user : users) {
        std::cout << "Имя: " << user.username
                  << " | Статус: " << user.status << std::endl;
    }
}

void AuthSystemUser::login() {
    std::string username, password;
    std::cout << "Логин: ";
    std::cin >> username;
    std::cout << "Пароль: ";
    std::cin >> password;

    if(!check_bot.verify()) {
        std::cerr << "Ошибка авторизации пользователя" << std::endl;
        return;
    }

    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            Logger::log_attempt(username, true);
            std::cout << "Добро пожаловать, " << username << "!" << std::endl;
            start_storage.start(user.status);
        }
    }

    Logger::log_attempt(username, false);
    std::cout << "Неверный логин или пароль" << std::endl;
    return;
}

bool AuthSystemUser::is_super_admin_exists() const {
    for (const auto& user : users) {
        if (user.status == "superadmin")
            return true;
    }
    return false;
}
void AuthSystemUser::remove_user(const std::string& get_definite_status) {
    if (!check_bot.verify()) {
        std::cerr << "Ошибка смены статуса пользователя" << std::endl;
        return;
    }

    if(get_definite_status != "supadmin") {
        std::cerr << "Редактировать персонал может только суперпользователь" << std::endl;
        return;
    }

    std::ifstream read_from("Users.txt");

    if(!read_from.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return;
    }

    std::string get_name, get_password, get_status;

    User get_users;

    while(std::getline(read_from, get_name, '|') &&
           std::getline(read_from, get_password, '|') &&
           std::getline(read_from, get_status)) {

        get_users.username = get_name;
        get_users.password = get_password;
        get_users.status = get_status;
        users.push_back(get_users);
    }

    read_from.close();

    std::string name, password, status;
    while (true) {
        std::cout << "Введите через пробел (логин пароль текущий_статус): ";
        std::cin >> name >> password >> status;
        for (auto& user : users) {
            if (user.username == name && user.password == password && user.status == status) {
                users.erase(std::remove(users.begin(),users.end(),user),users.end());
                std::filesystem::remove("Users.txt");
                std::cout << "Пользователь успешно удален" << std::endl;
                break;
            }
        }
    }

    std::ofstream rewrite_to("Users.txt",std::ios::trunc);
    if(rewrite_to.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return;
    }

    for(auto& write_users : users) {
        rewrite_to << write_users.username << "|" << write_users.password << "|" << write_users.status << '\n';
    }
    std::cout << std::endl;

    rewrite_to.close();
}
void AuthSystemUser::user_pass_change() {

    std::string name, password, status, new_password;

    std::ifstream save("Users.txt");
    if(!save.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return;
    }

    users.clear();

    std::string get_name, get_password, get_status;
    User get_user, get_users;

    while(std::getline(save, get_name, '|') &&
           std::getline(save, get_password, '|') &&
           std::getline(save, get_status)) {

        get_users.username = get_name;
        get_users.password = get_password;
        get_users.status = get_status;
        users.push_back(get_users);
    }

    save.close();

    while (true) {
        std::cout << "Введите через пробел (логин пароль текущий_статус): ";
        std::cin >> name >> password >> status;

        for (auto& user : users) {
            if (user.username == name && user.password == password && user.status == status) {
                users.erase(std::remove(users.begin(),users.end(),user),users.end());
                std::cout << "Новый пароль: ";
                std::cin >> new_password;
                get_user.username = name;
                get_user.password = new_password;
                get_user.status = status;
                break;
            }
        }
    }
    save_to_file(get_user);
}
void AuthSystemUser::user_status_change() {

    std::string name, password, status, new_status;

    std::ifstream save("Users.txt");
    if(!save.is_open()) {
        std::cerr << "Ошибка открытия файла" << std::endl;
        return;
    }

    users.clear();

    std::string get_name, get_password, get_status;
    User get_user, get_users;

    while(std::getline(save, get_name, '|') &&
           std::getline(save, get_password, '|') &&
           std::getline(save, get_status)) {

        get_users.username = get_name;
        get_users.password = get_password;
        get_users.status = get_status;
        users.push_back(get_users);
    }

    save.close();

    while (true) {
        std::cout << "Введите через пробел (логин пароль текущий_статус): ";
        std::cin >> name >> password >> status;

        for (auto& user : users) {
            if (user.username == name && user.password == password && user.status == status) {
                users.erase(std::remove(users.begin(),users.end(),user),users.end());
                std::cout << "Новый статус: ";
                std::cin >> new_status;
                get_user.username = name;
                get_user.password = password;
                get_user.status = new_status;
                break;
            }
        }
    }
    save_to_file(get_user);
}

void AuthSystemUser::change_user(const std::string& get_status) {

    if(!check_bot.verify()) {
        std::cerr << "Ошибка смены пользователя" << std::endl;
        return;
    }

    if(get_status == "supadmin") {
        std::cerr << "Редактировать персонал может только суперпользователь" << std::endl;
        return;
    }

    std::size_t choice_action;
    std::cout << "1) Изменить пароль пользователя\n2) Изменить статус пользователя\n3)Изменить все данные пользователя\nВыберите действие: ";
    std::cin >> choice_action;

    if(choice_action == 1) user_pass_change();
    if(choice_action == 2) user_status_change();

    std::string name, password, status, new_name, new_password, new_status;
    std::cout << "Логин: ";
    std::cin >> name;
    std::cout << "Пароль: ";
    std::cin >> password;
    std::cout << "Текущий статус: ";
    std::cin >> status;

    for (auto& user : users) {
        if (user.username == name && user.password == password && user.status == status) {
            std::cout << "Новый логин: ";
            std::cin >> new_name;
            std::cout << "Новый пароль: ";
            std::cin >> new_password;
            std::cout << "Новый статус: ";
            std::cin >> new_status;

            user.username = new_name;
            user.password = new_password;
            user.status = new_status;

            save_to_file(user);
            return;
        }
    }

    std::cerr << "Пользователь не найден" << std::endl;
    return;
}
