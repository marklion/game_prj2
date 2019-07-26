#include "GameMsg.h"
#include <iostream>

using namespace std;

GameMsg::GameMsg(MSG_TYPE _type, std::string _V):m_msg_type(_type)
{
	/*打印消息内容（用来测试）*/
	cout << _V << endl;
}

GameMsg::~GameMsg()
{
}

std::string GameMsg::serielize()
{
	/*返回hello*/
	return "hello";
}

