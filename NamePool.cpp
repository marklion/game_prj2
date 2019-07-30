#include "NamePool.h"
#include <fstream>
#include <random>

using namespace std;

NamePool NamePool::name_pool;

NamePool::NamePool()
{
}


NamePool::~NamePool()
{
}
static default_random_engine e(time(NULL));
std::string NamePool::GetName()
{
	if (m_pool.size() <= 0)
	{
		return "û��������";
	}
	/*���ȡ����*/
	int first_index = e() % m_pool.size();
	FirstName_vector &first_vec = m_pool[first_index];
	string firstname = first_vec.m_first_name;
	/*���ȡ����*/
	int name_index = e() % first_vec.m_last_vect.size();
	string lastname = first_vec.m_last_vect[name_index];
	/*ժ������*/
	first_vec.m_last_vect.erase(first_vec.m_last_vect.begin() + name_index);

	/*��ժ������������--��ժ����*/
	if (first_vec.m_last_vect.size() <= 0)
	{
		m_pool.erase(m_pool.begin() + first_index);
	}

	/*ƴ������*/
	return firstname+" " + lastname;
}

void NamePool::ReleaseName(std::string _name)
{
	/*���ȫ��--���պ���*/
	int space_pos = _name.find(' ');
	string first_name = _name.substr(0, space_pos);
	string last_name = _name.substr(space_pos + 1, _name.size() - space_pos - 1);

	bool flag = false;

	/*�������ҵ���Ӧ�ô�ŵ�����*/
	for (auto first : m_pool)
	{
		if (first_name == first.m_first_name)
		{
			/*�ҵ���-�������浽������*/
			first.m_last_vect.push_back(last_name);
			flag = true;
			break;
		}
	}
	
	if (false == flag)
	{
		/*û�ҵ�---�������պ�������Ӧ���������Ķ���---������*/
		FirstName_vector tmp;
		tmp.m_first_name = first_name;
		tmp.m_last_vect.push_back(last_name);

		/*�浽��������*/
		m_pool.push_back(tmp);
	}
}

bool NamePool::LoadFile()
{
	/*�����ļ���ÿ�����ֶ��浽vector��*/
	ifstream name_file("random_last.txt");
	if (true != name_file.is_open())
	{
		return false;
	}
	string tmp;
	vector<string> tmp_last_name;
	while (getline(name_file, tmp))
	{
		tmp_last_name.push_back(tmp);
	}
	/*�����ļ���ÿ���ն�Ҫ���һ�������صĳ�Ա*/
	ifstream first_file("random_first.txt");
	if (true != first_file.is_open())
	{
		return false;
	}
	while (getline(first_file, tmp))
	{
		/*�����պ����Ӧ����������Ķ���*/
		FirstName_vector tmp_vect;
		tmp_vect.m_first_name = tmp;
		tmp_vect.m_last_vect = tmp_last_name;
		m_pool.push_back(tmp_vect);
	}

	return true;
}
