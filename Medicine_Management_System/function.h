#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H

#include<iostream>
#include<string>
#include<cstdio>
#include"Menu.h"
#include"Array.h"
#include<cstdlib>

extern ArrayOfPersonnel array_of_personnel;             //声明外部对象
extern ArrayOfUser array_of_user;                       //声明外部对象
extern ArrayOfMedicine array_of_medicine;               //声明外部对象

using std::cout;
using std::cin;
using std::wcin;
using std::endl;
using std::string;
using std::system;

void Function_Personnel_Menu_Sub();                     //人员管理菜单的子菜单
void Function_Main_Menu();                              //主菜单
void Function_Medicine_Management_Menu();               //库存管理菜单
void Function_Login_Menu();                             //登录菜单

void Add_input_personnel();                             //添加Personnel的功能函数
void Delete_input_personnel();                          //删除Personnel的功能函数
void Show_all_input_personnel();                        //查询所有Personnel的功能函数
void Show_input_personnel();                            //查询Personnel的功能函数

void Add_input_medicine();                              //药品入库的功能函数
void Add_output_medicine();								//药品出库的功能函数
void Show_input_medicine();                             //药品查询的功能函数
void Show_all_input_medicine();                         //查询所有药品的功能函数

void wflush();											// 刷新缓冲区


														// 菜单实现
void Function_Personnel_Menu()                          //人员管理菜单的功能实现
{
	system("cls");
	Person_Menu();                                      //主菜单的界面
	int temp;
	cin >> temp;
	if (temp >= 1 && temp <= 4)
	{
		switch (temp)
		{
		case 1: Function_Personnel_Menu_Sub(); break;   //跳转到人员管理菜单的子菜单
		case 2: Function_Personnel_Menu_Sub(); break;   //跳转到人员管理菜单的子菜单
		case 3: Function_Personnel_Menu_Sub(); break;   //跳转到人员管理菜单的子菜单
		case 4:
			system("cls");
			Function_Main_Menu();                       //返回主菜单
			break;
		}
	}
	else
	{
		cout << "Error!" << endl;
		system("pause");
		system("cls");
		Function_Personnel_Menu();                      //返回人员管理菜单
	}
}

void Function_Personnel_Menu_Sub()                      //人员管理菜单的子菜单的功能实现
{
	system("cls");
	Person_Menu_1();                                    //人员管理菜单的子菜单的界面
	int temp;
	cin >> temp;
	if (temp >= 1 && temp <= 4)
	{
		switch (temp)
		{
		case 1: Show_input_personnel(); break;          //查询Personnnel
		case 2: Add_input_personnel(); break;           //添加Personnnel
		case 3: Delete_input_personnel(); break;        //删除Personnnel
		case 4:
			system("cls");
			Function_Personnel_Menu();                  //返回人员管理菜单
			break;
		}
	}
	else
	{
		cout << "Error!" << endl;
		system("pause");
		system("cls");
		Function_Personnel_Menu_Sub();                  //返回人员管理菜单的子菜单
	}
}

void Function_Main_Menu()                               //主菜单的功能实现
{
	system("cls");
	if (array_of_personnel.getCurrentUser() == NO_USERS)
	{
		Function_Login_Menu();                          //返回登录界面
	}
	MainMenu();                                         //主菜单的界面
	int temp;
	cin >> temp;
	if (temp >= 1 && temp <= 3)
	{
		switch (temp)
		{
		case 1:Function_Medicine_Management_Menu(); break;     //库存管理
		case 2:Function_Personnel_Menu(); break;        //人员管理
		case 3:array_of_user.login(NO_USERS); Function_Login_Menu(); break;
														//注销用户
		}
	}
	else
	{
		cout << "Error!" << endl;
		system("pause");
		Function_Main_Menu();                           //返回主菜单
	}
}
// User相关功能函数实现包含在这里
void Function_Login_Menu()                              //登录界面的功能实现
{
	system("cls");
	Login_Menu();                                       //登录界面
	int login_select;
	cin >> login_select;
	if (login_select >= 1 && login_select <= 3)
	{
		string account;
		string password;
		int authority;
		bool is_successfully_login = false;
		bool is_account_found = false;
		bool is_password_correct = false;

		switch (login_select)
		{
		case 1:
			if (array_of_user.calNumberOfUser() == 0)
			{
				cout << "目前系统中还没有账户，请创建账户后进行操作。" << endl;
				cout << "注意：必须有系统管理员账户才能创建普通账户，请先联系管理员创建系统管理员账户。" << endl;
				cout << endl << "--注册账户--" << endl;
				cout << "用户名： ";
				wflush();
				getline(cin, account);
				cout << "密码： ";
				getline(cin, password);
				cout << "账户类型(<1>系统管理员 <2>销售人员 <3>仓库管理人员): ";
				cin >> authority;
				while (authority != 1)
				{
					while (!(authority >= 1 && authority <= 3))
						//防止输入错误的账户类型编号
					{
						cout << "输入有误，请重新输入。" << endl;
						cout << "账户类型(<1>系统管理员 <2>销售人员 <3>仓库管理人员): ";
						cin >> authority;
					}
					cout << "必须先创建系统管理员账户！" << endl;
					cout << "账户类型(<1>系统管理员 <2>销售人员 <3>仓库管理人员): ";
					cin >> authority;
				}

				switch (authority)
				{
				case 1:
					authority = IS_ADMIN;               //身份是系统管理员
					break;

				case 2:
					authority = IS_MANAGER;             //身份是销售人员
					break;

				case 3:
					authority = IS_WAREHOUSE;           //身份是库存管理人员
					break;
				}

				array_of_user.Add(authority, array_of_user.calNumberOfUser() + 1, account, password);

				system("pause");
				Function_Login_Menu();                  //跳转到主菜单
			}

			while (!is_successfully_login)
			{
				cout << endl << "--登陆账户--" << endl;
				cout << "用户名： ";
				wflush();
				getline(cin, account);
				cout << "密码： ";
				getline(cin, password);

				for (int i = 0; i < array_of_user.calNumberOfUser(); i++)
				{
					if (array_of_user.user[i].account == account)
					{
						is_account_found = true;
						cout << "--已找到账户，检查密码...--" << endl;
						if (array_of_user.user[i].password == password)
						{
							array_of_user.user[i].is_login = true;
							Array::login(array_of_user.user[i].authority);
							is_password_correct = true;
							is_successfully_login = true;
							cout << "--验证通过。欢迎登陆！--" << endl << endl;
							system("pause");
							Function_Main_Menu();       //跳转到主菜单
							break;
						}
						else
						{
							cout << "--（错误）密码输入错误，请重新输入。--" << endl;
							is_successfully_login = false;
							is_password_correct = false;
							break;
						}
					}
				}
				if (is_account_found != true)
				{
					cout << "--（错误）未找到该账户，请检查输入是否有误。--" << endl;
				}
			}
			break;

		case 2:
			cout << "--注册账户--" << endl;
			cout << "用户名： ";
			wflush();
			getline(cin, account);
			cout << "密码： ";
			getline(cin, password);
			cout << "账户类型(<1>系统管理员 <2>销售人员 <3>仓库管理人员): ";
			cin >> authority;
			while (authority == 1 && array_of_user.admin_existed)
			{
				while (!(authority >= 1 && authority <= 3))
				{
					cout << "--（错误）输入有误，请重新输入。--" << endl;
					cout << "账户类型(<1>系统管理员 <2>销售人员 <3>仓库管理人员): ";
					cin >> authority;
				}
				cout << "--(严重错误）系统管理员账户已经存在，操作被禁止--" << endl;
				cout << "账户类型(<1>系统管理员 <2>销售人员 <3>仓库管理人员): ";
				cin >> authority;
			}


			switch (authority)
			{
			case 1:
				authority = IS_ADMIN;                   //身份是系统管理员
				break;

			case 2:
				authority = IS_MANAGER;                 //身份是销售人员
				break;

			case 3:
				authority = IS_WAREHOUSE;               //身份是库存管理人员
				break;
			}

			array_of_user.Add(authority, array_of_user.calNumberOfUser() + 1, account, password);

			system("pause");

			Function_Login_Menu();                      //返回登录界面
			break;

		case 3:
			char type;
			cout << "您确定退出吗？(Y/N)" << endl;
			cin >> type;
			if (type == 'Y' || type == 'y')
			{
				exit(0);
			}
			else
			{
				setbuf(stdin, NULL);
				Function_Login_Menu();
			}
			break;
		}
	}
}

void Function_Medicine_Management_Menu()                //库存管理菜单的功能实现
{
	system("cls");
	Medicine_Management_Menu();                         //库存管理菜单的界面
	int temp;
	cin >> temp;
	if (temp >= 1 && temp <= 4)
	{
		switch (temp)
		{
		case 1:                                         //药品入库
			Add_input_medicine();
			break;

		case 2:                                         //药品出库
			Add_output_medicine();
			break;

		case 3:                                         //库存查询
			Show_input_medicine();
			break;

		case 4:Function_Main_Menu();                    //返回主菜单
			break;
		}
	}
	else
	{
		cout << "Error!" << endl;
		system("pause");
		Function_Medicine_Management_Menu();            //返回库存管理菜单
	}
}
//

// Personnel相关功能函数实现
void Add_input_personnel()                              //添加Personnel的功能实现
{
	int age;
	string name;
	int number;
	int authority;

	cout << "编号: ";
	cin >> number;
	cout << "姓名: ";
	wflush();
	getline(cin, name);
	cout << "年龄: ";
	cin >> age;
	cout << "请输入你的身份(<1>员工 <2>顾客 <3>供销商): ";
	cout << "身份: ";
	cin >> authority;

	if (authority != 1 && authority != 2 && authority != 3)
	{
		cout << "Error!" << endl;
		system("pause");
		Function_Personnel_Menu_Sub();                  //返回人员管理菜单的子菜单
	}

	switch (authority)
	{
	case 1:
		authority = STAFF;                              //身份是员工
		break;
	case 2:
		authority = CUSTOMER;                           //身份是顾客
		break;
	case 3:
		authority = VENDOR;                             //身份是供销商
		break;
	}

	array_of_personnel.Add(number, name, age, authority);

	system("pause");
	Function_Personnel_Menu_Sub();                      //返回人员管理菜单的子菜单
}

void Show_all_input_personnel()                         //查询所有人信息的功能实现
{
	array_of_personnel.ShowAllPersonnel();
	system("pause");
}

void Show_input_personnel()                             //查询Personnel的功能实现
{
	string  temp;

	cout << "如需查询所有信息，请输入ALL。（不区分大小写）" << endl;
	cout << "编号: ";
	cin >> temp;

	if (temp == "ALL" || temp == "all")
	{
		Show_all_input_personnel();                     //调用查询所有人信息的功能函数
	}
	else
	{
		int number;
		number = atoi(temp.c_str());                            //将string转换成int型
		int index = array_of_personnel.FindIndex(number);
		//按照编号查下标
		array_of_personnel.Show(index, SHOW_TITLES);
		system("pause");
	}
	Function_Personnel_Menu_Sub();                      //返回人员管理菜单的子菜单
}

void Delete_input_personnel()                           //删除Personnel的功能实现
{
	cout << "--当前人员信息情况：--" << endl << endl;

	array_of_personnel.ShowAllPersonnel();

	int number;
	cout << "编号: ";
	cin >> number;
	int index = array_of_personnel.FindIndex(number);   //按照编号查下标
	array_of_personnel.Delete(index);
	system("pause");
	Function_Personnel_Menu_Sub();                      //返回人员管理菜单的子菜单
}
//

// Medicine相关功能函数实现
void Show_all_input_medicine()                          //查询所有药品的功能实现
{
	array_of_medicine.ShowAllMedicine();
	system("pause");
}

void Show_input_medicine()                              //查询药品的功能实现
{
	string  temp;

	cout << "如需查询所有信息，请输入ALL。（不区分大小写）" << endl;
	cout << "编号: ";
	cin >> temp;

	if (temp == "ALL" || temp == "all")
	{
		Show_all_input_medicine();
	}
	else
	{
		int number;
		number = atoi(temp.c_str());
		int index = array_of_medicine.FindIndex(number);
		array_of_medicine.Show(index, SHOW_TITLES);
		system("pause");
	}
	Function_Medicine_Management_Menu();                //返回人员管理菜单的子菜单
}

void Add_input_medicine()                               //添加药品的功能实现
{
	string name;
	int number;
	int amount;
	double price;

	cout << "--当前药品库存情况：--" << endl << endl;
	array_of_medicine.ShowAllMedicine();
	cout << "--------------------------------------------------------------------------------" << endl << endl;

	cout << "药品名： ";
	wflush();
	getline(cin, name);
	cout << "药品编号： ";
	cin >> number;
	cout << "数量： ";
	cin >> amount;
	cout << "价格： ";
	cin >> price;

	array_of_medicine.Add(number, name, amount, price);

	system("pause");
	Function_Medicine_Management_Menu();
}

void Add_output_medicine()
{
	string name;
	int number;
	int amount;
	int old_authority;
	double price;
	int index;
	bool judge = false;

	cout << "--当前药品库存情况：--" << endl << endl;
	array_of_medicine.ShowAllMedicine();
	cout << "--------------------------------------------------------------------------------" << endl << endl;

	cout << "药品编号： ";
	cin >> number;
	judge = array_of_medicine.catchError(array_of_medicine.checkFormat(number, IGNORE));
	if (judge == true)
	{
		index = array_of_medicine.FindIndex(number);
		cout << "出库量： ";
		cin >> amount;
		name = array_of_medicine.medicine[index].getName();
		price = array_of_medicine.medicine[index].getPrice();

		old_authority = array_of_user.getCurrentUser();
		array_of_user.login(IS_ADMIN);

		array_of_medicine.Add(number, name, -amount, price);

		array_of_user.login(old_authority);
	}

	system("pause");
	Function_Medicine_Management_Menu();
}

void wflush()
{
	wchar_t c;
	while (1)
	{
		c = wcin.get();
		if (c == EOF)
			break;
		else if (c == '\n')
			break;
		else if (c == NULL)
			break;
	}
}

#endif // FUNCTION_H
