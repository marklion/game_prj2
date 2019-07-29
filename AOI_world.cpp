#include "AOI_world.h"
#include <iostream>

AOI_world::AOI_world(int _xbegin, int _xend, int _ybegin, int _yend, int _xcount, int _ycount):
					x_begin(_xbegin),x_end(_xend),y_begin(_ybegin),y_end(_yend),x_count(_xcount),y_count(_ycount)
{
	/*创建所有空网格*/
	grid tmp;
	for (int i = 0; i < x_count * y_count; i++)
	{
		m_grids.push_back(tmp);
	}
}

AOI_world::~AOI_world()
{
}

std::list<AOI_Player*> AOI_world::GetSrdPlayers(AOI_Player * _player)
{
	std::list<AOI_Player *> ret;
	/*将周围玩家append到ret中*/

	/*计算玩家所属网格*/
	int gid = (_player->getX() - x_begin) / GetXWildth() + (_player->getY() - y_begin) / GetYWildth()*x_count;

	/*计算该格子x轴方向数第几个，y轴方向数第几个*/
	int x_index = gid % x_count;
	int y_index = gid / x_count;

	if (x_index > 0 && y_index > 0)
	{
		/*有左上角玩家，则将左上角格子所有玩家append到ret中*/
		auto playerlist = m_grids[gid - 1 - x_count].GetList();
		ret.insert(ret.end(), playerlist.begin(),playerlist.end());
	}
	if (y_index > 0)
	{
		auto playerlist = m_grids[gid - x_count].GetList();
		ret.insert(ret.end(), playerlist.begin(), playerlist.end());
	}
	if (x_index <x_count-1 &&  y_index > 0)
	{
		auto playerlist = m_grids[gid - x_count + 1].GetList();
		ret.insert(ret.end(), playerlist.begin(), playerlist.end());
	}
	if (x_index > 0)
	{
		auto playerlist = m_grids[gid - 1].GetList();
		ret.insert(ret.end(), playerlist.begin(), playerlist.end());
	}

	auto playerlist = m_grids[gid].GetList();
	ret.insert(ret.end(), playerlist.begin(), playerlist.end());

	if ( x_index < x_count - 1)
	{
		auto playerlist = m_grids[gid + 1].GetList();
		ret.insert(ret.end(), playerlist.begin(), playerlist.end());
	}
	if (x_index > 0 && y_index < y_count-1)
	{
		auto playerlist = m_grids[gid -1 + x_count].GetList();
		ret.insert(ret.end(), playerlist.begin(), playerlist.end());
	}
	if (y_index < y_count - 1)
	{
		auto playerlist = m_grids[gid + x_count].GetList();
		ret.insert(ret.end(), playerlist.begin(), playerlist.end());
	}
	if (x_index < x_count - 1 && y_index < y_count - 1)
	{
		auto playerlist = m_grids[gid + x_count + 1].GetList();
		ret.insert(ret.end(), playerlist.begin(), playerlist.end());
	}

	return ret;
}

void AOI_world::AddPlayer(AOI_Player * _player)
{
	/*计算玩家所属网格*/
	int gid = (_player->getX() - x_begin) / GetXWildth() + (_player->getY() - y_begin) / GetYWildth()*x_count;
	/*将玩家添加到该网格*/
	m_grids[gid].AddPlayer(_player);
}

void AOI_world::DelPlayer(AOI_Player * _player)
{
	/*计算玩家所属网格*/
	int gid = (_player->getX() - x_begin) / GetXWildth() + (_player->getY() - y_begin) / GetYWildth()*x_count;
	/*将玩家摘出该网格*/
	m_grids[gid].DelPlayer(_player);
}
