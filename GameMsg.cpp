#include "GameMsg.h"
#include <iostream>
#include "msg.pb.h"
using namespace std;

/*��ȡ������Ϣ����*/
std::string GameMsg::GetTalkContent()
{
	/*�ó�ԱpMsgContent��ȡ������Ϣ����*/
	/*��̬����ת��   pMsgContent----��pb::Talk*/
	auto pTalk = dynamic_cast<pb::Talk *>(pMsgContent);

	/*gamemsg���еĳ�Ա��ȡ��������*/
	return pTalk->content();
}

GameMsg::GameMsg(MSG_TYPE _type, std::string _V):m_msg_type(_type)
{
	/*���ݲ���_type��ͬ,���첻ͬ�ľ������Ϣ����*/
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
	/*������Ա����*/
	pMsgContent->ParseFromString(_V);

	/*��ӡ������Ϣ*/
	cout << "recv message:"<<pMsgContent->Utf8DebugString() << endl;
}

GameMsg::GameMsg(MSG_TYPE _type):m_msg_type(_type)
{
	/*��Ϣ����ָ�빹�����*/
	/*���ݲ���_type��ͬ,���첻ͬ�ľ������Ϣ����*/
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

/*����Ϣ���л����ֽڣ���string�棩*/
std::string GameMsg::serielize()
{
	std::string ret;

	/*��ӡ������Ϣ*/
	cout << "send message:" << pMsgContent->Utf8DebugString() << endl;

	/*�ó�Ա��æ���л�*/
	pMsgContent->SerializeToString(&ret);

	return ret;
}

