#ifndef GAME_H

#define GAME_H

#include "Role.h"
#include <list>
#include <vector>

class Game
{
public: 
	const std::vector<std::vector<int>>& GetGameMap(unsigned int num) const; 

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
};

#endif	//#ifndef GAME_H
