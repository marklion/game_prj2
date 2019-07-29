#pragma once
#include <list>
#include <vector>

class AOI_Player {
public:
	//有横坐标和纵坐标
	virtual int getX() = 0;
	virtual int getY() = 0;
};

class grid {
	/*存放若干玩家对象（有横坐标和纵坐标的对象）*/
	std::list<AOI_Player *> m_players;
public:
	void AddPlayer(AOI_Player *_player) {
		m_players.push_back(_player);
	}
	void DelPlayer(AOI_Player *_player) {
		m_players.remove(_player);
	}
	std::list<AOI_Player *> &GetList() {
		return m_players;
	}
};

class AOI_world
{
	/*存储若干格子对象(按照序号访问)*/
	std::vector<grid> m_grids;

	/*矩形的参数*/
	int x_begin;
	int x_end;
	int y_begin;
	int y_end;
	/*x轴和y轴分别分几个格子*/
	int x_count;
	int y_count;
	int GetXWildth() {
		return (x_end - x_begin) / x_count;
	}
	int GetYWildth() {
		return (y_end - y_begin) / y_count;
	}
public:
	AOI_world(int _xbegin, int _xend, int _ybegin, int _yend, int _xcount, int _ycount);
	virtual ~AOI_world();
	/*获取XXX的周围玩家*/
	std::list<AOI_Player *> GetSrdPlayers(AOI_Player *_player);
	/*添加玩家到游戏世界*/
	void AddPlayer(AOI_Player *_player);
	void DelPlayer(AOI_Player *_player);
};

