#pragma once
#include <zinx.h>
class game_protocol :
	public Iprotocol
{
	/*绑定的输出通道*/
	Ichannel *pGameChannel = NULL;
	/*绑定处理角色对象*/
	Irole *pGameRole = NULL;

	/*缓存上次还未来的级处理的报文*/
	std::string last_dgram;
public:
	game_protocol();
	virtual ~game_protocol();
	void bind_channel(Ichannel *_channel)
	{
		pGameChannel = _channel;
	}
	void bind_role(Irole *_role)
	{
		pGameRole = _role;
	}

	// 通过 Iprotocol 继承
	virtual UserData * raw2request(std::string _szInput) override;
	virtual std::string * response2raw(UserData & _oUserData) override;
	virtual Irole * GetMsgProcessor(UserDataMsg & _oUserDataMsg) override;
	virtual Ichannel * GetMsgSender(BytesMsg & _oBytes) override;
};

