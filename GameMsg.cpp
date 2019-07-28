#include "GameMsg.h"
#include <iostream>
#include "msg.pb.h"
using namespace std;

/*获取聊天消息内容*/
std::string GameMsg::GetTalkContent()
{
	/*让成员pMsgContent获取聊天消息内容*/
	/*动态类型转换   pMsgContent----》pb::Talk*/
	auto pTalk = dynamic_cast<pb::Talk *>(pMsgContent);

	/*gamemsg类中的成员获取聊天内容*/
	return pTalk->content();
}

GameMsg::GameMsg(MSG_TYPE _type, std::string _V):m_msg_type(_type)
{
	/*根据参数_type不同,构造不同的具体的消息对象*/
	switch (_type)
	{
	case GameMsg::MSG_TYPE_LOGIN_ID:
		pMsgContent = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_TALK_CONTENT:
		pMsgContent = new pb::Talk();
		break;
	case GameMsg::MSG_TYPE_NEW_POSTION:
		pMsgContent = new pb::Position();
		break;
	case GameMsg::MSG_TYPE_BROAD_CAST:
		pMsgContent = new pb::BroadCast();
		break;
	case GameMsg::MSG_TYPE_LOGOFF_ID:
		pMsgContent = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_SRD_PLAYERS:
		pMsgContent = new pb::SyncPlayers();
		break;
	default:
		break;
	}
	/*交给成员解析*/
	pMsgContent->ParseFromString(_V);

	/*打印调试信息*/
	cout << "recv message:"<<pMsgContent->Utf8DebugString() << endl;
}

GameMsg::GameMsg(MSG_TYPE _type):m_msg_type(_type)
{
	/*消息内容指针构造对象*/
	/*根据参数_type不同,构造不同的具体的消息对象*/
	switch (_type)
	{
	case GameMsg::MSG_TYPE_LOGIN_ID:
		pMsgContent = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_TALK_CONTENT:
		pMsgContent = new pb::Talk();
		break;
	case GameMsg::MSG_TYPE_NEW_POSTION:
		pMsgContent = new pb::Position();
		break;
	case GameMsg::MSG_TYPE_BROAD_CAST:
		pMsgContent = new pb::BroadCast();
		break;
	case GameMsg::MSG_TYPE_LOGOFF_ID:
		pMsgContent = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_SRD_PLAYERS:
		pMsgContent = new pb::SyncPlayers();
		break;
	default:
		break;
	}
}

GameMsg::~GameMsg()
{
	if (NULL != pMsgContent)
	{
		delete pMsgContent;
		pMsgContent= NULL;
	}
}

/*将消息序列化成字节（用string存）*/
std::string GameMsg::serielize()
{
	std::string ret;

	/*打印调试信息*/
	cout << "send message:" << pMsgContent->Utf8DebugString() << endl;

	/*让成员帮忙序列化*/
	pMsgContent->SerializeToString(&ret);

	return ret;
}

