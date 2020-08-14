#ifndef GAME_H

#define GAME_H

#include "Role.h"
#include "Obstacle.h"
#include "TNT.h"
#include "BombArea.h"
#include <ctime>
#include <random>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <thread>
#include <functional>

#ifdef _DEBUG
#include <iostream>
#endif

class Game
{
private: 

	static const int playerInitialLife;						//��ҳ�ʼ����
	static const int playerMissingTime;						//����ܵ���������ܱ���ʱ��
	static const obj_base::sigPosType defPosUnitPerCell;	//��ÿ��ĳ���
	static const obj_base::sigPosType roleInitialMoveSpeed;	//��ҳ�ʼ�ƶ��ٶ�
	static const obj_base::sigPosType tntInitialMoveSpeed;	//ը����ʼ�ƶ��ٶ�
	static const int tntInitialDistance;					//ը����ʼ��ը����
	static const int tntInitialTime;						//ը����ʼ��ըʱ�䣨���룩
	static const int tntBombAreaTime;						//ը���ı�ը�������ʱ��
	static const int mineInitialTime;						//ը���ʹ�����˹��ʼ����ʱ��
	static const int grenadeMaxDistance;					//�����ƶ�����
	static const int grenadeInitialTime;					//���񵯱�ը����ʱ��
	static const int fireMaxDistance;						//����ǹ��������
	static const int fireInitialTime;						//����ǹ�������ʱ��
	static const int scoreOfDestroyObstacle;				//�ݻ��ϰ���÷�
	static const int scoreOfHitOthers;						//����������ɫ�÷�
	static const int scoreOfPickProp;						//����ߵ÷�

	//������ը���ƶ��ٶ�
	static const std::map<Prop::propType, obj_base::sigPosType> propMoveSpeed;
	//�������ߵĴ���
	static const std::vector<Prop::propType> propNums; 
	//���������
	std::default_random_engine randNum;	

public: 
	Game(int numOfPlayer, int id1, int id2); 

	int PosToCell(obj_base::sigPosType x) const { return x / defPosUnitPerCell; }
	obj_base::sigPosType CellToPos(int x) const { return x * defPosUnitPerCell + defPosUnitPerCell / 2; }

	const std::vector<std::vector<int>>& GetGameMap(unsigned int num) const; 
	std::list<obj_base*> GetMapObj(int x, int y) const; 
	int GetID1() const { return id1; }
	int GetID2() const { return id2; }
	int GetNowLevel() const { return nowLevel; }
	static int GetPosUnitPerCell() { return defPosUnitPerCell; }
	const Role* GetRole(int roleID) { return roles[roleID]; }
	//////const std::vector<Role*>& GetRoles() const { return roles; }
	//////const std::list<Obstacle*>& GetObstacles() const { return obstacles; }
	//////const std::list<obj_base*>& GetOtherGameObjs() const { return otherGameObjs; }

	int GetNumOfPlayer() const { return numOfPlayer; }
	int GetNumOfLevel() const { return (int)gameMap.size(); }

	//��ʼ�µ�һ��
	void InitNewLevel(int newLevel, bool mergeScore); 

	//��������Ϸ��Ϊ
	
	//����
	void WalkUpOneCell(int roleID, int stepInterval) { WalkOneCell(roleID, direction::Up, stepInterval); }			//������һ��
	void WalkDownOneCell(int roleID, int stepInterval) { WalkOneCell(roleID, direction::Down, stepInterval); }		//������һ��
	void WalkLeftOneCell(int roleID, int stepInterval) { WalkOneCell(roleID, direction::Left, stepInterval); }		//������һ��
	void WalkRightOneCell(int roleID, int stepInterval) { WalkOneCell(roleID, direction::Right, stepInterval); }	//������һ��

	//����ը��
	void LayTnt(int roleID); 

	//���������ݼ��

	//�������
	void CheckRole(); 
	//���ը���Լ���ը���򣬶�����
	void CheckBomb(int dataScanInterval);
	//�����Ϸ�Ƿ����
	bool CheckGameEnd() const; 
	
	//��ȡ�����
	auto GetRandom() { return randNum(); }

	~Game(); 

private: 

	using direction = obj_base::direction; 

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
	mutable std::mutex obstaclesMutex; 
	//����
	std::list<obj_base*> otherGameObjs; 
	mutable std::mutex otherGameObjsMutex; 
	//����վ����ֹ�����߳�ɾ��ͬһ������ʱ�������쳣��Ϊ
	std::list<obj_base*> deletedObjs; 
	mutable std::mutex deletedObjsMutex; 

	int numOfPlayer;						//��Ϸ����
	int id1;								//1P��ID
	int id2;								//2P��ID������еĻ���
	int nowLevel;							//��ǰ��

	
	
	//��Ϸ��ͼ
	static const std::vector<std::vector<std::vector<int>>> gameMap; 

	//��Ϸ��Ϊ
	void WalkOneCell(int roleID, direction direct, int stepInterval);		//����һ��
	bool MoveTnt(TNT* pTnt, direction direct);								//�ƶ�ը��
	void BombTnt(TNT* pTnt);												//ʹ��ը����ը
	void BombFire(Fire* pFire);												//ʹ�û���ǹ��ը
	void BombGrenade(Grenade* pGrenade);									//ʹ�����񵯱�ը
	void BombMissile(Missile* pMissile);									//������ը
	void BombMapCell(BombArea* pBombArea);									//���Ƶ�ͼ��һ��
	void RoleMiss(Role *pRole); 											//�����ɫ���˺�ı���״̬
	void CreateProp(int xc, int yc);										//�ڸõ��������
};

#endif	//#ifndef GAME_H
