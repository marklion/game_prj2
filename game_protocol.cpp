#include "game_protocol.h"
#include <string>
#include "GameMsg.h"

using namespace std;


game_protocol::game_protocol()
{
}


game_protocol::~game_protocol()
{
}

UserData * game_protocol::raw2request(std::string _szInput)
{
	/*原始数据（TLV格式数据）----》返回包含multi_msg(若干用户请求GameMsg对象的容器的类)*/
	multi_msg *pret = new multi_msg();
	/*组合上次的报文和这次收到的报文*/
	last_dgram.append(_szInput);
	/*有粘包现象---》处理粘包*/
	while (true)
	{
		/*判断报文是否够长--》缓存*/
		if (last_dgram.size() < 8)
		{
			//肯定短--》return 空
			break;
		}
		/*取出长度和ID*/
		unsigned int uilength = (unsigned int)(unsigned char)last_dgram[0];
		uilength += ((unsigned int)(unsigned char)last_dgram[1]) * 256;
		uilength += ((unsigned int)(unsigned char)last_dgram[2]) * 256 *256;
		uilength += ((unsigned int)(unsigned char)last_dgram[3]) * 256 * 256 * 256;

		unsigned int uiID = (unsigned int)(unsigned char)last_dgram[4];
		uiID |= ((unsigned int)(unsigned char)last_dgram[5]) << 8;
		uiID |= ((unsigned int)(unsigned char)last_dgram[6]) << 16;
		uiID |= ((unsigned int)(unsigned char)last_dgram[7]) << 24;

		/*判断后续内容是否够长*/
		if (uilength > last_dgram.size() - 8)
		{
			/*不够长*/
			break;
		}

		/*取出消息内容，和ID一起构造游戏消息*/
		string msg_content = last_dgram.substr(8, uilength);
		auto pgamemsg = new GameMsg((GameMsg::MSG_TYPE)uiID, msg_content);

		pret->m_msg_list.push_back(pgamemsg);

		/*判断后续是否还有报文--->还会产生很多gamemsg对象*/
		/*弹出已处理的报文，继续循环*/
		last_dgram.erase(0, 8 + uilength);
	}
	
	return pret;
}

std::string * game_protocol::response2raw(UserData & _oUserData)
{
	/*游戏消息(gamemsg)---》字节*/
	auto pret = new string();

	GET_REF2DATA(GameMsg, output_msg, _oUserData);
	auto msg_content = output_msg.serielize();
	/*拼消息长度和消息类型*/
	unsigned int uilength = msg_content.size();
	unsigned int uiID = output_msg.m_msg_type;

	pret->push_back(uilength & 0xff);
	pret->push_back((uilength >> 8) & 0xff);
	pret->push_back((uilength >> 16) & 0xff);
	pret->push_back((uilength >> 24) & 0xff);

	pret->push_back(uiID & 0xff);
	pret->push_back((uiID >> 8) & 0xff);
	pret->push_back((uiID >> 16) & 0xff);
	pret->push_back((uiID >> 24) & 0xff);

	/*追加消息内容*/
	pret->append(msg_content);
	return pret;
}

Irole * game_protocol::GetMsgProcessor(UserDataMsg & _oUserDataMsg)
{
	/*返回绑定的处理role对象*/
	return pGameRole;
}

Ichannel * game_protocol::GetMsgSender(BytesMsg & _oBytes)
{
	/*返回绑定的输出通道*/
	return pGameChannel;
}
