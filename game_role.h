#pragma once
#include <zinx.h>
#include "AOI_world.h"
#include "GameMsg.h"

class game_role :
	public Irole,public AOI_Player
{
	/*定义ID和姓名*/
	int iPid = 0;
	std::string m_username = "abc";
	/*定义坐标*/
	float x = 0;//横坐标
	float y = 0;
	float z = 0;//纵坐标
	float v = 0;
	/*绑定的输出协议*/
	Iprotocol *pGameProtocol = NULL;

	//构造一个包含id和名称的消息
	GameMsg *MakeLoginIdName();
	//构造周围玩家消息
	GameMsg *MakeSrdPlayers();
	//构造自己出生位置的消息
	GameMsg *MakeInitPos();
	//构造断开的消息
	GameMsg *MakeLogoff();
	//构造广播聊天消息
	GameMsg *MakeTalkBroadCast(std::string _content);
	//构造新位置广播消息
	GameMsg *MakeNewPostionBroadCast();

	/*处理移动消息*/
	void proc_new_position(float _x, float _y, float _z, float _v);
public:
	game_role();
	virtual ~game_role();
	void bind_protocol(Iprotocol *_proto)
	{
		pGameProtocol = _proto;
	}

	// 通过 Irole 继承
	virtual bool Init() override;
	virtual UserData * ProcMsg(UserData & _poUserData) override;
	virtual void Fini() override;

	// 通过 AOI_Player 继承
	virtual int getX() override;
	virtual int getY() override;
};

