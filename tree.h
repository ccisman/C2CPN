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
	�ؼ��� = 1, ��ʶ��, �޷�������, ��ֵ��, ���, ���, ð��, �ָ���, ע�ͺ�, ������, ������, ��������, ��������, �������, �Ҵ�����, ��ĸ, �޷���ʵ��, ������, �ַ���, �ַ�,
	����, ����˵��, ��������, ����, ʵ��, ����ͷ��, ����˵��, ��������, ��ö�ٳ���, ���ͱ�ʶ��, ��������, �������, ������, ������, ���ʽ,
	��, ����, ���, ��ֵ���, �������, ����, ѭ�����, ������, �����, ��������, ȱʡ, �����������, ֵ������, �����, �����, д���, �������, ������,����,break���
};




class gtree
{
public:
	//�������֣����������Ϣ
	int num;//����ڵ��
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
	//�������֣����������Ϣ
	gtree *next;//�ֵ�ָ��
	gtree *child;//����ָ��
	gtree *parent;//���ڵ�ָ��
	int type;//�ڵ�����syn
	int type_1;//�ڵ��������syn_1,�������͹ؼ���
	void init(int root);//��ʼ��
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