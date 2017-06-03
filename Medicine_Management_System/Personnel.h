#pragma once
#ifndef PERSONNEL_H
#define PERSONNEL_H

#include<string>
#include"Array.h"
#include"Users.h"

using std::string;

const int MAX_RECORD_PERSONNEL = 10000;

class ArrayOfPersonnel;
class Personnel
{
public:
	Personnel() {}
    Personnel(int t_number,string t_name,int t_age,int t_authority);
    Personnel(Personnel & t_Personnel);
    friend class ArrayOfPersonnel;

private:
    string name;
    int number;
    int age;
	int identity;
	int index;
    bool accessibility;
};

#endif // PERSONNEL_H
