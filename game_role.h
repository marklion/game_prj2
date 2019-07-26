#pragma once
#include <zinx.h>
class game_role :
	public Irole
{
	/*�󶨵����Э��*/
	Iprotocol *pGameProtocol = NULL;
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
};

