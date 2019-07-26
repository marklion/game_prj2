#include "game_channel.h"



game_channel::game_channel(int _fd):ZinxTcpData(_fd)
{
}


game_channel::~game_channel()
{
}

AZinxHandler * game_channel::GetInputNextStage(BytesMsg & _oInput)
{
	return nullptr;
}

ZinxTcpData * game_channel_fact::CreateTcpDataChannel(int _fd)
{
	return nullptr;
}
