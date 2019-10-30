#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<iomanip>
#include<fstream>

#include"tree.h"
#include"cpn.h"
#include"AST_compare.h"
#define keywordNum 20
using namespace std;

gtree *tree;
int offset1 = 0;
char prog[MAX_LENGTH], token[MAX_BUFF_SIZE], ch;//缓冲区最大大小100，标识符最长不超过40
int store[MAX_WORD_NUM][2];
char store_char[MAX_WORD_NUM][MAX_BUFF_SIZE];
int syn, syn_1, p, m, n, sum, store_num, read_num;
int sort_num = 0;

int gen_fun_num = 0;//左右大括号下标
int gen_if_num = 0, gen_while_num = 0, gen_break_num = 0;
char *rwtab[keywordNum] = { (char*)("int"),(char*)("void"),(char*)("if"),(char*)("else"),(char*)("while"),(char*)("return"),(char*)("char"),(char*)("float"),(char*)("double"),(char*)("const"),(char*)("switch"),(char*)("case"),(char*)("scanf"),(char*)("printf"),(char*)("default"),(char*)("long"),(char*)("short"),(char*)("signed"),(char*)("unsigned"),(char*)("break") };
char *caltab[14] = { (char*)("<="),(char*)("<"),(char*)(">="),(char*)(">"),(char*)("+"),(char*)("-"),(char*)("!="),(char*)("=="),(char*)("*"),(char*)("/"),(char*)("&"),(char*)("|"),(char*)("!"),(char*)("^") };
char *type[20] = { (char*)("关键字"),(char*)("标识符"),(char*)("无符号整数"),(char*)("赋值号"),(char*)("算符"),(char*)("界符"),(char*)("冒号"),(char*)("分隔符"),(char*)("注释号"),(char*)("左括号"),(char*)("右括号"),(char*)("左中括号"),(char*)("右中括号"),
(char*)("左大括号"),(char*)("右大括号"),(char*)("字母"),(char*)("无符号实数"),(char*)("结束符"),(char*)("字符串"),(char*)("字符") };


string gen_if()
{
	return "if" + to_string(gen_if_num++);
}
string gen_while()
{
	return "while" + to_string(gen_while_num++);
}
string gen_break()
{
	return "break" + to_string(gen_break_num++);
}
string gen_fun()
{
	string temp = "";
	return temp + "fun" + to_string(gen_fun_num++);

}

void TraverseTree1(gtree *p)//先第一遍遍历语法树，对place值进行更新
{
	if (p == NULL) return;
	//trans_print(p->type, 1, true);

	if (p->type == 算符)//算符place=算符本身
	{
		switch (p->type_1)
		{
		case 1:
			p->place = "<=";
			break;
		case 2:
			p->place = "<";
			break;
		case 3:
			p->place = ">=";
			break;
		case 4:
			p->place = ">";
			break;
		case 5:
			p->place = "+";
			break;
		case 6:
			p->place = "-";
			break;
		case 7:
			p->place = "!=";
			break;
		case 8:
			p->place = "==";
			break;
		case 9:
			p->place = "*";
			break;
		case 10:
			p->place = "/";
			break;
		case 11:
			p->place = "&";
			break;
		case 12:
			p->place = "|";
			break;
		case 13:
			p->place = "!";
			break;
		case 14:
			p->place = "^";
			break;
		}
	}
	else if (p->type == 赋值号)//赋值号place=本身
		p->place = "=";
	else if (p->type == 左括号)
		p->place = "(";
	else if (p->type == 右括号)
		p->place = ")";
	else if (p->type == 左中括号)
		p->place = "[";
	else if (p->type == 右中括号)
		p->place = "]";
	else if (p->type == 冒号)
		p->place = ":";
	else if (p->type == 分隔符)
		p->place = ",";
	else if (p->type == 界符)
		p->place = ";";




	TraverseTree1(p->child);
	TraverseTree1(p->next);

}
void TraverseTree2(gtree *p)//对表达式进行操作
{
	if (p == NULL)return;

	TraverseTree2(p->child);
	TraverseTree2(p->next);

	if (p->type == 因子 || p->type == 表达式 || p->type == 项 || p->type == 子项 || p->type == 值参数表 
		|| p->type == 条件 || p->type == 条件组 || p->type == 赋值语句 || p->type == 整数 || p->type == 实数)//内部变量声明place需要带有函数前缀
	{
		gtree *p1 = p->child;
		string temp_place = "";
		while (p1)
		{
			temp_place += p1->place;
			p1 = p1->next;
		}
		p->place = temp_place;
	}
	else if (p->type == 函数调用语句)
	{
		if (p->parent->type != 语句)
			p->place = p->child->place + "_v";
		else
		{
			p->place = p->child->place + "_call";
		}
	}
	else if (p->type == 条件语句)
	{
		p->place = gen_if();
	}
	else if (p->type == 循环语句)
	{
		p->place = gen_while();
	}
	else if (p->type == break语句)
	{
		p->place = gen_break();
	}
	else if (p->type == 类型标识符)
	{
		gtree *p1 = p->child;
		if (p->child->type_1 == 18)
		{
			p->place += 's';
			p1 = p1->next;
		}
		else if (p->child->type_1 == 19)
		{
			p->place += 'u';
			p1 = p1->next;
		}
		else
			p->place += 'n';

		while (p1)
		{
			if (p1->type_1 == 1)//int
			{
				p->place += '1';
			}
			else if (p1->type_1 == 7)//char
			{
				p->place += '2';
			}
			else if (p1->type_1 == 8)//float
			{
				p->place += '3';
			}
			else if (p1->type_1 == 9)//double
			{
				p->place += '4';
			}
			else if (p1->type_1 == 16)//long
			{
				p->place += '5';
			}
			else if (p1->type_1 == 17)//short
			{
				p->place += '6';
			}
			p1 = p1->next;
		}
	}
	else if (p->type == 函数定义)
	{
		p->place = p->child->child->next->place;
	}
	return;
}
void TraverseTree3(gtree *p)
{
	if (p == NULL)
		return;
	p->num = sort_num++;
	
	TraverseTree3(p->child);
	TraverseTree3(p->next);
}

bool isletter(char ch)
{
	if ((ch >= 'A'&&ch <= 'Z') || (ch >= 'a'&&ch <= 'z'))
		return 1;
	else
		return 0;
}

bool isnumber(char ch)
{
	if (ch >= '0'&&ch <= '9')
		return 1;
	else
		return 0;
}

void scaner()
{
	sum = 0;
	syn_1 = 0;
	for (m = 0; m < 20; m++)
		token[m] = NULL;

	ch = prog[p++];
	m = 0;

	while ((ch == ' ') || (ch == '\n') || (ch == '\t'))
		ch = prog[p++];

	if (isletter(ch) || ch == '_')
	{
		int n = 0;
		while (isletter(ch) || isnumber(ch) || ch == '_')
		{
			token[m++] = ch;
			ch = prog[p++];
			n++;
		}

		p--;

		syn = 标识符;
		syn_1 = 0;

		for (n = 0; n < keywordNum; n++)
			if (strcmp(token, rwtab[n]) == 0)
			{
				syn = 关键字;
				syn_1 = n + 1;
				break;
			}
	}
	else if (isnumber(ch))
	{
		bool flag = false;
		//cout << "num" << endl;
		int n = 0;
		while (isnumber(ch))
		{
			//sum = sum * 10 + ch - '0';
			token[m++] = ch;
			ch = prog[p++];

			n++;
		}
		if (ch == '.')
		{
			flag = true;
			token[m++] = ch;
			ch = prog[p++];
			n++;
			while (isnumber(ch))
			{
				//sum = sum * 10 + ch - '0';
				token[m++] = ch;
				ch = prog[p++];

				n++;
			}
		}
		p--;
		if (flag == false)
			syn = 无符号整数;
		else
			syn = 无符号实数;//正确性已保证，数字一律作为数值处理
	}
	else if (ch == '\'')
	{
		token[m++] = ch;
		ch = prog[p++];
		if (int(ch) == 32 || int(ch) == 33 || (int(ch) >= 35 && int(ch) <= 126))
		{
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '\'')
			{
				token[m++] = ch;
			}
			else
			{
				cout << "缺少右单引号！" << endl;
			}
		}
		syn = 字符;

	}
	else if (ch == '\"')
	{
		token[m++] = ch;
		ch = prog[p++];
		while (int(ch) == 32 || int(ch) == 33 || (int(ch) >= 35 && int(ch) <= 126))
		{
			token[m++] = ch;
			ch = prog[p++];

		}
		if (ch == '\"')
		{
			token[m++] = ch;
		}
		else
			cout << "缺少右双引号！" << endl;
		syn = 字符串;
	}
	else
	{
		switch (ch)
		{
		case '<':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn_1 = 1;
				token[m++] = ch;
			}
			else
			{
				syn_1 = 2;
				p--;
			}
			syn = 算符;
			break;

		case '>':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn_1 = 3;
				token[m++] = ch;
			}
			else
			{
				syn_1 = 4;
				p--;
			}
			syn = 算符;
			break;

		case '+':
			token[m++] = ch;
			//ch = prog[p++];
			//if (ch == '+')
			//{
			//	syn_1 = 5;
			//	token[m++] = ch;
			//}
			//else
			//{
			//	syn_1 = 6;
			//	p--;
			//}
			syn_1 = 5;
			syn = 算符;
			break;

		case '-':
			token[m++] = ch;
			//ch = prog[p++];
			//if (ch == '-')
			//{
			//	syn_1 = 7;
			//	token[m++] = ch;
			//}
			//else
			//{
			//	syn_1 = 8;
			//	p--;
			//}
			syn_1 = 6;
			syn = 算符;
			break;

		case '!':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn_1 = 7;
				token[m++] = ch;
				syn = 算符;
			}
			else
			{
				syn = 算符;
				syn_1 = 13;
				p--;
			}

			break;

		case '=':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn_1 = 8;
				token[m++] = ch;
				syn = 算符;
			}
			else
			{
				syn = 赋值号;
				p--;
			}
			break;

		case '*':
			syn = 算符;
			syn_1 = 9;
			token[m++] = ch;
			break;

		case '&':
			syn = 算符;
			syn_1 = 11;
			token[m++] = ch;
			break;

		case '|':
			syn = 算符;
			syn_1 = 12;
			token[m++] = ch;
			break;

		case '/':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '/')
			{
				token[m++] = ch;
				syn = 注释号;
				while (1)
				{
					ch = prog[p++];
					if (ch == '\n')
						break;

				}
			}
			else if (ch == '*')
			{
				token[m++] = ch;
				while (prog[p] != '*')
				{
					token[m++] = prog[p];
					p++;
				}
				token[m++] = prog[p];
				if (prog[++p] == '/')
				{
					token[m++] = prog[p];
					syn = 注释号;
					p++;
				}
			}
			else
			{
				syn = 算符;
				syn_1 = 10;
				p--;
				//token[m++] = ch;
			}
			break;
		case '^':
			syn = 算符;
			syn_1 = 14;
			token[m++] = ch;
			break;
		case '(':
			syn = 左括号;
			token[m++] = ch;
			break;

		case ')':
			syn = 右括号;
			token[m++] = ch;
			break;

		case '[':
			syn = 左中括号;
			token[m++] = ch;
			break;

		case ']':
			syn = 右中括号;
			token[m++] = ch;
			break;

		case '{':
			syn = 左大括号;
			token[m++] = ch;
			break;

		case '}':
			syn = 右大括号;
			token[m++] = ch;
			break;

		case ';':
			syn = 界符;
			token[m++] = ch;
			break;

		case ':':
			syn = 冒号;
			token[m++] = ch;
			break;

		case ',':
			syn = 分隔符;
			token[m++] = ch;
			break;

		case '#':
			syn = 结束符;
			token[m++] = ch;
			break;


		default:
			syn = -1;
			break;
		}
	}
	token[m++] = '\0';
}

bool judge_yuju()
{
	if (store[read_num][0] == 标识符 || store[read_num][0] == 字母 || (store[read_num][0] == 关键字 && (store[read_num][1] == 3 || store[read_num][1] == 5 || store[read_num][1] == 6)))
		return 1;
	else
		return 0;
}

bool is_int()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 1)
		return 1;
	else
		return 0;
}

bool is_long()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 16)
		return 1;
	else
		return 0;
}

bool is_short()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 17
		)
		return 1;
	else
		return 0;
}

bool is_signed1()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 18)
		return 1;
	else
		return 0;
}

bool is_unsigned1()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 19)
		return 1;
	else
		return 0;
}

bool is_void1()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 2)
		return 1;
	else
		return 0;
}

bool is_if()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 3)
		return 1;
	else
		return 0;
}

bool is_else()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 4)
		return 1;
	else
		return 0;
}

bool is_while()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 5)
		return 1;
	else
		return 0;
}

bool is_switch()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 11)//11是switch
		return 1;
	else
		return 0;
}

bool is_case()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 12)//12是case
		return 1;
	else
		return 0;
}

bool is_return()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 6)
		return 1;
	else
		return 0;
}

bool is_break()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 20)
		return 1;
	else
		return 0;
}

bool is_char()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 7)
		return 1;
	else
		return 0;
}

bool is_float()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 8)
		return 1;
	else
		return 0;
}

bool is_double()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 9)
		return 1;
	else
		return 0;
}

bool is_const1()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 10)
		return 1;
	else
		return 0;
}

bool is_scanf()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 13)
		return 1;
	else
		return 0;
}

bool is_printf()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 14)
		return 1;
	else
		return 0;
}

bool is_default()
{
	if (store[read_num][0] == 关键字 && store[read_num][1] == 15)
		return 1;
	else
		return 0;
}

bool is_relop()
{
	if (store[read_num][0] == 算符 && ((store[read_num][1] >= 1 && store[read_num][1] <= 4) || (store[read_num][1] == 7 || store[read_num][1] == 8)))
		return 1;
	else
		return 0;
}

void trans_print(int num, int n, bool flag)//终结成分为false,非终结成分为true
{
	//int tag;
	if (flag == false)
	{
		for (int i = 0; i <= n; i++)
			cout << '	';
		cout << store_char[read_num - 1] << endl;
	}
	else
	{
		for (int i = 0; i <= n - 1; i++)
			cout << '	';
		//cout << num << endl;
		switch (num)
		{
		case 21:
			cout << "<程序>" << endl;
			break;
		case 22:
			cout << "<常量说明>" << endl;
			break;
		case 23:
			cout << "<常量定义>" << endl;
			break;
		case 24:
			cout << "<整数>" << endl;
			break;
		case 25:
			cout << "<实数>" << endl;
			break;
		case 26:
			cout << "<声明头部>" << endl;
			break;
		case 27:
			cout << "<变量说明>" << endl;
			break;
		case 28:
			cout << "<变量定义>" << endl;
			break;
		case 29:
			cout << "<可枚举常量>" << endl;
			break;
		case 30:
			cout << "<类型标识符>" << endl;
			break;
		case 31:
			cout << "<函数定义>" << endl;
			break;
		case 32:
			cout << "<复合语句>" << endl;
			break;
		case 33:
			cout << "<参数表>" << endl;
			break;
		case 34:
			cout << "<主函数>" << endl;
			break;
		case 35:
			cout << "<表达式>" << endl;
			break;
		case 36:
			cout << "<项>" << endl;
			break;
		case 37:
			cout << "<因子>" << endl;
			break;
		case 38:
			cout << "<语句>" << endl;
			break;
		case 39:
			cout << "<赋值语句>" << endl;
			break;
		case 40:
			cout << "<条件语句>" << endl;
			break;
		case 41:
			cout << "<条件>" << endl;
			break;
		case 42:
			cout << "<循环语句>" << endl;
			break;
		case 43:
			cout << "<情况语句>" << endl;
			break;
		case 44:
			cout << "<情况表>" << endl;
			break;
		case 45:
			cout << "<情况子语句>" << endl;
			break;
		case 46:
			cout << "<缺省>" << endl;
			break;
		case 47:
			cout << "<函数调用语句>" << endl;
			break;
		case 48:
			cout << "<值参数表>" << endl;
			break;
		case 49:
			cout << "<语句列>" << endl;
			break;
		case 50:
			cout << "<读语句>" << endl;
			break;
		case 51:
			cout << "<写语句>" << endl;
			break;
		case 52:
			cout << "<返回语句>" << endl;
			break;
		case 53:
			cout << "<条件组>" << endl;
			break;
		case 54:
			cout << "<子项>" << endl;
			break;
		case 55:
			cout << "<break语句>" << endl;
			break;
		}
	}
}

int P(int num, int n, gtree *tree1)
{
	trans_print(num, n, true);
	//	gtree *root_tree = tree1;
	if (num == 程序)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		bool flag = false;
		if (is_const1())//keywordNum=10表示const
		{

			tree1->type = 常量说明;
			tree1->type_1 = 0;
			flag = true;
			//			cout << "program" << endl;
			if (P(常量说明, n + 1, tree1) == ERROR1)
				return ERROR1;

		}
		if ((is_int() || is_char() || is_float() || is_double()) && store[read_num + 2][0] != 左括号)//keywordNum=1.7.8.9分别对应int char float double
		{
			if (flag == true)
			{
				tree1->cnode(变量说明, 0, tree1);
				tree1 = tree1->next;
			}
			else
			{
				tree1->type = 变量说明;
				tree1->type_1 = 0;
				flag = true;
			}
			if (P(变量说明, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		if (is_int() || is_void1() || is_char() || is_float() || is_double())
		{
			if (flag == true)
			{
				tree1->cnode(函数定义, 0, tree1);
				tree1 = tree1->next;
			}
			else
			{
				tree1->type = 函数定义;
				tree1->type_1 = 0;
				flag = true;
			}
			if (P(函数定义, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		while (is_int() || is_void1() || is_char() || is_float() || is_double())//keywordNum=1.2.7.8.9分别对应int void char float double
		{
			tree1->cnode(函数定义, 0, tree1);
			tree1 = tree1->next;
			if (P(函数定义, n + 1, tree1) == ERROR1)
				return ERROR1;
		}


	}
	else if (num == 常量说明)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (is_const1())//keywordNum=10表示const
		{
			read_num++;
			trans_print(num, n, false);
			tree1 = tree1->next;
			if (P(常量定义, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == 界符)
			{
				read_num++;
				trans_print(num, n, false);
				while (is_const1())//keywordNum=10表示const
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(关键字, 10, tree1);
					tree1 = tree1->next;
					tree1->cnode(常量定义, 0, tree1);
					tree1 = tree1->next;
					if (P(常量定义, n + 1, tree1) == ERROR1)
						return ERROR1;
					tree1->cnode(界符, 0, tree1);
					if (store[read_num][0] == 界符)
					{
						read_num++;
						trans_print(num, n, false);
					}
					else
					{
						cout << "常量说明缺少界符!" << endl;
						return ERROR1;
					}
				}
			}
			else
			{
				cout << "常量说明缺少界符!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "常量说明缺少const!" << endl;
			return ERROR1;
		}

	}
	else if (num == 常量定义)
	{

		if (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1())//keywordNum=1.7.8.9分别对应int char float double
		{
			int temp_syn1;
			if (is_signed1() || is_unsigned1())
				temp_syn1 = store[read_num + 1][1];
			else
				temp_syn1 = store[read_num][1];//用于区分初始化赋值的类型
			if (P(类型标识符, n + 1, tree1) == ERROR1)
				return ERROR1;
			sum = temp_syn1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1 = tree1->next;
			if (store[read_num][0] == 标识符)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->place = store_char[read_num - 1];
				tree1 = tree1->next;
				if (store[read_num][0] == 赋值号)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (temp_syn1 == 1)
					{
						if (P(整数, n + 1, tree1) == ERROR1)
							return ERROR1;
					}
					else if (temp_syn1 == 7)
					{
						if (store[read_num][0] == 字符)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->place = store_char[read_num - 1];
						}
						else
						{
							cout << "常量定义缺少字符!" << endl;
							return ERROR1;
						}
					}
					else if (temp_syn1 == 8 || temp_syn1 == 9)
					{
						if (P(实数, n + 1, tree1) == ERROR1)
							return ERROR1;
					}
					while (store[read_num][0] == 分隔符)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(分隔符, 0, tree1);
						tree1 = tree1->next;

						if (store[read_num][0] == 标识符)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->cnode(标识符, 0, tree1);
							tree1 = tree1->next;
							tree1->place = store_char[read_num - 1];

							if (store[read_num][0] == 赋值号)
							{

								read_num++;
								trans_print(num, n, false);
								tree1->cnode(赋值号, 0, tree1);
								tree1 = tree1->next;
								if (temp_syn1 == 1)
								{
									tree1->cnode(整数, 0, tree1);
									tree1 = tree1->next;
									if (P(整数, n + 1, tree1) == ERROR1)
										return ERROR1;
								}
								else if (temp_syn1 == 7)
								{
									tree1->cnode(字符, 0, tree1);
									tree1 = tree1->next;
									if (store[read_num][0] == 字符)
									{
										read_num++;
										trans_print(num, n, false);
										tree1->place = store_char[read_num - 1];
									}
									else
									{
										cout << "常量定义缺少字符!" << endl;
										return ERROR1;
									}
								}
								else if (temp_syn1 == 8 || temp_syn1 == 9)
								{
									tree1->cnode(实数, 0, tree1);
									tree1 = tree1->next;
									if (P(实数, n + 1, tree1) == ERROR1)
										return ERROR1;
								}
							}
							else
							{
								cout << "常量定义缺少赋值号!" << endl;
								return ERROR1;
							}
						}
						else
						{
							cout << "常量定义缺少标识符!" << endl;
							return ERROR1;
						}
					}
				}
				else
				{
					cout << "常量定义缺少赋值号!" << endl;
					return ERROR1;
				}
			}
			else
			{
				cout << "常量定义缺少标识符!" << endl;
				return ERROR1;
			}
		}
	}
	else if (num == 整数)
	{
		int flag = 0;
		if (store[read_num][0] == 算符 && (store[read_num][1] == 5 || store[read_num][1] == 6))//5.6代表+ -号
		{
			read_num++;
			trans_print(num, n, false);
			read_num++;	
			trans_print(num, n, false);
			flag = store[read_num - 2][1];
			sum = flag;
			tree1->expand(num, tree1, sum);
			tree1->child->next->place = store_char[read_num - 1];
		}
		else if (store[read_num][0] == 无符号整数)
		{
			read_num++;
			trans_print(num, n, false);
			sum = flag;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1->place = store_char[read_num - 1];
		}
		else
		{
			cout << "整数缺少无符号整数!" << endl;
			return ERROR1;
		}
	}
	else if (num == 实数)
	{
		int flag = 0;
		if (store[read_num][0] == 算符 && (store[read_num][1] == 5 || store[read_num][1] == 6))//5.6代表+ -号
		{
			read_num++;
			trans_print(num, n, false);
			flag = store[read_num][1];
		}
		if (store[read_num][0] == 无符号实数)
		{
			read_num++;
			trans_print(num, n, false);
			sum = flag;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1->place = store_char[read_num - 1];
		}
		else
		{
			cout << "实数缺少无符号实数!" << endl;
			return ERROR1;
		}
	}
	else if (num == 声明头部)
	{
		if (is_int() || is_void1() || is_char() || is_float() || is_double() || is_short() || is_signed1() || is_unsigned1())//keywordNum=1.2.7.8.9分别对应int void char float double
		{
			int temp_syn1;
			if (is_signed1() || is_unsigned1())
				temp_syn1 = store[read_num + 1][1];
			else
				temp_syn1 = store[read_num][1];//用于区分初始化赋值的类型

			sum = temp_syn1;//保存对应的int void char float double
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (temp_syn1 != 2)//void单独考虑
			{
				if (P(类型标识符, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
			else
			{
				read_num++;
				trans_print(num, n, false);
			}
			if (store[read_num][0] == 标识符)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->next->place = store_char[read_num - 1];
			}
			else
			{
				cout << "声明头部缺少标识符!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "声明头部缺少声明关键字!" << endl;
			return ERROR1;
		}
	}
	else if (num == 变量说明)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (store[read_num + 2][0] == 左括号)//区分函数和变量
		{

		}
		else
		{
			if (P(变量定义, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == 界符)
			{
				read_num++;
				trans_print(num, n, false);
				while ((is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1()) && store[read_num + 2][0] != 左括号)//keywordNum=1.7.8.9分别对应int char float double
				{
					tree1->cnode(变量定义, 0, tree1);
					tree1 = tree1->next;
					if (P(变量定义, n + 1, tree1) == ERROR1)
						return ERROR1;
					tree1->cnode(界符, 0, tree1);
					tree1 = tree1->next;
					if (store[read_num][0] == 界符)
					{
						read_num++;
						trans_print(num, n, false);
					}
					else
					{
						cout << "变量说明缺少界符!" << endl;
						return ERROR1;
					}
				}
			}
			else
			{
				cout << "变量说明缺少界符!" << endl;
				return ERROR1;
			}
		}
	}
	else if (num == 变量定义)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (store[read_num][0] == 关键字 && (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1()))//keywordNum=1.7.8.9分别对应int char float double
		{
			int temp_syn1;
			if (is_signed1() || is_unsigned1())
				temp_syn1 = store[read_num + 1][1];
			else
				temp_syn1 = store[read_num][1];//用于区分初始化赋值的类型
			if (store[read_num + 2][0] == 左中括号)//+2是因为类型标识符里有一个
			{

			}
			if (P(类型标识符, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == 标识符)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->place = store_char[read_num - 1];
				if (store[read_num][0] == 左中括号)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(左中括号, 0, tree1);
					tree1 = tree1->next;


					if (store[read_num][0] == 无符号整数)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(无符号整数, 0, tree1);
						tree1 = tree1->next;
						tree1->place = store_char[read_num - 1];
						if (store[read_num][0] == 右中括号)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->cnode(右中括号, 0, tree1);
							tree1 = tree1->next;
						}
						else
						{
							cout << "变量定义缺少右中括号!" << endl;
							return ERROR1;
						}
					}
					else
					{
						cout << "变量定义缺少无符号整数!" << endl;
						return ERROR1;
					}
				}
				if (store[read_num][0] == 赋值号)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(赋值号, 0, tree1);
					tree1 = tree1->next;
					if (store[read_num][0] == 左大括号)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(左大括号, 0, tree1);
						tree1 = tree1->next;
						if (store[read_num][0] == 右大括号)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->cnode(左大括号, 0, tree1);
							tree1 = tree1->next;
						}
						else
						{
							if (temp_syn1 == 1)
							{
								tree1->cnode(整数, 0, tree1);
								tree1 = tree1->next;
								if (P(整数, n + 1, tree1) == ERROR1)
									return ERROR1;
								while (store[read_num][0] == 分隔符)
								{
									read_num++;
									trans_print(num, n, false);
									tree1->cnode(分隔符, 0, tree1);
									tree1 = tree1->next;
									tree1->cnode(整数, 0, tree1);
									tree1 = tree1->next;
									if (P(整数, n + 1, tree1) == ERROR1)
										return ERROR1;
								}
								if (store[read_num][0] == 右大括号)
								{

								}
							}
							else if (temp_syn1 == 7)
							{
								tree1->cnode(实数, 0, tree1);
								tree1 = tree1->next;
								if (P(实数, n + 1, tree1) == ERROR1)
									return ERROR1;
							}
							else if (temp_syn1 == 8)
							{

							}
						}

					}
					else
					{
						if (temp_syn1 == 1)
						{
							tree1->cnode(整数, 0, tree1);
							tree1 = tree1->next;
							if (P(整数, n + 1, tree1) == ERROR1)
								return ERROR1;
						}
						else if (temp_syn1 == 7)
						{
							tree1->cnode(字符, 0, tree1);
							tree1 = tree1->next;
							if (store[read_num][0] == 字符)
							{
								read_num++;
								trans_print(num, n, false);
								tree1->place = store_char[read_num - 1];
							}
							else
							{
								cout << "变量定义缺少字符!" << endl;
								return ERROR1;
							}
						}
						else if (temp_syn1 == 8 || temp_syn1 == 9)
						{
							tree1->cnode(实数, 0, tree1);
							tree1 = tree1->next;
							if (P(实数, n + 1, tree1) == ERROR1)
								return ERROR1;
						}
					}

				}
			}
			else
			{
				cout << "变量定义缺少标识符!" << endl;
				return ERROR1;
			}
			while (store[read_num][0] == 分隔符)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->cnode(分隔符, 0, tree1);
				tree1 = tree1->next;
				if (store[read_num][0] == 标识符)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(标识符, 0, tree1);
					tree1 = tree1->next;
					tree1->place = store_char[read_num - 1];
					if (store[read_num][0] == 左中括号)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(左中括号, 0, tree1);
						tree1 = tree1->next;

						if (store[read_num][0] == 无符号整数)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->cnode(无符号整数, 0, tree1);
							tree1 = tree1->next;
							tree1->place = store_char[read_num - 1];

							if (store[read_num][0] == 右中括号)
							{
								read_num++;
								trans_print(num, n, false);
								tree1->cnode(右中括号, 0, tree1);
								tree1 = tree1->next;
							}
							else
							{
								cout << "变量定义缺少右中括号!" << endl;
								return ERROR1;
							}
						}
						else
						{
							cout << "变量定义缺少无符号整数!" << endl;
							return ERROR1;
						}
					}
					if (store[read_num][0] == 赋值号)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(赋值号, 0, tree1);
						tree1 = tree1->next;
						if (temp_syn1 == 1)
						{
							tree1->cnode(整数, 0, tree1);
							tree1 = tree1->next;
							if (P(整数, n + 1, tree1) == ERROR1)
								return ERROR1;
						}
						else if (temp_syn1 == 7)
						{
							tree1->cnode(字符, 0, tree1);
							tree1 = tree1->next;
							if (store[read_num][0] == 字符)
							{
								read_num++;
								trans_print(num, n, false);
								tree1->place = store_char[read_num - 1];
							}
							else
							{
								cout << "变量定义缺少字符!" << endl;
								return ERROR1;
							}
						}
						else if (temp_syn1 == 8 || temp_syn1 == 9)
						{
							tree1->cnode(实数, 0, tree1);
							tree1 = tree1->next;
							if (P(实数, n + 1, tree1) == ERROR1)
								return ERROR1;
						}

					}
				}
				else
				{
					cout << "变量定义缺少标识符!" << endl;
					return ERROR1;
				}
			}
		}
	}
	else if (num == 可枚举常量)
	{
		if ((store[read_num][0] == 算符 && (store[read_num][1] == 5 || store[read_num][1] == 6)) || store[read_num][0] == 无符号整数)//5 6代表+ -
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(整数, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		else if (store[read_num][0] == 字符)
		{
			sum = 2;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			read_num++;
			trans_print(num, n, false);
			tree1->place = store_char[read_num - 1];

		}
		else
		{
			cout << "可枚举常量错误!" << endl;
			return ERROR1;
		}
	}
	else if (num == 类型标识符)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		bool flag = false;
		int temp_syn1;
		if (is_signed1() || is_unsigned1())//是否有unsigned
		{
			read_num++;
			trans_print(num, n, false);
			temp_syn1 = store[read_num - 1][1];
			tree1->type = 关键字;
			tree1->type_1 = temp_syn1;
			flag = true;
		}
		if (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1())
		{
			read_num++;
			trans_print(num, n, false);
			int temp_syn11 = store[read_num - 1][1];
			if (flag == true)
			{
				tree1->cnode(关键字, temp_syn11, tree1);
				tree1 = tree1->next;
			}
			else
			{
				tree1->type = 关键字;
				tree1->type_1 = temp_syn11;
			}
		}
		if (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1())
		{
			read_num++;
			trans_print(num, n, false);
			int temp_syn11 = store[read_num - 1][1];
			tree1->cnode(关键字, temp_syn11, tree1);
			tree1 = tree1->next;

		}
	}
	else if (num == 函数定义)
	{
		if (is_int() || is_void1() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1())//keywordNum=1.2.7.8.9分别对应int void char float double
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(声明头部, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == 左括号)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				if (P(参数表, n + 1, tree1) == ERROR1)
					return ERROR1;
				tree1 = tree1->next;
				if (store[read_num][0] == 右括号)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (store[read_num][0] == 左大括号)
					{
						read_num++;
						trans_print(num, n, false);
						string fun = gen_fun();
						tree1->place = fun;
						tree1 = tree1->next;
						if (P(复合语句, n + 1, tree1) == ERROR1)
							return ERROR1;
						tree1 = tree1->next;
						if (store[read_num][0] == 右大括号)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->place = fun;
						}
						else
						{
							cout << "函数定义缺少右大括号!" << endl;
							return ERROR1;
						}
					}
					else
					{
						cout << "函数定义缺少左大括号!" << endl;
						return ERROR1;
					}
				}
				else
				{
					cout << "函数定义缺少右括号!" << endl;
					return ERROR1;
				}
			}
			else
			{
				cout << "函数定义缺少左括号!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "函数定义错误!" << endl;
			return ERROR1;
		}
	}
	else if (num == 复合语句)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		bool flag = false;
		if (store[read_num][0] == 关键字 && store[read_num][1] == 10)//keywordNum=10代表const
		{
			tree1->type = 常量说明;
			tree1->type_1 = 0;
			flag = true;
			if (P(常量说明, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		if (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1())//keywordNum=1.7.8.9分别对应int char float double
		{
			if (flag == true)
			{
				tree1->cnode(变量说明, 0, tree1);
				tree1 = tree1->next;
			}
			else
			{
				tree1->type = 变量说明;
				tree1->type_1 = 0;
				flag = true;
			}
			if (P(变量说明, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		if (flag == true)
		{
			tree1->cnode(语句列, 0, tree1);
			tree1 = tree1->next;
		}
		else
		{
			tree1->type = 语句列;
			tree1->type_1 = 0;
			flag = true;
		}
		if (P(语句列, n + 1, tree1) == ERROR1)
			return ERROR1;
	}
	else if (num == 参数表)
	{
		if (store[read_num][0] == 右括号)//表示空
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			return 0;
		}
		sum = 1;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(类型标识符, n + 1, tree1) == ERROR1)
			return ERROR1;
		tree1 = tree1->next;
		if (store[read_num][0] == 标识符)
		{
			read_num++;
			trans_print(num, n, false);
			tree1->place = store_char[read_num - 1];
			if (store[read_num][0] == 左中括号)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->cnode(左中括号, 0, tree1);
				tree1 = tree1->next;
				if (store[read_num][0] == 右中括号)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(右中括号, 0, tree1);
					tree1 = tree1->next;
				}
				else
				{
					tree1->cnode(表达式, 0, tree1);
					tree1 = tree1->next;
					if (P(表达式, n + 1, tree1) == ERROR1)
						return ERROR1;
					if (store[read_num][0] == 右中括号)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(右中括号, 0, tree1);
						tree1 = tree1->next;
					}
					else
					{
						cout << "参数表缺少右中括号!" << endl;
						return ERROR1;
					}
				}

			}
			while (store[read_num][0] == 分隔符)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->cnode(分隔符, 0, tree1);
				tree1 = tree1->next;
				tree1->cnode(类型标识符, 0, tree1);
				tree1 = tree1->next;
				if (P(类型标识符, n + 1, tree1) == ERROR1)
					return ERROR1;
				if (store[read_num][0] == 标识符)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(标识符, 0, tree1);
					tree1 = tree1->next;
					tree1->place = store_char[read_num - 1];
					if (store[read_num][0] == 左中括号)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(左中括号, 0, tree1);
						tree1 = tree1->next;
						if (store[read_num][0] == 右中括号)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->cnode(右中括号, 0, tree1);
							tree1 = tree1->next;
						}
						else
						{
							tree1->cnode(表达式, 0, tree1);
							tree1 = tree1->next;
							if (P(表达式, n + 1, tree1) == ERROR1)
								return ERROR1;
							if (store[read_num][0] == 右中括号)
							{
								read_num++;
								trans_print(num, n, false);
								tree1->cnode(右中括号, 0, tree1);
								tree1 = tree1->next;
							}
							else
							{
								cout << "参数表缺少右中括号!" << endl;
								return ERROR1;
							}
						}
					}
				}
				else
				{
					cout << "参数表缺少标识符!" << endl;
					return ERROR1;
				}
			}
		}
		else
		{
			cout << "参数表缺少标识符!" << endl;
			return ERROR1;
		}
	}
	else if (num == 表达式)
	{
		int flag = 0;
		if (store[read_num][0] == 算符 && (store[read_num][1] == 5 || store[read_num][1] == 6)
			&& (store[read_num + 1][0] != 无符号整数 && store[read_num + 1][0] != 无符号实数))//5.6代表+ -
		{
			read_num++;
			trans_print(num, n, false);
			flag = store[read_num - 1][1];
		}
		sum = flag;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (flag == 5 || flag == 6)
			tree1 = tree1->next;
		if (P(项, n + 1, tree1) == ERROR1)
			return ERROR1;
		while (store[read_num][0] == 算符 && (store[read_num][1] == 5 || store[read_num][1] == 6))//5.6代表+ -
		{
			read_num++;
			trans_print(num, n, false);
			tree1->cnode(算符, store[read_num - 1][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(项, 0, tree1);
			tree1 = tree1->next;
			if (P(项, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
	}
	else if (num == 项)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(子项, n + 1, tree1) == ERROR1)
			return ERROR1;
		while (store[read_num][0] == 算符 && (store[read_num][1] == 9 || store[read_num][1] == 10))//9 10代表 * /
		{
			read_num++;
			trans_print(num, n, false);
			tree1->cnode(算符, store[read_num - 1][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(子项, 0, tree1);
			tree1 = tree1->next;
			if (P(子项, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
	}
	else if (num == 子项)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(因子, n + 1, tree1) == ERROR1)
			return ERROR1;
		while (store[read_num][0] == 算符 && (store[read_num][1] == 11 || store[read_num][1] == 12) && (store[read_num + 1][1] != 11 && store[read_num + 1][1] != 12))//11 12代表 & |
		{
			read_num++;
			trans_print(num, n, false);
			tree1->cnode(算符, store[read_num - 1][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(因子, 0, tree1);
			tree1 = tree1->next;
			if (P(因子, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
	}
	else if (num == 因子)
	{
		if (store[read_num][0] == 标识符)
		{
			if (store[read_num + 1][0] == 左括号)//这里提前判断是否函数调用，在read_num++之前
			{
				//temp_place = tree1->child->place;
				sum = 7;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				if (P(函数调用语句, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
			else
			{
				read_num++;
				trans_print(num, n, false);
				sum = 1;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				tree1->place = store_char[read_num - 1];
				//temp_place += tree1->place;
				if (store[read_num][0] == 左中括号)
				{
					//temp_place += '[';
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(左中括号, 0, tree1);
					tree1 = tree1->next;
					tree1->cnode(表达式, 0, tree1);
					tree1 = tree1->next;
					if (P(表达式, n + 1, tree1) == ERROR1)
						return ERROR1;
					tree1->cnode(右中括号, 0, tree1);
					tree1 = tree1->next;
					if (store[read_num][0] == 右中括号)
					{
						read_num++;
						trans_print(num, n, false);
					}
					else
					{
						cout << "因子缺少右中括号!" << endl;
						return ERROR1;
					}
				}
			}

		}
		else if (store[read_num][0] == 左括号)
		{
			read_num++;
			trans_print(num, n, false);
			sum = 3;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1 = tree1->next;
			if (P(表达式, n + 1, tree1) == ERROR1)
				return ERROR1;

			if (store[read_num][0] == 右括号)
			{

				read_num++;
				trans_print(num, n, false);
			}
			else
			{
				cout << "因子缺少右括号!" << endl;
				return ERROR1;
			}
		}
		else if (store[read_num][0] == 无符号整数 || (store[read_num][0] == 算符 && store[read_num][1] == 6 && store[read_num + 1][0] == 无符号整数))//6代表-号
		{
			//read_num++;
			//trans_print(num, n, false);
			sum = 4;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(整数, n + 1, tree1) == ERROR1)
				return ERROR1;
			//tree1->place = store_char[read_num - 1];
		}
		else if (store[read_num][0] == 无符号实数 || (store[read_num][0] == 算符 && store[read_num][1] == 6 && store[read_num + 1][0] == 无符号实数))
		{
			//read_num++;
			//trans_print(num, n, false);
			sum = 5;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(实数, n + 1, tree1) == ERROR1)
				return ERROR1;
			//tree1->place = store_char[read_num - 1];
		}
		else if (store[read_num][0] == 字符)
		{
			read_num++;
			trans_print(num, n, false);
			sum = 6;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1->place = store_char[read_num - 1];
		}
		else
		{
			cout << "因子错误!" << endl;
			return ERROR1;
		}
	}
	else if (num == 语句)
	{
		if (is_if())
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(条件语句, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		else if (is_while())
		{
			sum = 2;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(循环语句, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		else if (is_break())
		{
			sum = 12;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(break语句, n + 1, tree1) == ERROR1)
				return ERROR1;
			if (store[read_num][0] == 界符)
			{
				read_num++;
				trans_print(num, n, false);
			}
			else
			{
				cout << "语句缺少界符!" << endl;
				return ERROR1;
			}
		}
		else if (store[read_num][0] == 左大括号)
		{
			read_num++;
			trans_print(num, n, false);
			sum = 3;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			string fun = gen_fun();
			tree1->place = fun;
			tree1 = tree1->next;
			if (P(语句列, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == 右大括号)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->place = fun;
			}
			else
			{
				cout << "语句缺少右大括号!" << endl;
				return ERROR1;
			}
		}
		else if (store[read_num][0] == 标识符)
		{
			if (store[read_num + 1][0] == 左括号)
			{
				sum = 4;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				if (P(函数调用语句, n + 1, tree1) == ERROR1)
					return ERROR1;
				if (store[read_num][0] == 界符)
				{
					read_num++;
					trans_print(num, n, false);
				}
				else
				{
					cout << "语句缺少界符!" << endl;
					return ERROR1;
				}
			}
			else if (store[read_num + 1][0] == 赋值号 || store[read_num + 1][0] == 左中括号)
			{
				sum = 5;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				if (P(赋值语句, n + 1, tree1) == ERROR1)
					return ERROR1;
				if (store[read_num][0] == 界符)
				{
					read_num++;
					trans_print(num, n, false);
				}
				else
				{
					cout << "语句缺少界符!" << endl;
					return ERROR1;
				}
			}
			else
			{
				sum = 11;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				if (P(表达式, n + 1, tree1) == ERROR1)
					return ERROR1;
				if (store[read_num][0] == 界符)
				{
					read_num++;
					trans_print(num, n, false);
				}
				else
				{
					cout << "语句缺少界符!" << endl;
					return ERROR1;
				}
			}
		}
		else if (store[read_num][0] == 界符)
		{
			sum = 8;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			read_num++;
			trans_print(num, n, false);
		}
		else if (is_scanf())
		{
			sum = 6;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(读语句, n + 1, tree1) == ERROR1)
				return ERROR1;
			if (store[read_num][0] == 界符)
			{
				read_num++;
				trans_print(num, n, false);
			}
			else
			{
				cout << "语句缺少界符!" << endl;
				return ERROR1;
			}
		}
		else if (is_printf())
		{
			sum = 7;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(写语句, n + 1, tree1) == ERROR1)
				return ERROR1;
			if (store[read_num][0] == 界符)
			{
				read_num++;
				trans_print(num, n, false);
			}
			else
			{
				cout << "语句缺少界符!" << endl;
				return ERROR1;
			}
		}
		else if (is_switch())
		{
			sum = 9;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(情况语句, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		else if (is_return())
		{
			sum = 10;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(返回语句, n + 1, tree1) == ERROR1)
				return ERROR1;
			if (store[read_num][0] == 界符)
			{
				read_num++;
				trans_print(num, n, false);
			}
			else
			{
				cout << "语句缺少界符!" << endl;
				return ERROR1;
			}
		}
	}
	else if (num == 赋值语句)
	{
		if (store[read_num][0] == 标识符)
		{
			read_num++;
			trans_print(num, n, false);
			if (store[read_num][0] == 左中括号)
			{
				read_num++;
				trans_print(num, n, false);
				sum = 2;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				tree1->place = store_char[read_num - 2];
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(表达式, n + 1, tree1) == ERROR1)
					return ERROR1;
				tree1 = tree1->next;
				if (store[read_num][0] == 右中括号)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (store[read_num][0] == 赋值号)
					{
						read_num++;
						trans_print(num, n, false);
						tree1 = tree1->next;
						if (P(表达式, n + 1, tree1) == ERROR1)
							return ERROR1;
					}
					else
					{
						cout << "赋值语句缺少赋值号!" << endl;
						return ERROR1;
					}
				}
				else
				{
					cout << "赋值语句缺少右中括号!" << endl;
					return ERROR1;
				}
			}
			else if (store[read_num][0] == 赋值号)
			{
				read_num++;
				trans_print(num, n, false);
				sum = 1;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				tree1->place = store_char[read_num - 2];
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(表达式, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
		}
		else
		{
			cout << "赋值语句缺少标识符!" << endl;
			return ERROR1;
		}
	}
	else if (num == 条件语句)
	{
		if (is_if())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (store[read_num][0] == 左括号)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(条件组, n + 1, tree1) == ERROR1)
					return ERROR1;
				tree1 = tree1->next;
				if (store[read_num][0] == 右括号)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (P(语句, n + 1, tree1) == ERROR1)
						return ERROR1;
					if (is_else())
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(关键字, 4, tree1);
						tree1 = tree1->next;
						tree1->cnode(语句, 0, tree1);
						tree1 = tree1->next;
						if (P(语句, n + 1, tree1) == ERROR1)
							return ERROR1;
					}
				}
				else
				{
					cout << "条件语句缺少右括号!" << endl;
					return ERROR1;
				}
			}
		}
		else
		{
			cout << "条件语句缺少if!" << endl;
			return ERROR1;
		}
	}
	else if (num == 条件组)
	{
		int left = 0;
		int count = 0;
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (store[read_num][0] == 左括号)
		{
			read_num++;
			trans_print(num, n, false);
			tree1->type = 左括号;
			tree1->type_1 = 0;
			count++;
		}
		if (store[read_num][0] == 算符 && store[read_num][1] == 13)//代表！符号
		{
			read_num++;
			trans_print(num, n, false);
			if (count == 0)
			{
				tree1->type = 算符;
				tree1->type_1 = 13;
			}
			else
			{
				tree1->cnode(算符, 13, tree1);
				tree1 = tree1->next;
			}
		}
		if (count == 0)
		{
			tree1->type = 条件;
			tree1->type_1 = 0;
		}
		else
		{
			tree1->cnode(条件, 0, tree1);
			tree1 = tree1->next;
		}
		if (P(条件, n + 1, tree1) == ERROR1)
			return ERROR1;
		while (store[read_num][0] == 算符 && (store[read_num][1] == 11 || store[read_num][1] == 12) || (left > 0 && store[read_num][0] == 右括号))//11 12代表 & |
		{
			if (left > 0 && store[read_num][0] == 右括号)
			{
				read_num++;
				trans_print(num, n, false);
				left--;
				tree1->cnode(右括号, 0, tree1);
				tree1 = tree1->next;
				if (store[read_num][0] == 右括号)
					break;
			}
			read_num++;
			read_num++;//因为&&和||都有两个
			trans_print(num, n, false);
			trans_print(num, n, false);
			tree1->cnode(算符, store[read_num - 2][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(算符, store[read_num - 2][1], tree1);
			tree1 = tree1->next;

			if (store[read_num][0] == 算符 && store[read_num][1] == 13)
			{
				read_num++;
				trans_print(num, n, false);

				tree1->cnode(算符, 13, tree1);
				tree1 = tree1->next;
			}
			if (store[read_num][0] == 左括号)
			{
				read_num++;
				trans_print(num, n, false);
				left++;

				tree1->cnode(左括号, 0, tree1);
				tree1 = tree1->next;
			}

			tree1->cnode(条件, 0, tree1);
			tree1 = tree1->next;
			if (P(条件, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
	}
	else if (num == 条件)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(表达式, n + 1, tree1) == ERROR1)
			return ERROR1;
		if (is_relop())
		{
			read_num++;
			trans_print(num, n, false);
			tree1->cnode(算符, store[read_num - 1][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(表达式, store[read_num - 1][1], tree1);
			tree1 = tree1->next;
			if (P(表达式, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
	}
	else if (num == 循环语句)
	{
		if (is_while())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (store[read_num][0] == 左括号)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(条件组, n + 1, tree1) == ERROR1)
					return ERROR1;
				tree1 = tree1->next;
				if (store[read_num][0] == 右括号)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (P(语句, n + 1, tree1) == ERROR1)
						return ERROR1;
				}
				else
				{
					cout << "循环语句缺少右括号!" << endl;
					return ERROR1;
				}
			}
		}
		else
		{
			cout << "循环语句缺少while!" << endl;
			return ERROR1;
		}
	}
	else if (num == 情况语句)
	{
		if (is_switch())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (store[read_num][0] == 左括号)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(表达式, n + 1, tree1) == ERROR1)
					return ERROR1;
				tree1 = tree1->next;
				if (store[read_num][0] == 右括号)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (store[read_num][0] == 左大括号)
					{
						read_num++;
						trans_print(num, n, false);
						string fun = gen_fun();
						tree1->place = fun;
						tree1 = tree1->next;
						if (P(情况表, n + 1, tree1) == ERROR1)
							return ERROR1;
						tree1 = tree1->next;
						if (P(缺省, n + 1, tree1) == ERROR1)
							return ERROR1;
						tree1 = tree1->next;
						if (store[read_num][0] == 右大括号)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->place = fun;
						}
						else
						{
							cout << "情况语句缺少右大括号!" << endl;
							return ERROR1;
						}
					}
					else
					{
						cout << "情况语句缺少左大括号!" << endl;
						return ERROR1;
					}
				}
				else
				{
					cout << "情况语句缺少右括号!" << endl;
					return ERROR1;
				}
			}
		}
		else
		{
			cout << "情况语句缺少switch!" << endl;
			return ERROR1;
		}
	}
	else if (num == 情况表)
	{
		if (is_case())
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(情况子语句, n + 1, tree1) == ERROR1)
				return ERROR1;
			while (is_case())
			{
				tree1->cnode(情况子语句, 0, tree1);
				tree1 = tree1->next;
				if (P(情况子语句, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
		}
		else
		{
			cout << "情况表缺少case!" << endl;
			return ERROR1;
		}
	}
	else if (num == 情况子语句)
	{
		if (is_case())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1 = tree1->next;
			if (P(可枚举常量, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == 冒号)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				if (P(语句, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
			else
			{
				cout << "情况子语句缺少冒号!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "情况子语句缺少case!" << endl;
			return ERROR1;
		}
	}
	else if (num == 缺省)
	{
		if (is_default())
		{
			read_num++;
			trans_print(num, n, false);
			if (store[read_num][0] == 冒号)
			{
				read_num++;
				trans_print(num, n, false);
				sum = 1;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(语句, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
			else
			{
				cout << "缺省缺少冒号!" << endl;
				return ERROR1;
			}
		}
		else if (store[read_num][0] == 右大括号)
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
		}
		else
		{
			cout << "缺省错误!" << endl;
			return ERROR1;
		}
	}
	else if (num == 函数调用语句)
	{
		if (store[read_num][0] == 标识符)
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1->place = store_char[read_num - 1];
			if (store[read_num][0] == 左括号)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(值参数表, n + 1, tree1) == ERROR1)
					return ERROR1;
				if (store[read_num][0] == 右括号)
				{
					read_num++;
					trans_print(num, n, false);
				}
				else
				{
					cout << "函数调用语句缺少右括号!" << endl;
					return ERROR1;
				}
			}
			else
			{
				cout << "函数调用语句缺少左括号!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "函数调用语句缺少标识符!" << endl;
			return ERROR1;
		}
	}
	else if (num == 值参数表)
	{
		if (store[read_num][0] == 右括号)
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
		}
		else//这里偷懒，默认输入正确
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(条件, n + 1, tree1) == ERROR1)
				return ERROR1;
			while (store[read_num][0] == 分隔符)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->cnode(分隔符, 0, tree1);
				tree1 = tree1->next;
				tree1->cnode(条件, 0, tree1);
				tree1 = tree1->next;
				if (P(条件, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
		}
	}
	else if (num == 语句列)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (store[read_num][0] == 右大括号)
		{

		}
		else
		{
			int count = 0;//第一个语句的处理方式不同
			while (store[read_num][0] != 右大括号)
			{
				if (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1() || is_break())//keywordNum=1.7.8.9分别对应int char float double
				{
					if (count == 0)
					{
						tree1->type = 变量说明;
						tree1->type_1 = 0;
						count++;
					}
					else
					{
						tree1->cnode(变量说明, 0, tree1);
						tree1 = tree1->next;
					}
					if (P(变量说明, n + 1, tree1) == ERROR1)
						return ERROR1;
				}

				if (count == 0)
				{
					tree1->type = 语句;
					tree1->type_1 = 0;
					count++;
				}
				else
				{
					tree1->cnode(语句, 0, tree1);
					tree1 = tree1->next;
				}
				if (P(语句, n + 1, tree1) == ERROR1)
					return ERROR1;

			}
		}
	}
	else if (num == 读语句)
	{
		if (is_scanf())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (store[read_num][0] == 左括号)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (store[read_num][0] == 标识符)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->place = store_char[read_num - 1];
				}
				else
				{
					cout << "读语句缺少标识符!" << endl;
					return ERROR1;
				}
				while (store[read_num][0] == 分隔符)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(分隔符, 0, tree1);
					tree1 = tree1->next;
					tree1->cnode(标识符, 0, tree1);
					tree1 = tree1->next;
					if (store[read_num][0] == 标识符)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->place = store_char[read_num - 1];
					}
					else
					{
						cout << "读语句缺少标识符!" << endl;
						return ERROR1;
					}
				}
				if (store[read_num][0] == 右括号)
				{
					read_num++;
					trans_print(num, n, false);
					//此处是读语句特殊处理
					tree1->cnode(右括号, 0, tree1);
				}
				else
				{
					cout << "读语句缺少右括号!" << endl;
					return ERROR1;
				}
			}
			else
			{
				cout << "读语句缺少左括号!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "读语句缺少scanf!" << endl;
			return ERROR1;
		}
	}
	else if (num == 写语句)
	{
		if (is_printf())
		{
			read_num++;
			trans_print(num, n, false);
			if (store[read_num][0] == 左括号)
			{
				read_num++;
				trans_print(num, n, false);
				if (store[read_num][0] == 字符串)
				{
					read_num++;
					trans_print(num, n, false);
					sum = 2;
					tree1->expand(num, tree1, sum);
					tree1 = tree1->child;
					tree1 = tree1->next;
					tree1 = tree1->next;
					tree1->place = store_char[read_num - 1];
					if (store[read_num][0] == 分隔符)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(分隔符, 0, tree1);
						tree1 = tree1->next;
						tree1->cnode(表达式, 0, tree1);
						tree1 = tree1->next;
						if (P(表达式, n + 1, tree1) == ERROR1)
							return ERROR1;
						if (store[read_num][0] == 右括号)
						{
							read_num++;
							trans_print(num, n, false);
							//此处写语句特殊处理
							tree1->cnode(右括号, 0, tree1);
						}
						else
						{
							cout << "写语句缺少右括号!" << endl;
							return ERROR1;
						}
					}
					else if (store[read_num][0] == 右括号)
					{
						read_num++;
						trans_print(num, n, false);
						//此处写语句特殊处理
						tree1->cnode(右括号, 0, tree1);
					}
					else
					{
						cout << "写语句错误!" << endl;
						return ERROR1;
					}
				}
				else//偷懒
				{
					sum = 3;
					tree1->expand(num, tree1, sum);
					tree1 = tree1->child;
					tree1 = tree1->next;
					tree1 = tree1->next;
					if (P(表达式, n + 1, tree1) == ERROR1)
						return ERROR1;
					if (store[read_num][0] == 右括号)
					{
						read_num++;
						trans_print(num, n, false);
					}
					else
					{
						cout << "写语句缺少右括号!" << endl;
						return ERROR1;
					}
				}
			}
			else
			{
				cout << "写语句缺少左括号!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "写语句缺少printf!" << endl;
			return ERROR1;
		}
	}
	else if (num == 返回语句)
	{
		if (is_return())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (store[read_num][0] == 界符)
			{

			}
			else
			{
				tree1->cnode(表达式, 0, tree1);
				tree1 = tree1->next;
				if (P(表达式, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
		}
	}
	else if (num == break语句)
	{
		read_num++;
		trans_print(num, n, false);
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
	}
	else
	{
		cout << "input ERROR1!" << endl;
		return ERROR1;
	}
	if (n == 0)
	{
		if (store[read_num][0] == 结束符)
			return 0;
		else
		{
			//			cout << "end ERROR1!" << endl;
			return ERROR1 + 1;

		}
	}
}

void analysis_yufa(int root, gtree *tree1);
void analysis_cifayufa(int root, gtree *tree1)
{
	p = 0;
	do {

		scaner();
		bool flag = false;

		if (syn == 注释号)
			continue;
		store[store_num][0] = syn;
		store[store_num][1] = syn_1;
		strcpy(store_char[store_num++], token);


		switch (syn)
		{
		case 算符:

			cout << "<	" << setw(10) << token << "	" << type[syn - 1] << "	" << syn_1 << "	>" << endl;
			break;

		case -1:
			//printf("you have input a wrong string\n");
			//getch();  
			//return 0;
			syn = 0;
			break;

		default:
			cout << "<	" << setw(10) << token << "	" << type[syn - 1] << "	>" << endl;
			break;
		}
	} while (syn != 0);

	analysis_yufa(root, tree1);
}

void analysis_yufa(int root, gtree *tree1)
{
	if (P(root, 0, tree1) == ERROR1)
		cout << "ERROR1" << endl;
	else
		cout << "successful" << endl;
}

gtree *&create_tree(string text, int root, int &if_num, int &while_num, int &break_num, int &fun_num)
{
	gen_if_num = if_num;
	gen_while_num = while_num;
	gen_break_num = break_num;
	gen_fun_num = fun_num;
	memset(prog, '\0', sizeof(prog));
	memset(store, '\0', sizeof(store));
	memset(store_char, '\0', sizeof(store_char));
	store_num = 0;
	read_num = 0;
	//	char sentence[MAX_LENGTH] = ;
	strcpy(prog, text.c_str());
	gtree* temp_tree = new gtree;
	temp_tree->init(root);
	analysis_cifayufa(root, temp_tree);
	TraverseTree1(temp_tree);
	TraverseTree2(temp_tree);
	sort_num = 0;
	TraverseTree3(temp_tree);

	if_num = gen_if_num;
	while_num = gen_while_num;
	break_num = gen_break_num;
	fun_num = gen_fun_num;
	return temp_tree;
}

void Traverse(gtree *p)//测试函数
{
	if (p == NULL) return;
	if (p->type == 赋值语句)
		cout << p->place << endl;
	Traverse(p->child);
	Traverse(p->next);
}

void reset_tree_num()
{
	gen_if_num = 0;
	gen_while_num = 0;
	gen_break_num = 0;
	gen_fun_num = 0;
}

vector<AST_change> compare_AST(string file1,string file2)
{
	char content[MAX_LENGTH], content1[MAX_LENGTH];
	ifstream fin;
	fin.open(file1, ios::in);
	if (!fin.is_open())
	{
		cout << "open failed!" << endl;
	}

	fin.read(content, MAX_LENGTH);
	fin.close();

	fin.open(file2, ios::in);
	if (!fin.is_open())
	{
		cout << "open failed!" << endl;
	}

	fin.read(content1, MAX_LENGTH);
	fin.close();

	reset_tree_num();
	gtree *tree1 = create_tree(content, 程序, gen_if_num, gen_while_num, gen_break_num, gen_fun_num);
	//	Traverse(tree1);

	reset_tree_num();
	gtree *tree2 = create_tree(content1, 程序, gen_if_num, gen_while_num, gen_break_num, gen_fun_num);

	vector<Mapping> M;
	top_down(tree1, tree2, M);
	vector<AST_change> changes = extract_change(tree1, tree2, M);
	//	cout << Equal_Tree(tree1, tree2);
	//	Traverse(tree2);
	return changes;
}