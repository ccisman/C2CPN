#pragma once
#include"tree.h"
#include<string>
class table//含过程调用
{
public:
	string name;
	int type;
	int value;
	int offset;
	void enter(string name, int type, int value, int offset);
};

void table::enter(string name1, int type1, int value1, int offset1)
{
	name = name1;
	type = type1;
	value = value1;
	offset = offset1;
}
