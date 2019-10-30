#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<stack>
#include<sstream>
using namespace std;
//有色Petri网的结构

//const string colorset[] = { "int","char","float","double","short","long" };

class Arc
{
public:
	int static id_num;

	string source;//源的名称
	string target;//目标的名称
	string V;//记录弧上的变量
	int sourceNum = 0;
	int targetNum = 0;
	bool sourceP;//false代表源节点是变迁

//	int weight = 1;//记录弧上的权值

	Arc(string s, string t, string v, bool s_P);
};


class Place
{
public:
	static int id_num;

	string name;//库所的名称（用于弧的标记）
	string v_name;//用来存放变量库所的变量名
	string str;//colorset为string或者char时存储值
	int token_num;//token的个数
	string colorset_tag;//对应colorset的数值
	int *num;//colorset为整型时存储值
	int n_num;//num数量
	double *decimal;//colorset为小数时存储值
	int n_decimal;//decimal数量
	bool controlP;//区分控制库所还是变量库所

	vector<string> enter;//标记开始变迁
	vector<string> exit;//标记结尾变迁
//	vector<string> c_transition;//记录条件语句和循环语句的控制变迁

	int flag;//用于一些特殊标记,1代表while语句条件内有函数调用
	vector<string> information;//用于传递flag标注出来的信息
	bool call_last;//函数调用上一语句
	bool point_flag;//表示是否为指针

	Place(string n, string v_n, string colorset_t, bool c_P);
	void set_Place_value(int num);
	void set_Place_value(double decimal);
	void set_Place_value(string str);

};

class Transition
{
public:
	static int id_num;

	string name;//变迁的名称（用于弧的标记）
	string booleanExpression;//控制哨用字符串存储bool表达式
	string v_Expression;//变量哨存放变量表达式
//	int num = 0;
	int current_P_num;
	bool controlT;//区分控制哨还是变量哨
	//bool TorF;//控制哨的值

	Transition(string n, bool c_T, int current_P_num);
	void set_C_Transition_value(string str);
	void set_V_Transition_value(string str);
};

class C_Petri
{
public:
	int p_num;//库所个数
	int t_num;//变迁个数
	int arcnum;//弧个数


	vector<Place> place;//库所
	vector<Transition> transition;//变迁
	vector<Arc> arc;//弧


public:
	C_Petri();

	void Add_Place(Place p);
	void Add_Place(string name, string v_name, string colorset_tag, bool controlP, bool t, int n, double dec, string s, int array_size);//t=true表示有初始值


	void Add_Transition(Transition t);
	void Add_Transition(string name, bool controlT, string s,int current_P_num);


	void Add_Arc(Arc a);
	void Add_Arc(string source, string target, string V, bool sourceP);

	string find_place(string t_name, string V);//通过变迁和弧上的变量值找到对应库所,返回库所name

	bool Add_Place_enter(string name, string s);
	bool Add_Place_enter(string name, vector<string> s);
	bool Add_Place_exit(string name, string s);
	bool Add_Place_exit(string name, vector<string> s);
//	bool Add_Place_controlT(string name, vector<string> s);
	bool is_enable(string t);//判断变迁t是否可发生
	bool guard(string booleanExpression,string T); //guard函数的处理
	bool guard_process(string booleanExpression, string T);
	string Delete_Arc(int i);
	string Delete_Arc(string source, string target);
	void Change_Arc(int i, string source, string target, string V, bool sourceP);
	void delete_all_connected_arc(string place);

	vector<string> get_enter(string name);
	vector<string> get_exit(string name);
	
//	vector<string> get_controlT(string name);
	vector<string> enable_T(); //返回网内所有可发生的变迁
	vector<string> find_all_place(string t_name);//找变迁的所有前驱库所
	
	int get_flag(string name);
	void set_flag(string name, int flag);
	vector<string> get_information(string name);
	void Add_information(string name, string information);
	bool get_call_last(string name);
	void set_call_last(string name, bool call_last);
	int get_current_P_num(string T);
	void set_point_flag(string p_name);
	bool get_point_flag(string p_name);
};

int priority(char c);
bool is_operator(char c);
double get_value(string s, vector<Place> place,int current);
double cal(double top_1, double top_2, char c);

string find_P_name(C_Petri petri, string v_name);
string find_T_name(C_Petri petri, string v_name);
double change_and_cal(string s, vector<Place> place,int current);

C_Petri changeAnalyse(C_Petri &petri, vector<string> change_places);