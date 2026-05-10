#include "methodsSuperAdmin.h"
#include <iostream>
#include "Product.h"
#include "AuthSystemUser.h"
#include <windows.h>

void StorageUserMethod(AuthSystemUser* auth)
{
	std::string choose;
	system("cls");
	while (true)
	{
		
		std::cout << "Вы выбрали опцию редактирование персонала\n";
		std::cout << "Вам доступны такие опции:\n1 - Показ всех пользователей\n2 - Добавить нового пользователя\n";
		std::cout << "3 - Изменить пароль пользователю или статус\n4 - Удалить пользователя\n5 - Выход из опции в меню\n";
		std::cout << "Ввод: ";
		Getline(choose);
		if (choose == "1")
		{
			auth->show_all_users();
		}
		else if (choose == "2")
		{
			auth->register_user();
		}
		else if (choose == "3")
		{
			auth->change_user();
		}
		else if (choose == "4")
		{
			auth->remove_user();
		}
		else if (choose == "5")
		{
			break;
			// возвращение в меню
		}
		else
		{
			std::cerr << "Ошибка ввода! Повторите попытку\n";
			Sleep(1500);
			system("cls");
		}

	}

}
