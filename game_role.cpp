#include "game_role.h"
#include "GameMsg.h"
#include <iostream>
#include "msg.pb.h"

using namespace std;
static AOI_world g_world(0, 400, 0, 400, 20, 20);
GameMsg * game_role::MakeLoginIdName()
{
	auto ret = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID);

	/*将gamerole的属性赋值给ret->pMsgContent*/
	auto syncpid_msg = dynamic_cast<pb::SyncPid *> (ret->pMsgContent);
	syncpid_msg->set_pid(iPid);
	syncpid_msg->set_username(m_username);

	return ret;
}

GameMsg * game_role::MakeSrdPlayers()
{
	auto ret = new GameMsg(GameMsg::MSG_TYPE_SRD_PLAYERS);

	/*将周围玩家信息给ret->pMsgContent*/
	auto syncplayers_msg = dynamic_cast<pb::SyncPlayers *> (ret->pMsgContent);
	/*获取周围玩家，循环设置到syncplayers_msg里*/
	auto player_list = g_world.GetSrdPlayers(this);
	for (auto itr : player_list)
	{
		auto single_player = syncplayers_msg->add_ps();
		auto player = dynamic_cast<game_role *>(itr);
		single_player->set_pid(player->iPid);
		single_player->set_username(player->m_username);
		auto position = single_player->mutable_p();
		position->set_x(player->x);
		position->set_y(player->y);
		position->set_z(player->z);
		position->set_v(player->v);
	}
	
	return ret;
}

GameMsg * game_role::MakeInitPos()
{
	auto ret = new GameMsg(GameMsg::MSG_TYPE_BROAD_CAST);

	/*设置广播消息类型字段为2，设置自己的坐标到消息里*/
	auto inipos_msg = dynamic_cast<pb::BroadCast *>(ret->pMsgContent);
	inipos_msg->set_pid(iPid);
	inipos_msg->set_username(m_username);
	inipos_msg->set_tp(2);
	auto position = inipos_msg->mutable_p();
	position->set_x(x);
	position->set_y(y);
	position->set_z(z);
	position->set_v(v);

	return ret;
}

static int g_curID = 0;
game_role::game_role()
{
	x = 100;
	z = 100;
	iPid = g_curID++;
}


game_role::~game_role()
{
}

/*新客户端连接后*/
bool game_role::Init()
{
	/*将玩家添加到游戏世界中*/
	g_world.AddPlayer(this);

	//向其发送ID和名称
	//构造一个包含id和名称的消息
	auto plogin_msg = MakeLoginIdName();
	//sendout通过自己绑定的协议对象发送
	ZinxKernel::Zinx_SendOut(*plogin_msg, *pGameProtocol);

	//向其发送周围玩家的位置
	//构造周围玩家消息
	auto pSrdPlayer = MakeSrdPlayers();
	ZinxKernel::Zinx_SendOut(*pSrdPlayer, *pGameProtocol);

	//向周围玩家发送其位置
	/*获取周围玩家*/
	auto player_list = g_world.GetSrdPlayers(this);
	/*循环发送*/
	for (auto single : player_list)
	{
		//构造自己出生位置的消息
		auto pinit_pos = MakeInitPos();
		ZinxKernel::Zinx_SendOut(*pinit_pos, *(dynamic_cast<game_role *>(single)->pGameProtocol));
	}


	return true;
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
	/*从游戏世界摘出玩家*/
	g_world.DelPlayer(this);
}

int game_role::getX()
{
	return int(x);
}

int game_role::getY()
{
	return int (z);
}
