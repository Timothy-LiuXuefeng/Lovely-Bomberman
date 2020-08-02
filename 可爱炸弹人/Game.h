#ifndef GAME_H

#define GAME_H

#include "Role.h"
#include <list>
#include <vector>

class Game
{
private: 

	static const int playerInitialLife;		//��ҳ�ʼ����
	static const int defPosUnitPerCell;		//��ÿ��ĳ���

public: 
	const std::vector<std::vector<int>>& GetGameMap(unsigned int num) const; 
	Game(int numOfPlayer) : numOfPlayer(numOfPlayer) {}

	int GetNumOfPlayer() const { return numOfPlayer; }

	

	//ǿ��ֹͣ��Ϸ
	void StopGame() { isGaming = false; }

private: 
	
	enum class mapItem
	{
		blank = 0,			//�ո�
		birthPoint = 1,		//������
		softObstacle = 2,	//���ϰ�
		hardObstacle = 3	//Ӳ�ϰ�
	}; 

	//��Ϸ��ͼ
	static const std::vector<std::vector<std::vector<int>>> gameMap; 
	static const unsigned int numOfMap; 
	int numOfPlayer;		//��Ϸ����
	bool isGaming;			//�Ƿ�������Ϸ

};

#endif	//#ifndef GAME_H
