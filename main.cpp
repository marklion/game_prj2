#include <iostream>
#include "game_channel.h"
#include "AOI_world.h"
#include "NamePool.h"
#include "timer_channel.h"

using namespace std;

void daemonlize()
{
	/*fork------>�˵�������*/
	int ichildpid = fork();
	if (0 > ichildpid)
	{
		exit(-1);
	}
	if (ichildpid > 0)
	{
		/*������*/
		exit(0);
	}
	
	/*�ӽ���*/
	/*���ûỰid*/
	setsid();
	/*�ض���0 1 2*/
	int nullfd = open("/dev/null", O_RDWR);
	if (nullfd >= 0)
	{
		dup2(nullfd, 0);
		dup2(nullfd, 1);
		dup2(nullfd, 2);

		close(nullfd);
	}

	/*ѭ��fork---��������wait--���ӽ���ִ����Ϸҵ��*/
	while (1)
	{
		int ipid = fork();
		if (ipid < 0)
		{
			exit(-1);
		}
		if (ipid > 0)
		{
			//������
			int status;
			wait(&status);
			/*���ӽ��������˳�--����Ҫѭ��fork���ӽ���*/
			if (0 == status)
			{
				exit(0);
			}
		}
		else
		{
			//�ӽ���
			break;
		}
	}
}

int main()
{
	/*�ý��̱���ػ�����*/
	daemonlize();

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
		ZinxKernel::Zinx_Add_Channel(*(new timer_channel()));
		/*run*/
		ZinxKernel::Zinx_Run();
		ZinxKernel::ZinxKernelFini();
	}

	return 0;
}
