#include"Lexer.h"

Lexer::Lexer(char *form, int len)
{
	length = len;
	pointer = 0;
	for (int i = 0; i < len; i++)
	{
		formula[i] = form[i];
	}
	formula[len] = '\0';
}

int Lexer::GetWord(word &w)
{
	switch (formula[pointer++])
	{
	case '!': {
		w.character = "!";
		w.typenum = $Neg;
		return OK;
		break;
	}
	case '-': {
		if (formula[pointer] == '>')
		{
			pointer++;
			w.character = "->";
			w.typenum = $Impli;
			return OK;
		}
		else
			return ERROR;
		break;
	}
	case '<': {
		if (formula[pointer] == '-'&&formula[pointer + 1] == '>')
		{
			pointer += 2;
			w.character = "<->";
			w.typenum = $Equ;
			return OK;
		}
		else if (formula[pointer] == '>')
		{
			pointer++;
			w.character = "<>";
			w.typenum = $Eve;
			return OK;
		}
		else
			return ERROR;
		break;
	}
	case '&': {
		if (formula[pointer] == '&')
		{
			pointer++;
			w.character = "&&";
			w.typenum = $And;
			return OK;
		}
		else
			return ERROR;
		break;
	}
	case '|': {
		if (formula[pointer] == '|')
		{
			pointer++;
			w.character = "||";
			w.typenum = $Or;
			return OK;
		}
		else
			return ERROR;
		break;
	}
	case '[': {
		if (formula[pointer] == ']')
		{
			pointer++;
			w.character = "[]";
			w.typenum = $Alw;
			return OK;
		}
		else
			return ERROR;
		break;
	}
	case 'U': {
		w.character = "U";
		w.typenum = $Until;
		return OK;
		break;
	}
	case 'V': {
		w.character = "V";
		w.typenum = $Rele;
		return OK;
		break;
	}
	case 'X': {
		w.character = "X";
		w.typenum = $Next;
		break;
	}
	case '(': {
		w.character = "(";
		w.typenum = $Lpar;
		return OK;
		break;
	}
	case ')': {
		w.character = ")";
		w.typenum = $Rpar;
		return OK;
		break;
	}
	case '\0': {
		return $End;
	}
	case '{': {
		char p[10000];
		int i = 0;
		while (formula[pointer - 1] != '}')
		{
			p[i++] = formula[pointer - 1];
			pointer++;
		}
		p[i++] = formula[pointer - 1];
		p[i] = '\0';
		w.character = p;
		w.typenum = $ID;
		return OK;
	}
	case '#': {
		return $End;
		break;
	}
	default: {
		/*char p[10];
		int i = 0;
		while (formula[pointer]!='\0' && formula[pointer] != '!' && formula[pointer] != '-'
			&& formula[pointer] != '<' && formula[pointer] != '&' && formula[pointer] != '|'
			&& formula[pointer] != '[' && formula[pointer] != 'U' && formula[pointer] != 'R'
			&& formula[pointer] != 'X' && formula[pointer] != '(' && formula[pointer] != ')')
		{
			p[i++] = formula[pointer-1];
			pointer++;
		}
		p[i++] = formula[pointer - 1];
		p[i] = '\0';
		w.character = p;
		w.typenum = $ID;
		return OK;
		break;*/
	}
	}
}

void SplitString1(const string& s, vector<string>& v, const string& c)
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

void AtmoicProps::GetAllAPs(Lexer lex) {
	word curw;
	int result;
	result = lex.GetWord(curw);

	while (result != $End)
	{
		if (curw.typenum == $ID)
		{
			APs.push_back(curw);
		}
		result = lex.GetWord(curw);
	}
}


//从形如{T1,T2,}这样的字符串中提取出所有的库所变迁名，放入数组place
void AtmoicProps::extract_PT(string s)
{
	vector<string> v;
	SplitString1(s, v, ",");
	for (unsigned int k = 0; k < v.size(); k++)
		if (v[k].length() > 1)
		{
			if (v[k][0] == 'P')
				place.push_back(v[k]);
			else if (v[k][0] == 'T')
				transition.push_back(v[k]);
			else
			{
				cout << "提取出非T，非P因子：" << v[k] << endl;
				exit(1);
			}
		}
}

void AtmoicProps::AP2PT()
{
	if (APs.size() <= 0)
		return;

	string temp;
	if (APs[0].character[1] == 'T')
	{
		for (unsigned int i = 0; i < APs.size(); i++)
		{
			string target = APs[i].character;
			temp = target.substr(1, target.length() - 2);
			extract_PT(temp);
		}
	}
	regex r(".*?[(](.*?)[)].*");
	for (unsigned int i = 0; i < APs.size(); i++)
	{
		string target = APs[i].character;
		smatch result;

		while (1)
		{
			bool flag = regex_search(target, result, r);
			if (flag == false)
				break;
			int position = result.position(1);
			temp = result[1];
			extract_PT(temp);
			target = target.substr(position);

		}
	}
}
