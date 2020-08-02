#ifndef GAME_H

#define GAME_H

#include "Role.h"
#include "Obstacle.h"
#include <list>
#include <vector>
#include <functional>

class Game
{
private: 

	static const int playerInitialLife;		//��ҳ�ʼ����
	static const int defPosUnitPerCell;		//��ÿ��ĳ���
	static const int roleInitialMoveSpeed;	//��ҳ�ʼ�ƶ��ٶ�

public: 
	Game(int numOfPlayer, int id1, int id2); 

	int PosToCell(obj_base::sigPosType x) const { return x / defPosUnitPerCell; }
	obj_base::sigPosType CellToPos(int x) const { return x * defPosUnitPerCell + defPosUnitPerCell / 2; }

	const std::vector<std::vector<int>>& GetGameMap(unsigned int num) const; 
	std::list<obj_base*> GetMapObj(int x, int y) const; 
	const std::vector<Role*>& GetRoles() const { return roles; }
	const std::list<Obstacle*>& GetObstacles() const { return obstacles; }
	const std::list<obj_base*>& GetOtherGameObjs() const { return otherGameObjs; }

	int GetNumOfPlayer() const { return numOfPlayer; }
	int GetNumOfLevel() const { return numOfLevel; }

	//��ʼ�µ�һ��
	void InitNewLevel(int newLevel, bool mergeScore); 

	~Game(); 

private: 
	
	enum class mapItem
	{
		blank = 0,			//�ո�
		birthPoint = 1,		//������
		softObstacle = 2,	//���ϰ�
		hardObstacle = 3	//Ӳ�ϰ�
	}; 

	//��ɫ��roles[0]��δʹ�õ�
	std::vector<Role*> roles; 
	//�ϰ�
	std::list<Obstacle*> obstacles; 
	//����
	std::list<obj_base*> otherGameObjs; 

	int numOfPlayer;		//��Ϸ����
	int id1;				//1P��ID
	int id2;				//2P��ID������еĻ���
	
	//��Ϸ��ͼ
	static const std::vector<std::vector<std::vector<int>>> gameMap; 
	static const unsigned int numOfLevel; 
};

#endif	//#ifndef GAME_H
