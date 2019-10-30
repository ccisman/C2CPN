#pragma once
/*�ʷ�������*/
#include<iostream>
#include<string>
#include<vector>
#include<regex>
using namespace std;
#define ERROR 0
#define OK 1
#define $Neg 1     //! ��
#define $Impli 2   //-> �̺�
#define $Equ 3    //<-> �ȼ�
#define $And 4   //&& ��ȡ
#define $Or 5     //|| ��ȡ
#define $Alw 6    // [] G
#define $Eve 7    // <> F
#define $Until 8   // U
#define $Rele 9   // R
#define $Next 10  // X
#define $Lpar 11   // (
#define $Rpar 12  // )
#define $ID 13      //ԭ������
#define $End 14
#define $root 15

//该状态的label集合，以如下方式给出：{} && {} && {}，其中每一个{}就是一个原子命题
//如果是LTLFirability公式，大括号里是如下形式：{t1,t2,...}，其中t1,t2表示变迁的名字
//如果是LTLCardinality公式，大括号里面是如下形式{token-count(p1,p3,...)<=token-count(p,p,...)}，其中pn表示库所的名字
//注意！：label集合中的原子命题可能会出现其否定形式，即!{}的形式，例如 !{}&&!{}&&{}
//注意！：LTLCardinality公式中token-count()可能会是一个常数的形式，例如 {2<=token-count(p,p,...)}或{token-count(p1,p3,...)<=2}
//建议的处理方式：首先对label这个string，以“&&”符号为切割符，分出一个一个 {} 或 !{} ,然后再对{}解析
//判断{}里是否出现“<=”符号，如果有则表示是LTLCardinality公式，否则则表示是LTLFirability公式
//如果是LTLCardinality公式，则以“<=”为切割符号，分出前后两部分，如果是常数就无需处理，如果是token-count()形式，则以“,”为切割符号得到一个一个p
//如果是LTLFirability公式，则以“,”为切割符号，分出一个一个t

typedef struct Lexer_word
{
	string character;
	int typenum;
}word;

class Lexer    //�ʷ�������
{
private:
	char formula[10000];
	int pointer;
	int length;
public:
	Lexer(char *form, int len);
	int GetWord(word &w);
};

class AtmoicProps
{
public:
	vector<word> APs;
	vector<string> place;
	vector<string> transition;
	void GetAllAPs(Lexer lex);
	void AP2PT();
	void extract_PT(string s);
};
