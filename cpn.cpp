#include"cpn.h"


string colorset[] = { "int","char","float","double","string" };

int Arc::id_num = 0;
int Place::id_num = 0;
int Transition::id_num = 0;


Arc::Arc(string s, string t, string v, bool s_P)
{
	source = s;
	target = t;
	V = v;
	sourceP = s_P;
	id_num++;
}

Place::Place(string n, string v_n, string colorset_t, bool c_P)
{
	name = n;
	v_name = v_n;
	colorset_tag = colorset_t;
	controlP = c_P;
	id_num++;
	point_flag = false;
}

void Place::set_Place_value(int n)
{
	num[0] = n;
	token_num = 1;
}

void Place::set_Place_value(double dec)
{
	decimal[0] = dec;
	token_num = 1;
}

void Place::set_Place_value(string s)
{
	str = s;
	token_num = 1;
}

Transition::Transition(string n, bool c_T,int current)
{
	name = n;
	controlT = c_T;
	current_P_num = current;
	id_num++;
}

void Transition::set_C_Transition_value(string str)
{
	booleanExpression = str;
}

void Transition::set_V_Transition_value(string str)
{
	v_Expression = str;
}

C_Petri::C_Petri()
{

}

void C_Petri::Add_Arc(Arc a)
{
	arc.push_back(a);
	arcnum++;
}

void C_Petri::Add_Arc(string source, string target, string V, bool sourceP)
{
	int sourceid, targetid;
	Arc a(source, target, V, sourceP);
	for (vector<Arc>::iterator it = arc.begin(); it != arc.end(); it++)
	{
		if (it->source == source && it->target == target)
		{

			if (it->V == V)
			{
				cout << "重复添加边" << source << "-->" << target << endl;
				//arcnum--;
				return;
			}
			else
			{
				cout << "覆盖边" << source << "-->" << target << endl;
				arc.erase(it);
				arcnum--;
				break;
			}
		}
	}
	if (sourceP)
	{
		int i, j;
		for (i = 0; i < p_num; i++)
		{
			if (place[i].name == source)
			{
				sourceid = place[i].id_num;
				break;
			}
		}
		if (i == p_num)
		{
			cout << "can't find source:" << source << endl;
			exit(1);
		}
		for (j = 0; j < t_num; j++)
		{
			if (transition[j].name == target)
			{
				targetid = transition[j].id_num;
				break;
			}
		}
		if (j == t_num)
		{
			cout << "can't find target:" << target << endl;
			exit(1);
		}
	}
	else
	{
		int i, j;
		for (i = 0; i < p_num; i++)
		{
			if (place[i].name == target)
			{
				targetid = place[i].id_num;
				break;
			}
		}
		if (i == p_num)
		{
			cout << "can't find target!" << endl;
			exit(1);
		}
		for (j = 0; j < t_num; j++)
		{
			if (transition[j].name == source)
			{
				sourceid = transition[j].id_num;
				break;
			}
		}
		if (j == t_num)
		{
			cout << "can't find source!" << endl;
			exit(1);
		}
	}
	a.sourceNum = sourceid;
	a.targetNum = targetid;
	Add_Arc(a);
}

void C_Petri::Add_Place(Place p)
{
	place.push_back(p);
	p_num++;
}

void C_Petri::Add_Place(string name, string v_name, string colorset_tag, bool controlP, bool t, int n, double dec, string s, int array_size)
{
//	int judge = find_exist(place, v_name);
//	if (judge != 0)
//	{
//		char temp[5];
//		itoa(judge, temp, 10);
//		v_name = v_name + "@" + temp;
//	}
	Place place(name, v_name, colorset_tag, controlP);
	if (!t)
	{
		//		place.token_num = 0;//表示未赋初值
		if (colorset_tag == "")
		{

		}
		else
		{
			switch (colorset_tag[1])
			{
			case '1':
			case '5':
			case '6':
				place.num = new int[array_size];
				place.n_num = array_size;
				break;
			case '3':
			case '4':
				place.decimal = new double[array_size];
				place.n_decimal = array_size;
				break;
			default:
				break;
			}
		}
		if (controlP == true)
			place.token_num = 0;
		else
			place.token_num = 1;
		Add_Place(place);
	}
	else
	{
		if (controlP == true)
			place.token_num = 0;
		else
			place.token_num = 1;
	
		switch (colorset_tag[1])
		{
		case '1':
		case '5':
		case '6':
			place.num = new int[array_size];
			place.n_num = array_size;
			place.set_Place_value(n);
			break;
		case '3':
		case '4':
			place.decimal = new double[array_size];
			place.n_decimal = array_size;
			place.set_Place_value(dec);
			break;
		default:
			place.set_Place_value(s);
			break;
		}
		Add_Place(place);
	}
}

void C_Petri::Add_Transition(Transition t)
{
	transition.push_back(t);
	t_num++;
}

void C_Petri::Add_Transition(string name, bool controlT, string s,int current)
{
	Transition t(name, controlT, current);
	if (controlT)
		t.set_C_Transition_value(s);
	else
		t.set_V_Transition_value(s);
	Add_Transition(t);


}

string C_Petri::find_place(string t_name, string V)
{
	for (int i = 0; i < arcnum; i++)
	{
		if ((arc[i].sourceP == true) && (arc[i].target == t_name) && arc[i].V == V)
		{
			return arc[i].source;
		}
	}
	cout << "can't find place" << endl;
	return "";
}

bool C_Petri::Add_Place_enter(string name, string s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].enter.push_back(s);
			return true;
		}
	}
	return false;
}

bool C_Petri::Add_Place_enter(string name, vector<string> s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].enter.insert(place[i].enter.end(), s.begin(), s.end());
			return true;
		}
	}
	return false;
}

bool C_Petri::Add_Place_exit(string name, string s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].exit.push_back(s);
			return true;
		}
	}
	return false;
}

bool C_Petri::Add_Place_exit(string name, vector<string> s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].exit.insert(place[i].exit.end(), s.begin(), s.end());
			return true;
		}
	}
	return false;
}

vector<string> C_Petri::get_enter(string name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			return place[i].enter;
		}
	}
	cout << "error in get_enter()" << endl;
	exit(1);
}

vector<string> C_Petri::get_exit(string name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			return place[i].exit;
		}
	}
	cout << "error in get_exit()" << endl;
	exit(1);
}

int C_Petri::get_flag(string name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			return place[i].flag;
		}
	}
	cout << "error in get_flag()" << endl;
	exit(1);
}

bool C_Petri::get_call_last(string name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			return place[i].call_last;
		}
	}
	cout << "error in get_call_last()" << endl;
	exit(1);
}

vector<string> C_Petri::get_information(string name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			return place[i].information;
		}
	}
	cout << "error in get_information()" << endl;
	exit(1);
}

void C_Petri::set_flag(string name, int flag)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].flag = flag;
			break;
		}
	}
	cout << "error in set_flag()" << endl;
	exit(1);
}

void C_Petri::set_call_last(string name, bool flag)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].call_last = flag;
			break;
		}
	}
	cout << "error in set_call_last()" << endl;
	exit(1);
}

void C_Petri::Add_information(string name, string information)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].information.push_back(information);
			break;
		}
	}
	cout << "error in set_information()" << endl;
	exit(1);
}

bool C_Petri::is_enable(string t_name)
{
	vector<string> all_P;
	all_P = find_all_place(t_name);
	bool flag = true;
	for (unsigned int i = 0; i < all_P.size(); i++)
	{
		for (int j = 0; j < p_num; j++)
		{
			if (place[j].name == all_P[i])
			{
				if (place[j].token_num < 1)
				{
					flag = false;
					break;
				}
			}
		}
	}

	string guard_Expression;
	for (int i = 0; i < t_num; i++)
	{
		if (transition[i].name == t_name)
		{
			guard_Expression = transition[i].booleanExpression;
			break;
		}
	}
	if (guard_Expression != "")
	{
		bool flag1 = guard(guard_Expression, t_name);
		if (flag1 == true && flag == true)
			return true;
		else
			return false;
	}
	else
	{
		if (flag == false)
			return false;
		else
			return true;
	}

}

vector<string> C_Petri::enable_T()
{
	vector<string> T;
	for (int i = 0; i < t_num; i++)
	{
		if (is_enable(transition[i].name) == true)
		{
			T.push_back(transition[i].name);
		}
	}

	return T;
}

void SplitString_1(const string& s, vector<string>& v, const string& c)
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

void splitExpression_1(string &s, vector<string>& v)//传入字符串和结果集合，将字符串根据>,<等分割并放入结果集
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
				cout << "出现异常!=或==" << endl;
			v.push_back(s1);
			s1.clear();
		}
		else
			s1 = s1 + s[i];
	}
	v.push_back(s1);
}

int priority(char c)
{
	if (c == '#')
		return -1;
	else
	{
		switch (c)
		{
		case'(':
			return 0;
			break;
		case '+':
		case '-':
			return 1;
			break;
		case '*':
		case '/':
			return 2;
			break;
		case '&':
		case '|':
			return 3;
		case ')':
			return 4;
		}
	}
	cout << "priority error!" << endl;
	return -2;
}

bool is_operator(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '#' || c == '&' || c == '|' || c == '^')
	{
		return true;
	}
	return false;
}

double get_value(string s, vector<Place> place,int current)//通过变量名寻找变量库所value的值，未考虑数组
{
	int position = 0;
	int array_num = 0;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == '[')
		{
			position = i;
			string temp_s;
			for (unsigned int j = i + 1; j < s.length(); j++)
			{
				if (s[j] == ']')
					break;
				temp_s += s[j];
			}
			array_num = int(change_and_cal(temp_s, place, current));
			break;
		}
	}
	if (position != 0)
		s = s.substr(0, position);

	if (s[0] >= '0'&&s[0] <= '9' || (s[0] == '-'&&s[1] >= '0'&&s[1] <= '9'))
	{
		stringstream ss;
		ss << s;
		double temp;
		ss >> temp;
		return temp;
	}
	for (int i = current < int(place.size()) ? current - 1 : int(place.size()) - 1; i >= 0; i--)
	{
		string name = place[i].name;
		name = name.substr(1);
		int name_num = atoi(name.c_str());
		if (name_num < current)
		{
			vector<string> v;
			SplitString_1(place[i].v_name, v, "@");
			if (s == v[v.size() - 1])
			{
				if (place[i].colorset_tag[0] == '1' || place[i].colorset_tag[1] == '1' || place[i].colorset_tag[0] == '5' || place[i].colorset_tag[1] == '5' || place[i].colorset_tag[0] == '6' || place[i].colorset_tag[1] == '6')
					return place[i].num[array_num];
				else if (place[i].colorset_tag[0] == '3' || place[i].colorset_tag[1] == '3' || place[i].colorset_tag[0] == '4' || place[i].colorset_tag[1] == '4')
					return place[i].decimal[array_num];
				else
				{
					cout << "库所colortag出错!" << endl;
					return -1;
				}
			}
		}
	}
	cout << "get_value error!" << endl;
	exit(1);
}

double cal(double top_1, double top_2, char c)
{
	if (c == '+')
		return top_1 + top_2;
	else if (c == '-')
		return top_1 - top_2;
	else if (c == '*')
		return top_1 * top_2;
	else if (c == '/')
		return top_1 / top_2;
	else if (c == '&')
		return int(top_1) & int(top_2);
	else if (c == '|')
		return int(top_1) | int(top_2);
	else if (c == '^')
		return int(top_1) ^ int(top_2);
	else
		return -1;
}

double change_and_cal(string s, vector<Place> place, int current)//将中缀式s转换成后缀式并且将变量进行代换
{
	if (s == "" || s == "executed")
		return 0;
	string temp_s;


	stack<double> Stack;
	stack<char> Stack_c;
	Stack_c.push('#');
	s.append(1, '#');
	bool ignore = false;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == ' ')
			continue;
		if (s[i] == '[')
			ignore = true;
		if (s[i] == ']')
			ignore = false;
		if (is_operator(s[i]) == true && ignore == false && i != 0)
		{
			if (i >= 1 && is_operator(s[i - 1]) == false)
			{
				double value = get_value(temp_s, place, current);
				temp_s.clear();
				Stack.push(value);
			}
			if (s[i] == '(')
			{
				Stack_c.push(s[i]);
			}
			else if (s[i] == ')')
			{
				while (Stack_c.top() != '(')
				{
					double top_1 = Stack.top();
					Stack.pop();
					double top_2 = Stack.top();
					Stack.pop();
					char c = Stack_c.top();
					Stack_c.pop();
					double res = cal(top_2, top_1, c);
					Stack.push(res);
				}
				Stack_c.pop();
			}
			else
			{
				while (priority(s[i]) <= priority(Stack_c.top()))
				{
					if (s[i] == '#' && Stack_c.top() == '#')
					{
						double result = Stack.top();
						Stack.pop();
						return result;
					}
					double top_1 = Stack.top();
					Stack.pop();
					double top_2 = Stack.top();
					Stack.pop();
					char c = Stack_c.top();
					Stack_c.pop();
					double res = cal(top_2, top_1, c);
					Stack.push(res);



				}
				Stack_c.push(s[i]);
			}
		}
		else
			temp_s.append(1, s[i]);
	}
	cout << "change error!" << endl;
	return -1;
}

bool C_Petri::guard_process(string booleanExpression,string T)
{
	vector<string> v;
	int flag;
	if (booleanExpression.find(">=") != string::npos)
	{
		flag = 0;//>=
		SplitString_1(booleanExpression, v, ">=");
	}
	else if (booleanExpression.find("<=") != string::npos)
	{
		flag = 1;//<=
		SplitString_1(booleanExpression, v, "<=");
	}
	else if (booleanExpression.find("<") != string::npos)
	{
		flag = 2;//<
		SplitString_1(booleanExpression, v, "<");
	}
	else if (booleanExpression.find(">") != string::npos)
	{
		flag = 3;//>
		SplitString_1(booleanExpression, v, ">");
	}
	else if (booleanExpression.find("==") != string::npos)
	{
		flag = 4;//==
		SplitString_1(booleanExpression, v, "==");
	}
	else if (booleanExpression.find("!=") != string::npos)
	{
		flag = 5;//!=
		SplitString_1(booleanExpression, v, "!=");
	}
	else
		flag = 6;
	if (flag == 6)
	{
		double value = change_and_cal(booleanExpression, place, get_current_P_num(T));
		if (value == 0)
			return false;
		else
			return true;
	}
	if (v.size() == 2)
	{
		double left = change_and_cal(v[0], place, get_current_P_num(T));
		double right = change_and_cal(v[1], place, get_current_P_num(T));
		if (flag == 0)
		{
			if (left >= right)
				return true;
			else
				return false;
		}
		else if (flag == 1)
		{
			if (left <= right)
				return true;
			else
				return false;
		}
		else if (flag == 2)
		{
			if (left < right)
				return true;
			else
				return false;
		}
		else if (flag == 3)
		{
			if (left > right)
				return true;
			else
				return false;
		}
		else if (flag == 4)
		{
			if (left == right)
				return true;
			else
				return false;
		}
		else if (flag == 5)
		{
			if (left != right)
				return true;
			else
				return false;
		}
		else
		{
			cout << "guard error!" << endl;
			exit(1);
		}
	}
	else if (v.size() == 1)
	{
		double res = change_and_cal(v[0], place, get_current_P_num(T));
		if (res != 0)
			return true;
		else
			return false;
	}
	else
	{
		cout << "guard error! no expression!" << endl;
		exit(1);
	}
}

bool C_Petri::guard(string booleanExpression,string T)
{
	stack<bool> Stack;
	stack<char> Stack_c;
	string temp_s = "";
	for (unsigned int i = 0; i < booleanExpression.length(); i++)
	{
		if (booleanExpression[i] == '&' || booleanExpression[i] == '|' || booleanExpression[i] == '(' || booleanExpression[i] == ')')
		{
			if (booleanExpression[i] == '&' || booleanExpression[i] == '|')
			{

				i++;
			}
			if (booleanExpression[i] != '(')
			{
				bool value = guard_process(temp_s,T);
				temp_s.clear();
				Stack.push(value);
			}

		}
		else
			temp_s += booleanExpression[i];
		if (booleanExpression[i] == '(')
		{
			Stack_c.push(booleanExpression[i]);
		}
		else if (booleanExpression[i] == ')')
		{
			while (Stack_c.top() != '(')
			{
				bool top_1 = Stack.top();
				Stack.pop();
				bool top_2 = Stack.top();
				Stack.pop();
				char c = Stack_c.top();
				Stack_c.pop();
				bool res;
				if (c == '&')
					res = top_1 && top_2;
				else
					res = top_1 || top_2;
				Stack.push(res);
			}
			Stack_c.pop();
		}
		else if (booleanExpression[i] == '&' || booleanExpression[i] == '|')
		{
			while (!Stack_c.empty()&&Stack_c.top() != '(')
			{
				bool top_1 = Stack.top();
				Stack.pop();
				bool top_2 = Stack.top();
				Stack.pop();
				char c = Stack_c.top();
				Stack_c.pop();
				bool res;
				if (c == '&')
					res = top_1 && top_2;
				else
					res = top_1 || top_2;
				Stack.push(res);
			}
			Stack_c.push(booleanExpression[i]);
		}
		
	}
	if (Stack_c.empty())
	{
		return guard_process(temp_s,T);
	}
	else
	{
		if (!Stack.empty())
		{
			if (temp_s.length() != 0)
			{
				bool top_1 = Stack.top();
				Stack.pop();
				bool top_2 = guard_process(temp_s,T);
				char c = Stack_c.top();
				Stack_c.pop();
				if (c == '&')
					return top_1 && top_2;
				else
					return top_1 || top_2;
			}
			else
			{
				bool top_1 = Stack.top();
				Stack.pop();
				return top_1;
			}
		}
		else
		{
			return guard_process(temp_s,T);
		}

	}
}//a&&b||c&&(d||e)

vector<string> C_Petri::find_all_place(string t_name)
{
	vector<string> v;

	for (int i = 0; i < arcnum; i++)
	{
		if (arc[i].sourceP == true && arc[i].V != "#" && arc[i].V!="executed#")//隐式弧
			if (arc[i].target == t_name)
				v.push_back(arc[i].source);
	}
	return v;
}

string C_Petri::Delete_Arc(int num)
{
	int i = 0;
	for (vector<Arc>::iterator it = arc.begin(); it != arc.end();it++,i++)
	{
		if (i == num)
		{
//			arc.erase(it);
//			arcnum--;
			string temp = it->V;
			if (it->V == "executed")
				it->V = "executed#";
			else
				it->V = "#";
			return temp;
		}
	}
	return "";
}

string C_Petri::Delete_Arc(string source, string target)
{
	for (vector<Arc>::iterator it = arc.begin(); it != arc.end(); it++)
	{
		if (it->source == source && it->target == target)
		{
//			arc.erase(it);
//			arcnum--;
			string temp = it->V;
			if (it->V == "executed")
				it->V = "executed#";
			else
				it->V = "#";
			return temp;
		}
	}
	return "";
}

void C_Petri::Change_Arc(int num, string source, string target, string V, bool sourceP)
{
	for (int i = 0; i < arcnum; i++)
	{
		if (i == num)
		{
			arc[i].source = source;
			arc[i].target = target;
			arc[i].V = V;
			arc[i].sourceP = sourceP;
		}
	}
}

void C_Petri::delete_all_connected_arc(string place)
{
	for (int i = 0; i < arcnum; i++)
	{
		if (arc[i].source == place || arc[i].target == place)
		{
			Delete_Arc(i);
		}
	}
}

int C_Petri::get_current_P_num(string T)
{
	for (unsigned int i = 0; i < t_num; i++)
	{
		if (transition[i].name == T)
			return transition[i].current_P_num;
	}
}

void C_Petri::set_point_flag(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			place[i].point_flag = true;
			return;
		}
	}
}

bool C_Petri::get_point_flag(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			return place[i].point_flag;
		}
	}
}