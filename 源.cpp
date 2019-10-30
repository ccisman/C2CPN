#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdio.h>  
#include<string>
#include<iomanip>
#include<fstream>
#include<stack>
#include<sstream>
#include<vector>

#include"tree.h"
#include"cpn.h"
#include"RG.h"
#include"AST_compare.h"
#define keywordNum 20

using namespace std;
int tb_num = 0;
int nextnum = 100;//���(��100��ʼ)
int labelnum = 1;
int labelnum1 = 1;

//string last_sentence = "", last_func = "";
//C_Petri petri;
int gen_P_num = 0, gen_T_num = 0;//���ɿ����ͱ�Ǩ�����±�
int gen_if_num1 = 0, gen_while_num1 = 0, gen_break_num1 = 0, gen_fun_num1 = 0;
void emit(string op, string arg1, string arg2, string result, int num, int flag)
{
	if (flag == 0)
		cout << num << ":(" << op << "  ," << arg1 << "  ," << arg2 << "  ," << result << ')' << endl;
	else if (flag == 1)
		cout << num << ": " << "  if " << arg1 << "  " << op << "  " << arg2 << "  goto " << result << endl;
	else if (flag == 2)
		cout << num << ": " << "goto " << result << endl;
	else
	{
	};
	nextnum++;

	ofstream out;
	out.open("out.txt", ios::app);
	if (flag == 0)
		out << num << ":(" << op << "  ," << arg1 << "  ," << arg2 << "  ," << result << ')' << endl;
	else if (flag == 1)
		out << num << ": " << "  if " << arg1 << "  " << op << "  " << arg2 << "  goto " << result << endl;
	else if (flag == 2)
		out << num << ": " << "goto " << result << endl;
	else
	{
	};
	out.close();
}

string gen_P()
{
	string temp = "";
	return temp + 'P' + to_string(gen_P_num++);

}
string gen_T()
{
	string temp = "";
	return temp + 'T' + to_string(gen_T_num++);
}

string get_gen_P()
{
	string temp = "";
	return temp + 'P' + to_string(gen_P_num);
}

void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
void splitExpression(string &s, vector<string>& v)//�����ַ����ͽ�����ϣ����ַ�������>,<�ȷָ��������
{
	string s1;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == '(' || s[i] == ')')
			continue;
		if (s[i] == '<' || s[i] == '>')
		{
			if (s[i + 1] == '=')
				i++;
			v.push_back(s1);
			s1.clear();
		}
		else if (s[i] == '|' )
		{
			if (s[i + 1] == '|')
				i++;
			v.push_back(s1);
			s1.clear();
		}
		else if (s[i] == '&')
		{
			if (s[i + 1] == '&')
				i++;
			v.push_back(s1);
			s1.clear();
		}
		else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')
		{
			v.push_back(s1);
			s1.clear();
		}
		else if (s[i] == '!' || s[i] == '=')
		{
			if (s[i + 1] == '=')
				i++;
			else
				cout << "�����쳣!=��==" << endl;
			v.push_back(s1);
			s1.clear();
		}
		else
			s1 = s1 + s[i];
	}
	v.push_back(s1);
}
string find_P_name(C_Petri petri, string v_name)//ͨ��������v_name�ҿ�����name
{

	for (int i = petri.p_num - 1; i >= 0; i--)
	{
		string s = petri.place[i].v_name;
		vector<string> v;
		SplitString(s, v, "@");

		int pos = v.size() - 1;
		if (pos >= 0 && v[pos] == v_name)
			return petri.place[i].name;
	}
	cout << "can't find P" << endl;
	return "";
}
string find_P_name_1(C_Petri petri, string v_name, int current)//ͨ��������v_name�ҿ�����name
{

	for (int i = current - 1; i >= 0; i--)
	{
		string s = petri.place[i].v_name;
		vector<string> v;
		SplitString(s, v, "@");

		int pos = v.size() - 1;
		if (pos >= 0 && v[pos] == v_name)
			return petri.place[i].name;
	}
	cout << "can't find P" << endl;
	return "";
}
string find_T_name(C_Petri petri, string v_name)//ͨ�����ʽ��v_name�ұ�Ǩ��name
{
	for (int i = petri.t_num - 1; i >= 0; i--)
	{
		string s = petri.transition[i].v_Expression;
		string s1 = petri.transition[i].booleanExpression;
		if (s == v_name || s1 == v_name)
			return petri.transition[i].name;
	}
	cout << "can't find T" << endl;
	return "";
}

int string_replace(string &s1, const string &s2, const string &s3)
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
	return 0;
}
string opposite(string s)
{
	if (s.find(">=") != string::npos)
		string_replace(s, ">=", "<");
	else if (s.find("<=") != string::npos)
		string_replace(s, "<=", ">");
	else if (s.find("<") != string::npos)
		string_replace(s, "<", ">=");
	else if (s.find(">") != string::npos)
		string_replace(s, ">", "<=");
	else if (s.find("==") != string::npos)
		string_replace(s, "==", "!=");
	else if (s.find("!=") != string::npos)
		string_replace(s, "!=", "==");
	else//�����ȽϷ�
	{

	}
	if (s.find("&&") != string::npos)
		string_replace(s, "&&", "||");
	else if (s.find("||") != string::npos)
		string_replace(s, "||", "&&");

	return s;

}
string opposite_all(string s)
{
	vector<string> v;
	vector<string>v1;
	string temp_s;
	string result;
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (s[i] == '&' )
		{
			v1.push_back("||");
			v.push_back(temp_s);
			temp_s.clear();
			i += 2;
		}
		else if (s[i] == '|')
		{
			v1.push_back("&&");
			v.push_back(temp_s);
			temp_s.clear();
			i += 2;
		}
		temp_s += s[i];
	}
	v.push_back(temp_s);
	for (unsigned int i = 0; i < v.size(); i++)
	{
		v[i] = opposite(v[i]);
	}
	result = v[0];
	for (unsigned int i = 0; i < v1.size(); i++)
	{
		result += v1[i] + v[i + 1];
	}
	return result;
}

string find_first_yuju(gtree *p)
{
	if (p == NULL) return "";
	if (p->type == ���)
		return p->child->place;
	string child = find_first_yuju(p->child);
	string next = find_first_yuju(p->next);
	if (child != "")
		return child;
	if (next != "")
		return next;
	return "";
}
string find_last_yuju(gtree *p)//��p�����������һ�����
{
	if (p == NULL) return "";
	if (p->type == ������� || p->type == ѭ����� || p->type == ��ֵ��� || p->type == break��� || p->type == �����������)
		return p->place;
	string next = find_last_yuju(p->next);
	if (next != "")
		return next;
	string child = find_last_yuju(p->child);
	if (child != "")
		return child;
	return "";
}
gtree* find_yujuchuan(gtree *&p)
{
	if (p == NULL) return NULL;
	if (p->type == �����)
	{
		return p;
	}
	gtree* child = find_yujuchuan(p->child);
	gtree* next = find_yujuchuan(p->next);
	if (child)
		return child;
	if (next)
		return next;
	return NULL;
}
gtree* search_call(gtree *p)
{
	if (p == NULL)
		return NULL;
	else if (p->type == �����������)
		return p;
	gtree* child = search_call(p->child);
	if (child !=NULL)
		return child;
	gtree* next = search_call(p->next);
	if (next != NULL)
		return next;
	return NULL;
}

void create_connect(C_Petri &petri,string T, string express,int current)//������Ǩ�ͱ��ʽ�������ʽ�����п������Ǩ����ϵ
{
	string P2;
	vector<string> v;
	string V;
	bool sourceP;
	splitExpression(express, v);
	for (unsigned int i = 0; i < v.size(); i++)
	{
		if (v[i][0] == '_' || (v[i][0] >= 'a'&&v[i][0] <= 'z'))
		{
			int position = 0;
			for (unsigned int j = 0; j < v[i].length(); j++)
			{
				if (v[i][j] == '[')
				{
					position = j;
					break;
				}
			}
			if (position != 0)
				v[i] = v[i].substr(0, position);
			P2 = find_P_name_1(petri, v[i], current);
			V = v[i];
			sourceP = true;
			petri.Add_Arc(P2, T, V, sourceP);
			sourceP = false;
			//			V = "";
			petri.Add_Arc(T, P2, V, sourceP);
		}
	}
}
void inside_block(C_Petri &petri,gtree *tree1, string T)//�����ڽ�ģ
{
	gtree *node, *tr;
	bool flag = false;
	if (tree1->type == ��� && tree1->child->type != �������)
	{
		tr = tree1;
		flag = true;
	}
	else
	{
		node = find_yujuchuan(tree1);
		tr = node->child;
	}
	vector<string> now;
	vector<string> last;
	while (tr)
	{
		now.clear();
		if (tr->type == ��� && tr->child->type != ������� && tr->child->type != ���ʽ )
		{
			bool control_P, t;
			int n1 = 0;
			double d = 0.0;
			string tag;
			string V = "", V_name = "";
			string s = "";
			string _P;
			bool sourceP = false;

			string temp_P;

			_P = find_P_name(petri, tr->child->place);

			petri.Add_Arc(T, _P, V, sourceP);
			if (tr->child->type == ��ֵ��� || tr->child->type == �����������)
				now.push_back(find_T_name(petri, tr->child->place));
			else if (tr->child->type == �������)
			{
				vector<string> v;
				temp_P = find_P_name(petri, tr->child->place);
				now = petri.get_enter(temp_P);
			}
			else if (tr->child->type == ѭ�����)
			{
				vector<string> v;
				temp_P = find_P_name(petri, tr->child->place);
				now = petri.get_enter(temp_P);
			}
			else if (tr->child->type == �������)
			{

			}

			if (last.size() != 0)
			{
				string newP = gen_P();
				V_name = "executedP";
				tag = "";
				control_P = true;
				t = false;
				petri.Add_Place(newP, V_name, tag, control_P, t, n1, d, s, 0);
				V = "executed";
				sourceP = false;
				for (unsigned int i = 0; i < last.size(); i++)
					petri.Add_Arc(last[i], newP, V, sourceP);

				//break��䲻������
				if (tr->child->type == break���)
					break;


				sourceP = true;
				for (unsigned int i = 0; i < now.size(); i++)
					petri.Add_Arc(newP, now[i], V, sourceP);
				if (tr->child->type == ѭ�����)
				{
					string while_name = tr->child->place;
					for (int i = 0; i < petri.p_num; i++)
					{
						if (petri.place[i].v_name == while_name)
						{
							//string enter = petri.place[i].enter[0];
							for (unsigned int j = 0; j < petri.place[i].exit.size() - 1; j++)
							{
								V = "";
								sourceP = false;
								petri.Add_Arc(petri.place[i].exit[j], newP, V, sourceP);
							}
							break;
						}
					}
				}

			}
			if (tr->child->type == ��ֵ��� || tr->child->type == �����������)
				last = now;
			else if (tr->child->type == �������)
			{
				last = petri.get_exit(temp_P);
			}
			else if (tr->child->type == ѭ�����)//while������ʵ����ֻ��exit[-1]
			{
				last.clear();
				vector<string> temp_last = petri.get_exit(temp_P);
				last.push_back(temp_last[temp_last.size() - 1]);
			}

		}
		tr = tr->next;
		if (flag == true)
			break;
	}
}
void inside_block1(C_Petri &petri,gtree *tree1, string T)//�����ڽ�ģ
{
	gtree *node, *tr;
	int sum = 0;//��������begin�Ƿ����ӵ����ƿ���
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].sourceP == false && petri.arc[i].source == T)
		{
			sum++;
			break;
		}
	}
	bool flag = false;
	if (tree1->type == ��� && tree1->child->type != �������)
	{
		tr = tree1;
		flag = true;
	}
	else
	{
		node = find_yujuchuan(tree1);
		tr = node->child;
	}
	string now;
	vector<string> last;
	while (tr)
	{
		
		
		if (tr->type == ��� && tr->child->type != ������� && tr->child->type != ���ʽ && tr->child->type != break���)
		{
			gtree *flag = NULL;
			if (tr->child->type == ��ֵ���)
			{
				flag = search_call(tr->child->child);

			}
			else if (tr->child->type == ������� || tr->child->type == ѭ�����)
			{
				flag = search_call(tr->child->child->next->next->child);

			}
			else if (tr->child->type == �����������)
				flag = tr->child;
			//bool control_P, t;
			int n1 = 0;
			double d = 0.0;
			string tag;
			string V = "", V_name = "";
			string s = "";
			string _P;
			bool sourceP = false;

			string temp_P;

			if (sum == 0)//��һ�������ƻ�
			{
				if (flag == NULL)
				{
					V = "executed";
					_P = find_P_name(petri, tr->child->place);
					petri.Add_Arc(T, _P, V, sourceP);
				}
				sum++;
			}
			if (tr->child->type == ��ֵ��� || tr->child->type == ������� || tr->child->type == ѭ����� || tr->child->type == �����������)
			{
				//now.clear();
				//now.push_back(find_P_name(petri, tr->child->place));
				temp_P = find_P_name(petri, tr->child->place);
				now = find_P_name(petri, tr->child->place);
			}


			if (last.size() != 0 && flag == NULL)
			{
				for (unsigned int i = 0; i < last.size(); i++)
				{
					V = "executed";
					sourceP = false;
					petri.Add_Arc(last[i], now, V, sourceP);
				}

			}
	
			if (tr->child->type == ��ֵ��� || tr->child->type == �����������)
			{
				last.clear();
				last.push_back(find_T_name(petri, tr->child->place));
			}
			else if (tr->child->type == �������)
			{
				last = petri.get_exit(temp_P);
			}
			else if (tr->child->type == ѭ�����)//while������ʵ����ֻ��exit[-1]
			{
				last.clear();
				vector<string> temp_last = petri.get_exit(temp_P);
				last.push_back(temp_last[temp_last.size() - 1]);
			}
		}
		tr = tr->next;
		if (flag != NULL)
			break;
	}
}


bool control_P = true;
bool control_T = true;
bool t = false;
bool sourceP = true;
int n1 = 0;
double d = 0.0;
string tag = "";
string V_name = "";
string s = "";
string P1 = "";
string T = "";
string V = "";

int find_P_exist(vector<Place> place, string s)//�ҿ������Ƿ���v_name����s�ģ����ҷ����м���
{
	vector<string> v;
	for (int i = place.size() - 1; i >= 0; i--)
	{
		v.clear();
		SplitString(place[i].v_name, v, "#");
		if (v[0] == s)
		{
			if (v.size() == 1)
			{
				return 1;
			}
			else
			{
				return atoi(v[1].c_str()) + 1;

			}
		}
		
	}
	return 0;
}

int find_T_exist(vector<Transition> transition, string s)//�ҿ������Ƿ���v_name����s�ģ����ҷ����м���
{
	vector<string> v;
	for (int i = transition.size() - 1; i >= 0; i--)
	{
		if (transition[i].booleanExpression == "")
			SplitString(transition[i].v_Expression, v, "#");
		else
			SplitString(transition[i].booleanExpression, v, "#");
		if (v[0] == s)
		{
			if (v.size() == 1)
			{
				return 1;
			}
			else
			{
				return atoi(v[1].c_str()) + 1;

			}
		}
	}
	return 0;
}

string find_executedP(C_Petri petri,string T)//������Ǩ����Ѱ��ָ������ִ�п�����û�з���""
{
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].V == "executed" && petri.arc[i].sourceP == true && petri.arc[i].target == T && petri.arc[i].V != "#")
		{
			for (int j = 0; j < petri.p_num; j++)
			{
				if (petri.place[j].name == petri.arc[i].source)
					if (petri.place[j].v_name == "executedP")
						return petri.arc[i].source;
			}
		}
	}
	return "";
}

gtree* find_last_sentence(gtree *node,bool &flag,bool &flag1)//nodeָ�����ڵ㣬�����ҵ�����һ��������ڵ��ָ��,��һ��Ϊ�������flagΪtrue,flag1Ϊtrue����else������
{
	flag = false;
	flag1 = false;
	gtree *p = node;
	gtree *test_else;
	if (p->parent->type == ������� || p->parent->type == ѭ�����)
	{
		test_else = p->parent->child;
		while (test_else->next != p)
			test_else = test_else->next;
		if (test_else->type == �ؼ��� && test_else->type_1 == 4)//else
			flag1 = true;
		else
			flag1 = false;
		flag = true;
		return p->parent;
	}
	else//�����Ϊ�����
	{
		gtree *p1 = p->parent->child;
		while (p1->type != ���)
			p1 = p1->next;
		if (p1 == p)//��һ��
		{
			if (p->parent->parent->type == �������)
				return NULL;
			else
			{
				test_else = p->parent->parent->parent->child;
				while (test_else->next != p->parent->parent)
					test_else = test_else->next;
				if (test_else->type == �ؼ��� && test_else->type_1 == 4)//else
					flag1 = true;
				else
					flag1 = false;
				flag = true;
				return p->parent->parent->parent;
			}
		}
		else//�ǵ�һ��
		{
			while (p1->next != p)
				p1 = p1->next;

			gtree *last_point=p1;
			while (1)
			{
				if (p1->next->type != ��� || p1->next->child->type == ���ʽ)
					p1 = p1->next->next;
				else
				{
					last_point = p1;
					p1 = p1->next;
				}
				if (p1 == p)
					break;
			}
			return last_point->child;
		}
	}

}

gtree* find_next_sentence(gtree *node)//nodeָ�����ڵ㣬�����ҵ�����һ��������ڵ��ָ��,������NULL��ʾ�����Ϊ���������һ��
{
	gtree *p = node;
	gtree *test_else;
	if (p->parent->type == ѭ�����)
	{
		return p->parent;
	}
	else if(p->parent->type == ����� && p->parent->parent->parent->type == ѭ�����)
	{
		return p->parent->parent->parent;
	}
	while (p->next == NULL)
	{
		if (p->parent->type == �����)
		{
			if (p->parent->parent->type == �������)
				return NULL;
			else
				p = p->parent->parent->parent->parent;
		}
		else
			p = p->parent->parent;
	}
	return p->next->child;
}

void declaration(C_Petri &petri,  gtree *p, string func)//func���ڱ��������
{
	if (p == NULL) return;
	tag = "";
	control_P = true;
	control_T = true;
	t = false;
	sourceP = true;
	n1 = 0;
	d = 0.0;
	V_name = "";
	s = "";
	P1 = "";
	T = "";
	V = "";
	if (p->type == �������)
	{
		func += p->place;
	}
	else if (p->type == �Ҵ�����)
	{
		func = func.substr(0, func.length() - p->place.length());
	}
	else if (p->type == ��������)
	{
		control_P = false;
		//V_name = p->place;
		//cout << p->place << endl;
		//getchar();
		if (func != "")
			V_name = func + '@' + p->child->next->place;
		else
			V_name = p->child->next->place;
		if (p->child->next->next != NULL && p->child->next->next->type == ��������)//���������
		{

		}

		if (p->child->next->next == NULL)
		{
			t = false;
			tag = p->child->place;
			P1 = gen_P();
			petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, 1);
		}
		else
		{
			int array_size = 1;
			tag = p->child->place;
			gtree *p1;
			if (p->child->next->next->type == ��ֵ��)
			{
				p1 = p->child->next->next->next->next;
				string temp_s = p->child->next->next->next->place;
				stringstream ss;
				ss << temp_s;
				t = true;
				switch (tag[1])
				{
				case '1':
				case '5':
				case '6':
					ss >> n1;
					break;
				case '3':
				case '4':
					ss >> d;
					break;
				default:
					s = temp_s;
					break;
				}
			}
			else if (p->child->next->next->type == ��������)
			{
				p1 = p->child->next->next->next->next;
				array_size = atoi(p->child->next->next->next->place.c_str());
				//string temp_s = p->child->next->next->next->place;
				t = false;
			}
			else
				p1 = p->child->next->next;
			tag = p->child->place;
			P1 = gen_P();
			petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, array_size);



			while (p1)
			{
				if (p1->type == �ָ���)
				{
					control_P = false;
					p1 = p1->next;
					if (func != "")
						V_name = func + '@' + p1->place;
					else
						V_name = p1->place;
					if (p1->next->type == ��������)
					{

					}
					else
					{
						if (p1->next->type == ��ֵ��)
						{
							p1 = p1->next;
							string temp_s = p1->next->place;
							stringstream ss;
							ss << temp_s;
							t = true;
							switch (tag[1])
							{
							case '1':
							case '5':
							case '6':
								ss >> n1;
								break;
							case '3':
							case '4':
								ss >> d;
								break;
							default:
								s = temp_s;
								break;
							}
							
						}
						else if (p1->next->type == ��������)
						{
							p = p->next;
							array_size = atoi(p1->next->next->place.c_str());
							t = false;
						}
						P1 = gen_P();
						petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, array_size);
					}
				}
				p1 = p1->next;
			}
		}
		
	}
	else if (p->type == ��������)
	{
		control_P = true;
		control_T = false;
		tag = "";
		t = false;
		sourceP = true;

		P1 = gen_P();
		T = gen_T();
		V_name = p->child->child->next->place + " begin";
		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, 0);
		s = p->child->child->next->place + " begin";
		
		//transition���ں��棬Ϊ���ܹ��ں�����ʹ���β�

		//��¼�ϴη��ʵĺ������ƿ��������ڹ��̵���
		//last_func = P1;

		int num = 0;
		if (p->child->next->next->child!=NULL && p->child->next->next->child->type != kong)//������������βο���
		{
			gtree *p1 = p->child->next->next->child;//������

			string P2 = gen_P();
			V_name = p->child->child->next->place + "@" + p1->next->place;

			tag = p1->place;
			control_P = false;
			t = false;

			//������Ϊ�β���ʱ�������鸴��
			int array_size = 1;
			if (p->next->next!=NULL && p1->next->next->type == ��������)
			{
				array_size= atoi(p1->next->next->next->place.c_str());
				petri.Add_Place(P2, V_name, tag, control_P, t, n1, d, s, array_size);
				petri.set_point_flag(P2);
				p1 = p1->next->next->next->next->next;
			}
			else
			{
				petri.Add_Place(P2, V_name, tag, control_P, t, n1, d, s, 1);
				p1 = p1->next->next;
			}
			while (p1)
			{
				p1 = p1->next;
				string _P = gen_P();
				V_name = p->child->child->next->place + "@" + p1->next->place;
				tag = p1->place;
				control_P = false;
				t = false;
				if (p1->next->next != NULL && p1->next->next->type == ��������)
				{
					array_size= atoi(p1->next->next->next->place.c_str());
					petri.Add_Place(_P, V_name, tag, control_P, t, n1, d, s, array_size);
					petri.set_point_flag(_P);
					p1 = p1->next->next->next->next->next;
				}
				else
				{
					petri.Add_Place(_P, V_name, tag, control_P, t, n1, d, s, 1);
					p1 = p1->next->next;
				}
			}
		}


		string P2 = gen_P();
		V_name = p->child->child->next->place + "_end";
		if (p->child->child->type == ���ͱ�ʶ��)
		{
			tag = p->child->child->place;
		}
		else
			tag = "";
		control_P = true;
		t = false;
		petri.Add_Place(P2, V_name, tag, control_P, t, n1, d, s, 0);
		string P3;
		if (tag != "")
		{
			P3 = gen_P();
			V_name = p->child->child->next->place + "_v";//func_v���ڴ�ŷ���ֵ
			control_P = false;
			t = false;
			petri.Add_Place(P3, V_name, tag, control_P, t, n1, d, s, 1);//������ʱ��֧��ָ��
		}

		petri.Add_Transition(T, control_T, s, gen_P_num - 1);//beginǰ�пո�
		petri.Add_Arc(P1, T, V, sourceP);

		//		string T1 = gen_T();
		//		control_T = false;
		//		s = p->child->child->next->place + "_end";
		//		petri.Add_Transition(T1, control_T, s);
		//
		//		V = "";
		//		sourceP = true;
		//		petri.Add_Arc(P2, T1, V, sourceP);


		vector<string> v;
		v.push_back(T);
//		petri.Add_Place_enter(P1, v);
		//���last����
		//last_sentence = P1;

	}
	else if (p->type == ��ֵ��� || p->type == ������� || p->type == ѭ�����)
	{
		P1 = gen_P();
		control_P = true;
		t = false;
		V_name = p->place;
		int judge = find_P_exist(petri.place, V_name);
		if (judge != 0)
		{
			char temp[5];
			_itoa(judge, temp, 10);
			V_name = V_name + "#" + temp;
			p->place = V_name;
		}
		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, 0);

		if (p->type == ������� || p->type == ѭ�����)
		{
			string T1 = gen_T();
			string T2 = gen_T();
			vector<string> v;
			v.push_back(T1);
			v.push_back(T2);
			control_T = true;
			petri.Add_Transition(T1, control_T, s, gen_P_num - 1);
			petri.Add_Transition(T2, control_T, s, gen_P_num - 1);
			petri.Add_Place_enter(P1, v);

		}
		else if (p->type == ��ֵ���)
		{
			string T1 = gen_T();
			vector<string> v;
			v.push_back(T1);

			control_T = false;
			petri.Add_Transition(T1, control_T, s, gen_P_num - 1);
			petri.Add_Place_enter(P1, v);
			petri.Add_Place_exit(P1, v);
		}

	}
	else if (p->type == break���)
	{
		P1 = gen_P();
		control_P = true;
		t = false;
		V_name = p->place;
		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, 0);

		T = gen_T();
		control_T = false;
		s = p->place;
		petri.Add_Transition(T, control_T, s, gen_P_num - 1);

		V = "executed";
		sourceP = true;
		petri.Add_Arc(P1, T, V, sourceP);

//		petri.Add_Place_exit(P1, T);
		string last_sentence;
		string last_func;
		gtree *find_func = p;

		while (find_func->type != ��������)
		{
			find_func = find_func->parent;
		}
		last_func = find_P_name(petri, find_func->child->child->next->place + " begin");
		if (p->parent->parent->type == �����)
		{
			gtree *temp_p = p->parent->parent->child;
			while (temp_p->type != ���)
			{
				temp_p = temp_p->next;
			}
			if (temp_p->next == NULL)
				last_sentence = find_P_name(petri, p->parent->parent->parent->parent->place);//�ҵ������ifwhile�����Ϊlast���
			else
			{
				while (temp_p->next->child != p)
				{
					temp_p = temp_p->next;
				}
				last_sentence = find_P_name(petri, temp_p->child->place);
			}
		}
		else//��������ѭ�����ֱ���Ƴ�break
		{
			last_sentence = find_P_name(petri, p->parent->parent->place);
		}

		vector<string> v = petri.get_exit(last_sentence);
		for (unsigned int i = 0; i < v.size(); i++)
		{
			V = "executed";
			sourceP = false;
			petri.Add_Arc(v[i], P1, V, sourceP);
		}

		gtree *temp_p = p;
		while (temp_p->type != ѭ�����)
		{
			temp_p = temp_p->parent;
		}
//		string loop_P = find_P_name(petri, temp_p->place);
//		petri.Add_Place_exit(loop_P, T);

		temp_p = temp_p->parent;
		while (temp_p->type != ��������)
		{
			if (temp_p->next == NULL)
			{
				temp_p = temp_p->parent;
				while (temp_p->type != ��� && temp_p->type != ��������)
				{
					temp_p = temp_p->parent;
				}
			}
			else
			{
				string P2 = find_P_name(petri, temp_p->next->place);
				string T2 = petri.get_enter(P2)[0];
				string executed_P = find_executedP(petri, T2);
				
				V = "executed";
				sourceP = false;
				petri.Add_Arc(T, P2, V, sourceP);
				if (executed_P != "")
					petri.Add_Arc(T, executed_P, V, sourceP);
				break;
			}
		}
		if (temp_p->type == ��������)
		{
			for (int i = 0; i < petri.p_num; i++)
			{
				if (petri.place[i].name == last_func)
				{
					string temp_s = petri.place[i].v_name;
					string_replace(temp_s, " begin", "_end");
					string P2 = find_P_name(petri,temp_s);
					V = "executed";
					sourceP = false;
					petri.Add_Arc(T, P2, V, sourceP);
					break;
				}
			}
			
		}
	}


	declaration(petri, p->child, func);

	if (p->type == ��ֵ���)
	{

//			int array_num = 0;
//			if (p->child->next->type == ��������)
//				array_num = int(change_and_cal(p->child->next->next->place, petri.place, gen_P_num));
			P1 = find_P_name(petri, p->place);

			//��¼��һ����������䣬���ڹ��̵���
			//last_sentence = P1;

			vector<string> temp_v = petri.get_enter(P1);
			T = temp_v[0];
			control_T = false;
			for(int i=0;i<petri.p_num;i++)
				if (petri.place[i].name == P1)
				{
					s = petri.place[i].v_name;
					break;
				}
//			s = p->place;
			//֮ǰ���ñ�Ǩ������bool���ʽ��ֵ
			for (int i = 0; i < petri.t_num; i++)
			{
				if (petri.transition[i].name == T)
				{
					petri.transition[i].v_Expression = s;
					break;
				}
			}
//			int judge = find_T_exist(petri.transition, s);
//			if (judge > 1)
//			{
//				char temp[5];
//				_itoa(judge, temp, 10);
//				s = s + "#" + temp;
//				p->place = s;
//				for (int i = 0; i < petri.t_num; i++)
//				{
//					if (petri.transition[i].name == T)
//					{
//						petri.transition[i].booleanExpression = s;
//						break;
//					}
//				}
//			}
//			petri.Add_Transition(T, control_T, s);
			

			sourceP = true;
			petri.Add_Arc(P1, T, V, sourceP);


			create_connect(petri,  T, p->child->place, petri.get_current_P_num(T));
			create_connect(petri,  T, p->child->next->next->place, petri.get_current_P_num(T));
			string P2 = find_P_name(petri, p->child->place);
			//cout << "P2=" << (P2=="P0") << endl;
			if (p->child->next->type != ��������)
				V = p->child->next->next->place + "$" + "0";
			else
				V = p->child->next->next->next->next->next->place + "$" + p->child->next->next->place;
			sourceP = false;
			petri.Add_Arc(T, P2, V, sourceP);

			//������м���enter��exit
//			petri.Add_Place_enter(P1, T);
//			petri.Add_Place_exit(P1, T);

	}
	else if (p->type == �������)
	{

		//Ĭ�ϳ�����ȷ
		P1 = find_P_name(petri, p->place);

		//��¼��һ����������䣬���ڹ��̵���
		//last_sentence = P1;
		vector<string> temp_v = petri.get_enter(P1);
		T = temp_v[0];
		control_T = true;
		s = p->child->next->next->place;
		//֮ǰ���ñ�Ǩ������bool���ʽ��ֵ
		for (int i = 0; i < petri.t_num; i++)
		{
			if (petri.transition[i].name == T)
			{
				petri.transition[i].booleanExpression = s;
				break;
			}
		}
//		petri.Add_Transition(T, control_T, s);
		string T1 = temp_v[1];
		create_connect(petri,  T, s,petri.get_current_P_num(T));
		s = opposite_all(s);
		for (int i = 0; i < petri.t_num; i++)
		{
			if (petri.transition[i].name == T1)
			{
				petri.transition[i].booleanExpression = s;
				break;
			}
		}
//		petri.Add_Transition(T1, control_T, s);
		create_connect(petri,  T1, s, petri.get_current_P_num(T));
		string T2 = "";
		string T3 = "";
		//string p2 = find_first_yuju(p->child->next->next->next->next);//��if������ĵ�һ�����ʽ
		//string P2 = find_P_name(petri,p2);
		string t2 = find_last_yuju(p->child->next->next->next->next->child);

		T2 = find_P_name(petri, t2);//����T2��ʵ��P
		vector<string> temp_v1 = petri.get_exit(T2);
		if (t2.rfind("while", 1) != string::npos&& temp_v1.size() > 0)//while���ֻ�������һ����ʵ����
		{
			string temp_exit = temp_v1[temp_v1.size() - 1];
			temp_v1.clear();
			temp_v1.push_back(temp_exit);
		}
		vector<string> temp_v2;


		inside_block(petri,p->child->next->next->next->next, T);

		string t3;

		if (p->child->next->next->next->next->next)
		{

			t3 = find_last_yuju(p->child->next->next->next->next->next->next->child);
			T3 = find_P_name(petri, t3);//����T3��ʵ��P
			temp_v2 = petri.get_exit(T3);
			if (t3.rfind("while", 1) != string::npos && temp_v2.size() > 0)//while���ֻ�������һ����ʵ����
			{
				string temp_exit = temp_v2[temp_v2.size() - 1];
				temp_v2.clear();
				temp_v2.push_back(temp_exit);
			}
			inside_block(petri,p->child->next->next->next->next->next->next, T1);
		}
		sourceP = true;
		V = "";
		petri.Add_Arc(P1, T, V, sourceP);
		petri.Add_Arc(P1, T1, V, sourceP);

		//������м���enter��exit
//		petri.Add_Place_enter(P1, T);
//		petri.Add_Place_enter(P1, T1);
		petri.Add_Place_exit(P1, temp_v1);
		if (p->child->next->next->next->next->next)
			petri.Add_Place_exit(P1, temp_v2);
		else
			petri.Add_Place_exit(P1, T1);

	}
	else if (p->type == ѭ�����)
	{
		P1 = find_P_name(petri, p->place);

		//��¼��һ����������䣬���ڹ��̵���
		//last_sentence = P1;
		vector<string> temp_v = petri.get_enter(P1);
		T = temp_v[0];
		control_T = true;
		string s = p->child->next->next->place;

		//petri.Add_Transition(T, control_T, s);
		//֮ǰ���ñ�Ǩ������bool���ʽ��ֵ
		for (int i = 0; i < petri.t_num; i++)
		{
			if (petri.transition[i].name == T)
			{
				petri.transition[i].booleanExpression = s;
				break;
			}
		}
		string T1 = temp_v[1];
		create_connect(petri,  T, s, petri.get_current_P_num(T));
		s = opposite_all(s);

		//petri.Add_Transition(T1, control_T, s);
		//֮ǰ���ñ�Ǩ������bool���ʽ��ֵ
		for (int i = 0; i < petri.t_num; i++)
		{
			if (petri.transition[i].name == T1)
			{
				petri.transition[i].booleanExpression = s;
				break;
			}
		}
		create_connect(petri,  T1, s, petri.get_current_P_num(T));
		string T2 = "";
		string T3 = "";
		//string p2 = find_first_yuju(p->child->next->next->next->next);//��if������ĵ�һ�����ʽ
		//string P2 = find_P_name(petri,p2);
		string t2 = find_last_yuju(p->child->next->next->next->next);

		T2 = find_P_name(petri, t2);//����T2��ʵ��P
		vector<string> temp_v1 = petri.get_exit(T2);
		if (t2.rfind("while", 1) != string::npos && temp_v1.size() > 0)//while���ֻ�������һ����ʵ����
		{
			string temp_exit = temp_v1[temp_v1.size() - 1];
			temp_v1.clear();
			temp_v1.push_back(temp_exit);
		}
		//����while���Ƿ��й��̵���
		int while_flag = petri.get_flag(P1);
		vector<string> while_information = petri.get_information(P1);
		for (unsigned int i = 0; i < temp_v1.size(); i++)
		{
			V = "executed";
			sourceP = false;
			if (while_flag == 1)//while�����д��ڹ��̵���
			{
				for (unsigned int j = 0; j < while_information.size(); j++)
					petri.Add_Arc(temp_v1[i], while_information[j], V, sourceP);
				if (p->parent->parent->type == ����� && p->parent->parent->parent->type == �������)
					;
				else
					petri.Add_Arc(temp_v1[i], P1, V, sourceP);
			}
			else
				petri.Add_Arc(temp_v1[i], P1, V, sourceP);

		}

		inside_block(petri,p->child->next->next->next->next, T);

		sourceP = true;
		V = "";
		petri.Add_Arc(P1, T, V, sourceP);
		petri.Add_Arc(P1, T1, V, sourceP);

		//������м���enter��exit
	//	petri.Add_Place_enter(P1, T);
	//	petri.Add_Place_enter(P1, T1);
		petri.Add_Place_exit(P1, temp_v1);//����temp_v1��ʵ������ʵ���ڣ�ֻ��T1��while���ĳ���
		petri.Add_Place_exit(P1, T1);

	}
	else if (p->type == ��������)
	{
		string func = p->place + " begin";
		T = find_T_name(petri, func);
		inside_block1(petri,p, T);//�����inside_block���в�ͬ


		//���ҵ��������֮ǰһ�����ĳ���
		//vector<string> v;
		//for (int i = 0; i < petri.p_num; i++)
		//{
		//	if (petri.place[i].name == last_sentence)
		//	{
		//		if (petri.place[i].v_name.find("while") != string::npos)
		//		{
		//			v.push_back(petri.place[i].exit[petri.place[i].exit.size() - 1]);
		//		}
		//		else
		//			v = petri.place[i].exit;
		//		break;
		//	}
		//}
		vector<string> v;
		string begin_place = find_P_name(petri, p->child->child->next->place + " begin");
		v = petri.get_exit(begin_place);

		//�ҵ�һ������������һ����䣬��������ڼ��뺯������
		gtree *sentence_set = p->child->next->next->next->next->next->child;
		while (sentence_set->type != �����)
			sentence_set = sentence_set->next;

		gtree *find_last = sentence_set->child;
		bool already_return = false;
		while (1)
		{
			if (find_last->child->type == �������)
			{
				already_return = true;
				break;
			}
			if (find_last->next == NULL)
				break;
			
			find_last = find_last->next;
		}
		if (already_return == false)
		{
			vector<string> v1 = petri.get_exit(find_P_name(petri, find_last->child->place));
			v.insert(v.end(), v1.begin(), v1.end());
		}

		//���췵�ؿ����Ļ�
		for (int i = 0; i < petri.p_num; i++)
		{
			if (petri.place[i].name == begin_place)
			{
				string temp_P = petri.place[i].v_name;
				string_replace(temp_P, " begin", "_end");
				for (int j = 0; j < petri.p_num; j++)
				{
					if (petri.place[j].v_name == temp_P)
					{

						for (unsigned int k = 0; k < v.size(); k++)
						{
							bool flag = false;
							for (int q = 0; q < petri.arcnum; q++)
							{
								if (petri.arc[q].sourceP == false && petri.arc[q].source == v[k] && petri.arc[q].target == petri.place[j].name)
								{
									flag = true;
									break;
								}
							}
							if (flag == true)
								break;
							V = "executed";
							sourceP = false;
							petri.Add_Arc(v[k], petri.place[j].name, V, sourceP);
						}

						break;
					}
				}
				break;
			}
		}
	}
	else if (p->type == �����������)
	{
		bool noreturn = false;
		string last_sentence;
		string last_func;
		gtree *find_func = p;
		string this_func_name;
		string new_place;
		while (find_func->type != ��������)
		{
			find_func = find_func->parent;
		}
		this_func_name = find_func->child->child->next->place;
		last_func = find_P_name(petri, this_func_name + " begin");
		if (p->parent->type == ���)//��ʾ��������ֵ�ĵ���
		{
			noreturn = true;
			new_place = gen_P();
			control_P = true;
			t = false;
			V_name = p->place;
			int judge = find_P_exist(petri.place, V_name);
			if (judge != 0)
			{
				char temp[5];
				_itoa(judge, temp, 10);
				V_name = V_name + "#" + temp;
				p->place = V_name;
			}
			petri.Add_Place(new_place, V_name, tag, control_P, t, n1, d, s, 0);
			string T1 = gen_T();
			vector<string> v;
			v.push_back(T1);

			control_T = false;
			petri.Add_Transition(T1, control_T, V_name, gen_P_num - 1);
			petri.Add_Arc(new_place, T1, "", true);
			petri.Add_Place_enter(new_place, v);
			petri.Add_Place_exit(new_place, v);
		}
		//�ж��Ƿ���������
		string inside = "";
		gtree *temp_tree1 = p;
		bool flag_condition = false;

		while (temp_tree1->type != ���)
		{
			temp_tree1 = temp_tree1->parent;
		}

		if (temp_tree1->parent->type == ����� && temp_tree1->parent->parent->type == �������)
		{
			inside = "";
		}
		else
		{
			inside = "in";
		}
		
		//�ҵ���һ�����


		gtree *find_last = p;
		while (find_last->type != ���)
		{
			if (find_last->type == ������)
				flag_condition = true;//flag_conditionΪ���ʾ������������������
			find_last = find_last->parent;
		}
		bool last_father_flag,flag_else;

		gtree *last_node = find_last_sentence(find_last, last_father_flag, flag_else);
		if (last_node == NULL)
			last_sentence = last_func;
		else
			last_sentence = find_P_name(petri, last_node->place);
		
		P1 = gen_P();
		control_P = true;
		t = false;
		V_name = p->child->place + "()";

		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, 0);

		T = gen_T();
		control_T = false;
		s = p->child->place + "()";
		petri.Add_Transition(T, control_T, s, gen_P_num - 1);

		V = "";
		sourceP = true;
		petri.Add_Arc(P1, T, V, sourceP);
		string P3;
		if (inside == "")
		{
			//������ƿ���
			P3 = gen_P();
			control_P = true;
			t = false;
			V_name = p->child->place + "()_c";

			petri.Add_Place(P3, V_name, tag, control_P, t, n1, d, s, 0);
		}

		if (find_last->child->type == ѭ����� && flag_condition == true)
		{

			for (int i = 0; i < petri.p_num; i++)
			{
				if (petri.place[i].v_name == find_last->child->place)
				{
					petri.place[i].flag = 1;//1����while����������к�������
					petri.place[i].information.push_back(P1);
					if (inside == "")
						petri.place[i].information.push_back(P3);
					break;
				}
			}
		}

		//���췵�ر�Ǩ
		string T2 = "";
		if (inside == "")
		{
			T2 = gen_T();
			control_T = false;
			s = p->child->place + "_end";
			petri.Add_Transition(T2, control_T, s, gen_P_num - 1);
			string temp_P = find_P_name(petri, s);

			V = "";
			sourceP = true;
			petri.Add_Arc(temp_P, T2, V, sourceP);
			petri.Add_Arc(P3, T2, V, sourceP);
		}

		string temp_s = p->child->place + " begin";
		string P4 = find_P_name(petri, temp_s);
		V = "";
		sourceP = false;
		petri.Add_Arc(T, P4, V, sourceP);

		
				///�������ݴ���
		gtree *temp_tree = p->child->next->next->child;//ֵ������->child
		vector<string> v;
		while (temp_tree)
		{
			string value = temp_tree->place;
			v.push_back(value);
			if (temp_tree->next == NULL)
				break;
			temp_tree = temp_tree->next->next;
		}
		int sum = 0;
		for (int i = 0; i < petri.p_num; i++)
		{
			string temp_s = petri.place[i].v_name;
			vector<string> v1;
			SplitString(temp_s, v1, "@");
			if (v1[0] == p->child->place)
			{
				V = v[sum++];
				sourceP = false;
				petri.Add_Arc(T, petri.place[i].name, V, sourceP);
				sourceP = true;
				petri.Add_Arc(petri.place[i].name, T, V, sourceP);

				//����
				create_connect(petri,  T, V, petri.get_current_P_num(T));
			}
		}


		//��ת���ش���



		for (int i = 0; i < petri.p_num; i++)
		{
			if (petri.place[i].name == last_sentence)
			{
				//������ת����
				if (last_node == NULL)//�ȴ�������һ�亯���������
				{ 
					V = "";
					string this_begin_T = find_T_name(petri, this_func_name + " begin");
					petri.Add_Arc(this_begin_T, P1, V, false);
					if (inside == "")
						petri.Add_Arc(this_begin_T, P3, V, false);
				}
				else
				{
					if (petri.place[i].v_name.find("while") == string::npos)//while�����ʵ����ֻ��һ��
					{
						if (last_father_flag == false)
						{
							for (unsigned int j = 0; j < petri.place[i].exit.size(); j++)
							{
								V = "";
								sourceP = false;
								petri.Add_Arc(petri.place[i].exit[j], P1, V, sourceP);
								if (inside == "")
									petri.Add_Arc(petri.place[i].exit[j], P3, V, sourceP);
							}
							petri.place[i].call_last = true;
						}
						else
						{
							V = "";
							if (flag_else == false)//����else�����䣬���඼�ǵ�һ�������ת
							{
								petri.Add_Arc(petri.place[i].enter[0], P1, V, false);
								if (inside == "")
									petri.Add_Arc(petri.place[i].enter[0], P3, V, false);
							}
							else
							{
								petri.Add_Arc(petri.place[i].enter[1], P1, V, false);
								if (inside == "")
									petri.Add_Arc(petri.place[i].enter[1], P3, V, false);
							}
						}
					}
					else
					{
						if (last_father_flag == false)
						{
							V = "";
							sourceP = false;
							petri.Add_Arc(petri.place[i].exit[petri.place[i].exit.size() - 1], P1, V, sourceP);
							if (inside == "")
								petri.Add_Arc(petri.place[i].exit[petri.place[i].exit.size() - 1], P3, V, sourceP);
							petri.place[i].call_last = true;
						}
						else
						{
							V = "";
							petri.Add_Arc(petri.place[i].enter[0], P1, V, false);
							if (inside == "")
								petri.Add_Arc(petri.place[i].enter[0], P3, V, false);
						}
					}
				}
				//���ز���
				string temp_P = p->child->place + "_end";
				//last_call = find_T_name(petri, temp_P);

				if (inside == "")
				{
					string T1 = find_T_name(petri, temp_P);
					gtree *temp_p = p;
					while (temp_p->type != ���)
					{
						temp_p = temp_p->parent;
					}

					string temp_place, P2;
					temp_place = temp_p->child->place;
					P2 = find_P_name(petri, temp_place);
					V = "";//��ʱ����Ϊִ�л�
					sourceP = false;
					petri.Add_Arc(T1, P2, V, sourceP);
					string_replace(temp_P, "_end", " begin");
					if (noreturn == false)
					{
						string call_P = find_P_name(petri, temp_P);
						vector<string> v = petri.get_enter(call_P);

						string_replace(temp_P, " begin", "_v");
						string v_P = find_P_name(petri, temp_P);
						V = v[0];
						sourceP = false;
						petri.Add_Arc(T1, v_P, V, sourceP);
						sourceP = true;
						petri.Add_Arc(v_P, T1, V, sourceP);
					}
				}
				else
				{
					//����end����������enter
					gtree *temp_p = p;
					while (temp_p->type != ���)
					{
						temp_p = temp_p->parent;
					}
					//���ӱ����ú������ڵ�_v����

					string temp_place = temp_p->child->place;
					string P2 = find_P_name(petri, temp_place);
					string P3 = find_P_name(petri, temp_P);
					vector<string> v1 = petri.get_enter(P2);
					for (unsigned k = 0; k < v1.size(); k++)
					{
						V = "";//��ʱ����Ϊִ�л�
						sourceP = true;
						petri.Add_Arc(P3, v1[k], V, sourceP);
					}
					if (noreturn == false)
					{
						string_replace(temp_P, "_end", " begin");
						string call_P = find_P_name(petri, temp_P);
						vector<string> v = petri.get_enter(call_P);
						vector<string> v2 = petri.get_exit(call_P);
						string_replace(temp_P, " begin", "_v");
						string v_P = find_P_name(petri, temp_P);
						for (unsigned int k = 0; k < v2.size(); k++)
						{
							V = v[0];
							sourceP = false;
							petri.Add_Arc(v2[k], v_P, V, sourceP);
							sourceP = true;
							petri.Add_Arc(v_P, v2[k], V, sourceP);
						}
					}

				}
				break;
			}
		}
	}
	else if (p->type == �������)//��return�ı��ʽ���ں������ƿ�����enter�У���return���ǰһ��ĳ��ڷ��ں������ƿ�����exit��
	{
		//string last_sentence;
		string last_func;
		gtree *find_func = p;

		while (find_func->type != ��������)
		{
			find_func = find_func->parent;
		}
		last_func = find_P_name(petri, find_func->child->child->next->place + " begin");
		string expression;
		if (p->child->next)
			expression = p->child->next->place;
		else
			expression = "0";



		vector<string> v1;
		v1.push_back(expression);
		petri.Add_Place_enter(last_func, v1);//�ѷ���ֵ���ʽ���ں������ƿ�����enter����

		gtree *temp_p = p->parent->parent->child;
		vector<string> v2;
		while (temp_p->type != ���)
		{
			temp_p = temp_p->next;
		}
		if (temp_p->next == NULL)
		{

			string temp_s;
			for (int i = 0; i < petri.p_num; i++)
			{
				if (petri.place[i].name == last_func)
				{
					temp_s = petri.place[i].v_name;
					break;
				}
			}
			temp_s = find_T_name(petri, temp_s);
			v2.push_back(temp_s);
			petri.Add_Place_exit(last_func, v2);
		}
		else
		{
			while (temp_p->next->child->type != �������)
			{
				temp_p = temp_p->next;
			}
			string last_P = find_P_name(petri, temp_p->child->place);
			v2 = petri.get_exit(last_P);
			petri.Add_Place_exit(last_func, v2);
		}
	}

	declaration(petri, p->next, func);
}

void intofile(C_Petri petri)
{
	ofstream out;
	out.open("output.txt", ios::out);
	//out << "Place:" << endl;
	//out << "-----------------------------------" << endl;

	string fillcolor="chartreuse";
	for (int i = 0; i < petri.p_num; i++)
	{
		if (petri.place[i].controlP == false)
			out << "subgraph cluster_" << petri.place[i].name << "{label=\"" <<
			petri.place[i].v_name << "\"color=\"white\"" << petri.place[i].name <<
			"[shape=circle, style=\"filled\",color=\"black\",fillcolor=\"" << fillcolor << "\"]}" << endl;
		else
		{
			out << petri.place[i].name << "[shape=circle]" << endl;
		}
	}
	//out << "-----------------------------------" << endl; 
	//out << "Transition:" << endl;
	//out << "-----------------------------------" << endl;
	for (int i = 0; i < petri.t_num; i++)
	{
		out << petri.transition[i].name << "[shape=box]" << endl;
	}
	//out << "-----------------------------------" << endl;
	//out << "Arc:" << endl;
	//out << "-----------------------------------" << endl;

	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].V != "#" && petri.arc[i].V != "executed#" && petri.arc[i].V != "executed")//��ʽ��
			out << "{" << petri.arc[i].source << "," << petri.arc[i].target << "}" << endl;
		else if(petri.arc[i].V=="executed")
			out << "{" << petri.arc[i].source << "," << petri.arc[i].target << "[style=\"dashed\"]}" << endl;
	}
	out.close();
}

void readGraph(string input, string output) //.txt ת .dot
{

	const char* in = input.data();
	const char* ou = output.data();

	ifstream fin;
	fin.open(in, ios::in);

	ofstream fout;
	fout.open(ou, ios::out);

	fout << "digraph G{" << endl << "rankdir = LR" << endl;
	string s;
	while (getline(fin, s))
	{
		if (s[0] != '{') {
			fout << s << '\n';
			continue;
		}

		string u, v, lable;
		int n = s.length();
		int i = 1;
		//cout << s << n << endl;
		while (s[i] != ',') i++;
		//cout << i << endl;
		u += s.substr(1, i - 1);
		//cout << u << endl;
		int j = n - 2;
		while (s[j] != ',') j--;
		//cout << j << endl;
		v += s.substr(j + 1, n - 1 - j - 1);
		//cout << v << endl;
		//lable = s.substr(i + 1, j - i - 1);

		string edge = "";
		edge += u;
		edge += "->";
		edge += v;
		//edge += "[label=\"";
		//edge += lable;
		//edge += "\"];";
		fout << edge << endl;
	}
	fout << "}" << endl;
	fin.close();
	fout.close();
}

void makeGraph(string inputname, string outputname) //����pngͼƬ
{
	string s = "";
	s += "dot -Tpng ";
	s += inputname;
	s += " -o ";
	s += outputname;
	const char* cmd = s.data();
	const char* iname = inputname.data();
	system(cmd);
}

void create_RG(RG &rg)
{
	stack<int> newNode;
	newNode.push(0);
	while (!newNode.empty())
	{
		int node_id = newNode.top();
		newNode.pop();
		rg.add_next(node_id, newNode);
	}
}

void print_RG(RG rg)
{
	for (int i = 0; i < rg.node_num; i++)
	{
		cout << i << endl;
		for (unsigned int j = 0; j < rg.rgnode[i].m.size(); j++)
		{
			int n_n = rg.rgnode[i].m[j].n_n;
			cout << "(" << rg.petri.place[j].v_name << "," << rg.rgnode[i].m[j].token_num << ",";
			for (int k = 0; k < n_n; k++)
				cout << rg.rgnode[i].m[j].n[k] << " ";
			cout << ")    ";
		}
		cout << endl;
		cout << "��̽ڵ�:";
		for (unsigned int j = 0; j < rg.rgnode[i].next.size(); j++)
		{
			cout << rg.rgnode[i].next[j].num << "    ������Ǩ:" << rg.rgnode[i].next[j].T << "    ";
		}
		cout << endl;
	}
}


//evolution���ֽ���
//********************//
//********************//

bool exist_in(vector<string> v, string s);

void initializing(C_Petri &petri)
{
	///��ʼ��main��token��ֵ
	for (int i = 0; i < petri.p_num; i++)
	{
		if (petri.place[i].v_name == "main begin")
		{
			petri.place[i].token_num = 1;
			break;
		}
	}

	string main_v = find_P_name(petri, "main_v");
	vector<string> main_exit_T = petri.get_exit(find_P_name(petri, "main begin"));
	for (unsigned int i = 0; i < main_exit_T.size(); i++)
		petri.Add_Arc(main_exit_T[i], main_v, "", false);
}

void initial_changeAnalyse_cpn(C_Petri &petri1, C_Petri &petri, vector<string> change_P, vector<string> change_T, vector<Arc> &change_Arc);

int main()
{
	C_Petri petri;
	char content[MAX_LENGTH];
	ifstream fin;
	fin.open("program.txt", ios::in);
	if (!fin.is_open())
	{
		cout << "open failed!" << endl;
	}
	fin.read(content, MAX_LENGTH);
	fin.close();

	gtree *tree = create_tree(content, ����, gen_if_num1, gen_while_num1, gen_break_num1, gen_fun_num1);
	//Traverse(tree);
	declaration(petri, tree, "");

	initializing(petri);

	intofile(petri); //���CPN

	readGraph("output.txt", "output.dot");
	makeGraph("output.dot", "output.png");
	//cout << tb1[0].name << endl;
	//cout << tb1[1].name << endl;
	//getch();  
	vector<string> v;
	RG rg(petri); //����ɴ�ͼ
	create_RG(rg);
	print_RG(rg);


	//�ݻ�����
	vector<string> change_places;
	//	vector<string> array_this_sentence, array_this_sentence_after, array_father_sentence, array_last_sentence, array_next_sentence, array_next_father_sentence, array_next_next_sentence;
	vector<int> array_flag;
	//program_compare("program.txt", "program1.txt", array_this_sentence, array_father_sentence, array_this_sentence_after, array_last_sentence, array_next_sentence, array_next_father_sentence, array_next_next_sentence, array_flag);
	//delete_places = evolution(array_this_sentence, array_father_sentence, array_this_sentence_after, array_last_sentence, array_next_sentence, array_next_father_sentence, array_next_next_sentence, array_flag, change_places, tree);
	vector<AST_change> changes = compare_AST("program.txt", "program1.txt");
	change_places = evolution(petri, changes);

	intofile(petri); //���CPN

	readGraph("output.txt", "output.dot");
	makeGraph("output.dot", "output.png");
	RG rg1(petri); //����ɴ�ͼ
	create_RG(rg1);
	print_RG(rg1);


	//�仯Ӱ���������
//	vector<string> place;
	string main_begin = find_P_name(petri, "main begin");
	change_places.push_back(main_begin);

	C_Petri petri1;
	petri1 = changeAnalyse(petri, change_places);
	intofile(petri1); //���CPN

	readGraph("output.txt", "output.dot");
	makeGraph("output.dot", "output.png");
	RG rg2(petri1); //����ɴ�ͼ
	create_RG(rg2);
	print_RG(rg2);


}

