#pragma once
#include <ZinxTCP.h>
class game_channel :
	public ZinxTcpData
{
	Iprotocol *pGameProtocol = NULL;
public:
	game_channel(int _fd);
	virtual ~game_channel();

	// ͨ�� ZinxTcpData �̳�
	virtual AZinxHandler * GetInputNextStage(BytesMsg & _oInput) override;
	void bind_protocol(Iprotocol *_proto)
	{
		pGameProtocol = _proto;
	}
};

class game_channel_fact :public IZinxTcpConnFact {
	// ͨ�� IZinxTcpConnFact �̳�
	virtual ZinxTcpData * CreateTcpDataChannel(int _fd) override;
};
