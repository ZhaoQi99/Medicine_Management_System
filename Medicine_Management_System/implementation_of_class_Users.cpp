#include<string>
#include"Users.h"

using std::string;


User::User(string t_account, string t_password, int userType)
{
	account = t_account;
	password = t_password;
	authority = userType;
}


User::User(User & t_user)
{
	account = t_user.account;
	password = t_user.password;
	authority = t_user.authority;
	accessibility = t_user.accessibility;
}
