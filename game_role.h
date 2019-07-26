#pragma once
#include <zinx.h>
class game_role :
	public Irole
{
	/*绑定的输出协议*/
	Iprotocol *pGameProtocol = NULL;
public:
	game_role();
	virtual ~game_role();

	// 通过 Irole 继承
	virtual bool Init() override;
	virtual UserData * ProcMsg(UserData & _poUserData) override;
	virtual void Fini() override;
};

