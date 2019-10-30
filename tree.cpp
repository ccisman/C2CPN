#include"tree.h"

void gtree::init(int root)
{
	this->type = root;
	this->type_1 = 0;
	this->child = NULL;
	this->next = NULL;
}

void gtree::cnode(int syn, int syn_1, gtree *t)
{
	t->next = new gtree;
	t->next->type = syn;
	t->next->type_1 = syn_1;
	t->next->child = NULL;
	t->next->next = NULL;
	t->next->parent = t->parent;
}

void gtree::newlable(int a)
{
	char b[3] = "0";
	if (a >= 10)
	{
		a = a - 10 + 'a' - '0';
	}
	b[0] += a;
	string c = b;
	place = "T" + c;
}

string gtree::newlable1(int a, int flag)
{
	char b[3] = "0";
	b[0] += a;
	string c = b;
	return("L" + c);
}

void gtree::expand(int num, gtree *t, int sum)//sum表示一些特殊的内容重复次数
{
	t->child = new gtree;
	gtree *t1 = t->child;
	t1->child = NULL;
	t1->next = NULL;
	t1->parent = t;

	if (num == 程序)//sum=0代表无常量、无变量说名，1代表无常量、有变量，2代表有变量、无常量，3代表都有
	{
		//特殊处理
		t1->type = kong;
		t1->type_1 = 0;
	}
	else if (num == 常量说明)//sum无用
	{
		t1->type = 关键字;
		t1->type_1 = 10;//10代表const
		cnode(常量定义, 0, t1);
		cnode(界符, 0, t1->next);
	}
	else if (num == 常量定义)//1代表int，2代表float，3代表double，4代表char
	{
		if (sum == 1)
		{
			t1->type = 类型标识符;
			t1->type_1 = 0;
			cnode(标识符, 0, t1);
			cnode(赋值号, 0, t1->next);
			cnode(整数, 0, t1->next->next);
		}
		else if (sum == 8)
		{
			t1->type = 类型标识符;
			t1->type_1 = 0;
			cnode(标识符, 0, t1);
			cnode(赋值号, 0, t1->next);
			cnode(实数, 0, t1->next->next);
		}
		else if (sum == 9)
		{
			t1->type = 类型标识符;
			t1->type_1 = 0;
			cnode(标识符, 0, t1);
			cnode(赋值号, 0, t1->next);
			cnode(实数, 0, t1->next->next);
		}
		else if (sum == 7)
		{
			t1->type = 类型标识符;
			t1->type_1 = 0;
			cnode(标识符, 0, t1);
			cnode(赋值号, 0, t1->next);
			cnode(字符, 0, t1->next->next);
		}
	}
	else if (num == 整数)//sum代表+ -号的值5.6，若没有+-则为0
	{
		if (sum == 0)
		{
			t1->type = 无符号整数;
			t1->type_1 = 0;
		}
		else if (sum == 5 || sum == 6)
		{
			t1->type = 算符;
			t1->type_1 = sum;
			cnode(无符号整数, 0, t1);
		}
	}
	else if (num == 实数)//sum代表+ -号的值5.6，若没有+-则为0
	{
		if (sum == 0)
		{
			t1->type = 无符号实数;
			t1->type_1 = 0;
		}
		else if (sum == 5 || sum == 6)
		{
			t1->type = 算符;
			t1->type_1 = sum;
			cnode(无符号实数, 0, t1);
		}
	}
	else if (num == 声明头部)//1代表int，2代表void，3代表double，4代表char，5代表void
	{

		if (sum == 2)
		{
			t1->type = 关键字;
			t1->type_1 = 2;//2代表void
			cnode(标识符, 0, t1);
		}
		else
		{
			t1->type = 类型标识符;
			t1->type_1 = 0;
			cnode(标识符, 0, t1);
		}
	}
	else if (num == 变量说明)//sum无用
	{
		t1->type = 变量定义;
		t1->type_1 = 0;
		cnode(界符, 0, t1);
	}
	else if (num == 变量定义)//sum无用
	{

		t1->type = 类型标识符;
		t1->type_1 = 0;
		cnode(标识符, 0, t1);

	}
	else if (num == 可枚举常量)//sum=1代表整数 sum=2代表字符
	{
		if (sum == 1)
		{
			t1->type = 整数;
			t1->type_1 = 0;
		}
		else if (sum == 2)
		{
			t1->type = 字符;
			t1->type_1 = 0;
		}
	}
	else if (num == 类型标识符)//sum无用
	{

		t1->type = kong;
		t1->type_1 = 0;

	}
	else if (num == 函数定义)//sum无用
	{
		t1->type = 声明头部;
		t1->type_1 = 0;
		cnode(左括号, 0, t1);
		cnode(参数表, 0, t1->next);
		cnode(右括号, 0, t1->next->next);
		cnode(左大括号, 0, t1->next->next->next);
		cnode(复合语句, 0, t1->next->next->next->next);
		cnode(右大括号, 0, t1->next->next->next->next->next);
	}
	else if (num == 复合语句)//sum=0代表无常量、无变量说名，1代表无常量、有变量，2代表有变量、无常量，3代表都有
	{
		//特殊处理
		t1->type = kong;
		t1->type_1 = 0;
	}
	else if (num == 参数表)//sum=0代表空，sum=1代表有参数
	{
		if (sum == 0)
		{
			t1->type = kong;
			t1->type_1 = 0;
		}
		else if (sum == 1)
		{
			t1->type = 类型标识符;
			t1->type_1 = 0;
			cnode(标识符, 0, t1);
		}
	}
	else if (num == 表达式)//sum代表+ -号的值5.6，若没有+-则为0
	{
		if (sum == 0)
		{
			t1->type = 项;
			t1->type_1 = 0;
		}
		else if (sum == 5 || sum == 6)
		{
			t1->type = 算符;
			t1->type_1 = sum;
			cnode(项, 0, t1);
		}
	}
	else if (num == 项)//sum无用
	{
		t1->type = 子项;
		t1->type_1 = 0;
	}
	else if (num == 子项)//sum无用
	{
		t1->type = 因子;
		t1->type_1 = 0;
	}
	else if (num == 因子)//sum=1代表标识符，sum=2代表标识符+[整数]，sum=3代表（表达式），sum=4代表无符号整数，sum=5代表无符号实数，sum=6代表字符，sum=7代表函数调用语句
	{
		if (sum == 1)
		{
			t1->type = 标识符;
			t1->type_1 = 0;
		}
		else if (sum == 2)
		{
			t1->type = 标识符;
			t1->type_1 = 0;
			cnode(左中括号, 0, t1);
			cnode(表达式, 0, t1->next);
			cnode(右中括号, 0, t1->next->next);
		}
		else if (sum == 3)
		{
			t1->type = 左中括号;
			t1->type_1 = 0;
			cnode(表达式, 0, t1);
			cnode(右中括号, 0, t1->next);
		}
		else if (sum == 4)
		{
			t1->type = 整数;
			t1->type_1 = 0;
		}
		else if (sum == 5)
		{
			t1->type = 实数;
			t1->type_1 = 0;
		}
		else if (sum == 6)
		{
			t1->type = 字符;
			t1->type_1 = 0;
		}
		else if (sum == 7)
		{
			t1->type = 函数调用语句;
			t1->type_1 = 0;
		}
	}
	else if (num == 语句)//sum=1代表条件语句，sum=2代表循环语句，sum=3代表{语句列}，sum=4代表函数调用语句，sum=5代表赋值语句，sum=6代表读语句，sum=7代表写语句，sum=8代表空，sum=9代表情况语句，sum=10代表返回语句，sum=11代表表达式
	{
		if (sum == 1)
		{
			t1->type = 条件语句;
			t1->type_1 = 0;
		}
		else if (sum == 2)
		{
			t1->type = 循环语句;
			t1->type_1 = 0;
		}
		else if (sum == 3)
		{
			t1->type = 左大括号;
			t1->type_1 = 0;
			cnode(语句列, 0, t1);
			cnode(右大括号, 0, t1->next);
		}
		else if (sum == 4)
		{
			t1->type = 函数调用语句;
			t1->type_1 = 0;
			cnode(界符, 0, t1);
		}
		else if (sum == 5)
		{
			t1->type = 赋值语句;
			t1->type_1 = 0;
			cnode(界符, 0, t1);
		}
		else if (sum == 6)
		{
			t1->type = 读语句;
			t1->type_1 = 0;
			cnode(界符, 0, t1);
		}
		else if (sum == 7)
		{
			t1->type = 写语句;
			t1->type_1 = 0;
			cnode(界符, 0, t1);
		}
		else if (sum == 8)
		{
			t1->type = kong;
			t1->type_1 = 0;
		}
		else if (sum == 9)
		{
			t1->type = 情况语句;
			t1->type_1 = 0;
		}
		else if (sum == 10)
		{
			t1->type = 返回语句;
			t1->type_1 = 0;
			cnode(界符, 0, t1);
		}
		else if (sum == 11)
		{
			t1->type = 表达式;
			t1->type_1 = 0;
			cnode(界符, 0, t1);
		}
		else if (sum == 12)
		{
			t1->type = break语句;
			t1->type_1 = 0;
			cnode(界符, 0, t1);
		}
	}
	else if (num == 赋值语句)//sum=1代表变量赋值，sum=2代表数组元素赋值
	{
		if (sum == 1)
		{
			t1->type = 标识符;
			t1->type_1 = 0;
			cnode(赋值号, 0, t1);
			cnode(表达式, 0, t1->next);
		}
		else if (sum == 2)
		{
			t1->type = 标识符;
			t1->type_1 = 0;
			cnode(左中括号, 0, t1);
			cnode(表达式, 0, t1->next);
			cnode(右中括号, 0, t1->next->next);
			cnode(赋值号, 0, t1->next->next->next);
			cnode(表达式, 0, t1->next->next->next->next);
		}
	}
	else if (num == 条件语句)//sum无用
	{
		t1->type = 关键字;
		t1->type_1 = 3;//3代表if
		cnode(左括号, 0, t1);
		cnode(条件组, 0, t1->next);
		cnode(右括号, 0, t1->next->next);
		cnode(语句, 0, t1->next->next->next);
	}
	else if (num == 条件组)//sum无用
	{
		t1->type = kong;
		t1->type_1 = 0;
	}
	else if (num == 条件)//sum无用
	{
		t1->type = 表达式;
		t1->type_1 = 0;
	}
	else if (num == 循环语句)//sum无用，for转while处理
	{

		t1->type = 关键字;
		t1->type_1 = 5;//5代表while
		cnode(左括号, 0, t1);
		cnode(条件组, 0, t1->next);
		cnode(右括号, 0, t1->next->next);
		cnode(语句, 0, t1->next->next->next);

	}
	else if (num == 情况语句)//sum无用
	{
		t1->type = 关键字;
		t1->type_1 = 11;//11代表switch
		cnode(左括号, 0, t1);
		cnode(表达式, 0, t1->next);
		cnode(右括号, 0, t1->next->next);
		cnode(左大括号, 0, t1->next->next->next);
		cnode(情况表, 0, t1->next->next->next->next);
		cnode(缺省, 0, t1->next->next->next->next->next);
		cnode(右大括号, 0, t1->next->next->next->next->next->next);
	}
	else if (num == 情况表)//sum无用
	{
		t1->type = 情况子语句;
		t1->type_1 = 0;
	}
	else if (num == 情况子语句)//sum无用
	{
		t1->type = 关键字;
		t1->type_1 = 12;//12代表case
		cnode(可枚举常量, 0, t1);
		cnode(冒号, 0, t1->next);
		cnode(语句, 0, t1->next->next);
	}
	else if (num == 缺省)//sum=0代表空，sum=1代表非空
	{
		if (sum == 0)
		{
			t1->type = kong;
			t1->type_1 = 0;
		}
		else if (sum == 1)
		{
			t1->type = 关键字;
			t1->type_1 = 15;//15代表default
			cnode(冒号, 0, t1);
			cnode(语句, 0, t1->next);
		}
	}
	else if (num == 函数调用语句)//sun无用
	{
		t1->type = 标识符;
		t1->type_1 = 0;
		cnode(左括号, 0, t1);
		cnode(值参数表, 0, t1->next);
		cnode(右括号, 0, t1->next->next);
	}
	else if (num == 值参数表)//sum=0代表空，sum=1代表非空
	{
		if (sum == 0)
		{
			t1->type = kong;
			t1->type_1 = 0;
		}
		else if (sum == 1)
		{
			t1->type = 条件;
			t1->type_1 = 0;
		}
	}
	else if (num == 语句列)//sum无用
	{
		t1->type = kong;
		t1->type_1 = 0;
	}
	else if (num == 读语句)//读语句需要自主添加右大括号
	{
		t1->type = 关键字;
		t1->type_1 = 13;//13代表scanf
		cnode(左括号, 0, t1);
		cnode(标识符, 0, t1->next);
	}
	else if (num == 写语句)//sum=2代表字符串（括号自加），sum=3代表表达式
	{
		t1->type = 关键字;
		t1->type_1 = 14;//14代表printf
		cnode(左括号, 0, t1);

		if (sum == 2)
		{
			cnode(字符串, 0, t1->next);
		}
		else if (sum == 3)
		{
			cnode(表达式, 0, t1->next);
			cnode(右括号, 0, t1->next->next);
		}
	}
	else if (num == 返回语句)//sum无用
	{
		t1->type = 关键字;
		t1->type_1 = 6;//6代表return
	}
	else if (num == break语句)
	{
		t1->type = 关键字;
		t1->type_1 = 20;//break
	}
	else
	{
		exit(-1);
	}
}