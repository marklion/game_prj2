#include "game_protocol.h"



game_protocol::game_protocol()
{
}


game_protocol::~game_protocol()
{
}

UserData * game_protocol::raw2request(std::string _szInput)
{
	return nullptr;
}

std::string * game_protocol::response2raw(UserData & _oUserData)
{
	return nullptr;
}

Irole * game_protocol::GetMsgProcessor(UserDataMsg & _oUserDataMsg)
{
	return nullptr;
}

Ichannel * game_protocol::GetMsgSender(BytesMsg & _oBytes)
{
	return nullptr;
}
