#include<iostream>
#include<string>
#include"Personnel.h"

using std::cout;
using std::endl;
using std::string;


Personnel::Personnel(int t_number,string t_name,int t_age,int t_identity)//Personnel类的构造函数
{
    number=t_number;
    name=t_name;
    age=t_age;
    accessibility=true;
    identity=t_identity;
}

Personnel::Personnel(Personnel &t_Personnel)//Personnel类的拷贝构造函数
{
    number=t_Personnel.number;
    name=t_Personnel.name;
    age=t_Personnel.age;
    accessibility=t_Personnel.accessibility;
    identity=t_Personnel.identity;
}
