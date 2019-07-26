#pragma once
#include <zinx.h>
class GameMsg :
	public UserData
{
public:
	/*��Ա��ϢID ö�ٱ�ʾ*/
	enum MSG_TYPE {
		MSG_TYPE_LOGIN_ID = 1,
		MSG_TYPE_TALK_CONTENT = 2,
		MSG_TYPE_NEW_POSTION= 3,
		MSG_TYPE_BROAD_CAST = 200,
		MSG_TYPE_LOGOFF_ID = 201,
		MSG_TYPE_SRD_PLAYERS = 202
	} m_msg_type;
	/*��Ա��Ϣ���ݣ���ʽδ֪��*/

public:
	/*���ӹ��캯�������ֽ�(��string�洢)תΪ����*/
	GameMsg(MSG_TYPE _type, std::string _V);
	virtual ~GameMsg();

	/*���л����������л�����ֽڣ���string��*/
	std::string serielize();
};
/*���������socket����������Ϣ*/
class multi_msg :public UserData {
public:
	std::list<GameMsg *> m_msg_list;
	~multi_msg()
	{
		for (auto single : m_msg_list)
		{
			delete single;
		}
	}
};

