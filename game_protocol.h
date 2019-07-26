#pragma once
#include <zinx.h>
class game_protocol :
	public Iprotocol
{
	/*�󶨵����ͨ��*/
	Ichannel *pGameChannel = NULL;
	/*�󶨴����ɫ����*/
	Irole *pGameRole = NULL;

	/*�����ϴλ�δ���ļ�����ı���*/
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

	// ͨ�� Iprotocol �̳�
	virtual UserData * raw2request(std::string _szInput) override;
	virtual std::string * response2raw(UserData & _oUserData) override;
	virtual Irole * GetMsgProcessor(UserDataMsg & _oUserDataMsg) override;
	virtual Ichannel * GetMsgSender(BytesMsg & _oBytes) override;
};

