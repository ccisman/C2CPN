

//evolution����
//********************//
//********************//

#include"tree.h"
#include"AST_compare.h"
#include"cpn.h"

bool judge_while_P(C_Petri petri, string place)
{
	for (int i = 0; i < petri.p_num; i++)
	{
		if (petri.place[i].name == place)
		{
			if (petri.place[i].v_name.rfind("while", 1) != string::npos)
				return true;
			else
				return false;
		}
	}
	cout << "judge_while_P() can't find place!" << endl;
	exit(1);
}

string create_sentence(string sentence1, int gen_if_num,int gen_while_num,int gen_break_num,int gen_fun_num,int gen_P_num)
{
	string place = "P" + to_string(gen_P_num);
	sentence1 = sentence1 + '#';
	gtree *temp_tree = create_tree(sentence1, ���, gen_if_num, gen_while_num, gen_break_num, gen_fun_num);
//	declaration(temp_tree, "");

	return place;
}

void delete_last_executed(C_Petri &petri, string _P1)
{
	//	string _P1 = find_P_name(petri, v_name);
	vector<string> v1 = petri.get_exit(_P1);
	if (judge_while_P(petri, _P1))
	{
		string temp = v1[v1.size() - 1];
		v1.clear();
		v1.push_back(temp);
	}
	bool flag = false;
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].source == v1[0] && petri.arc[i].V == "executed")
		{
			for (int j = 0; j < petri.p_num; j++)
			{
				if (petri.arc[i].target == petri.place[j].name)
				{
					if (petri.place[j].v_name == "executedP")
					{
						petri.delete_all_connected_arc(petri.place[j].name);
						flag = true;
						break;
					}

				}
			}
			//			petri.delete_all_connected_arc(petri.arc[i].source);
		}
	}
	if (flag == true)
		return;
	for (unsigned int j = 0; j < v1.size(); j++)
	{
		for (int i = 0; i < petri.arcnum; i++)
		{
			if (petri.arc[i].source == v1[j] && petri.arc[i].V == "executed")
			{
				petri.Delete_Arc(i);
			}
		}
	}
}

string add_executed(C_Petri &petri,string P_last, string P_next, bool whileorif)
{
	string P1 = gen_P();
	petri.Add_Place(P1, "executedP", "", true, false, 0, 0, "", 0);
	string P2 = P_next;
	vector<string> v1 = petri.get_exit(P_last);

	vector<string> v2 = petri.get_enter(P2);
	if (whileorif)//while�����ʵ����ֻ��һ��
	{
		petri.Add_Arc(v1[v1.size() - 1], P1, "executed", false);
	}
	else
	{
		for (unsigned int j = 0; j < v1.size(); j++)
		{
			petri.Add_Arc(v1[j], P1, "executed", false);
		}
	}
	for (unsigned int j = 0; j < v2.size(); j++)
	{
		petri.Add_Arc(P1, v2[j], "executed", true);
	}
	return P1;
}

void add_executed_1(C_Petri &petri,string P_last, string P_next, bool whileorif)
{

	vector<string> v1 = petri.get_exit(P_last);

	//	vector<string> v2 = petri.get_enter(P2);
	if (whileorif)//while�����ʵ����ֻ��һ��
	{
		petri.Add_Arc(v1[v1.size() - 1], P_next, "executed", false);
	}
	else
	{
		for (unsigned int j = 0; j < v1.size(); j++)
		{
			petri.Add_Arc(v1[j], P_next, "executed", false);
		}
	}

}

void while_executed(C_Petri petri, string sentence, string executed_P)
{
	//while��ִ�п���
	if (sentence.rfind("while", 1) != string::npos)
	{
		string _P = find_P_name(petri, sentence);
		vector<string> v1 = petri.get_exit(_P);
		for (unsigned int j = 0; j < v1.size() - 1; j++)
		{
			petri.Add_Arc(v1[j], executed_P, "", false);
		}
	}
}

string find_father_func(string father_sentence)
{
	string func;
	string temp_func = father_sentence;
	if (temp_func.rfind("int", 1) != string::npos)
	{
		temp_func = temp_func.substr(3);
	}
	else if (temp_func.rfind("void", 1) != string::npos)
	{
		temp_func = temp_func.substr(4);
	}
	else if (temp_func.rfind("double", 1) != string::npos)
	{
		temp_func = temp_func.substr(6);
	}
	else if (temp_func.rfind("float", 1) != string::npos)
	{
		temp_func = temp_func.substr(5);
	}
	else if (temp_func.rfind("char", 1) != string::npos)
	{
		temp_func = temp_func.substr(4);
	}
	else if (temp_func.rfind("short", 1) != string::npos)
	{
		temp_func = temp_func.substr(5);
	}
	else if (temp_func.rfind("long", 1) != string::npos)
	{
		temp_func = temp_func.substr(4);
	}
	for (unsigned int i = 0; i < temp_func.size(); i++)
	{
		if (temp_func[i] == '(')
			break;
		func = func + temp_func[i];
	}
	return func;
}

void travel_by_place(gtree *p, string place, gtree *&temp_t)
{
	if (p == NULL)
		return;
	if (p->place == place)
		temp_t = p;

	travel_by_place(p->child, place, temp_t);
	travel_by_place(p->next, place, temp_t);
}

gtree *replicate_tree(gtree *root)
{
	gtree *newRoot = NULL;
	gtree *newChild = NULL;
	gtree *newNext = NULL;
	
	if (!root)
		return NULL;
	else
	{
		newNext = replicate_tree(root->next);
		newChild = replicate_tree(root->child);
		newRoot = new gtree;
		newRoot->type = root->type;
		newRoot->type_1 = root->type_1;
		newRoot->place = root->place;
		newRoot->child = newChild;
		newRoot->next = newNext;
		
	}
	return newRoot;
}

void parent_son(gtree *&t)//��������parent��ֵ
{
	if (t == NULL)
		return;
	if (t->child != NULL)
	{
		t->child->parent = t;
		parent_son(t->child);
	}
	if (t->next != NULL)
	{
		t->next->parent = t->parent;
		parent_son(t->next);
	}
}

void real_exit(vector<string> &v)
{

	string temp = v[v.size() - 1];
	v.clear();
	v.push_back(temp);
}

void back_executed(C_Petri &petri, vector<string> last_T, vector<string> new_place_exit)//��last_T�ĳ��ڿ�����Ӧ������ִ�л���new_place_exit�ĳ��ڿ�������
{
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].V == "executed")
		{
			for (unsigned int j = 0; j < last_T.size(); j++)
			{
				if (petri.arc[i].source == last_T[j])
				{
					petri.arc[i].V = "executed#";
					for (unsigned int k = 0; k < new_place_exit.size(); k++)
						petri.Add_Arc(new_place_exit[k], petri.arc[i].target, "executed", false);
					break;
				}
			}
			
		}
	}
}

void pre_executed(C_Petri &petri, vector<string> next_T, vector<string> new_place_enter)//��ָ��new_place_enter�����ڵ�����ִ�л�ɾ������ָ����һ��������
{
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].V == "executed")
		{
			for (unsigned int j = 0; j < new_place_enter.size(); j++)
			{
				if (petri.arc[i].target == new_place_enter[j])
				{
					petri.arc[i].V = "executed#";
					for (unsigned int k = 0; k < next_T.size(); k++)
						petri.Add_Arc(petri.arc[i].source, next_T[k], "executed", true);
				}
			}

		}
	}
}

void pre_executed_1(C_Petri &petri, string P1, string P2)//��ָ��new_place_enter�����ڵ�����ִ�л�ɾ������ָ����һ��������
{
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].V == "executed")
		{

			if (petri.arc[i].target == P1)
			{
				petri.arc[i].V = "executed#";
				petri.Add_Arc(petri.arc[i].source, P2, "executed", false);
			}
		}
	}
}

void change_exit(C_Petri &petri, string father_P, vector<string>last_T, vector<string> new_place_exit)//�ı丸���ĳ��ڿ�����last_T��Ϊnew_place_exit
{
	for (int i = 0; i < petri.p_num; i++)
	{
		if (petri.place[i].name == father_P)
		{
			for (vector<string>::iterator it = petri.place[i].exit.begin(); it != petri.place[i].exit.end(); )
			{
				if (exist_in(last_T, *it))
				{
					it = petri.place[i].exit.erase(it);
				}
				else
					it++;
			}

			petri.place[i].exit.insert(petri.place[i].exit.begin(), new_place_exit.begin(), new_place_exit.end());

			break;
		}
	}
}

void inside_add(C_Petri &petri,string father_place,gtree *father_tree,string new_place,Mapping m)
{
	vector<string> v = petri.get_enter(father_place);

	if (father_tree->type == �������)
	{
		gtree *find_else = father_tree->child;
		bool else_flag = false;
		while (find_else != m.map1->parent)
		{
			if (find_else->type == �ؼ��� && find_else->type_1 == 4)//else
			{
				else_flag = true;
				break;
			}
			find_else = find_else->next;
		}
		if (else_flag == false)
			petri.Add_Arc(v[0], new_place, "", false);
		else
			petri.Add_Arc(v[1], new_place, "", false);
	}
	else
	{
		petri.Add_Arc(v[0], new_place, "", false);
	}
}

void inside_delete(C_Petri &petri,string father_place,string delete_place)
{
	vector<string> v = petri.get_enter(father_place);
	for (unsigned int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < petri.arcnum; j++)
		{
			if (petri.arc[j].source == v[i] && petri.arc[j].target == delete_place)
			{
				petri.arc[j].V += "#";
				break;
			}
		}
	}
}

void delete_node(gtree *&tree)
{
	if (tree->next)
		delete_node(tree->next);
	if (tree->child)
		delete_node(tree->child);
	delete tree;
}

void change_call_enter(C_Petri &petri, vector<string> source, vector<string> target, string place)//��source��place֮�仡ɾȥ�����target��place�Ļ�
{
	for (unsigned int i = 0; i < source.size(); i++)
		petri.Delete_Arc(source[i], place);
	for (unsigned int i = 0; i < target.size(); i++)
		petri.Add_Arc(target[i], place, "", false);
}

string operate_add(C_Petri &petri, Mapping m, pair<int, int> add)
{
	bool isfirst, islast,  inside;
	int position1 = add.first;
	int position2 = add.second;
	gtree *t1, *t2;
	gtree *last_tree = NULL, *next_tree = NULL;
	gtree *father_tree = m.map1->parent->parent;//father_treeֻ��ָ���������������
	t1 = m.map1->child;
	for (int i = 0; i < position1 - 1; i++)
		t1 = t1->next;
	t2 = m.map2->child;
	for (int i = 0; i < position2 - 1; i++)
		t2 = t2->next;

	if (position1 == 0)
	{
		isfirst = true;
		islast = false;
	}
	else
	{
		isfirst = false;
		last_tree = m.map1->child;
		for (int i = 0; i < position1 - 1; i++)
			last_tree = last_tree->next;
		if (t1->next == NULL)
			islast = true;
		else
			islast = false;
	}
	if (m.map1->parent->type == �������)
		inside = false;
	else
		inside = true;
	
	//�������ĸı�
	gtree *newtree = replicate_tree(t2);
	parent_son(newtree);

	if (isfirst == true)
	{
		next_tree = t1;
		t1->parent->child = newtree;
		newtree->parent = t1->parent;
		newtree->next = next_tree;
	}
	else if (islast == true)
	{
		t1->next = newtree;
		newtree->parent = t1->parent;
		newtree->next = NULL;
	}
	else
	{
		next_tree = t1->next;
		t1->next = newtree;
		newtree->parent = t1->parent;
		newtree->next = next_tree;
	}


	//����������cpn����
	string new_place = get_gen_P();
	declaration(petri,newtree->child, "");//����func����""��Ϊֻ������䣬û�ж��岿��

	//�������к��к������û�ֵ����к��к������ã�����Ҫ��ǰ��ִ�л�
	gtree *pre_executed;//���ʾǰ������ִ�л��������������룩

	gtree *search;
	if (newtree->child->type == ��ֵ���)
	{
		search = newtree->child;
	}
	else
	{
		search = newtree->child->child->next->next->child;//������
	}
	pre_executed = search_call(search);

	//�޸�ִ�л�
	string father_place;
	if (inside==true)
		father_place = find_P_name(petri, father_tree->place);
	
	//�����һ���к�������
	if (islast == false)
	{
		//�ҵ��ÿ���
		gtree *search;
		gtree *call;
		if (next_tree->child->type == ��ֵ���)
		{
			search = next_tree->child;
			call = search_call(search);
		}
		else if (next_tree->child->type == ������� || next_tree->child->type == ѭ�����)
		{
			search = next_tree->child->child->next->next->child;//������
			call = search_call(search);
		}
		else
			call = NULL;
		
		if (call != NULL)
		{
			string next_P = find_P_name(petri, next_tree->child->place);
			vector<string> next_enter_T = petri.get_enter(next_P);
			int current;
			if (next_enter_T.size() == 0)
			{
				cout << "error int evolution 492!" << endl;
				exit(1);
			}
			else
			{
				for (int i = 0; i < petri.t_num; i++)
					if (petri.transition[i].name == next_enter_T[0])
						current = petri.transition[i].current_P_num;
			}
			string fun = call->child->place;
			fun += "()";
			string call_P = find_P_name_1(petri, fun, current + 2);

			//����һ������
			string last_P;
			vector<string> last_T;
			bool flag, flag1;
			gtree *last_sentence = find_last_sentence(newtree, flag, flag1);
			last_P = find_P_name(petri, last_sentence->place);
			if (flag == false)
				last_T = petri.get_exit(last_P);
			else
			{
				vector<string> last_enter = petri.get_enter(last_P);
				if (flag1 == true)
					last_T.push_back(last_enter[1]);
				else
					last_T.push_back(last_enter[0]);
			}
			//last_T = petri.get_exit(last_P);

			vector<string> new_exit = petri.get_exit(new_place);
			change_call_enter(petri, last_T, new_exit, call_P);
		}
	}
	
	if (isfirst == true)
	{
		string next_P = find_P_name(petri, next_tree->child->place);
		//ǰ��
		if (pre_executed == NULL)
		{
			if (inside == true)
			{
				inside_add(petri, father_place, father_tree, new_place, m);
			}
			else
			{
				string func = father_tree->child->child->next->place;
				string func_T = find_T_name(petri, func + " begin");
				petri.Add_Arc(func_T, new_place, "", false);
				petri.Delete_Arc(func_T, next_P);
			}
		}
		else if (inside == true)
			inside_add(petri, father_place, father_tree, new_place, m);
		//����
		
		bool whileorif = false;
		if (newtree->child->type == ѭ�����)
			whileorif = true;
		if (inside == true)
		{
			add_executed(petri, new_place, next_P, whileorif);
		}
		else
		{
			add_executed_1(petri, new_place, next_P, whileorif);
		}
	}
	else
	{
		string last_P = find_P_name(petri, last_tree->child->place);
		//����
		vector<string> last_T = petri.get_exit(last_P);
		vector<string> new_place_exit = petri.get_exit(new_place);
		if (last_tree->child->type == ѭ�����)
		{
			real_exit(last_T);
		}
		if (newtree->child->type == ѭ�����)
		{
			real_exit(new_place_exit);
		}

		back_executed(petri, last_T, new_place_exit);

		//ǰ��
		
		if (pre_executed == NULL)
		{
			
			bool whileorif = false;
			if (inside == true)
			{
				inside_add(petri, father_place, father_tree, new_place, m);
				if (last_tree->child->type == ѭ�����)
					whileorif = true;
				string executed_P = add_executed(petri, last_P, new_place, whileorif);

				//����while���Ҫע��ǰ��ִ�п���
				if (newtree->child->type == ѭ�����)
				{
					vector<string> new_exit = petri.get_exit(new_place);
					for (unsigned int i = 0; i < new_exit.size() - 1; i++)
						petri.Add_Arc(new_exit[i], executed_P, "", false);
				}
			}
			else
			{
				if (last_tree->child->type == ѭ�����)
					whileorif = true;
				add_executed_1(petri, last_P, new_place, whileorif);
			}
		}
		else if (inside == true)
			inside_add(petri, father_place, father_tree, new_place, m);

		if (next_tree != NULL && next_tree->child->type == �������)
		{
			gtree *find_this_func = m.map1;
			while (find_this_func->type != ��������)
				find_this_func = find_this_func->parent;
			string this_func_v_P = find_P_name(petri, find_this_func->child->child->next->place + "_v");
			string value;
			for (unsigned int i = 0; i < last_T.size(); i++)
			{
				value = petri.Delete_Arc(last_T[i], this_func_v_P);
				value = petri.Delete_Arc(this_func_v_P, last_T[i]);
			}
			vector<string> this_exit_T = petri.get_exit(new_place);
			for (unsigned int i = 0; i < this_exit_T.size(); i++)
			{
				petri.Add_Arc(this_exit_T[i], this_func_v_P, value, false);
				petri.Add_Arc(this_func_v_P, this_exit_T[i], value, true);
			}
		}
		//�޸ĳ���
		if (islast == true && inside == true)
			change_exit(petri, father_place, last_T, new_place_exit);
	}
	return new_place;
}

string operate_del(C_Petri &petri, Mapping m, pair<int, int> del)
{
	bool isfirst, islast, inside;
	int position1 = del.first;
	int position2 = del.second;
	gtree *t1, *t2;
	gtree *last_tree = NULL, *next_tree = NULL;
	gtree *father_tree = m.map1->parent->parent;//father_treeֻ��ָ���������������
	t1 = m.map1->child;
	for (int i = 0; i < position1 - 1; i++)
		t1 = t1->next;
	t2 = m.map2->child;
	for (int i = 0; i < position2 - 1; i++)
		t2 = t2->next;

	if (position1 == 1)
	{
		isfirst = true;
		islast = false;
	}
	else
	{
		isfirst = false;
		last_tree = m.map1->child;
		for (int i = 0; i < position1 - 2; i++)
			last_tree = last_tree->next;
		if (t1->next == NULL)
			islast = true;
		else
			islast = false;
	}
	if (m.map1->parent->type == �������)
		inside = false;
	else
		inside = true;

	//�������ĸı�
	bool iswhile = false;
	string delete_place = find_P_name(petri, t1->child->place);
	if (t1->child->type == ѭ�����)
		iswhile = true;
	gtree *search;
	if (t1->child->type == ��ֵ���)
	{
		search = t1->child;
	}
	else
	{
		search = t1->child->child->next->next->child;//������
	}
	gtree *call = search_call(search);
	if (call)
	{
		vector<string> enter_T = petri.get_enter(delete_place);
		int current;
		if (enter_T.size() == 0)
		{
			cout << "error int evolution 492!" << endl;
			exit(1);
		}
		else
		{
			for (int i = 0; i < petri.t_num; i++)
				if (petri.transition[i].name == enter_T[0])
					current = petri.transition[i].current_P_num;
		}
		string fun = call->child->place;
		fun += "()";
		string place = find_P_name_1(petri, fun, current + 2);

		bool flag, flag1;
		gtree *last_call_sentence = find_last_sentence(t1,flag,flag1);
		string last_call_P = find_P_name(petri, last_call_sentence->place);
		vector<string> v;
		if (flag == false)
		{
			v = petri.get_exit(last_call_P);
			for (unsigned int i = 0; i < v.size(); i++)
				petri.Delete_Arc(v[i], place);
		}
		else
		{
			v = petri.get_enter(last_call_P);
			for (unsigned int i = 0; i < v.size(); i++)
				petri.Delete_Arc(v[i], place);
		}
	}

	if (isfirst == true)
	{
		t1->parent->child = t1->next;
		delete_node(t1->child);
		delete t1;
	}
	else
	{
		last_tree->next = t1->next;
		delete_node(t1->child);
		delete t1;
	}

	//�޸�ִ�л�
	//�����һ���к�������
	bool next_call = false;
	if (islast == false)
	{
		//�ҵ��ÿ���
		if (isfirst == true)
			next_tree = m.map1->child;
		else
			next_tree = last_tree->next;
		gtree *search;
		if (next_tree->child->type == ��ֵ���)
		{
			search = next_tree->child;
		}
		else
		{
			search = next_tree->child->child->next->next->child;//������
		}
		gtree *call = search_call(search);
		if (call != NULL)
		{
			next_call = true;
			string next_P = find_P_name(petri, next_tree->child->place);
			vector<string> next_enter_T = petri.get_enter(next_P);
			int current;
			if (next_enter_T.size() == 0)
			{
				cout << "error int evolution 492!" << endl;
				exit(1);
			}
			else
			{
				for (int i = 0; i < petri.t_num; i++)
					if (petri.transition[i].name == next_enter_T[0])
						current = petri.transition[i].current_P_num;
			}
			string fun = call->child->place;
			fun += "()";
			string call_P = find_P_name_1(petri, fun, current + 2);

			//����һ������
			string last_P;
			vector<string> last_T;
			bool flag, flag1;
			gtree *last_sentence = find_last_sentence(next_tree, flag, flag1);
			last_P = find_P_name(petri, last_sentence->place);
			if (flag == false)
				last_T = petri.get_exit(last_P);
			else 
			{
				vector<string> last_enter = petri.get_enter(last_P);
				if (flag1 == true)
					last_T.push_back(last_enter[1]);
				else
					last_T.push_back(last_enter[0]);
			}


			vector<string> new_exit = petri.get_exit(delete_place);
			change_call_enter(petri, new_exit, last_T, call_P);
		}
	}



	if (isfirst == true)
	{
		string next_P = find_P_name(petri, m.map1->child->child->place);
		if (inside == true)
		{
			string father_place = find_P_name(petri, father_tree->place);
			//ǰ��
			inside_delete(petri, father_place, delete_place);
			//����
			delete_last_executed(petri, delete_place);
		}
		else
		{
			string func = father_tree->child->child->next->place;
			string func_T = find_T_name(petri, func + " begin");
			//ǰ��
			for (int i = 0; i < petri.arcnum; i++)
			{
				if (petri.arc[i].source == func_T && petri.arc[i].target == delete_place)
				{
					petri.arc[i].V += "#";
					petri.Add_Arc(petri.arc[i].source, next_P, "", false);
					break;
				}
			}
			//����
			delete_last_executed(petri, delete_place);
		}
		
	}
	else
	{
		string last_P = find_P_name(petri, last_tree->child->place);
		vector<string> last_T = petri.get_exit(last_P);
		vector<string> delete_place_enter = petri.get_enter(delete_place);
		vector<string> delete_place_exit = petri.get_exit(delete_place);
		vector<string> last_exit = petri.get_exit(last_P);
		if (iswhile == true)
			real_exit(delete_place_exit);
		if (last_tree->child->type == ѭ�����)
			real_exit(last_exit);
		
		//gtree *next_tree = NULL;
		vector<string> next_enter;
		string next_P;
		if (islast == false)
		{
			next_tree = last_tree->next;
			next_P = find_P_name(petri, next_tree->child->place);
			next_enter = petri.get_enter(find_P_name(petri, next_tree->child->place));
		}
		if (inside == true)
		{
			string father_place = find_P_name(petri, father_tree->place);
			//ǰ��
			inside_delete(petri, father_place, delete_place);
			//����
			if (islast == false)
			{
				pre_executed(petri, next_enter, delete_place_enter);
				delete_last_executed(petri, delete_place);
				
			}
			else
			{
				change_exit(petri, father_place, delete_place_exit, last_exit);
				back_executed(petri, delete_place_exit, last_exit);
			}
		}
		else
		{
			string func = father_tree->child->child->next->place;
			string func_end_P = find_P_name(petri, func + "_end");
			//ǰ��
			//�˴����ô���
			//����
			if (islast == false)
			{
				//pre_executed_1(petri, delete_place, next_P);
				//delete_last_executed(petri, delete_place);
				//change_exit(petri, father_P, last_T, delete_place_exit);
				
				for (unsigned int i = 0; i < last_exit.size(); i++)
				{
					if (next_call == false)
						petri.Add_Arc(last_exit[i], next_P, "executed", false);
					petri.Delete_Arc(last_exit[i], delete_place);
				}
			}
			else
			{
				for (unsigned int i = 0; i < last_exit.size(); i++)
				{
					petri.Add_Arc(last_exit[i], func_end_P, "executed", false);
					petri.Delete_Arc(last_exit[i], delete_place);
				}
				
			}
		}

		if (next_tree != NULL && next_tree->child->type == �������)
		{
			gtree *find_this_func = m.map1;
			while (find_this_func->type != ��������)
				find_this_func = find_this_func->parent;
			string this_func_v_P = find_P_name(petri, find_this_func->child->child->next->place + "_v");
			string value;
			vector<string> this_exit_T = petri.get_exit(delete_place);
			for (unsigned int i = 0; i < this_exit_T.size(); i++)
			{
				value = petri.Delete_Arc(this_exit_T[i], this_func_v_P);
				value = petri.Delete_Arc(this_func_v_P, this_exit_T[i]);
			}
			
			for (unsigned int i = 0; i < last_T.size(); i++)
			{
				petri.Add_Arc(last_T[i], this_func_v_P, value, false);
				petri.Add_Arc(this_func_v_P, last_T[i], value, true);
			}
		}
	}

	return delete_place;
}

vector<string> operate_modify(C_Petri &petri, Mapping m, pair<int, int> modify)
{
	vector<string> change_P;
	pair<int, int> add, del;
	del.first = modify.first + 1;
	del.second = modify.second - 1;
	add.first = modify.first - 1;
	add.second = modify.second;
	string add_P = operate_add(petri, m, add);
	string del_P = operate_del(petri, m, del);
	
	change_P.push_back(del_P);
	change_P.push_back(add_P);
	return change_P;
}

vector<string> operate_move(C_Petri &petri, Mapping m, pair<int, int> move)
{
	vector<string> change_P;
	pair<int, int> add, del;
	del.first = move.first;
	del.second = move.first - 1;
	add.first = move.second - 1;
	add.second = move.second;
	string del_P = operate_del(petri, m, del);
	string add_P = operate_add(petri, m, add);
	change_P.push_back(del_P);
	change_P.push_back(add_P);
	return change_P;
}

vector<string> evolution(C_Petri &petri, vector<AST_change> changes)//t1,t2���������﷨�����ڵ�
{
	vector<string> change_P;
	vector<string> temp_P;
	for (unsigned int i = 0; i < changes.size(); i++)
	{
		for (unsigned int j = 0; j < changes[i].modify.size(); j++)
		{
			temp_P = operate_modify(petri, changes[i].m, changes[i].modify[j]);
			change_P.insert(change_P.end(), temp_P.begin(), temp_P.end());
		}
		for (unsigned int j = 0; j < changes[i].add.size(); j++)
		{
			change_P.push_back(operate_add(petri, changes[i].m, changes[i].add[j]));
		}
		for (unsigned int j = 0; j < changes[i].del.size(); j++)
		{
			change_P.push_back(operate_del(petri, changes[i].m, changes[i].del[j]));
		}
		for (unsigned int j = 0; j < changes[i].move.size(); j++)
		{
			temp_P = operate_move(petri, changes[i].m, changes[i].move[j]);
			change_P.insert(change_P.end(), temp_P.begin(), temp_P.end());
		}
	}
	return change_P;
}