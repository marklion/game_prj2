#include "GameMsg.h"
#include <iostream>

using namespace std;

GameMsg::GameMsg(MSG_TYPE _type, std::string _V):m_msg_type(_type)
{
	/*��ӡ��Ϣ���ݣ��������ԣ�*/
	cout << _V << endl;
}

GameMsg::~GameMsg()
{
}

std::string GameMsg::serielize()
{
	/*����hello*/
	return "hello";
}

