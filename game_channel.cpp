#include "game_channel.h"
#include "game_protocol.h"
#include "game_role.h"


game_channel::game_channel(int _fd):ZinxTcpData(_fd)
{
}


game_channel::~game_channel()
{
	/*连接断了--》摘出协议对象--》释放掉*/
	ZinxKernel::Zinx_Del_Proto(*pGameProtocol);
	delete pGameProtocol;
}

AZinxHandler * game_channel::GetInputNextStage(BytesMsg & _oInput)
{
	/*返回绑定的协议对象*/
	return pGameProtocol;
}

ZinxTcpData * game_channel_fact::CreateTcpDataChannel(int _fd)
{
	/*创建GameChannel对象，并返回之*/
	auto pchannel = new game_channel(_fd);
	/*创建协议对象*/
	auto pProtocol = new game_protocol();
	/*创建角色对象*/
	auto pRole = new game_role();
	/*协议对象存到通道对象里，角色对象存到协议对象里，
	通道对象要存到协议对象里, 协议对象存到角色对象*/
	pchannel->bind_protocol(pProtocol);
	pProtocol->bind_role(pRole);
	pProtocol->bind_channel(pchannel);
	pRole->bind_protocol(pProtocol);

	/*存role对象和protocol对象到kernel中*/
	ZinxKernel::Zinx_Add_Proto(*pProtocol);
	ZinxKernel::Zinx_Add_Role(*pRole);

	return pchannel;
}
