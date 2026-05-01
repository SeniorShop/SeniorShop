#include "Personal.h"

void Personal::ManagePersonal() {
	while (true) {
		int choose = -1;
		printf("Выберите действие:\n1. добавить пользователя\n2. удалить пользователя\n3. изменить пароль пользователю\n4. сменить роль пользователю\n5. вывести всех пользователей\n0. отмена\n>>: ");
		scanf_s("%d", &choose);
		switch (choose) {
		case 1:
			this->register_user();
            break;
		case 2:
			this->RemoveUser();
            break;
		case 3:
			this->UserPassChange();
            break;
		case 4:
			this->UserRoleChange();
            break;
		case 5:
			this->show_all_users();
            break;
		case 0:
			break;
		}
	}
}
void Personal::RemoveUser() {

}
void Personal::UserPassChange() {
    std::string name, password, role, newPassword;
    while (true) {
        printf("Введите через пробел: логин пароль текущий_статус\n>>: ");
        scanf_s("%s %s %s", &name, &password, &role);

        if (!check_bot.verify()) {
            printf("Ошибка смены пользователя");
            continue;
        }

        for (auto& user : users) {
            if (user.username == name && user.password == password && user.status == role) {
                std::cout << "Новый пароль: ";
                std::cin >> newPassword;

                user.password = newPassword;

                save_to_file(user);
                break;
            }
        }
    }
}
void Personal::UserRoleChange() {
    std::string name, password, role, newRole;
    while (true) {
        printf("Введите через пробел: логин пароль текущий_статус\n>>: ");
        scanf_s("%s %s %s", &name, &password, &role);

        if (!check_bot.verify()) {
            std::cerr << "Ошибка смены пользователя" << std::endl;
            continue;
        }

        for (auto& user : users) {
            if (user.username == name && user.password == password && user.status == role) {

                std::cout << "Новый статус: ";
                std::cin >> newRole;

                user.status = newRole;

                save_to_file(user);
                break;
            }
        }
    }
}