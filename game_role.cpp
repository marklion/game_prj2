#include "game_role.h"
#include "GameMsg.h"
#include <iostream>
#include "msg.pb.h"

using namespace std;

game_role::game_role()
{
}


game_role::~game_role()
{
}

bool game_role::Init()
{
	return false;
}

UserData * game_role::ProcMsg(UserData & _poUserData)
{
	/*测试，所有消息拷贝后发掉*/
	GET_REF2DATA(multi_msg, input_msg, _poUserData);
	for (auto singlemsg : input_msg.m_msg_list)
	{
		cout << singlemsg->GetTalkContent() << endl;
		/*创建一个1号消息发送出去（测试）*/
		auto poutput = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID);
		/*设置pid=100  username="apple"*/
		dynamic_cast<pb::SyncPid *> (poutput->pMsgContent)->set_pid(100);
		dynamic_cast<pb::SyncPid *> (poutput->pMsgContent)->set_username("apple");
		/*直接发送消息*/
		ZinxKernel::Zinx_SendOut(*poutput, *pGameProtocol);
	}
	return NULL;
}

void game_role::Fini()
{
}
