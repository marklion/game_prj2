#pragma once
#include <vector>
#include <string>

/*�պ�������Ӧ��������*/
class FirstName_vector {
public:
	std::string m_first_name;
	/*������Ӧ��������*/
	std::vector<std::string> m_last_vect;
};

class NamePool
{
	/*�պ�������Ӧ��������    ������*/
	std::vector<FirstName_vector> m_pool;
	NamePool();
	virtual ~NamePool();
	static NamePool name_pool;
public:
	static NamePool &GetInstance()
	{
		return name_pool;
	}
	/*��ȡ�������*/
	std::string GetName();
	/*�ͷ�����*/
	void ReleaseName(std::string _name);
	/*���������ļ�*/
	bool LoadFile();
};

