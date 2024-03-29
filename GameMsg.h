#pragma once
#include <zinx.h>
#include <google/protobuf/message.h>

class GameMsg :
	public UserData
{
public:
	/*成员消息ID 枚举表示*/
	enum MSG_TYPE {
		MSG_TYPE_LOGIN_ID = 1,
		MSG_TYPE_TALK_CONTENT = 2,
		MSG_TYPE_NEW_POSTION= 3,
		MSG_TYPE_BROAD_CAST = 200,
		MSG_TYPE_LOGOFF_ID = 201,
		MSG_TYPE_SRD_PLAYERS = 202
	} m_msg_type;
	/*成员消息内容（用所有消息类的父类指针表示）*/
	google::protobuf::Message *pMsgContent = NULL; //可以指向任意具体的消息对象

	std::string GetTalkContent();

public:
	/*增加构造函数，将字节(用string存储)转为对象*/
	GameMsg(MSG_TYPE _type, std::string _V);

	/*构造空消息内容的构造函数---》发消息用*/
	GameMsg(MSG_TYPE _type);

	virtual ~GameMsg();

	/*序列化，返回序列化后的字节，用string存*/
	std::string serielize();
};
/*用来处理从socket来的若干消息*/
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

