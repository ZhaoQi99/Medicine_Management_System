#include<string>
#include<iostream>
#include<iomanip>
#include"Array.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::setw;

int Array::numArrayOfMedicine = 0;
int Array::numArrayOfPersonnel = 0;
int Array::numArrayOfUser = 0;
int Array::current_user = NO_USERS;

////////////////////////////////////////////////
// 基类 Array

// Array类复制构造函数
Array::Array(Array & t_array)
{
	numArrayOfMedicine = t_array.numArrayOfMedicine;
	numArrayOfPersonnel = t_array.numArrayOfPersonnel;
}

// 错误处理
bool Array::catchError(int error_code)
{
	switch (error_code)
	{
	case E_NUMBER:
		cout << "--(错误) 已存在该编号，操作被禁止--" << endl << endl;
		return false;
		break;

	case E_AUTHORITY:
		cout << "--(严重错误) 没有足够的权限，操作被禁止--" << endl << endl;
		return false;
		break;

	case E_ADMIN_EXISTED:
		cout << "--(严重错误) 已存在系统管理员账户，操作被禁止--" << endl << endl;
		return false;
		break;

	case E_ACCOUNT:
		cout << "--(错误) 已存在同名账户，操作被禁止--" << endl << endl;
		return false;
		break;

	case E_NO_RECORD:
		cout << "--(警告) 未找到该记录--" << endl << endl;
		return false;
		break;

	case E_AMOUNT:
		cout << "--(错误)药品库存余量不足--" << endl << endl;
		return false;
		break;

	case W_UPDATE_MEDICINE:
		cout << "--(警告) 已存在记录，记录信息将被更新--" << endl << endl;
		return true;
		break;

	case NO_ERROR:
		return true;
		break;

	default:
		cout << "--(错误) 未知错误--" << endl << endl;
		return false;
		break;
	}
}

// 获取当前登陆用户
int Array::getCurrentUser()
{
	return current_user;
}

// 更改Array类静态数据成员
void Array::modifyArray(int n, int type)
{
	switch (type)
	{
	case MEDICINE:
		numArrayOfMedicine += n;
		break;

	case PERSONNEL:
		numArrayOfPersonnel += n;
		break;

	case USER:
		numArrayOfUser += n;
		break;
	}
}

// 更改当前登陆用户
void Array::login(int type)
{
	current_user = type;
}

/////////////////////////////////////////////////
// 派生类 ArrayOfMedicine

// ArrayOfMedicine类的无参构造函数
ArrayOfMedicine::ArrayOfMedicine()
{
	medicine = new Medicine[MAX_RECORD_MEDICINE];
	record_index = 0;
	num_of_medicine = 0;
	modifyArray(1, MEDICINE);
}

// ArrayOfMedicine类的带一个形参的构造函数
ArrayOfMedicine::ArrayOfMedicine(int n)
{
	medicine = new Medicine[n];
	modifyArray(1, MEDICINE);
}

// ArrayOfMedicine类的复制构造函数
ArrayOfMedicine::ArrayOfMedicine(ArrayOfMedicine & array_of_medicine)
{
	// need to copy all the contents? or simply the header?
	record_index = array_of_medicine.record_index;
	medicine = new Medicine[MAX_RECORD_MEDICINE];
	for (int i = 0; i < record_index; i++)
	{
		medicine[i].name = array_of_medicine.medicine[i].name;
		medicine[i].number = array_of_medicine.medicine[i].number;
		medicine[i].amount = array_of_medicine.medicine[i].amount;
		medicine[i].price = array_of_medicine.medicine[i].price;
		medicine[i].accessibility = array_of_medicine.medicine[i].accessibility;
	}
}

// ArrayOfMedicine类的析构函数
ArrayOfMedicine::~ArrayOfMedicine()
{
	modifyArray(-1, MEDICINE);
}

// 添加或更新一条记录
void ArrayOfMedicine::Add(int t_number, string t_name, int t_amount, double t_price)
{
	if (getCurrentUser() == IS_MANAGER)
	{
		if (t_amount > 0)
		{
			catchError(E_AUTHORITY);
		}
		else
		{
			int t_index = FindIndex(t_number);
			if (medicine[t_index].amount + t_amount < 0)
			{
				bool check_amount = catchError(E_AMOUNT);
			}
			else
			{
				medicine[t_index].name = t_name;
				medicine[t_index].amount += t_amount;
				medicine[t_index].price = t_price;
				cout << "--药品信息更新成功！--" << endl << endl;
			}
		}
	}
	else
	{
		int judge = checkFormat(t_number, t_name);
		if (catchError(judge))
		{
			if (record_index > 900)
			{
				cout << "存储空间不足，剩余 " << 1000 - record_index << " 条记录空间可供使用。" << endl;
			}
			if (judge == W_UPDATE_MEDICINE)
			{
				int t_index = FindIndex(t_number);
				if (medicine[t_index].amount + t_amount < 0)
				{
					bool check_amount = catchError(E_AMOUNT);
				}
				else
				{
					medicine[t_index].name = t_name;
					medicine[t_index].amount += t_amount;
					medicine[t_index].price = t_price;
					cout << "--药品信息更新成功！--" << endl << endl;
				}
			}
			else
			{
				medicine[record_index].number = t_number;
				medicine[record_index].name = t_name;
				medicine[record_index].amount = t_amount;
				medicine[record_index].price = t_price;
				medicine[record_index].accessibility = true;
				medicine[record_index].index = record_index;
				record_index++;
				cout << "--药品入库成功！--" << endl << endl;
			}
		}
	}
}

// 删除一条记录
void ArrayOfMedicine::Delete(int index)
{
	int error_code = NO_ERROR;							// 先置error_code为NO_ERROR
	if (index >= record_index || index < 0)							// 如果输入的下标值超过当前最大下标
	{
		error_code = E_NO_RECORD;						// 返回错误代码E_NO_RECORD
	}
	else if (medicine[index].accessibility == false)	// 否则，如果存在该条记录但是不可访问，意味着这条记录已被删除
	{
		error_code = E_NO_RECORD;						// 返回错误代码E_NO_RECORD
	}
	bool judge = catchError(error_code);				// 设置布尔变量judge用于接收catchError成员函数的判断结果
	if (judge == true)									// 如果没有错误，则执行删除操作
	{
		medicine[index].accessibility = false;			// 将输入的下标对应的记录的accessibility属性设为false，意味着这条记录不可访问，也就是被删除
		cout << "删除成功！" << endl;
	}
}

// 根据模式选择参数输出药品信息
void ArrayOfMedicine::Show(int index, int mode)
{
	bool judge = false;
	if (index != IGNORE_INDEX)
	{
		int error_code = NO_ERROR;							// 先置error_code为NO_ERROR
		if (index >= record_index || index < 0 )
		{													// 如果输入的下标值大于目前最大下标值或者根据下标值找到的记录被删除
			error_code = E_NO_RECORD;						// 返回错误代码E_NO_RECORD
		}
		else if (medicine[index].accessibility == false)
		{
			error_code = E_NO_RECORD;
		}
		judge = catchError(error_code);				// 设置布尔变量judge用于接收catchError成员函数的判断结果
	}

	if (judge == true || index == IGNORE_INDEX)
	{
		switch (mode)
		{
		case SHOW_TITLES:								// 输出单条记录，包括标题栏
			cout << setw(20) << "药品编号" << setw(20) << "药品名称" << setw(20) << "药品余量" << setw(20) << "药品单价" << endl;
			cout << "--------------------------------------------------------------------------------" << endl;
			cout << setw(20) << medicine[index].number << setw(20) << medicine[index].name << setw(20) << medicine[index].amount << setw(20) << medicine[index].price << endl;
			break;

		case NO_SHOW_TITLES:							// 输出单条记录，不包括标题栏
			cout << setw(20) << medicine[index].number << setw(20) << medicine[index].name << setw(20) << medicine[index].amount << setw(20) << medicine[index].price << endl;
			break;

		case ONLY_SHOW_TITLES:							// 只输出标题栏
			cout << setw(20) << "药品编号" << setw(20) << "药品名称" << setw(20) << "药品余量" << setw(20) << "药品单价" << endl;
			cout << "--------------------------------------------------------------------------------" << endl;
			break;
		}
	}
	
}

// 显示当前所有药品信息
void ArrayOfMedicine::ShowAllMedicine()
{
	Show(IGNORE_INDEX, ONLY_SHOW_TITLES);						// 输出标题栏
	for (int i = 0; i < record_index; i++)				// 遍历之前的记录
	{
		if (medicine[i].accessibility == true)			// 如果找到没有被删除的记录
		{	
			Show(i, NO_SHOW_TITLES);					// 输出这条记录的详细信息
		}
	}
}

// 根据药品编号查询下标
int ArrayOfMedicine::FindIndex(int t_number)
{
	for (int i = 0; i < record_index; i++)				// 遍历之前的记录
	{
		if (medicine[i].accessibility == true && medicine[i].number == t_number)
		{												// 如果找到没有删除的记录并且该记录的编号与待查找的编号相同
			return medicine[i].index;					// 返回这条记录的下标值
		}
	}
	return E_NO_RECORD;									// 如果没有找到任何记录，返回错误代码E_NO_RECORD
}

// 得到当前的合法记录条数
int ArrayOfMedicine::calNumberOfMedicine()
{
	num_of_medicine = 0;								// 先置num_of_personnel为0，以防出现在同一过程中多次计算返回值不一致的问题
	for (int i = 0; i < record_index; i++)				// 遍历之前的记录进行查找
	{
		if (medicine[i].accessibility == true)			// 如果找到一条可以访问的记录，意味着它没有被删除
		{
			num_of_medicine++;							// 令已找到的可访问的记录数加1
		}
	}

	return num_of_medicine;							// 返回找到的可访问的记录数
}


// 检查用户的输入并找出问题
int ArrayOfMedicine::checkFormat(int t_number, string t_name)
{
	if (getCurrentUser() == IS_WAREHOUSE || getCurrentUser() == IS_ADMIN)
	{
		for (int i = 0; i < record_index; i++)
		{
			if (medicine[i].accessibility == true)
			{
				if (medicine[i].number == t_number)
				{
					if (medicine[i].name == t_name)
					{
						return W_UPDATE_MEDICINE;
						break;
					}
					else if(t_name == IGNORE )
					{
						return NO_ERROR;
						break;
					}
					else
					{
						return E_NUMBER;
					}
				}
			}
		}
		return NO_ERROR;
	}
	else if (getCurrentUser() == IS_MANAGER)
	{
		if (t_name == IGNORE)
		{
			return NO_ERROR;
		}
		else
		{
			return E_AUTHORITY;
		}
	}
}

///////////////////////////////////////////////////
// 派生类 ArrayOfUser

// ArrayOfUser类无参构造函数
ArrayOfUser::ArrayOfUser()
{
	user = new User[MAX_RECORD_USER];					// 默认最大记录数100条
	record_index = 0;
	num_of_user = 0;
	modifyArray(1, USER);								// 将Array类静态数据成员numArrayOfUser加1
}

// ArrayOfUser类带一个形参的构造函数
ArrayOfUser::ArrayOfUser(int n)
{
	user = new User[n];									// 手动指定最大记录数。当前版本中未使用，为后面版本预留。
	modifyArray(1, USER);
}

// ArrayOfUser类复制构造函数
ArrayOfUser::ArrayOfUser(ArrayOfUser & array_of_user)
{
														// 深复制。在当前版本中未使用，为后面版本预留。
	record_index = array_of_user.record_index;
	user = new User[MAX_RECORD_USER];
	for (int i = 0; i < record_index; i++)
	{
		user[i].authority = array_of_user.user[i].authority;
		user[i].account = array_of_user.user[i].account;
		user[i].password = array_of_user.user[i].password;
		user[i].accessibility = array_of_user.user[i].accessibility;
		record_index++;
	}
}

// ArrayOfUser类析构函数
ArrayOfUser::~ArrayOfUser()
{
	modifyArray(-1, USER);								// 将Array类中静态数据成员numArrayOfUser减1
}

// 添加一条记录
void ArrayOfUser::Add(int t_authority, int t_number, string t_account, string t_password)
{
	int judge = checkFormat(t_authority, t_number, t_account);
														// 检查输入
	if (catchError(judge))
	{
		if (record_index > 90)							// 如果记录数已经超过90，则提示存储空间余量不足
		{
			cout << "存储空间不足，剩余 " << 1000 - record_index << " 条记录空间可供使用。" << endl;
		}

		user[record_index].authority = t_authority;
		if (t_authority == IS_ADMIN)					// 登记系统管理员
		{
			admin_existed = true;
		}		
		user[record_index].number = t_number;
		user[record_index].account = t_account;
		user[record_index].password = t_password;
		user[record_index].accessibility = true;
		user[record_index].index = record_index;
		record_index++;
		cout << "--账户创建成功！--" << endl << endl;
	}
	
}

// 删除一条记录
void ArrayOfUser::Delete(int index)
{
	int error_code = NO_ERROR;							// 先置error_code为NO_ERROR
	if (index >= record_index || index < 0)							// 如果输入的下标值超过当前最大下标
	{
		error_code = E_NO_RECORD;						// 返回错误代码E_NO_RECORD
	}
	else if (user[index].accessibility == false)	// 否则，如果存在该条记录但是不可访问，意味着这条记录已被删除
	{
		error_code = E_NO_RECORD;						// 返回错误代码E_NO_RECORD
	}
	bool judge = catchError(error_code);				// 设置布尔变量judge用于接收catchError成员函数的判断结果
	if (judge == true)									// 如果没有错误，则执行删除操作
	{
		user[index].accessibility = false;			// 将输入的下标对应的记录的accessibility属性设为false，意味着这条记录不可访问，也就是被删除
		cout << "--删除成功！--" << endl << endl;
	}
}

// 根据输入的模式选择参数进行记录输出
void ArrayOfUser::Show(int index, int mode)
{
	bool judge = false;
	if (index != IGNORE_INDEX)
	{
		int error_code = NO_ERROR;							// 先置error_code为NO_ERROR
		if (index >= record_index || index < 0 )
		{													// 如果输入的下标值大于目前最大下标值或者根据下标值找到的记录被删除
			error_code = E_NO_RECORD;						// 返回错误代码E_NO_RECORD
		}
		else if (user[index].accessibility == false)
		{
			error_code = E_NO_RECORD;
		}
		judge = catchError(error_code);				// 设置布尔变量judge用于接收catchError成员函数的判断结果
	}

	if (judge == true || index == IGNORE_INDEX)
	{
		switch (mode)
		{
		case SHOW_TITLES:								// 输出单条记录，带标题栏
			cout << setw(20) << "权限" << setw(20) << "账户名" << endl;
			cout << "----------------------------------------" << endl;
			cout << setw(20) << macroToString(user[index].authority) << setw(20) << user[index].account << endl;
			break;

		case NO_SHOW_TITLES:							// 输出单条记录，不带标题栏
			cout << setw(20) << macroToString(user[index].authority) << setw(20) << user[index].account << endl;
			break;

		case ONLY_SHOW_TITLES:							// 只输出标题栏
			cout << setw(20) << "权限" << setw(20) << "账户名" << endl;
			cout << "----------------------------------------" << endl;
			break;
		}
	}

}

// 显示所有注册用户的信息
void ArrayOfUser::ShowAllUser()
{
	Show(IGNORE_INDEX, ONLY_SHOW_TITLES);						// 输出标题栏
	for (int i = 0; i < record_index; i++)				// 查找有效记录
	{
		if (user[i].accessibility == true)
		{
			Show(i, NO_SHOW_TITLES);					// 输出该记录详细信息	
		}
	}
}

// 将与ArrayOfUser类相关的宏转为字符串
string ArrayOfUser::macroToString(int macro_code)
{
	switch (macro_code)
	{
	case IS_ADMIN:
		return "系统管理员";
		break;

	case IS_MANAGER:
		return "销售人员";
		break;

	case IS_WAREHOUSE:
		return "仓库管理员";
		break;
	}
}

// 根据编号查找下标
int ArrayOfUser::FindIndex(int t_number)
{
	for (int i = 0; i < record_index; i++)				// 遍历之前的记录
	{
		if (user[i].accessibility == true && user[i].number == t_number)
		{												// 如果找到一条记录没有被删除且其编号与待查找的编号相同
			return user[i].index;						// 返回这条记录的下标
		}
	}
	return E_NO_RECORD;									// 如果找不到相关记录，返回错误代码E_NO_RECORD
}

// 得到当前用户的合法记录条数
int ArrayOfUser::calNumberOfUser()
{
	num_of_user = 0;								// 先置num_of_personnel为0，以防出现在同一过程中多次计算返回值不一致的问题
	for (int i = 0; i < record_index; i++)				// 遍历之前的记录进行查找
	{
		if (user[i].accessibility == true)			// 如果找到一条可以访问的记录，意味着它没有被删除
		{
			num_of_user++;							// 令已找到的可访问的记录数加1
		}
	}

	return num_of_user;							// 返回找到的可访问的记录数
}

// 检测当前登录的用户的权限
int ArrayOfUser::checkAuthority()
{
	for (int i = 0; i < record_index; i++)				// 遍历之前的记录
	{
		if (user[i].accessibility == true && user[i].is_login == true)
		{												// 如果这条记录没有被删除而且已登录
			return user[i].authority;					// 返回这条记录的权限信息
			break;
		}
	}
	return NO_USERS;									// 如果没有找到任何登录用户或是合法用户，返回代码NO_USERS
}
	 
// 检查用户的输入并找出问题
int ArrayOfUser::checkFormat(int t_authority, int t_number, string t_account)
{
	if (t_authority == IS_ADMIN)						// 检测输入信息的t_authority是否为IS_ADMIN，即判断用户是否想要创建系统管理员账户
	{
		if (admin_existed)								// 如果已经存在了系统管理员账户
		{			
			return E_ADMIN_EXISTED;						// 返回错误代码E_ADMIN_EXISTED
		}
		else											// 否则，允许用户创建管理员账户
		{
			return NO_ERROR;							// 返回代码NO_ERROR
		}
	}
	else												// 如果输入信息的t_authority不为IS_ADMIN，则认为用户想要创建一般账户
	if (!admin_existed)									// 如果不存在管理员，则不允许用户创建一般账户 
	{
		return E_AUTHORITY;
	}
	else
	{
		for (int i = 0; i < record_index; i++)			// 遍历之前的记录进行查找
		{
			if (user[i].accessibility == true)			// 如果找到一条可以访问的记录，意味着它没有被删除
			{
				if (user[i].number == t_number)			// 核对编号，如果存在同编号账户
				{
					return E_NUMBER;					// 返回错误代码E_NUMBER
					break;
				}
				if (user[i].account == t_account)		// 核对账户名，如果存在同名账户
				{
					return E_ACCOUNT;					// 返回错误代码E_ACCOUNT
					break;
				}
			}
		}
		return NO_ERROR;								// 如果没有找到任何问题，返回代码NO_ERROR
	}
}

/////////////////////////////////////////
// 派生类 ArrayOfPersonnel

// ArrayOfPersonnel类的无参构造函数
ArrayOfPersonnel::ArrayOfPersonnel()
{
	personnel = new Personnel[MAX_RECORD_PERSONNEL];	// 默认情况下最大记录条数为1000条
	record_index = 0;
	num_of_personnel = 0;
	modifyArray(1, PERSONNEL);							// 将基类Array中的静态数据成员numArrayOfPersonnel减1
}

// ArrayOfPersonnel类的带一个形参的构造函数
ArrayOfPersonnel::ArrayOfPersonnel(int n)
{
	personnel = new Personnel[n];						// 手动指定最大记录数。在目前版本中没有应用，为后面版本预留。
	modifyArray(1, PERSONNEL);
}

// ArrayOfPersonnel类的复制构造函数
ArrayOfPersonnel::ArrayOfPersonnel(ArrayOfPersonnel & array_of_personnel)
{				
														// 执行深复制。在当前版本没有应用，为后面版本中的数据转移和恢复功能预留。
	record_index = array_of_personnel.record_index;
	personnel = new Personnel[MAX_RECORD_PERSONNEL];
	for (int i = 0; i < record_index; i++)
	{
		personnel[i].number = array_of_personnel.personnel[i].number;
		personnel[i].name = array_of_personnel.personnel[i].name;
		personnel[i].age = array_of_personnel.personnel[i].age;
		personnel[i].identity = array_of_personnel.personnel[i].identity;
		personnel[i].accessibility = array_of_personnel.personnel[i].accessibility;
	}
}

// ArrayOfPersonnel类的析构函数
ArrayOfPersonnel::~ArrayOfPersonnel()
{
	modifyArray(-1, PERSONNEL);							// 将基类Array中的静态数据成员numArrayOfPersonnel减1
}

// 添加或更新人员记录
void ArrayOfPersonnel::Add(int t_number, string t_name, int t_age, int t_identity)
{
	int judge = checkFormat(t_number, t_name, t_age, t_identity);
														// 先检查输入信息的合法性，并判断操作类型
	if (catchError(judge))								// 如果检查通过，意味着输入信息是合法的
	{
		int t_index = FindIndex(t_number);				// 将根据编号找到的下标值存储在t_index中，以减少重复运算
		if (record_index > 900)							// 当记录数大于900时，提醒用户注意存储空间余量。当前版本最大存储记录数为1000条，包括已删除的在内。
		{
			cout << "存储空间不足，剩余 " << 1000 - record_index << " 条记录空间可供使用。" << endl;
		}
		switch (judge)									// 根据judge的结果进行选择
		{
		case W_UPDATE_PERSONNEL:						// 这个模式进行人员信息更新
			personnel[t_index].name = t_name;
			personnel[t_index].age = t_age;
			personnel[t_index].identity = t_identity;
			cout << "--人员信息更新成功！--" << endl << endl;
			break;

		case NO_ERROR:									// 这个模式进行人员信息添加
			personnel[record_index].number = t_number;
			personnel[record_index].name = t_name;
			personnel[record_index].age = t_age;
			personnel[record_index].identity = t_identity;
			personnel[record_index].accessibility = true;
			personnel[record_index].index = record_index;
			record_index++;
			cout << "--人员信息添加成功！--" << endl << endl;
			break;
		}
	}
}

// 根据输入的模式选择参数进行记录输出
void ArrayOfPersonnel::Show(int index, int mode)
{
	bool judge = false;
	if (index != IGNORE_INDEX)
	{
		int error_code = NO_ERROR;							// 先置error_code为NO_ERROR
		if (index >= record_index || index < 0 )
		{													// 如果输入的下标值大于目前最大下标值或者根据下标值找到的记录被删除
			error_code = E_NO_RECORD;						// 返回错误代码E_NO_RECORD
		}
		else if (personnel[index].accessibility == false)
		{
			error_code = E_NO_RECORD;
		}
		judge = catchError(error_code);				// 设置布尔变量judge用于接收catchError成员函数的判断结果
	}
	
	if (judge == true || index == IGNORE_INDEX)
	{
		switch (mode)
		{
		case SHOW_TITLES:								// 这个模式用于输出单条记录，包括标题栏和具体内容
			cout << setw(20) << "人员编号" << setw(20) << "姓名" << setw(20) << "年龄" << setw(20) << "身份" << endl;		
			cout << "--------------------------------------------------------------------------------" << endl;
			cout << setw(20) << personnel[index].number << setw(20) << personnel[index].name << setw(20) << personnel[index].age << setw(20) << macroToString(personnel[index].identity) << endl;
			break;

		case NO_SHOW_TITLES:							// 这个模式用于输出单条记录具体信息，不包括标题栏
			cout << setw(20) << personnel[index].number << setw(20) << personnel[index].name << setw(20) << personnel[index].age << setw(20) << macroToString(personnel[index].identity) << endl;
			break;

		case ONLY_SHOW_TITLES:							// 这个模式用于输出标题栏
			cout << setw(20) << "人员编号" << setw(20) << "姓名" << setw(20) << "年龄" << setw(20) << "身份" << endl;
			cout << "--------------------------------------------------------------------------------" << endl;
			break;
		}
	}
	
}

// 删除一条记录
void ArrayOfPersonnel::Delete(int index)
{
	int error_code = NO_ERROR;							// 先置error_code为NO_ERROR
	if (index >= record_index || index < 0)							// 如果输入的下标值超过当前最大下标
	{
		error_code = E_NO_RECORD;						// 返回错误代码E_NO_RECORD
	}					
	else if (personnel[index].accessibility == false)	// 否则，如果存在该条记录但是不可访问，意味着这条记录已被删除
	{
		error_code = E_NO_RECORD;						// 返回错误代码E_NO_RECORD
	}
	bool judge = catchError(error_code);				// 设置布尔变量judge用于接收catchError成员函数的判断结果
	if (judge == true)									// 如果没有错误，则执行删除操作
	{
		personnel[index].accessibility = false;			// 将输入的下标对应的记录的accessibility属性设为false，意味着这条记录不可访问，也就是被删除
		cout << "删除成功！" << endl;
	}
}

// 显示所有的人员信息
void ArrayOfPersonnel::ShowAllPersonnel()
{
	Show(IGNORE_INDEX, ONLY_SHOW_TITLES);						// 输出标题栏。使用参数ONLY_SHOW_TITLES调用Show（）成员函数指定只输出标题栏
	for (int i = 0; i < record_index; i++)				// 遍历之前的记录进行查找
	{
		if (personnel[i].accessibility == true)			// 如果找到一条可以访问的记录，意味着这条记录没有被删除
		{
			Show(i, NO_SHOW_TITLES);					// 输出找到的记录信息。使用参数NO_SHOW_TITLES调用Show（）成员函数指定只输出记录的具体信息
		}
	}
}

// 将与本类相关的宏转变为字符串
string ArrayOfPersonnel::macroToString(int macro_code)
{
	switch (macro_code)
	{
	case CUSTOMER:
		return "顾客";
		break;

	case STAFF:
		return "工作人员";
		break;

	case VENDOR:
		return "供销商";
		break;
	}
}

// 根据编号查找下标
int ArrayOfPersonnel::FindIndex(int t_number)
{
	for (int i = 0; i < record_index; i++)				// 遍历之前的记录进行查找
	{
		if (personnel[i].accessibility == true && personnel[i].number == t_number)
		{												// 如果找到一条可以访问的记录并且这条记录中的编号与待查找的编号相同
			return personnel[i].index;					// 则返回这条记录的下标
		}
	}
	return E_NO_RECORD;									// 如果遍历后未找到符合要求的记录，则返回错误代码E_NO_RECORD
}

// 得到当前人员信息的合法记录条数
int ArrayOfPersonnel::calNumberOfPersonnel()
{
	num_of_personnel = 0;								// 先置num_of_personnel为0，以防出现在同一过程中多次计算返回值不一致的问题
	for (int i = 0; i < record_index; i++)				// 遍历之前的记录进行查找
	{
		if (personnel[i].accessibility == true)			// 如果找到一条可以访问的记录，意味着它没有被删除
		{
			num_of_personnel++;							// 令已找到的可访问的记录数加1
		}
	}

	return num_of_personnel;							// 返回找到的可访问的记录数
}

// 检查用户的输入并找出问题
int ArrayOfPersonnel::checkFormat(int t_number,string t_name,int t_age,int t_identity)
{
	if (getCurrentUser() == IS_ADMIN)					// 检查当前登录用户的权限。只有系统管理员有权限进行人员信息修改的相关操作。
	{
		for (int i = 0; i < record_index; i++)			// 遍历之前的记录进行查找
		{
			if (personnel[i].accessibility == true)		// 如果找到一条可以访问的记录，意味着它没有被删除
			{
				if (personnel[i].number == t_number)	// 先检查编号
				{
					if (personnel[i].name == t_name)	// 如果已存在相同的编号，则继续检查名称
					{
						if (personnel[i].age == t_age)
						{
							if (personnel[i].identity - t_identity)
							{
								return E_NUMBER;		// 这种情况下，已存在完全相同的记录，系统认为是用户输入错误，返回错误代码E_NUMBER，因为系统以编号作为最终判断依据
							}
							else
							{
								return W_UPDATE_PERSONNEL;
														// 这种情况下，已存在只有身份信息不同的记录，系统认为用户想要更新该条记录，返回警告代码W_UPDATE_PERSONNEL
							}
						}
						else
						{
							return W_UPDATE_PERSONNEL;	// 这种情况下，已存在有相同编号相同名称的记录，系统认为用户想要更新该条记录，返回警告代码W_UPDATE_PERSONNEL
						}
						
					}
					else
					{
						return W_UPDATE_PERSONNEL;		// 这种情况下，已存在相同编号但是不同名称的记录，系统认为用户想要更新该条记录，返回警告代码W_UPDATE_PERSONNEL
						break;
					}
				}
			}
		}
		return NO_ERROR;								// 如果遍历之前的所有记录都没有发现异常，系统认为用户想要添加一条全新的记录，返回代码NO_ERROR
	}
	else
	{
		return E_AUTHORITY;								// 如果权限检查未通过，返回错误代码E_AUTHORITY
	}
}
