#include "game_channel.h"
#include "game_protocol.h"
#include "game_role.h"


game_channel::game_channel(int _fd):ZinxTcpData(_fd)
{
}


game_channel::~game_channel()
{
	/*���Ӷ���--��ժ��Э�����--���ͷŵ�*/
	ZinxKernel::Zinx_Del_Proto(*pGameProtocol);
	delete pGameProtocol;
}

AZinxHandler * game_channel::GetInputNextStage(BytesMsg & _oInput)
{
	/*���ذ󶨵�Э�����*/
	return pGameProtocol;
}

ZinxTcpData * game_channel_fact::CreateTcpDataChannel(int _fd)
{
	/*����GameChannel���󣬲�����֮*/
	auto pchannel = new game_channel(_fd);
	/*����Э�����*/
	auto pProtocol = new game_protocol();
	/*������ɫ����*/
	auto pRole = new game_role();
	/*Э�����浽ͨ���������ɫ����浽Э������
	ͨ������Ҫ�浽Э�������, Э�����浽��ɫ����*/
	pchannel->bind_protocol(pProtocol);
	pProtocol->bind_role(pRole);
	pProtocol->bind_channel(pchannel);
	pRole->bind_protocol(pProtocol);

	/*��role�����protocol����kernel��*/
	ZinxKernel::Zinx_Add_Proto(*pProtocol);
	ZinxKernel::Zinx_Add_Role(*pRole);

	return pchannel;
}
