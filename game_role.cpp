#include "game_role.h"
#include "GameMsg.h"
#include <iostream>
#include "msg.pb.h"
#include <algorithm>
#include <random>
#include "NamePool.h"
#include "timer_channel.h"

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

GameMsg * game_role::MakeLogoff()
{
	auto ret = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID);
	auto logoff_msg = dynamic_cast<pb::SyncPid *>(ret->pMsgContent);

	logoff_msg->set_pid(iPid);
	logoff_msg->set_username(m_username);

	return ret;
}

GameMsg * game_role::MakeTalkBroadCast(std::string _content)
{
	GameMsg *ret = new GameMsg(GameMsg::MSG_TYPE_BROAD_CAST);
	/*设置广播类型是1，把聊天内容设置到ret里*/
	auto broadcast_talk = dynamic_cast<pb::BroadCast *>(ret->pMsgContent);
	broadcast_talk->set_pid(iPid);
	broadcast_talk->set_username(m_username);
	broadcast_talk->set_tp(1);
	broadcast_talk->set_content(_content);
	return ret;
}

GameMsg * game_role::MakeNewPostionBroadCast()
{
	auto ret = new GameMsg(GameMsg::MSG_TYPE_BROAD_CAST);

	auto new_pos_msg = dynamic_cast<pb::BroadCast *>(ret->pMsgContent);
	new_pos_msg->set_pid(iPid);
	new_pos_msg->set_username(m_username);
	new_pos_msg->set_tp(4);

	auto position = new_pos_msg->mutable_p();
	position->set_x(x);
	position->set_y(y);
	position->set_z(z);
	position->set_v(v);

	return ret;
}

static int g_curID = 0;
void game_role::proc_new_position(float _x, float _y, float _z, float _v)
{
	//1. 跨网格视野处理
	/*获取旧邻居*/
	auto old_srd = g_world.GetSrdPlayers(this);

	/*摘除玩家--》更新坐标--》添加玩家*/
	g_world.DelPlayer(this);
	x = _x;
	y = _y;
	z = _z;
	v = _v;
	g_world.AddPlayer(this);
	/*获取新邻居*/
	auto new_srd = g_world.GetSrdPlayers(this);

	/*遍历旧邻居，在新邻居中查找，没找到--》视野消失*/
	for (auto single : old_srd)
	{
		/*find会返回查找到的元素的迭代器，若没找到返回end*/
		if (new_srd.end() == find(new_srd.begin(), new_srd.end(), single))
		{
			auto player = dynamic_cast<game_role*>(single);
			/*视野消失*/
			view_lost(player);
		}
	}
	/*遍历新邻居，在旧邻居中查找，没找到--》视野出现*/
	for (auto single : new_srd)
	{
		if (old_srd.end() == find(old_srd.begin(), old_srd.end(), single))
		{
			/*视野出现*/
			auto player = dynamic_cast<game_role*>(single);
			view_appear(player);
		}
	}
	//2. 广播新位置给周围玩家
	/*构造新位置广播消息，循环发送*/
	for (auto single : new_srd)
	{
		auto pmsg = MakeNewPostionBroadCast();
		auto player = dynamic_cast<game_role *>(single);
		ZinxKernel::Zinx_SendOut(*pmsg, *(player->pGameProtocol));
	}
}

void game_role::view_lost(game_role *_old_srd)
{
	/*向旧邻居发自己的下线消息*/
	auto logoff_msg = this->MakeLogoff();

	ZinxKernel::Zinx_SendOut(*logoff_msg, *(_old_srd->pGameProtocol));

	/*向自己发旧邻居的下线消息*/
	logoff_msg = _old_srd->MakeLogoff();
	ZinxKernel::Zinx_SendOut(*logoff_msg, *(this->pGameProtocol));
}

void game_role::view_appear(game_role *_new_srd)
{
	/*发送自己的出现消息（广播出生位置）给新邻居*/

	auto appear = this->MakeInitPos();
	ZinxKernel::Zinx_SendOut(*appear, *(_new_srd->pGameProtocol));
	
	/*发送邻居的出现消息给自己*/
	appear = _new_srd->MakeInitPos();
	ZinxKernel::Zinx_SendOut(*appear, *(this->pGameProtocol));
}

/*定义全局对象存随机数引擎*/
static default_random_engine e(time(NULL));

game_role::game_role()
{
	/*随机产生x和z的值*/
	x = 200 + e() % 10;
	z = 100 + e() % 10;

	/*获取随机姓名*/
	m_username = NamePool::GetInstance().GetName();
	iPid = g_curID++;
}


game_role::~game_role()
{
	NamePool::GetInstance().ReleaseName(m_username);
}

/*定义退出定时任务*/
class exit_framework_task :public timeout_task {
	// 通过 timeout_task 继承
	virtual void proc_timeout() override
	{
		ZinxKernel::Zinx_Exit();
	}
};

static exit_framework_task *peft = NULL;

/*新客户端连接后*/
bool game_role::Init()
{
	/*若本玩家是第一个玩家---》删掉退出定时任务*/
	if (ZinxKernel::Zinx_GetAllRole().size() <= 0)
	{
		if (NULL != peft)
		{
			timeout_deliver::GetInstance()->unregister_task(peft);
			delete peft;
			peft = NULL;
		}
	}

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
	string  talk_content;
	for (auto singlemsg : input_msg.m_msg_list)
	{
		/*根据消息类型不同进行不同处理*/
		switch (singlemsg->m_msg_type)
		{
		case GameMsg::MSG_TYPE_TALK_CONTENT:
			/*聊天消息的处理--》构造广播聊天消息---》发给所有玩家*/
			talk_content = singlemsg->GetTalkContent();
			for (auto itr : ZinxKernel::Zinx_GetAllRole())
			{
				auto player = dynamic_cast<game_role *>(itr);
				auto poutmsg = MakeTalkBroadCast(talk_content);
				ZinxKernel::Zinx_SendOut(*poutmsg, *(player->pGameProtocol));
			}
			break;
		case GameMsg::MSG_TYPE_NEW_POSTION:
			/*处理移动消息*/
			//取出坐标信息
			proc_new_position(dynamic_cast<pb::Position *>(singlemsg->pMsgContent)->x(),
							  dynamic_cast<pb::Position *>(singlemsg->pMsgContent)->y(),
							  dynamic_cast<pb::Position *>(singlemsg->pMsgContent)->z(),
				              dynamic_cast<pb::Position *>(singlemsg->pMsgContent)->v());
			break;
		default:
			break;
		}
	}
	return NULL;
}



void game_role::Fini()
{
	//客户端断开时，向周围玩家发送其断开的消息
	/*获取周围玩家，循环发送*/
	auto player_list = g_world.GetSrdPlayers(this);
	for (auto itr : player_list)
	{
		auto player = dynamic_cast<game_role *>(itr);
		auto logoff_msg = MakeLogoff();
		ZinxKernel::Zinx_SendOut(*logoff_msg, *(player->pGameProtocol));
	}
	/*从游戏世界摘出玩家*/
	g_world.DelPlayer(this);

	/*判断若此玩家是最后一人---》起退出定时器*/
	if (ZinxKernel::Zinx_GetAllRole().size() <= 1)
	{
		peft = new exit_framework_task();
		timeout_deliver::GetInstance()->register_task(20, peft);
	}
}

int game_role::getX()
{
	return int(x);
}

int game_role::getY()
{
	return int (z);
}
