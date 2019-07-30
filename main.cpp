#include <iostream>
#include "game_channel.h"
#include "AOI_world.h"
#include "NamePool.h"
using namespace std;



int main()
{
	/*加载随机姓名文件*/
	if (false == NamePool::GetInstance().LoadFile())
	{
		cout << "加载姓名文件失败" << endl;
		return -1;
	}
		
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