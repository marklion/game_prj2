#pragma once
#include <zinx.h>
#include "AOI_world.h"
#include "GameMsg.h"

class game_role :
	public Irole,public AOI_Player
{
	/*����ID������*/
	int iPid = 0;
	std::string m_username = "abc";
	/*��������*/
	float x = 0;//������
	float y = 0;
	float z = 0;//������
	float v = 0;
	/*�󶨵����Э��*/
	Iprotocol *pGameProtocol = NULL;

	//����һ������id�����Ƶ���Ϣ
	GameMsg *MakeLoginIdName();
	//������Χ�����Ϣ
	GameMsg *MakeSrdPlayers();
	//�����Լ�����λ�õ���Ϣ
	GameMsg *MakeInitPos();
	//����Ͽ�����Ϣ
	GameMsg *MakeLogoff();
	//����㲥������Ϣ
	GameMsg *MakeTalkBroadCast(std::string _content);
	//������λ�ù㲥��Ϣ
	GameMsg *MakeNewPostionBroadCast();

	/*�����ƶ���Ϣ*/
	void proc_new_position(float _x, float _y, float _z, float _v);
public:
	game_role();
	virtual ~game_role();
	void bind_protocol(Iprotocol *_proto)
	{
		pGameProtocol = _proto;
	}

	// ͨ�� Irole �̳�
	virtual bool Init() override;
	virtual UserData * ProcMsg(UserData & _poUserData) override;
	virtual void Fini() override;

	// ͨ�� AOI_Player �̳�
	virtual int getX() override;
	virtual int getY() override;
};

