#include "game_role.h"
#include "GameMsg.h"
#include <iostream>
#include "msg.pb.h"

using namespace std;
static AOI_world g_world(0, 400, 0, 400, 20, 20);
GameMsg * game_role::MakeLoginIdName()
{
	auto ret = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID);

	/*��gamerole�����Ը�ֵ��ret->pMsgContent*/
	auto syncpid_msg = dynamic_cast<pb::SyncPid *> (ret->pMsgContent);
	syncpid_msg->set_pid(iPid);
	syncpid_msg->set_username(m_username);

	return ret;
}

GameMsg * game_role::MakeSrdPlayers()
{
	auto ret = new GameMsg(GameMsg::MSG_TYPE_SRD_PLAYERS);

	/*����Χ�����Ϣ��ret->pMsgContent*/
	auto syncplayers_msg = dynamic_cast<pb::SyncPlayers *> (ret->pMsgContent);
	/*��ȡ��Χ��ң�ѭ�����õ�syncplayers_msg��*/
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

	/*���ù㲥��Ϣ�����ֶ�Ϊ2�������Լ������굽��Ϣ��*/
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

/*�¿ͻ������Ӻ�*/
bool game_role::Init()
{
	/*�������ӵ���Ϸ������*/
	g_world.AddPlayer(this);

	//���䷢��ID������
	//����һ������id�����Ƶ���Ϣ
	auto plogin_msg = MakeLoginIdName();
	//sendoutͨ���Լ��󶨵�Э�������
	ZinxKernel::Zinx_SendOut(*plogin_msg, *pGameProtocol);

	//���䷢����Χ��ҵ�λ��
	//������Χ�����Ϣ
	auto pSrdPlayer = MakeSrdPlayers();
	ZinxKernel::Zinx_SendOut(*pSrdPlayer, *pGameProtocol);

	//����Χ��ҷ�����λ��
	/*��ȡ��Χ���*/
	auto player_list = g_world.GetSrdPlayers(this);
	/*ѭ������*/
	for (auto single : player_list)
	{
		//�����Լ�����λ�õ���Ϣ
		auto pinit_pos = MakeInitPos();
		ZinxKernel::Zinx_SendOut(*pinit_pos, *(dynamic_cast<game_role *>(single)->pGameProtocol));
	}


	return true;
}

UserData * game_role::ProcMsg(UserData & _poUserData)
{
	/*���ԣ�������Ϣ�����󷢵�*/
	GET_REF2DATA(multi_msg, input_msg, _poUserData);
	for (auto singlemsg : input_msg.m_msg_list)
	{
		cout << singlemsg->GetTalkContent() << endl;
		/*����һ��1����Ϣ���ͳ�ȥ�����ԣ�*/
		auto poutput = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID);
		/*����pid=100  username="apple"*/
		dynamic_cast<pb::SyncPid *> (poutput->pMsgContent)->set_pid(100);
		dynamic_cast<pb::SyncPid *> (poutput->pMsgContent)->set_username("apple");
		/*ֱ�ӷ�����Ϣ*/
		ZinxKernel::Zinx_SendOut(*poutput, *pGameProtocol);
	}
	return NULL;
}

void game_role::Fini()
{
	/*����Ϸ����ժ�����*/
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
