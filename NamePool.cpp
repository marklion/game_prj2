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
		return "没有名字了";
	}
	/*随机取出姓*/
	int first_index = e() % m_pool.size();
	FirstName_vector &first_vec = m_pool[first_index];
	string firstname = first_vec.m_first_name;
	/*随机取出名*/
	int name_index = e() % first_vec.m_last_vect.size();
	string lastname = first_vec.m_last_vect[name_index];
	/*摘除名字*/
	first_vec.m_last_vect.erase(first_vec.m_last_vect.begin() + name_index);

	/*若摘完所有名字了--》摘掉姓*/
	if (first_vec.m_last_vect.size() <= 0)
	{
		m_pool.erase(m_pool.begin() + first_index);
	}

	/*拼接姓名*/
	return firstname+" " + lastname;
}

void NamePool::ReleaseName(std::string _name)
{
	/*拆分全名--》姓和名*/
	int space_pos = _name.find(' ');
	string first_name = _name.substr(0, space_pos);
	string last_name = _name.substr(space_pos + 1, _name.size() - space_pos - 1);

	bool flag = false;

	/*根据姓找到名应该存放的容器*/
	for (auto first : m_pool)
	{
		if (first_name == first.m_first_name)
		{
			/*找到了-》将名存到容器中*/
			first.m_last_vect.push_back(last_name);
			flag = true;
			break;
		}
	}
	
	if (false == flag)
	{
		/*没找到---》构造姓和姓所对应的名容器的对象---》存名*/
		FirstName_vector tmp;
		tmp.m_first_name = first_name;
		tmp.m_last_vect.push_back(last_name);

		/*存到姓名池中*/
		m_pool.push_back(tmp);
	}
}

bool NamePool::LoadFile()
{
	/*读名文件，每行名字都存到vector里*/
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
	/*读姓文件，每行姓都要添加一个姓名池的成员*/
	ifstream first_file("random_first.txt");
	if (true != first_file.is_open())
	{
		return false;
	}
	while (getline(first_file, tmp))
	{
		/*构造姓和其对应的名容器类的对象*/
		FirstName_vector tmp_vect;
		tmp_vect.m_first_name = tmp;
		tmp_vect.m_last_vect = tmp_last_name;
		m_pool.push_back(tmp_vect);
	}

	return true;
}
