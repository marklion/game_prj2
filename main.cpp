#include <iostream>
#include "game_channel.h"
#include "AOI_world.h"
using namespace std;

class my_player :public AOI_Player {
	
	int x;
	int y;
	// ͨ�� AOI_Player �̳�
	virtual int getX() override
	{
		return x;
	}
	virtual int getY() override
	{
		return y;
	}
	
public:
	std::string name;
	my_player(int _x, int _y, std::string _name) :x(_x), y(_y),name(_name) {}
};

int main()
{
	AOI_world w(20, 200, 50, 230, 6, 6);
	my_player p1(60, 107, "1");
	my_player p2(91, 118, "2");
	my_player p3(147, 133, "3");
	w.AddPlayer(&p1);
	w.AddPlayer(&p2);
	w.AddPlayer(&p3);

	/*��ȡp1��Χ��ң�������ӡ����*/
	for (auto p : w.GetSrdPlayers(&p2))
	{
		cout << dynamic_cast<my_player *>(p)->name << endl;
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