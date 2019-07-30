#include <iostream>
#include "game_channel.h"
#include "AOI_world.h"
#include "NamePool.h"
#include "timer_channel.h"

using namespace std;

void daemonlize()
{
	/*fork------>退掉父进程*/
	int ichildpid = fork();
	if (0 > ichildpid)
	{
		exit(-1);
	}
	if (ichildpid > 0)
	{
		/*父进程*/
		exit(0);
	}
	
	/*子进程*/
	/*设置会话id*/
	setsid();
	/*重定向0 1 2*/
	int nullfd = open("/dev/null", O_RDWR);
	if (nullfd >= 0)
	{
		dup2(nullfd, 0);
		dup2(nullfd, 1);
		dup2(nullfd, 2);

		close(nullfd);
	}

	/*循环fork---》父进程wait--》子进程执行游戏业务*/
	while (1)
	{
		int ipid = fork();
		if (ipid < 0)
		{
			exit(-1);
		}
		if (ipid > 0)
		{
			//父进程
			int status;
			wait(&status);
			/*若子进程正常退出--》不要循环fork新子进程*/
			if (0 == status)
			{
				exit(0);
			}
		}
		else
		{
			//子进程
			break;
		}
	}
}

int main()
{
	/*让进程变成守护进程*/
	daemonlize();

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
		ZinxKernel::Zinx_Add_Channel(*(new timer_channel()));
		/*run*/
		ZinxKernel::Zinx_Run();
		ZinxKernel::ZinxKernelFini();
	}

	return 0;
}
