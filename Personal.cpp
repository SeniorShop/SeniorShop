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
    char name[67], password[67], role[67], newPassword[67];
    while (true) {
        printf("Введите через пробел: логин пароль текущий_статус\n>>: ");
        scanf_s("%s %s %s", name, (unsigned int)sizeof(name), password, (unsigned int)sizeof(password), role, (unsigned int)sizeof(role));

        if (!check_bot.verify()) {
            printf("Ошибка смены пароля пользователя");
            continue;
        }

        for (auto& user : users) {
            if (user.username == name && user.password == password && user.status == role) {
                printf("Новый пароль\n>>: ");
                scanf_s("%s", name, (unsigned int)sizeof(name));

                user.password = newPassword;

                save_to_file(user);
                break;
            }
        }
    }
}
void Personal::UserRoleChange() {
    char name[67], password[67], role[67], newRole[67];
    while (true) {
        printf("Введите через пробел: логин пароль текущий_статус\n>>: ");
        scanf_s("%s %s %s", name, (unsigned int)sizeof(name), password, (unsigned int)sizeof(password), role, (unsigned int)sizeof(role));

        if (!check_bot.verify()) {
            printf("Ошибка смены статуса пользователя");
            continue;
        }

        for (auto& user : users) {
            if (user.username == name && user.password == password && user.status == role) {
                printf("Новый статус\n>>: ");
                scanf_s("%s", name, (unsigned int)sizeof(name));

                user.status = newRole;

                save_to_file(user);
                break;
            }
        }
    }
}