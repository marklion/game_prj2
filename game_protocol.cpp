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
	/*ԭʼ���ݣ�TLV��ʽ���ݣ�----�����ذ���multi_msg(�����û�����GameMsg�������������)*/
	multi_msg *pret = new multi_msg();
	/*����ϴεı��ĺ�����յ��ı���*/
	last_dgram.append(_szInput);
	/*��ճ������---������ճ��*/
	while (true)
	{
		/*�жϱ����Ƿ񹻳�--������*/
		if (last_dgram.size() < 8)
		{
			//�϶���--��return ��
			break;
		}
		/*ȡ�����Ⱥ�ID*/
		unsigned int uilength = (unsigned int)(unsigned char)last_dgram[0];
		uilength += ((unsigned int)(unsigned char)last_dgram[1]) * 256;
		uilength += ((unsigned int)(unsigned char)last_dgram[2]) * 256 *256;
		uilength += ((unsigned int)(unsigned char)last_dgram[3]) * 256 * 256 * 256;

		unsigned int uiID = (unsigned int)(unsigned char)last_dgram[4];
		uiID |= ((unsigned int)(unsigned char)last_dgram[5]) << 8;
		uiID |= ((unsigned int)(unsigned char)last_dgram[6]) << 16;
		uiID |= ((unsigned int)(unsigned char)last_dgram[7]) << 24;

		/*�жϺ��������Ƿ񹻳�*/
		if (uilength > last_dgram.size() - 8)
		{
			/*������*/
			break;
		}

		/*ȡ����Ϣ���ݣ���IDһ������Ϸ��Ϣ*/
		string msg_content = last_dgram.substr(8, uilength);
		auto pgamemsg = new GameMsg((GameMsg::MSG_TYPE)uiID, msg_content);

		pret->m_msg_list.push_back(pgamemsg);

		/*�жϺ����Ƿ��б���--->��������ܶ�gamemsg����*/
		/*�����Ѵ���ı��ģ�����ѭ��*/
		last_dgram.erase(0, 8 + uilength);
	}
	
	return pret;
}

std::string * game_protocol::response2raw(UserData & _oUserData)
{
	/*��Ϸ��Ϣ(gamemsg)---���ֽ�*/
	auto pret = new string();

	GET_REF2DATA(GameMsg, output_msg, _oUserData);
	auto msg_content = output_msg.serielize();
	/*ƴ��Ϣ���Ⱥ���Ϣ����*/
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

	/*׷����Ϣ����*/
	pret->append(msg_content);
	return pret;
}

Irole * game_protocol::GetMsgProcessor(UserDataMsg & _oUserDataMsg)
{
	/*���ذ󶨵Ĵ���role����*/
	return pGameRole;
}

Ichannel * game_protocol::GetMsgSender(BytesMsg & _oBytes)
{
	/*���ذ󶨵����ͨ��*/
	return pGameChannel;
}
