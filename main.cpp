#include <iostream>
#include "game_channel.h"
using namespace std;

int main()
{
	/*��ʼ�����*/
	if (true == ZinxKernel::ZinxKernelInit())
	{
		/*��Ӽ���ͨ����kernel*/
		if (true != ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899, new game_channel_fact()))))
		{
			cout << "listen failed" << endl;
		}
		/*run*/
		ZinxKernel::Zinx_Run();
		ZinxKernel::ZinxKernelFini();
	}

}