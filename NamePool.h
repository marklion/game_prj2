#pragma once
#include <vector>
#include <string>

/*姓和姓所对应的名容器*/
class FirstName_vector {
public:
	std::string m_first_name;
	/*姓所对应的名容器*/
	std::vector<std::string> m_last_vect;
};

class NamePool
{
	/*姓和姓所对应的名容器    的容器*/
	std::vector<FirstName_vector> m_pool;
	NamePool();
	virtual ~NamePool();
	static NamePool name_pool;
public:
	static NamePool &GetInstance()
	{
		return name_pool;
	}
	/*获取随机姓名*/
	std::string GetName();
	/*释放姓名*/
	void ReleaseName(std::string _name);
	/*加载姓名文件*/
	bool LoadFile();
};

