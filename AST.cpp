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
char prog[MAX_LENGTH], token[MAX_BUFF_SIZE], ch;//����������С100����ʶ���������40
int store[MAX_WORD_NUM][2];
char store_char[MAX_WORD_NUM][MAX_BUFF_SIZE];
int syn, syn_1, p, m, n, sum, store_num, read_num;
int sort_num = 0;

int gen_fun_num = 0;//���Ҵ������±�
int gen_if_num = 0, gen_while_num = 0, gen_break_num = 0;
char *rwtab[keywordNum] = { (char*)("int"),(char*)("void"),(char*)("if"),(char*)("else"),(char*)("while"),(char*)("return"),(char*)("char"),(char*)("float"),(char*)("double"),(char*)("const"),(char*)("switch"),(char*)("case"),(char*)("scanf"),(char*)("printf"),(char*)("default"),(char*)("long"),(char*)("short"),(char*)("signed"),(char*)("unsigned"),(char*)("break") };
char *caltab[14] = { (char*)("<="),(char*)("<"),(char*)(">="),(char*)(">"),(char*)("+"),(char*)("-"),(char*)("!="),(char*)("=="),(char*)("*"),(char*)("/"),(char*)("&"),(char*)("|"),(char*)("!"),(char*)("^") };
char *type[20] = { (char*)("�ؼ���"),(char*)("��ʶ��"),(char*)("�޷�������"),(char*)("��ֵ��"),(char*)("���"),(char*)("���"),(char*)("ð��"),(char*)("�ָ���"),(char*)("ע�ͺ�"),(char*)("������"),(char*)("������"),(char*)("��������"),(char*)("��������"),
(char*)("�������"),(char*)("�Ҵ�����"),(char*)("��ĸ"),(char*)("�޷���ʵ��"),(char*)("������"),(char*)("�ַ���"),(char*)("�ַ�") };


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

void TraverseTree1(gtree *p)//�ȵ�һ������﷨������placeֵ���и���
{
	if (p == NULL) return;
	//trans_print(p->type, 1, true);

	if (p->type == ���)//���place=�������
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
	else if (p->type == ��ֵ��)//��ֵ��place=����
		p->place = "=";
	else if (p->type == ������)
		p->place = "(";
	else if (p->type == ������)
		p->place = ")";
	else if (p->type == ��������)
		p->place = "[";
	else if (p->type == ��������)
		p->place = "]";
	else if (p->type == ð��)
		p->place = ":";
	else if (p->type == �ָ���)
		p->place = ",";
	else if (p->type == ���)
		p->place = ";";




	TraverseTree1(p->child);
	TraverseTree1(p->next);

}
void TraverseTree2(gtree *p)//�Ա��ʽ���в���
{
	if (p == NULL)return;

	TraverseTree2(p->child);
	TraverseTree2(p->next);

	if (p->type == ���� || p->type == ���ʽ || p->type == �� || p->type == ���� || p->type == ֵ������ 
		|| p->type == ���� || p->type == ������ || p->type == ��ֵ��� || p->type == ���� || p->type == ʵ��)//�ڲ���������place��Ҫ���к���ǰ׺
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
	else if (p->type == �����������)
	{
		if (p->parent->type != ���)
			p->place = p->child->place + "_v";
		else
		{
			p->place = p->child->place + "_call";
		}
	}
	else if (p->type == �������)
	{
		p->place = gen_if();
	}
	else if (p->type == ѭ�����)
	{
		p->place = gen_while();
	}
	else if (p->type == break���)
	{
		p->place = gen_break();
	}
	else if (p->type == ���ͱ�ʶ��)
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
	else if (p->type == ��������)
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

		syn = ��ʶ��;
		syn_1 = 0;

		for (n = 0; n < keywordNum; n++)
			if (strcmp(token, rwtab[n]) == 0)
			{
				syn = �ؼ���;
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
			syn = �޷�������;
		else
			syn = �޷���ʵ��;//��ȷ���ѱ�֤������һ����Ϊ��ֵ����
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
				cout << "ȱ���ҵ����ţ�" << endl;
			}
		}
		syn = �ַ�;

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
			cout << "ȱ����˫���ţ�" << endl;
		syn = �ַ���;
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
			syn = ���;
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
			syn = ���;
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
			syn = ���;
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
			syn = ���;
			break;

		case '!':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '=')
			{
				syn_1 = 7;
				token[m++] = ch;
				syn = ���;
			}
			else
			{
				syn = ���;
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
				syn = ���;
			}
			else
			{
				syn = ��ֵ��;
				p--;
			}
			break;

		case '*':
			syn = ���;
			syn_1 = 9;
			token[m++] = ch;
			break;

		case '&':
			syn = ���;
			syn_1 = 11;
			token[m++] = ch;
			break;

		case '|':
			syn = ���;
			syn_1 = 12;
			token[m++] = ch;
			break;

		case '/':
			token[m++] = ch;
			ch = prog[p++];
			if (ch == '/')
			{
				token[m++] = ch;
				syn = ע�ͺ�;
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
					syn = ע�ͺ�;
					p++;
				}
			}
			else
			{
				syn = ���;
				syn_1 = 10;
				p--;
				//token[m++] = ch;
			}
			break;
		case '^':
			syn = ���;
			syn_1 = 14;
			token[m++] = ch;
			break;
		case '(':
			syn = ������;
			token[m++] = ch;
			break;

		case ')':
			syn = ������;
			token[m++] = ch;
			break;

		case '[':
			syn = ��������;
			token[m++] = ch;
			break;

		case ']':
			syn = ��������;
			token[m++] = ch;
			break;

		case '{':
			syn = �������;
			token[m++] = ch;
			break;

		case '}':
			syn = �Ҵ�����;
			token[m++] = ch;
			break;

		case ';':
			syn = ���;
			token[m++] = ch;
			break;

		case ':':
			syn = ð��;
			token[m++] = ch;
			break;

		case ',':
			syn = �ָ���;
			token[m++] = ch;
			break;

		case '#':
			syn = ������;
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
	if (store[read_num][0] == ��ʶ�� || store[read_num][0] == ��ĸ || (store[read_num][0] == �ؼ��� && (store[read_num][1] == 3 || store[read_num][1] == 5 || store[read_num][1] == 6)))
		return 1;
	else
		return 0;
}

bool is_int()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 1)
		return 1;
	else
		return 0;
}

bool is_long()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 16)
		return 1;
	else
		return 0;
}

bool is_short()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 17
		)
		return 1;
	else
		return 0;
}

bool is_signed1()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 18)
		return 1;
	else
		return 0;
}

bool is_unsigned1()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 19)
		return 1;
	else
		return 0;
}

bool is_void1()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 2)
		return 1;
	else
		return 0;
}

bool is_if()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 3)
		return 1;
	else
		return 0;
}

bool is_else()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 4)
		return 1;
	else
		return 0;
}

bool is_while()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 5)
		return 1;
	else
		return 0;
}

bool is_switch()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 11)//11��switch
		return 1;
	else
		return 0;
}

bool is_case()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 12)//12��case
		return 1;
	else
		return 0;
}

bool is_return()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 6)
		return 1;
	else
		return 0;
}

bool is_break()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 20)
		return 1;
	else
		return 0;
}

bool is_char()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 7)
		return 1;
	else
		return 0;
}

bool is_float()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 8)
		return 1;
	else
		return 0;
}

bool is_double()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 9)
		return 1;
	else
		return 0;
}

bool is_const1()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 10)
		return 1;
	else
		return 0;
}

bool is_scanf()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 13)
		return 1;
	else
		return 0;
}

bool is_printf()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 14)
		return 1;
	else
		return 0;
}

bool is_default()
{
	if (store[read_num][0] == �ؼ��� && store[read_num][1] == 15)
		return 1;
	else
		return 0;
}

bool is_relop()
{
	if (store[read_num][0] == ��� && ((store[read_num][1] >= 1 && store[read_num][1] <= 4) || (store[read_num][1] == 7 || store[read_num][1] == 8)))
		return 1;
	else
		return 0;
}

void trans_print(int num, int n, bool flag)//�ս�ɷ�Ϊfalse,���ս�ɷ�Ϊtrue
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
			cout << "<����>" << endl;
			break;
		case 22:
			cout << "<����˵��>" << endl;
			break;
		case 23:
			cout << "<��������>" << endl;
			break;
		case 24:
			cout << "<����>" << endl;
			break;
		case 25:
			cout << "<ʵ��>" << endl;
			break;
		case 26:
			cout << "<����ͷ��>" << endl;
			break;
		case 27:
			cout << "<����˵��>" << endl;
			break;
		case 28:
			cout << "<��������>" << endl;
			break;
		case 29:
			cout << "<��ö�ٳ���>" << endl;
			break;
		case 30:
			cout << "<���ͱ�ʶ��>" << endl;
			break;
		case 31:
			cout << "<��������>" << endl;
			break;
		case 32:
			cout << "<�������>" << endl;
			break;
		case 33:
			cout << "<������>" << endl;
			break;
		case 34:
			cout << "<������>" << endl;
			break;
		case 35:
			cout << "<���ʽ>" << endl;
			break;
		case 36:
			cout << "<��>" << endl;
			break;
		case 37:
			cout << "<����>" << endl;
			break;
		case 38:
			cout << "<���>" << endl;
			break;
		case 39:
			cout << "<��ֵ���>" << endl;
			break;
		case 40:
			cout << "<�������>" << endl;
			break;
		case 41:
			cout << "<����>" << endl;
			break;
		case 42:
			cout << "<ѭ�����>" << endl;
			break;
		case 43:
			cout << "<������>" << endl;
			break;
		case 44:
			cout << "<�����>" << endl;
			break;
		case 45:
			cout << "<��������>" << endl;
			break;
		case 46:
			cout << "<ȱʡ>" << endl;
			break;
		case 47:
			cout << "<�����������>" << endl;
			break;
		case 48:
			cout << "<ֵ������>" << endl;
			break;
		case 49:
			cout << "<�����>" << endl;
			break;
		case 50:
			cout << "<�����>" << endl;
			break;
		case 51:
			cout << "<д���>" << endl;
			break;
		case 52:
			cout << "<�������>" << endl;
			break;
		case 53:
			cout << "<������>" << endl;
			break;
		case 54:
			cout << "<����>" << endl;
			break;
		case 55:
			cout << "<break���>" << endl;
			break;
		}
	}
}

int P(int num, int n, gtree *tree1)
{
	trans_print(num, n, true);
	//	gtree *root_tree = tree1;
	if (num == ����)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		bool flag = false;
		if (is_const1())//keywordNum=10��ʾconst
		{

			tree1->type = ����˵��;
			tree1->type_1 = 0;
			flag = true;
			//			cout << "program" << endl;
			if (P(����˵��, n + 1, tree1) == ERROR1)
				return ERROR1;

		}
		if ((is_int() || is_char() || is_float() || is_double()) && store[read_num + 2][0] != ������)//keywordNum=1.7.8.9�ֱ��Ӧint char float double
		{
			if (flag == true)
			{
				tree1->cnode(����˵��, 0, tree1);
				tree1 = tree1->next;
			}
			else
			{
				tree1->type = ����˵��;
				tree1->type_1 = 0;
				flag = true;
			}
			if (P(����˵��, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		if (is_int() || is_void1() || is_char() || is_float() || is_double())
		{
			if (flag == true)
			{
				tree1->cnode(��������, 0, tree1);
				tree1 = tree1->next;
			}
			else
			{
				tree1->type = ��������;
				tree1->type_1 = 0;
				flag = true;
			}
			if (P(��������, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		while (is_int() || is_void1() || is_char() || is_float() || is_double())//keywordNum=1.2.7.8.9�ֱ��Ӧint void char float double
		{
			tree1->cnode(��������, 0, tree1);
			tree1 = tree1->next;
			if (P(��������, n + 1, tree1) == ERROR1)
				return ERROR1;
		}


	}
	else if (num == ����˵��)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (is_const1())//keywordNum=10��ʾconst
		{
			read_num++;
			trans_print(num, n, false);
			tree1 = tree1->next;
			if (P(��������, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == ���)
			{
				read_num++;
				trans_print(num, n, false);
				while (is_const1())//keywordNum=10��ʾconst
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(�ؼ���, 10, tree1);
					tree1 = tree1->next;
					tree1->cnode(��������, 0, tree1);
					tree1 = tree1->next;
					if (P(��������, n + 1, tree1) == ERROR1)
						return ERROR1;
					tree1->cnode(���, 0, tree1);
					if (store[read_num][0] == ���)
					{
						read_num++;
						trans_print(num, n, false);
					}
					else
					{
						cout << "����˵��ȱ�ٽ��!" << endl;
						return ERROR1;
					}
				}
			}
			else
			{
				cout << "����˵��ȱ�ٽ��!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "����˵��ȱ��const!" << endl;
			return ERROR1;
		}

	}
	else if (num == ��������)
	{

		if (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1())//keywordNum=1.7.8.9�ֱ��Ӧint char float double
		{
			int temp_syn1;
			if (is_signed1() || is_unsigned1())
				temp_syn1 = store[read_num + 1][1];
			else
				temp_syn1 = store[read_num][1];//�������ֳ�ʼ����ֵ������
			if (P(���ͱ�ʶ��, n + 1, tree1) == ERROR1)
				return ERROR1;
			sum = temp_syn1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1 = tree1->next;
			if (store[read_num][0] == ��ʶ��)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->place = store_char[read_num - 1];
				tree1 = tree1->next;
				if (store[read_num][0] == ��ֵ��)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (temp_syn1 == 1)
					{
						if (P(����, n + 1, tree1) == ERROR1)
							return ERROR1;
					}
					else if (temp_syn1 == 7)
					{
						if (store[read_num][0] == �ַ�)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->place = store_char[read_num - 1];
						}
						else
						{
							cout << "��������ȱ���ַ�!" << endl;
							return ERROR1;
						}
					}
					else if (temp_syn1 == 8 || temp_syn1 == 9)
					{
						if (P(ʵ��, n + 1, tree1) == ERROR1)
							return ERROR1;
					}
					while (store[read_num][0] == �ָ���)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(�ָ���, 0, tree1);
						tree1 = tree1->next;

						if (store[read_num][0] == ��ʶ��)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->cnode(��ʶ��, 0, tree1);
							tree1 = tree1->next;
							tree1->place = store_char[read_num - 1];

							if (store[read_num][0] == ��ֵ��)
							{

								read_num++;
								trans_print(num, n, false);
								tree1->cnode(��ֵ��, 0, tree1);
								tree1 = tree1->next;
								if (temp_syn1 == 1)
								{
									tree1->cnode(����, 0, tree1);
									tree1 = tree1->next;
									if (P(����, n + 1, tree1) == ERROR1)
										return ERROR1;
								}
								else if (temp_syn1 == 7)
								{
									tree1->cnode(�ַ�, 0, tree1);
									tree1 = tree1->next;
									if (store[read_num][0] == �ַ�)
									{
										read_num++;
										trans_print(num, n, false);
										tree1->place = store_char[read_num - 1];
									}
									else
									{
										cout << "��������ȱ���ַ�!" << endl;
										return ERROR1;
									}
								}
								else if (temp_syn1 == 8 || temp_syn1 == 9)
								{
									tree1->cnode(ʵ��, 0, tree1);
									tree1 = tree1->next;
									if (P(ʵ��, n + 1, tree1) == ERROR1)
										return ERROR1;
								}
							}
							else
							{
								cout << "��������ȱ�ٸ�ֵ��!" << endl;
								return ERROR1;
							}
						}
						else
						{
							cout << "��������ȱ�ٱ�ʶ��!" << endl;
							return ERROR1;
						}
					}
				}
				else
				{
					cout << "��������ȱ�ٸ�ֵ��!" << endl;
					return ERROR1;
				}
			}
			else
			{
				cout << "��������ȱ�ٱ�ʶ��!" << endl;
				return ERROR1;
			}
		}
	}
	else if (num == ����)
	{
		int flag = 0;
		if (store[read_num][0] == ��� && (store[read_num][1] == 5 || store[read_num][1] == 6))//5.6����+ -��
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
		else if (store[read_num][0] == �޷�������)
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
			cout << "����ȱ���޷�������!" << endl;
			return ERROR1;
		}
	}
	else if (num == ʵ��)
	{
		int flag = 0;
		if (store[read_num][0] == ��� && (store[read_num][1] == 5 || store[read_num][1] == 6))//5.6����+ -��
		{
			read_num++;
			trans_print(num, n, false);
			flag = store[read_num][1];
		}
		if (store[read_num][0] == �޷���ʵ��)
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
			cout << "ʵ��ȱ���޷���ʵ��!" << endl;
			return ERROR1;
		}
	}
	else if (num == ����ͷ��)
	{
		if (is_int() || is_void1() || is_char() || is_float() || is_double() || is_short() || is_signed1() || is_unsigned1())//keywordNum=1.2.7.8.9�ֱ��Ӧint void char float double
		{
			int temp_syn1;
			if (is_signed1() || is_unsigned1())
				temp_syn1 = store[read_num + 1][1];
			else
				temp_syn1 = store[read_num][1];//�������ֳ�ʼ����ֵ������

			sum = temp_syn1;//�����Ӧ��int void char float double
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (temp_syn1 != 2)//void��������
			{
				if (P(���ͱ�ʶ��, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
			else
			{
				read_num++;
				trans_print(num, n, false);
			}
			if (store[read_num][0] == ��ʶ��)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->next->place = store_char[read_num - 1];
			}
			else
			{
				cout << "����ͷ��ȱ�ٱ�ʶ��!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "����ͷ��ȱ�������ؼ���!" << endl;
			return ERROR1;
		}
	}
	else if (num == ����˵��)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (store[read_num + 2][0] == ������)//���ֺ����ͱ���
		{

		}
		else
		{
			if (P(��������, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == ���)
			{
				read_num++;
				trans_print(num, n, false);
				while ((is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1()) && store[read_num + 2][0] != ������)//keywordNum=1.7.8.9�ֱ��Ӧint char float double
				{
					tree1->cnode(��������, 0, tree1);
					tree1 = tree1->next;
					if (P(��������, n + 1, tree1) == ERROR1)
						return ERROR1;
					tree1->cnode(���, 0, tree1);
					tree1 = tree1->next;
					if (store[read_num][0] == ���)
					{
						read_num++;
						trans_print(num, n, false);
					}
					else
					{
						cout << "����˵��ȱ�ٽ��!" << endl;
						return ERROR1;
					}
				}
			}
			else
			{
				cout << "����˵��ȱ�ٽ��!" << endl;
				return ERROR1;
			}
		}
	}
	else if (num == ��������)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (store[read_num][0] == �ؼ��� && (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1()))//keywordNum=1.7.8.9�ֱ��Ӧint char float double
		{
			int temp_syn1;
			if (is_signed1() || is_unsigned1())
				temp_syn1 = store[read_num + 1][1];
			else
				temp_syn1 = store[read_num][1];//�������ֳ�ʼ����ֵ������
			if (store[read_num + 2][0] == ��������)//+2����Ϊ���ͱ�ʶ������һ��
			{

			}
			if (P(���ͱ�ʶ��, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == ��ʶ��)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->place = store_char[read_num - 1];
				if (store[read_num][0] == ��������)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(��������, 0, tree1);
					tree1 = tree1->next;


					if (store[read_num][0] == �޷�������)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(�޷�������, 0, tree1);
						tree1 = tree1->next;
						tree1->place = store_char[read_num - 1];
						if (store[read_num][0] == ��������)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->cnode(��������, 0, tree1);
							tree1 = tree1->next;
						}
						else
						{
							cout << "��������ȱ����������!" << endl;
							return ERROR1;
						}
					}
					else
					{
						cout << "��������ȱ���޷�������!" << endl;
						return ERROR1;
					}
				}
				if (store[read_num][0] == ��ֵ��)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(��ֵ��, 0, tree1);
					tree1 = tree1->next;
					if (store[read_num][0] == �������)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(�������, 0, tree1);
						tree1 = tree1->next;
						if (store[read_num][0] == �Ҵ�����)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->cnode(�������, 0, tree1);
							tree1 = tree1->next;
						}
						else
						{
							if (temp_syn1 == 1)
							{
								tree1->cnode(����, 0, tree1);
								tree1 = tree1->next;
								if (P(����, n + 1, tree1) == ERROR1)
									return ERROR1;
								while (store[read_num][0] == �ָ���)
								{
									read_num++;
									trans_print(num, n, false);
									tree1->cnode(�ָ���, 0, tree1);
									tree1 = tree1->next;
									tree1->cnode(����, 0, tree1);
									tree1 = tree1->next;
									if (P(����, n + 1, tree1) == ERROR1)
										return ERROR1;
								}
								if (store[read_num][0] == �Ҵ�����)
								{

								}
							}
							else if (temp_syn1 == 7)
							{
								tree1->cnode(ʵ��, 0, tree1);
								tree1 = tree1->next;
								if (P(ʵ��, n + 1, tree1) == ERROR1)
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
							tree1->cnode(����, 0, tree1);
							tree1 = tree1->next;
							if (P(����, n + 1, tree1) == ERROR1)
								return ERROR1;
						}
						else if (temp_syn1 == 7)
						{
							tree1->cnode(�ַ�, 0, tree1);
							tree1 = tree1->next;
							if (store[read_num][0] == �ַ�)
							{
								read_num++;
								trans_print(num, n, false);
								tree1->place = store_char[read_num - 1];
							}
							else
							{
								cout << "��������ȱ���ַ�!" << endl;
								return ERROR1;
							}
						}
						else if (temp_syn1 == 8 || temp_syn1 == 9)
						{
							tree1->cnode(ʵ��, 0, tree1);
							tree1 = tree1->next;
							if (P(ʵ��, n + 1, tree1) == ERROR1)
								return ERROR1;
						}
					}

				}
			}
			else
			{
				cout << "��������ȱ�ٱ�ʶ��!" << endl;
				return ERROR1;
			}
			while (store[read_num][0] == �ָ���)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->cnode(�ָ���, 0, tree1);
				tree1 = tree1->next;
				if (store[read_num][0] == ��ʶ��)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(��ʶ��, 0, tree1);
					tree1 = tree1->next;
					tree1->place = store_char[read_num - 1];
					if (store[read_num][0] == ��������)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(��������, 0, tree1);
						tree1 = tree1->next;

						if (store[read_num][0] == �޷�������)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->cnode(�޷�������, 0, tree1);
							tree1 = tree1->next;
							tree1->place = store_char[read_num - 1];

							if (store[read_num][0] == ��������)
							{
								read_num++;
								trans_print(num, n, false);
								tree1->cnode(��������, 0, tree1);
								tree1 = tree1->next;
							}
							else
							{
								cout << "��������ȱ����������!" << endl;
								return ERROR1;
							}
						}
						else
						{
							cout << "��������ȱ���޷�������!" << endl;
							return ERROR1;
						}
					}
					if (store[read_num][0] == ��ֵ��)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(��ֵ��, 0, tree1);
						tree1 = tree1->next;
						if (temp_syn1 == 1)
						{
							tree1->cnode(����, 0, tree1);
							tree1 = tree1->next;
							if (P(����, n + 1, tree1) == ERROR1)
								return ERROR1;
						}
						else if (temp_syn1 == 7)
						{
							tree1->cnode(�ַ�, 0, tree1);
							tree1 = tree1->next;
							if (store[read_num][0] == �ַ�)
							{
								read_num++;
								trans_print(num, n, false);
								tree1->place = store_char[read_num - 1];
							}
							else
							{
								cout << "��������ȱ���ַ�!" << endl;
								return ERROR1;
							}
						}
						else if (temp_syn1 == 8 || temp_syn1 == 9)
						{
							tree1->cnode(ʵ��, 0, tree1);
							tree1 = tree1->next;
							if (P(ʵ��, n + 1, tree1) == ERROR1)
								return ERROR1;
						}

					}
				}
				else
				{
					cout << "��������ȱ�ٱ�ʶ��!" << endl;
					return ERROR1;
				}
			}
		}
	}
	else if (num == ��ö�ٳ���)
	{
		if ((store[read_num][0] == ��� && (store[read_num][1] == 5 || store[read_num][1] == 6)) || store[read_num][0] == �޷�������)//5 6����+ -
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(����, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		else if (store[read_num][0] == �ַ�)
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
			cout << "��ö�ٳ�������!" << endl;
			return ERROR1;
		}
	}
	else if (num == ���ͱ�ʶ��)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		bool flag = false;
		int temp_syn1;
		if (is_signed1() || is_unsigned1())//�Ƿ���unsigned
		{
			read_num++;
			trans_print(num, n, false);
			temp_syn1 = store[read_num - 1][1];
			tree1->type = �ؼ���;
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
				tree1->cnode(�ؼ���, temp_syn11, tree1);
				tree1 = tree1->next;
			}
			else
			{
				tree1->type = �ؼ���;
				tree1->type_1 = temp_syn11;
			}
		}
		if (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1())
		{
			read_num++;
			trans_print(num, n, false);
			int temp_syn11 = store[read_num - 1][1];
			tree1->cnode(�ؼ���, temp_syn11, tree1);
			tree1 = tree1->next;

		}
	}
	else if (num == ��������)
	{
		if (is_int() || is_void1() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1())//keywordNum=1.2.7.8.9�ֱ��Ӧint void char float double
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(����ͷ��, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == ������)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				if (P(������, n + 1, tree1) == ERROR1)
					return ERROR1;
				tree1 = tree1->next;
				if (store[read_num][0] == ������)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (store[read_num][0] == �������)
					{
						read_num++;
						trans_print(num, n, false);
						string fun = gen_fun();
						tree1->place = fun;
						tree1 = tree1->next;
						if (P(�������, n + 1, tree1) == ERROR1)
							return ERROR1;
						tree1 = tree1->next;
						if (store[read_num][0] == �Ҵ�����)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->place = fun;
						}
						else
						{
							cout << "��������ȱ���Ҵ�����!" << endl;
							return ERROR1;
						}
					}
					else
					{
						cout << "��������ȱ���������!" << endl;
						return ERROR1;
					}
				}
				else
				{
					cout << "��������ȱ��������!" << endl;
					return ERROR1;
				}
			}
			else
			{
				cout << "��������ȱ��������!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "�����������!" << endl;
			return ERROR1;
		}
	}
	else if (num == �������)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		bool flag = false;
		if (store[read_num][0] == �ؼ��� && store[read_num][1] == 10)//keywordNum=10����const
		{
			tree1->type = ����˵��;
			tree1->type_1 = 0;
			flag = true;
			if (P(����˵��, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		if (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1())//keywordNum=1.7.8.9�ֱ��Ӧint char float double
		{
			if (flag == true)
			{
				tree1->cnode(����˵��, 0, tree1);
				tree1 = tree1->next;
			}
			else
			{
				tree1->type = ����˵��;
				tree1->type_1 = 0;
				flag = true;
			}
			if (P(����˵��, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		if (flag == true)
		{
			tree1->cnode(�����, 0, tree1);
			tree1 = tree1->next;
		}
		else
		{
			tree1->type = �����;
			tree1->type_1 = 0;
			flag = true;
		}
		if (P(�����, n + 1, tree1) == ERROR1)
			return ERROR1;
	}
	else if (num == ������)
	{
		if (store[read_num][0] == ������)//��ʾ��
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			return 0;
		}
		sum = 1;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(���ͱ�ʶ��, n + 1, tree1) == ERROR1)
			return ERROR1;
		tree1 = tree1->next;
		if (store[read_num][0] == ��ʶ��)
		{
			read_num++;
			trans_print(num, n, false);
			tree1->place = store_char[read_num - 1];
			if (store[read_num][0] == ��������)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->cnode(��������, 0, tree1);
				tree1 = tree1->next;
				if (store[read_num][0] == ��������)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(��������, 0, tree1);
					tree1 = tree1->next;
				}
				else
				{
					tree1->cnode(���ʽ, 0, tree1);
					tree1 = tree1->next;
					if (P(���ʽ, n + 1, tree1) == ERROR1)
						return ERROR1;
					if (store[read_num][0] == ��������)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(��������, 0, tree1);
						tree1 = tree1->next;
					}
					else
					{
						cout << "������ȱ����������!" << endl;
						return ERROR1;
					}
				}

			}
			while (store[read_num][0] == �ָ���)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->cnode(�ָ���, 0, tree1);
				tree1 = tree1->next;
				tree1->cnode(���ͱ�ʶ��, 0, tree1);
				tree1 = tree1->next;
				if (P(���ͱ�ʶ��, n + 1, tree1) == ERROR1)
					return ERROR1;
				if (store[read_num][0] == ��ʶ��)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(��ʶ��, 0, tree1);
					tree1 = tree1->next;
					tree1->place = store_char[read_num - 1];
					if (store[read_num][0] == ��������)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(��������, 0, tree1);
						tree1 = tree1->next;
						if (store[read_num][0] == ��������)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->cnode(��������, 0, tree1);
							tree1 = tree1->next;
						}
						else
						{
							tree1->cnode(���ʽ, 0, tree1);
							tree1 = tree1->next;
							if (P(���ʽ, n + 1, tree1) == ERROR1)
								return ERROR1;
							if (store[read_num][0] == ��������)
							{
								read_num++;
								trans_print(num, n, false);
								tree1->cnode(��������, 0, tree1);
								tree1 = tree1->next;
							}
							else
							{
								cout << "������ȱ����������!" << endl;
								return ERROR1;
							}
						}
					}
				}
				else
				{
					cout << "������ȱ�ٱ�ʶ��!" << endl;
					return ERROR1;
				}
			}
		}
		else
		{
			cout << "������ȱ�ٱ�ʶ��!" << endl;
			return ERROR1;
		}
	}
	else if (num == ���ʽ)
	{
		int flag = 0;
		if (store[read_num][0] == ��� && (store[read_num][1] == 5 || store[read_num][1] == 6)
			&& (store[read_num + 1][0] != �޷������� && store[read_num + 1][0] != �޷���ʵ��))//5.6����+ -
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
		if (P(��, n + 1, tree1) == ERROR1)
			return ERROR1;
		while (store[read_num][0] == ��� && (store[read_num][1] == 5 || store[read_num][1] == 6))//5.6����+ -
		{
			read_num++;
			trans_print(num, n, false);
			tree1->cnode(���, store[read_num - 1][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(��, 0, tree1);
			tree1 = tree1->next;
			if (P(��, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
	}
	else if (num == ��)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(����, n + 1, tree1) == ERROR1)
			return ERROR1;
		while (store[read_num][0] == ��� && (store[read_num][1] == 9 || store[read_num][1] == 10))//9 10���� * /
		{
			read_num++;
			trans_print(num, n, false);
			tree1->cnode(���, store[read_num - 1][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(����, 0, tree1);
			tree1 = tree1->next;
			if (P(����, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
	}
	else if (num == ����)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(����, n + 1, tree1) == ERROR1)
			return ERROR1;
		while (store[read_num][0] == ��� && (store[read_num][1] == 11 || store[read_num][1] == 12) && (store[read_num + 1][1] != 11 && store[read_num + 1][1] != 12))//11 12���� & |
		{
			read_num++;
			trans_print(num, n, false);
			tree1->cnode(���, store[read_num - 1][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(����, 0, tree1);
			tree1 = tree1->next;
			if (P(����, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
	}
	else if (num == ����)
	{
		if (store[read_num][0] == ��ʶ��)
		{
			if (store[read_num + 1][0] == ������)//������ǰ�ж��Ƿ������ã���read_num++֮ǰ
			{
				//temp_place = tree1->child->place;
				sum = 7;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				if (P(�����������, n + 1, tree1) == ERROR1)
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
				if (store[read_num][0] == ��������)
				{
					//temp_place += '[';
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(��������, 0, tree1);
					tree1 = tree1->next;
					tree1->cnode(���ʽ, 0, tree1);
					tree1 = tree1->next;
					if (P(���ʽ, n + 1, tree1) == ERROR1)
						return ERROR1;
					tree1->cnode(��������, 0, tree1);
					tree1 = tree1->next;
					if (store[read_num][0] == ��������)
					{
						read_num++;
						trans_print(num, n, false);
					}
					else
					{
						cout << "����ȱ����������!" << endl;
						return ERROR1;
					}
				}
			}

		}
		else if (store[read_num][0] == ������)
		{
			read_num++;
			trans_print(num, n, false);
			sum = 3;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1 = tree1->next;
			if (P(���ʽ, n + 1, tree1) == ERROR1)
				return ERROR1;

			if (store[read_num][0] == ������)
			{

				read_num++;
				trans_print(num, n, false);
			}
			else
			{
				cout << "����ȱ��������!" << endl;
				return ERROR1;
			}
		}
		else if (store[read_num][0] == �޷������� || (store[read_num][0] == ��� && store[read_num][1] == 6 && store[read_num + 1][0] == �޷�������))//6����-��
		{
			//read_num++;
			//trans_print(num, n, false);
			sum = 4;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(����, n + 1, tree1) == ERROR1)
				return ERROR1;
			//tree1->place = store_char[read_num - 1];
		}
		else if (store[read_num][0] == �޷���ʵ�� || (store[read_num][0] == ��� && store[read_num][1] == 6 && store[read_num + 1][0] == �޷���ʵ��))
		{
			//read_num++;
			//trans_print(num, n, false);
			sum = 5;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(ʵ��, n + 1, tree1) == ERROR1)
				return ERROR1;
			//tree1->place = store_char[read_num - 1];
		}
		else if (store[read_num][0] == �ַ�)
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
			cout << "���Ӵ���!" << endl;
			return ERROR1;
		}
	}
	else if (num == ���)
	{
		if (is_if())
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(�������, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		else if (is_while())
		{
			sum = 2;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(ѭ�����, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		else if (is_break())
		{
			sum = 12;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(break���, n + 1, tree1) == ERROR1)
				return ERROR1;
			if (store[read_num][0] == ���)
			{
				read_num++;
				trans_print(num, n, false);
			}
			else
			{
				cout << "���ȱ�ٽ��!" << endl;
				return ERROR1;
			}
		}
		else if (store[read_num][0] == �������)
		{
			read_num++;
			trans_print(num, n, false);
			sum = 3;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			string fun = gen_fun();
			tree1->place = fun;
			tree1 = tree1->next;
			if (P(�����, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == �Ҵ�����)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->place = fun;
			}
			else
			{
				cout << "���ȱ���Ҵ�����!" << endl;
				return ERROR1;
			}
		}
		else if (store[read_num][0] == ��ʶ��)
		{
			if (store[read_num + 1][0] == ������)
			{
				sum = 4;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				if (P(�����������, n + 1, tree1) == ERROR1)
					return ERROR1;
				if (store[read_num][0] == ���)
				{
					read_num++;
					trans_print(num, n, false);
				}
				else
				{
					cout << "���ȱ�ٽ��!" << endl;
					return ERROR1;
				}
			}
			else if (store[read_num + 1][0] == ��ֵ�� || store[read_num + 1][0] == ��������)
			{
				sum = 5;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				if (P(��ֵ���, n + 1, tree1) == ERROR1)
					return ERROR1;
				if (store[read_num][0] == ���)
				{
					read_num++;
					trans_print(num, n, false);
				}
				else
				{
					cout << "���ȱ�ٽ��!" << endl;
					return ERROR1;
				}
			}
			else
			{
				sum = 11;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				if (P(���ʽ, n + 1, tree1) == ERROR1)
					return ERROR1;
				if (store[read_num][0] == ���)
				{
					read_num++;
					trans_print(num, n, false);
				}
				else
				{
					cout << "���ȱ�ٽ��!" << endl;
					return ERROR1;
				}
			}
		}
		else if (store[read_num][0] == ���)
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
			if (P(�����, n + 1, tree1) == ERROR1)
				return ERROR1;
			if (store[read_num][0] == ���)
			{
				read_num++;
				trans_print(num, n, false);
			}
			else
			{
				cout << "���ȱ�ٽ��!" << endl;
				return ERROR1;
			}
		}
		else if (is_printf())
		{
			sum = 7;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(д���, n + 1, tree1) == ERROR1)
				return ERROR1;
			if (store[read_num][0] == ���)
			{
				read_num++;
				trans_print(num, n, false);
			}
			else
			{
				cout << "���ȱ�ٽ��!" << endl;
				return ERROR1;
			}
		}
		else if (is_switch())
		{
			sum = 9;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(������, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
		else if (is_return())
		{
			sum = 10;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(�������, n + 1, tree1) == ERROR1)
				return ERROR1;
			if (store[read_num][0] == ���)
			{
				read_num++;
				trans_print(num, n, false);
			}
			else
			{
				cout << "���ȱ�ٽ��!" << endl;
				return ERROR1;
			}
		}
	}
	else if (num == ��ֵ���)
	{
		if (store[read_num][0] == ��ʶ��)
		{
			read_num++;
			trans_print(num, n, false);
			if (store[read_num][0] == ��������)
			{
				read_num++;
				trans_print(num, n, false);
				sum = 2;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				tree1->place = store_char[read_num - 2];
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(���ʽ, n + 1, tree1) == ERROR1)
					return ERROR1;
				tree1 = tree1->next;
				if (store[read_num][0] == ��������)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (store[read_num][0] == ��ֵ��)
					{
						read_num++;
						trans_print(num, n, false);
						tree1 = tree1->next;
						if (P(���ʽ, n + 1, tree1) == ERROR1)
							return ERROR1;
					}
					else
					{
						cout << "��ֵ���ȱ�ٸ�ֵ��!" << endl;
						return ERROR1;
					}
				}
				else
				{
					cout << "��ֵ���ȱ����������!" << endl;
					return ERROR1;
				}
			}
			else if (store[read_num][0] == ��ֵ��)
			{
				read_num++;
				trans_print(num, n, false);
				sum = 1;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				tree1->place = store_char[read_num - 2];
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(���ʽ, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
		}
		else
		{
			cout << "��ֵ���ȱ�ٱ�ʶ��!" << endl;
			return ERROR1;
		}
	}
	else if (num == �������)
	{
		if (is_if())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (store[read_num][0] == ������)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(������, n + 1, tree1) == ERROR1)
					return ERROR1;
				tree1 = tree1->next;
				if (store[read_num][0] == ������)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (P(���, n + 1, tree1) == ERROR1)
						return ERROR1;
					if (is_else())
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(�ؼ���, 4, tree1);
						tree1 = tree1->next;
						tree1->cnode(���, 0, tree1);
						tree1 = tree1->next;
						if (P(���, n + 1, tree1) == ERROR1)
							return ERROR1;
					}
				}
				else
				{
					cout << "�������ȱ��������!" << endl;
					return ERROR1;
				}
			}
		}
		else
		{
			cout << "�������ȱ��if!" << endl;
			return ERROR1;
		}
	}
	else if (num == ������)
	{
		int left = 0;
		int count = 0;
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (store[read_num][0] == ������)
		{
			read_num++;
			trans_print(num, n, false);
			tree1->type = ������;
			tree1->type_1 = 0;
			count++;
		}
		if (store[read_num][0] == ��� && store[read_num][1] == 13)//��������
		{
			read_num++;
			trans_print(num, n, false);
			if (count == 0)
			{
				tree1->type = ���;
				tree1->type_1 = 13;
			}
			else
			{
				tree1->cnode(���, 13, tree1);
				tree1 = tree1->next;
			}
		}
		if (count == 0)
		{
			tree1->type = ����;
			tree1->type_1 = 0;
		}
		else
		{
			tree1->cnode(����, 0, tree1);
			tree1 = tree1->next;
		}
		if (P(����, n + 1, tree1) == ERROR1)
			return ERROR1;
		while (store[read_num][0] == ��� && (store[read_num][1] == 11 || store[read_num][1] == 12) || (left > 0 && store[read_num][0] == ������))//11 12���� & |
		{
			if (left > 0 && store[read_num][0] == ������)
			{
				read_num++;
				trans_print(num, n, false);
				left--;
				tree1->cnode(������, 0, tree1);
				tree1 = tree1->next;
				if (store[read_num][0] == ������)
					break;
			}
			read_num++;
			read_num++;//��Ϊ&&��||��������
			trans_print(num, n, false);
			trans_print(num, n, false);
			tree1->cnode(���, store[read_num - 2][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(���, store[read_num - 2][1], tree1);
			tree1 = tree1->next;

			if (store[read_num][0] == ��� && store[read_num][1] == 13)
			{
				read_num++;
				trans_print(num, n, false);

				tree1->cnode(���, 13, tree1);
				tree1 = tree1->next;
			}
			if (store[read_num][0] == ������)
			{
				read_num++;
				trans_print(num, n, false);
				left++;

				tree1->cnode(������, 0, tree1);
				tree1 = tree1->next;
			}

			tree1->cnode(����, 0, tree1);
			tree1 = tree1->next;
			if (P(����, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
	}
	else if (num == ����)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (P(���ʽ, n + 1, tree1) == ERROR1)
			return ERROR1;
		if (is_relop())
		{
			read_num++;
			trans_print(num, n, false);
			tree1->cnode(���, store[read_num - 1][1], tree1);
			tree1 = tree1->next;
			tree1->cnode(���ʽ, store[read_num - 1][1], tree1);
			tree1 = tree1->next;
			if (P(���ʽ, n + 1, tree1) == ERROR1)
				return ERROR1;
		}
	}
	else if (num == ѭ�����)
	{
		if (is_while())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (store[read_num][0] == ������)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(������, n + 1, tree1) == ERROR1)
					return ERROR1;
				tree1 = tree1->next;
				if (store[read_num][0] == ������)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (P(���, n + 1, tree1) == ERROR1)
						return ERROR1;
				}
				else
				{
					cout << "ѭ�����ȱ��������!" << endl;
					return ERROR1;
				}
			}
		}
		else
		{
			cout << "ѭ�����ȱ��while!" << endl;
			return ERROR1;
		}
	}
	else if (num == ������)
	{
		if (is_switch())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (store[read_num][0] == ������)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(���ʽ, n + 1, tree1) == ERROR1)
					return ERROR1;
				tree1 = tree1->next;
				if (store[read_num][0] == ������)
				{
					read_num++;
					trans_print(num, n, false);
					tree1 = tree1->next;
					if (store[read_num][0] == �������)
					{
						read_num++;
						trans_print(num, n, false);
						string fun = gen_fun();
						tree1->place = fun;
						tree1 = tree1->next;
						if (P(�����, n + 1, tree1) == ERROR1)
							return ERROR1;
						tree1 = tree1->next;
						if (P(ȱʡ, n + 1, tree1) == ERROR1)
							return ERROR1;
						tree1 = tree1->next;
						if (store[read_num][0] == �Ҵ�����)
						{
							read_num++;
							trans_print(num, n, false);
							tree1->place = fun;
						}
						else
						{
							cout << "������ȱ���Ҵ�����!" << endl;
							return ERROR1;
						}
					}
					else
					{
						cout << "������ȱ���������!" << endl;
						return ERROR1;
					}
				}
				else
				{
					cout << "������ȱ��������!" << endl;
					return ERROR1;
				}
			}
		}
		else
		{
			cout << "������ȱ��switch!" << endl;
			return ERROR1;
		}
	}
	else if (num == �����)
	{
		if (is_case())
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(��������, n + 1, tree1) == ERROR1)
				return ERROR1;
			while (is_case())
			{
				tree1->cnode(��������, 0, tree1);
				tree1 = tree1->next;
				if (P(��������, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
		}
		else
		{
			cout << "�����ȱ��case!" << endl;
			return ERROR1;
		}
	}
	else if (num == ��������)
	{
		if (is_case())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1 = tree1->next;
			if (P(��ö�ٳ���, n + 1, tree1) == ERROR1)
				return ERROR1;
			tree1 = tree1->next;
			if (store[read_num][0] == ð��)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				if (P(���, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
			else
			{
				cout << "��������ȱ��ð��!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "��������ȱ��case!" << endl;
			return ERROR1;
		}
	}
	else if (num == ȱʡ)
	{
		if (is_default())
		{
			read_num++;
			trans_print(num, n, false);
			if (store[read_num][0] == ð��)
			{
				read_num++;
				trans_print(num, n, false);
				sum = 1;
				tree1->expand(num, tree1, sum);
				tree1 = tree1->child;
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(���, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
			else
			{
				cout << "ȱʡȱ��ð��!" << endl;
				return ERROR1;
			}
		}
		else if (store[read_num][0] == �Ҵ�����)
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
		}
		else
		{
			cout << "ȱʡ����!" << endl;
			return ERROR1;
		}
	}
	else if (num == �����������)
	{
		if (store[read_num][0] == ��ʶ��)
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			tree1->place = store_char[read_num - 1];
			if (store[read_num][0] == ������)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (P(ֵ������, n + 1, tree1) == ERROR1)
					return ERROR1;
				if (store[read_num][0] == ������)
				{
					read_num++;
					trans_print(num, n, false);
				}
				else
				{
					cout << "�����������ȱ��������!" << endl;
					return ERROR1;
				}
			}
			else
			{
				cout << "�����������ȱ��������!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "�����������ȱ�ٱ�ʶ��!" << endl;
			return ERROR1;
		}
	}
	else if (num == ֵ������)
	{
		if (store[read_num][0] == ������)
		{
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
		}
		else//����͵����Ĭ��������ȷ
		{
			sum = 1;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (P(����, n + 1, tree1) == ERROR1)
				return ERROR1;
			while (store[read_num][0] == �ָ���)
			{
				read_num++;
				trans_print(num, n, false);
				tree1->cnode(�ָ���, 0, tree1);
				tree1 = tree1->next;
				tree1->cnode(����, 0, tree1);
				tree1 = tree1->next;
				if (P(����, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
		}
	}
	else if (num == �����)
	{
		sum = 0;
		tree1->expand(num, tree1, sum);
		tree1 = tree1->child;
		if (store[read_num][0] == �Ҵ�����)
		{

		}
		else
		{
			int count = 0;//��һ�����Ĵ���ʽ��ͬ
			while (store[read_num][0] != �Ҵ�����)
			{
				if (is_int() || is_char() || is_float() || is_double() || is_short() || is_long() || is_signed1() || is_unsigned1() || is_break())//keywordNum=1.7.8.9�ֱ��Ӧint char float double
				{
					if (count == 0)
					{
						tree1->type = ����˵��;
						tree1->type_1 = 0;
						count++;
					}
					else
					{
						tree1->cnode(����˵��, 0, tree1);
						tree1 = tree1->next;
					}
					if (P(����˵��, n + 1, tree1) == ERROR1)
						return ERROR1;
				}

				if (count == 0)
				{
					tree1->type = ���;
					tree1->type_1 = 0;
					count++;
				}
				else
				{
					tree1->cnode(���, 0, tree1);
					tree1 = tree1->next;
				}
				if (P(���, n + 1, tree1) == ERROR1)
					return ERROR1;

			}
		}
	}
	else if (num == �����)
	{
		if (is_scanf())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (store[read_num][0] == ������)
			{
				read_num++;
				trans_print(num, n, false);
				tree1 = tree1->next;
				tree1 = tree1->next;
				if (store[read_num][0] == ��ʶ��)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->place = store_char[read_num - 1];
				}
				else
				{
					cout << "�����ȱ�ٱ�ʶ��!" << endl;
					return ERROR1;
				}
				while (store[read_num][0] == �ָ���)
				{
					read_num++;
					trans_print(num, n, false);
					tree1->cnode(�ָ���, 0, tree1);
					tree1 = tree1->next;
					tree1->cnode(��ʶ��, 0, tree1);
					tree1 = tree1->next;
					if (store[read_num][0] == ��ʶ��)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->place = store_char[read_num - 1];
					}
					else
					{
						cout << "�����ȱ�ٱ�ʶ��!" << endl;
						return ERROR1;
					}
				}
				if (store[read_num][0] == ������)
				{
					read_num++;
					trans_print(num, n, false);
					//�˴��Ƕ�������⴦��
					tree1->cnode(������, 0, tree1);
				}
				else
				{
					cout << "�����ȱ��������!" << endl;
					return ERROR1;
				}
			}
			else
			{
				cout << "�����ȱ��������!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "�����ȱ��scanf!" << endl;
			return ERROR1;
		}
	}
	else if (num == д���)
	{
		if (is_printf())
		{
			read_num++;
			trans_print(num, n, false);
			if (store[read_num][0] == ������)
			{
				read_num++;
				trans_print(num, n, false);
				if (store[read_num][0] == �ַ���)
				{
					read_num++;
					trans_print(num, n, false);
					sum = 2;
					tree1->expand(num, tree1, sum);
					tree1 = tree1->child;
					tree1 = tree1->next;
					tree1 = tree1->next;
					tree1->place = store_char[read_num - 1];
					if (store[read_num][0] == �ָ���)
					{
						read_num++;
						trans_print(num, n, false);
						tree1->cnode(�ָ���, 0, tree1);
						tree1 = tree1->next;
						tree1->cnode(���ʽ, 0, tree1);
						tree1 = tree1->next;
						if (P(���ʽ, n + 1, tree1) == ERROR1)
							return ERROR1;
						if (store[read_num][0] == ������)
						{
							read_num++;
							trans_print(num, n, false);
							//�˴�д������⴦��
							tree1->cnode(������, 0, tree1);
						}
						else
						{
							cout << "д���ȱ��������!" << endl;
							return ERROR1;
						}
					}
					else if (store[read_num][0] == ������)
					{
						read_num++;
						trans_print(num, n, false);
						//�˴�д������⴦��
						tree1->cnode(������, 0, tree1);
					}
					else
					{
						cout << "д������!" << endl;
						return ERROR1;
					}
				}
				else//͵��
				{
					sum = 3;
					tree1->expand(num, tree1, sum);
					tree1 = tree1->child;
					tree1 = tree1->next;
					tree1 = tree1->next;
					if (P(���ʽ, n + 1, tree1) == ERROR1)
						return ERROR1;
					if (store[read_num][0] == ������)
					{
						read_num++;
						trans_print(num, n, false);
					}
					else
					{
						cout << "д���ȱ��������!" << endl;
						return ERROR1;
					}
				}
			}
			else
			{
				cout << "д���ȱ��������!" << endl;
				return ERROR1;
			}
		}
		else
		{
			cout << "д���ȱ��printf!" << endl;
			return ERROR1;
		}
	}
	else if (num == �������)
	{
		if (is_return())
		{
			read_num++;
			trans_print(num, n, false);
			sum = 0;
			tree1->expand(num, tree1, sum);
			tree1 = tree1->child;
			if (store[read_num][0] == ���)
			{

			}
			else
			{
				tree1->cnode(���ʽ, 0, tree1);
				tree1 = tree1->next;
				if (P(���ʽ, n + 1, tree1) == ERROR1)
					return ERROR1;
			}
		}
	}
	else if (num == break���)
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
		if (store[read_num][0] == ������)
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

		if (syn == ע�ͺ�)
			continue;
		store[store_num][0] = syn;
		store[store_num][1] = syn_1;
		strcpy(store_char[store_num++], token);


		switch (syn)
		{
		case ���:

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

void Traverse(gtree *p)//���Ժ���
{
	if (p == NULL) return;
	if (p->type == ��ֵ���)
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
	gtree *tree1 = create_tree(content, ����, gen_if_num, gen_while_num, gen_break_num, gen_fun_num);
	//	Traverse(tree1);

	reset_tree_num();
	gtree *tree2 = create_tree(content1, ����, gen_if_num, gen_while_num, gen_break_num, gen_fun_num);

	vector<Mapping> M;
	top_down(tree1, tree2, M);
	vector<AST_change> changes = extract_change(tree1, tree2, M);
	//	cout << Equal_Tree(tree1, tree2);
	//	Traverse(tree2);
	return changes;
}