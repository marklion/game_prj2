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
	/*���ù㲥������1���������������õ�ret��*/
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
	//1. ��������Ұ����
	/*��ȡ���ھ�*/
	auto old_srd = g_world.GetSrdPlayers(this);

	/*ժ�����--����������--��������*/
	g_world.DelPlayer(this);
	x = _x;
	y = _y;
	z = _z;
	v = _v;
	g_world.AddPlayer(this);
	/*��ȡ���ھ�*/
	auto new_srd = g_world.GetSrdPlayers(this);

	/*�������ھӣ������ھ��в��ң�û�ҵ�--����Ұ��ʧ*/
	for (auto single : old_srd)
	{
		/*find�᷵�ز��ҵ���Ԫ�صĵ���������û�ҵ�����end*/
		if (new_srd.end() == find(new_srd.begin(), new_srd.end(), single))
		{
			auto player = dynamic_cast<game_role*>(single);
			/*��Ұ��ʧ*/
			view_lost(player);
		}
	}
	/*�������ھӣ��ھ��ھ��в��ң�û�ҵ�--����Ұ����*/
	for (auto single : new_srd)
	{
		if (old_srd.end() == find(old_srd.begin(), old_srd.end(), single))
		{
			/*��Ұ����*/
			auto player = dynamic_cast<game_role*>(single);
			view_appear(player);
		}
	}
	//2. �㲥��λ�ø���Χ���
	/*������λ�ù㲥��Ϣ��ѭ������*/
	for (auto single : new_srd)
	{
		auto pmsg = MakeNewPostionBroadCast();
		auto player = dynamic_cast<game_role *>(single);
		ZinxKernel::Zinx_SendOut(*pmsg, *(player->pGameProtocol));
	}
}

void game_role::view_lost(game_role *_old_srd)
{
	/*����ھӷ��Լ���������Ϣ*/
	auto logoff_msg = this->MakeLogoff();

	ZinxKernel::Zinx_SendOut(*logoff_msg, *(_old_srd->pGameProtocol));

	/*���Լ������ھӵ�������Ϣ*/
	logoff_msg = _old_srd->MakeLogoff();
	ZinxKernel::Zinx_SendOut(*logoff_msg, *(this->pGameProtocol));
}

void game_role::view_appear(game_role *_new_srd)
{
	/*�����Լ��ĳ�����Ϣ���㲥����λ�ã������ھ�*/

	auto appear = this->MakeInitPos();
	ZinxKernel::Zinx_SendOut(*appear, *(_new_srd->pGameProtocol));
	
	/*�����ھӵĳ�����Ϣ���Լ�*/
	appear = _new_srd->MakeInitPos();
	ZinxKernel::Zinx_SendOut(*appear, *(this->pGameProtocol));
}

/*����ȫ�ֶ�������������*/
static default_random_engine e(time(NULL));

game_role::game_role()
{
	/*�������x��z��ֵ*/
	x = 200 + e() % 10;
	z = 100 + e() % 10;

	/*��ȡ�������*/
	m_username = NamePool::GetInstance().GetName();
	iPid = g_curID++;
}


game_role::~game_role()
{
	NamePool::GetInstance().ReleaseName(m_username);
}

/*�����˳���ʱ����*/
class exit_framework_task :public timeout_task {
	// ͨ�� timeout_task �̳�
	virtual void proc_timeout() override
	{
		ZinxKernel::Zinx_Exit();
	}
};

static exit_framework_task *peft = NULL;

/*�¿ͻ������Ӻ�*/
bool game_role::Init()
{
	/*��������ǵ�һ�����---��ɾ���˳���ʱ����*/
	if (ZinxKernel::Zinx_GetAllRole().size() <= 0)
	{
		if (NULL != peft)
		{
			timeout_deliver::GetInstance()->unregister_task(peft);
			delete peft;
			peft = NULL;
		}
	}

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
	string  talk_content;
	for (auto singlemsg : input_msg.m_msg_list)
	{
		/*������Ϣ���Ͳ�ͬ���в�ͬ����*/
		switch (singlemsg->m_msg_type)
		{
		case GameMsg::MSG_TYPE_TALK_CONTENT:
			/*������Ϣ�Ĵ���--������㲥������Ϣ---�������������*/
			talk_content = singlemsg->GetTalkContent();
			for (auto itr : ZinxKernel::Zinx_GetAllRole())
			{
				auto player = dynamic_cast<game_role *>(itr);
				auto poutmsg = MakeTalkBroadCast(talk_content);
				ZinxKernel::Zinx_SendOut(*poutmsg, *(player->pGameProtocol));
			}
			break;
		case GameMsg::MSG_TYPE_NEW_POSTION:
			/*�����ƶ���Ϣ*/
			//ȡ��������Ϣ
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
	//�ͻ��˶Ͽ�ʱ������Χ��ҷ�����Ͽ�����Ϣ
	/*��ȡ��Χ��ң�ѭ������*/
	auto player_list = g_world.GetSrdPlayers(this);
	for (auto itr : player_list)
	{
		auto player = dynamic_cast<game_role *>(itr);
		auto logoff_msg = MakeLogoff();
		ZinxKernel::Zinx_SendOut(*logoff_msg, *(player->pGameProtocol));
	}
	/*����Ϸ����ժ�����*/
	g_world.DelPlayer(this);

	/*�ж�������������һ��---�����˳���ʱ��*/
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
