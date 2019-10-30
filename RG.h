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
	int num; //��һ���ڵ�ı��
	string T; //�����ı�Ǩ��

};

class RGNode
{
public:
	//string name; //�ýڵ������
	int num; // �ýڵ�ı��
	int enableNum; //�ɷ�����Ǩ��
	vector<M> m; //��״̬ÿһ��������M��ʶ
	vector<int> isfirable;  //�ɷ�����Ǩ����
	vector<nextNode> next; //��̽ڵ�ļ���
	bool end;//�˵���
};

class RG
{
public:
	int node_num; //�ڵ����
	C_Petri petri;
	vector<RGNode> rgnode; //�ɴ�ͼ���ڽӱ���ʽ

public:
	RG(C_Petri petri);
	int create_node(C_Petri petri, vector<string> T);
	void add_next(int node_num, stack<int> &newNode);

};

string find_P_name_1(C_Petri petri, string v_name, int current);