#include <iostream>
#include "game_channel.h"
using namespace std;

int main()
{
	/*初始化框架*/
	if (true == ZinxKernel::ZinxKernelInit())
	{
		/*添加监听通道到kernel*/
		if (true != ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899, new game_channel_fact()))))
		{
			cout << "listen failed" << endl;
		}
		/*run*/
		ZinxKernel::Zinx_Run();
		ZinxKernel::ZinxKernelFini();
	}

}