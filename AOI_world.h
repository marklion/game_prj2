#pragma once
#include <list>
#include <vector>

class AOI_Player {
public:
	//�к������������
	virtual int getX() = 0;
	virtual int getY() = 0;
};

class grid {
	/*���������Ҷ����к������������Ķ���*/
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
	/*�洢���ɸ��Ӷ���(������ŷ���)*/
	std::vector<grid> m_grids;

	/*���εĲ���*/
	int x_begin;
	int x_end;
	int y_begin;
	int y_end;
	/*x���y��ֱ�ּ�������*/
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
	/*��ȡXXX����Χ���*/
	std::list<AOI_Player *> GetSrdPlayers(AOI_Player *_player);
	/*�����ҵ���Ϸ����*/
	void AddPlayer(AOI_Player *_player);
	void DelPlayer(AOI_Player *_player);
};

