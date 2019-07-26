#include "game_role.h"
#include "GameMsg.h"


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
		GameMsg *pout = new GameMsg(*singlemsg);
		ZinxKernel::Zinx_SendOut(*pout, *pGameProtocol);
	}
	return NULL;
}

void game_role::Fini()
{
}
