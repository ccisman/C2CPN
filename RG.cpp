#include"RG.h"

bool array_compare(int *a1, int *a2, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (a1[i] != a2[i])
			return false;
	}
	return true;
}

bool array_compare(double *a1, double *a2, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (a1[i] != a2[i])
			return false;
	}
	return true;
}

void array_copy(int *a1, int *a2, int n)//a1数组内容拷贝到a2中
{
	for (int i = 0; i < n; i++)
		a2[i] = a1[i];
}

void array_copy(double *a1, double *a2, int n)//a1数组内容拷贝到a2中
{
	for (int i = 0; i < n; i++)
		a2[i] = a1[i];
}

RG::RG(C_Petri petri1)
{
	petri.p_num = petri1.p_num;
	petri.t_num = petri1.t_num;
	petri.arcnum = petri1.arcnum;
	for (int i = 0; i < petri1.p_num; i++)
	{
		petri.place.push_back(petri1.place[i]);
	}
	for (int i = 0; i < petri1.t_num; i++)
	{
		petri.transition.push_back(petri1.transition[i]);
	}
	for (int i = 0; i < petri1.arcnum; i++)
	{
		petri.arc.push_back(petri1.arc[i]);
	}

	create_node(petri, petri1.enable_T());
}

bool M::operator==(const M m1)
{
	if (control_P == m1.control_P&&token_num == m1.token_num&&color == m1.color&&s == m1.s&&n_n == m1.n_n&&n_dec == m1.n_dec)
	{
		if (array_compare(n, m1.n, n_n) && array_compare(dec, m1.dec, n_dec))
			return true;
		return false;
	}
	else
		return false;
}

bool equal_M(vector<M> m1, vector<M> m2)//要求m1.m2的size一样
{
	for (unsigned int i = 0; i < m1.size(); i++)
	{
		if (m1[i] == m2[i])
		{
		}
		else
			return false;
	}
	return true;
}

void load(vector<M> m, C_Petri &petri)
{
	for (int i = 0; i < petri.p_num; i++)
	{
		petri.place[i].controlP = m[i].control_P;
		petri.place[i].token_num = m[i].token_num;
		petri.place[i].colorset_tag = m[i].color;
		//petri.place[i].num = m[i].n;
		//petri.place[i].decimal = m[i].dec;
		int n_n = m[i].n_n;
		int n_dec = m[i].n_dec;
		petri.place[i].n_num = n_n;
		petri.place[i].n_decimal = n_dec;
		petri.place[i].num = new int[n_n];
		petri.place[i].decimal = new double[n_dec];
		array_copy(m[i].n, petri.place[i].num, n_n);
		array_copy(m[i].dec, petri.place[i].decimal, n_dec);
		petri.place[i].str = m[i].s;
	}

}

void occur_T(C_Petri &petri, string T)
{
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].sourceP == true && petri.arc[i].V != "#" && petri.arc[i].V != "executed#")//隐式弧
		{
			if (petri.arc[i].target == T)
			{
				string source = petri.arc[i].source;
				for (int j = 0; j < petri.p_num; j++)
				{
					if (petri.place[j].name == source)
					{
						if (petri.place[j].controlP == true)
						{
							petri.place[j].token_num--;
							if (petri.place[j].token_num < 0)
							{
								cout << "token_num < 0 !" << endl;
								exit(1);
							}

						}
					}
				}
			}
		}
		else
		{
			if (petri.arc[i].source == T && petri.arc[i].V != "#"&&petri.arc[i].V != "executed#")
			{
				string target = petri.arc[i].target;
				for (int j = 0; j < petri.p_num; j++)
				{
					if (petri.place[j].name == target)
					{
						if (petri.place[j].point_flag == true)//指针节点
						{
							if (petri.place[j].colorset_tag[0] == '1' || petri.place[j].colorset_tag[1] == '1'
								|| petri.place[j].colorset_tag[0] == '5' || petri.place[j].colorset_tag[1] == '5' || petri.place[j].colorset_tag[0] == '6' || petri.place[j].colorset_tag[1] == '6')
							{
								int current = petri.get_current_P_num(T);
								string name = find_P_name_1(petri, petri.arc[i].V, current);
								for (int k = 0; k < petri.p_num; k++)
								{
									if (petri.place[k].name == name)
									{
										for (int l = 0; l < petri.place[j].n_num; l++)
											petri.place[j].num[l] = petri.place[k].num[l];
										break;
									}
								}
							}
							else if (petri.place[j].colorset_tag[0] == '3' || petri.place[j].colorset_tag[1] == '3'
								|| petri.place[j].colorset_tag[0] == '4' || petri.place[j].colorset_tag[1] == '4')
							{
								int current = petri.get_current_P_num(T);
								string name = find_P_name_1(petri, petri.arc[i].V, current);
								for (int k = 0; k < petri.p_num; k++)
								{
									if (petri.place[k].name == name)
									{
										for (int l = 0; l < petri.place[j].n_decimal; l++)
											petri.place[j].decimal[l] = petri.place[k].decimal[l];
										break;
									}
								}
							}
						}
						else
						{
							if (petri.arc[i].V == "executed" || petri.arc[i].V == "")
							{
								petri.place[j].token_num++;
								break;
							}
							string temp_s = petri.arc[i].V;
							int position = 0;
							for (unsigned int i = 0; i < temp_s.length(); i++)
							{
								if (temp_s[i] == '$')
								{
									position = i;
									break;
								}
							}

							int array_num = 0;
							if (position != 0)
							{
								int current = petri.get_current_P_num(T);
								array_num = int(change_and_cal(temp_s.substr(position + 1), petri.place, current));
								temp_s = temp_s.substr(0, position);
							}
							double value = change_and_cal(temp_s, petri.place, petri.get_current_P_num(T));
							if (petri.place[j].colorset_tag[0] == '1' || petri.place[j].colorset_tag[1] == '1' 
								|| petri.place[j].colorset_tag[0] == '5' || petri.place[j].colorset_tag[1] == '5' || petri.place[j].colorset_tag[0] == '6' || petri.place[j].colorset_tag[1] == '6')
								petri.place[j].num[array_num] = value;
							else if (petri.place[j].colorset_tag[0] == '3' || petri.place[j].colorset_tag[1] == '3' 
								|| petri.place[j].colorset_tag[0] == '4' || petri.place[j].colorset_tag[1] == '4')
								petri.place[j].decimal[array_num] = value;
						}

					}
				}
			}
		}
	}
}

int RG::create_node(C_Petri petri, vector<string> T)
{
	vector<M> temp_M;
	M temp_m;
	for (int i = 0; i < petri.p_num; i++)
	{
		temp_m.control_P = petri.place[i].controlP;
		temp_m.token_num = petri.place[i].token_num;
		temp_m.color = petri.place[i].colorset_tag;
		//temp_m.n = petri.place[i].num;
		//temp_m.dec = petri.place[i].decimal;
		int n_n = petri.place[i].n_num;
		int n_dec = petri.place[i].n_decimal;
		temp_m.n_n = n_n;
		temp_m.n_dec = n_dec;
		temp_m.n = new int[n_n];
		temp_m.dec = new double[n_dec];
		array_copy(petri.place[i].num, temp_m.n, n_n);
		array_copy(petri.place[i].decimal, temp_m.dec, n_dec);
		temp_M.push_back(temp_m);
	}
	if (rgnode.size() > 0)
	{
		for (unsigned int i = 0; i < rgnode.size(); i++)
		{
			if (equal_M(rgnode[i].m, temp_M))
			{
				return rgnode[i].num;
			}
		}
		RGNode node;
		node.num = node_num++;
		node.m = temp_M;

		rgnode.push_back(node);
		return node.num;
	}
	else
	{
		RGNode node;
		node.num = node_num++;
		node.m = temp_M;

		rgnode.push_back(node);
		return node.num;
	}

}

void RG::add_next(int node_id, stack<int> &newnode)
{
	RGNode rgnode1 = rgnode[node_id];
	load(rgnode1.m, petri);
	vector<string> T = petri.enable_T();
	for (unsigned int i = 0; i < T.size(); i++)
	{
//		for (int j = 0; j < petri.t_num; j++)
//		{
//			if (T[i] == petri.transition[j].name)
//			{
//				rgnode[node_id].isfirable.push_back(j);
//				break;
//			}
//		}
		string temp_s = T[i].substr(1);
		int temp_i = atoi(temp_s.c_str());
		rgnode[node_id].isfirable.push_back(temp_i);

	}
	rgnode[node_id].enableNum = T.size();
	if (T.size() == 0)
	{
		rgnode[node_id].end = true;
		return;
	}
	for (unsigned int i = 0; i < T.size(); i++)
	{
		int old_num = node_num;
		nextNode next;
		next.T = T[i];
		load(rgnode1.m, petri);
		occur_T(petri, T[i]);
		int num = create_node(petri, T);
		next.num = num;
		rgnode[node_id].next.push_back(next);
		if (node_num > old_num)
			newnode.push(next.num);
	}
}

