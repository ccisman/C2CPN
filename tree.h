#pragma once
#include<iostream>
#include<string>

using namespace std;
#define MAX_LENGTH 3000
#define MAX_WORD_NUM 1000
#define MAX_BUFF_SIZE 40
#define tb_sum 20
#define kong -1
#define ERROR1 -1


enum item
{
	关键字 = 1, 标识符, 无符号整数, 赋值号, 算符, 界符, 冒号, 分隔符, 注释号, 左括号, 右括号, 左中括号, 右中括号, 左大括号, 右大括号, 字母, 无符号实数, 结束符, 字符串, 字符,
	程序, 常量说明, 常量定义, 整数, 实数, 声明头部, 变量说明, 变量定义, 可枚举常量, 类型标识符, 函数定义, 复合语句, 参数表, 主函数, 表达式,
	项, 因子, 语句, 赋值语句, 条件语句, 条件, 循环语句, 情况语句, 情况表, 情况子语句, 缺省, 函数调用语句, 值参数表, 语句列, 读语句, 写语句, 返回语句, 条件组,子项,break语句
};




class gtree
{
public:
	//保留部分，添加属性信息
	int num;//先序节点号
	string place;
	string _true;
	string _false;
	string begin;
	string end;
	string before;
	bool _before;
	string after;
	bool _after;
	//string code;
	//保留部分，添加属性信息
	gtree *next;//兄弟指针
	gtree *child;//孩子指针
	gtree *parent;//父节点指针
	int type;//节点类型syn
	int type_1;//节点具体类型syn_1,针对算符和关键字
	void init(int root);//初始化
	void expand(int num, gtree *t, int sum);
	void cnode(int syn, int syn_1, gtree *t);
	void newlable(int a);
	string newlable1(int a, int flag);
	//void gen(string a);
};

gtree *&create_tree(string text, int root, int &if_num, int &while_num, int &break_num, int &fun_num);

gtree* search_call(gtree *p);

string get_gen_P();

gtree* find_last_sentence(gtree *node, bool &flag, bool &flag1);