#include "Game.h"

const unsigned int Game::numOfLevel = 1; 
const int Game::playerInitialLife = 3;		//��ҳ�ʼ����
const int Game::defPosUnitPerCell = 1024;	//��ÿ��ĳ���
const int Game::roleInitialMoveSpeed = 20;	//��ҳ�ʼ�ƶ��ٶ�

const std::vector<std::vector<std::vector<int>>> Game::gameMap
{
	{
		{0, 0, 0, 0, 3, 2, 0, 0, 2, 0, 2, 2, 3, 0, 0}, 
		{2, 2, 3, 2, 0, 0, 3, 3, 0, 3, 3, 0, 0, 2, 3}, 
		{3, 2, 3, 0, 3, 3, 2, 3, 2, 0, 3, 0, 3, 2, 2}, 
		{3, 0, 0, 1, 0, 0, 2, 2, 3, 0, 0, 1, 0, 0, 3}, 
		{3, 0, 3, 0, 3, 0, 3, 2, 0, 0, 3, 0, 3, 2, 0}, 
		{2, 0, 2, 0, 3, 2, 3, 3, 3, 0, 2, 2, 3, 3, 2}, 
		{3, 0, 3, 2, 3, 0, 3, 3, 0, 3, 3, 0, 2, 3, 3}, 
		{2, 0, 0, 0, 2, 3, 0, 3, 0, 0, 2, 0, 3, 2, 3}, 
		{3, 0, 3, 0, 3, 3, 2, 2, 2, 3, 3, 0, 3, 0, 2}, 
		{2, 0, 0, 1, 0, 2, 2, 3, 3, 0, 0, 1, 0, 0, 2}, 
		{0, 3, 3, 0, 3, 3, 3, 3, 2, 2, 3, 0, 3, 2, 3}, 
		{0, 2, 3, 0, 3, 2, 0, 2, 3, 0, 2, 2, 0, 2, 2}, 
		{2, 3, 2, 2, 0, 0, 3, 2, 0, 2, 3, 0, 2, 3, 0}
	}
}; 

const std::vector<std::vector<int>>& Game::GetGameMap(unsigned int num) const
{
	return gameMap[num % numOfLevel]; 
}

Game::Game(int numOfPlayer, int id1, int id2) : numOfPlayer(numOfPlayer), id1(id1), id2(id2)
{
	int rows = gameMap[0].size(), cols = gameMap[0][0].size(); 
	roles.resize(5, nullptr); 
	obj_base::SetMapBound(rows, cols, defPosUnitPerCell); 
	std::function<int(int, int)> GetRoleID = [rows, cols](int x, int y)
	{
		if (x < rows / 2 && y < cols / 2) return 1; 
		if (x >= rows / 2 && y < cols / 2) return 2; 
		if (x < rows / 2 && y >= cols / 2) return 3; 
		return 4; 
	}; 
	for (int x = 0; x < rows; ++x)
		for (int y = 0; y < cols; ++y)
			if (static_cast<mapItem>(gameMap[0][x][y]) == mapItem::birthPoint)
			{
				int roleID = GetRoleID(x, y); 
				if (roleID == id1 || numOfPlayer == 2 && roleID == id2)
					roles[roleID] = new Role(CellToPos(x), CellToPos(y), roleInitialMoveSpeed, roleID, playerInitialLife);
				else roles[roleID] = new Role(CellToPos(x), CellToPos(y), roleInitialMoveSpeed, roleID, 1);
			}
}

Game::~Game()
{
	for (int i = 1; i <= 4; ++i)
		delete roles[i]; 
	for (auto p : obstacles)
		delete p; 
	for (auto p : otherGameObjs)
		delete p; 
}

void Game::InitNewLevel(int newLevel, bool mergeScore)
{
	for (int i = 1; i <= 4; ++i)
	{
		//�����
		if (i == id1 || numOfPlayer == 2 && i == id2)
		{
			if (mergeScore) roles[i]->MergeScore();
			roles[i]->Reset(false, false);
		}
		else roles[i]->Reset(true, true); 
	}
	int rows = gameMap[newLevel].size(), cols = gameMap[newLevel][0].size(); 
	for (int x = 0; x < rows; ++x)
		for (int y = 0; y < cols; ++y)
		{
			switch (static_cast<mapItem>(gameMap[newLevel][x][y]))
			{
			case mapItem::hardObstacle: 
				obstacles.push_back(new HardObstacle(CellToPos(x), CellToPos(y))); 
				break; 
			case mapItem::softObstacle:
				obstacles.push_back(new SoftObstacle(CellToPos(x), CellToPos(y)));
				break;
			}
		}
}

std::list<obj_base*> Game::GetMapObj(int x, int y) const
{
	std::list<obj_base*> res; 
	for (int i = 1; i <= 4; ++i)
	{
		obj_base::posType pos = roles[i]->GetPos(); 
		if (roles[i]->IsLiving() && PosToCell(pos.x) == x && PosToCell(pos.y) == y) 
			res.push_back(static_cast<obj_base*>(roles[i])); 
	}
	for (auto p : obstacles)
	{
		obj_base::posType pos = p->GetPos();
		if (PosToCell(pos.x) == x && PosToCell(pos.y) == y)
			res.push_back(static_cast<obj_base*>(p)); 
	}
	for (auto p : otherGameObjs)
	{
		//���������ĵ����ǿ�������
		if (p->GetObjType() == obj_base::objType::prop && dynamic_cast<Prop*>(p)->IsPicked()) continue; 
		obj_base::posType pos = p->GetPos();
		if (PosToCell(pos.x) == x && PosToCell(pos.y) == y)
			res.push_back(static_cast<obj_base*>(p));
	}
	return res; 
}
