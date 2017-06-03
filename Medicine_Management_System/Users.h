#pragma once
#ifndef ALL_USERS
#define ALL_USERS

#include<string>
#include"Array.h"

const int MAX_RECORD_USER = 100;

using std::string;

class User
{
public:
	User() {}
	User(string t_account, string t_password, int userType);
	User(User & t_user);
	friend class ArrayOfUser;
	friend void Function_Login_Menu();
private:
	string account;
	string password;
	int authority;
	int number;
	int index;
	bool accessibility;
	bool is_login;
};


#endif
