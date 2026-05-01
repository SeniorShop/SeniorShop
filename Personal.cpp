#include "Personal.h"

void Personal::ManagePersonal() {
	while (true) {
		int choose = -1;
		printf("Выберите действие:\n1. добавить пользователя\n2. удалить пользователя\n3. изменить пароль пользователю\n4. сменить роль пользователю\n5. вывести всех пользователей\n0. отмена\n>>: ");
		scanf_s("%d", &choose);
		switch (choose) {
		case 1:
			this->register_user();
		case 2:
			this->RemoveUser();
		case 3:
			this->UserPassChange();
		case 4:
			this->UserRoleChange();
		case 5:
			this->show_all_users();
		case 0:
			break;
		}
	}
}
void Personal::RemoveUser() {

}
void Personal::UserPassChange() {

}
void Personal::UserRoleChange() {

}