#include <iostream>
#include "game_channel.h"
#include "AOI_world.h"
#include "NamePool.h"
using namespace std;



int main()
{
	/*������������ļ�*/
	if (false == NamePool::GetInstance().LoadFile())
	{
		cout << "���������ļ�ʧ��" << endl;
		return -1;
	}
		
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