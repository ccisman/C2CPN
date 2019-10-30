#pragma once

#include "cpn.h"

class M
{
public:
	bool control_P;
	int token_num;
	string color;
	int *n;
	double *dec;
	int n_n;
	int n_dec;
	string s;

public:
	bool operator==(const M m1);
};

class nextNode
{
public:
	int num; //下一个节点的编号
	string T; //发生的变迁名

};

class RGNode
{
public:
	//string name; //该节点的名称
	int num; // 该节点的编号
	int enableNum; //可发生变迁数
	vector<M> m; //该状态每一个库所的M标识
	vector<int> isfirable;  //可发生变迁数组
	vector<nextNode> next; //后继节点的集合
	bool end;//端点标记
};

class RG
{
public:
	int node_num; //节点个数
	C_Petri petri;
	vector<RGNode> rgnode; //可达图，邻接表形式

public:
	RG(C_Petri petri);
	int create_node(C_Petri petri, vector<string> T);
	void add_next(int node_num, stack<int> &newNode);

};

string find_P_name_1(C_Petri petri, string v_name, int current);