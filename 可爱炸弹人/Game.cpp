#include "Game.h"

const int Game::playerInitialLife = 3;						//��ҳ�ʼ����
const int Game::playerMissingTime = 700;					//����ܵ���������ܱ���ʱ��
const obj_base::sigPosType Game::defPosUnitPerCell = 1024;	//ÿ��ĳ���
const obj_base::sigPosType Game::roleInitialMoveSpeed = 96;	//��ҳ�ʼ�ƶ��ٶ�
const obj_base::sigPosType Game::tntInitialMoveSpeed = 512;	//ը����ʼ�ƶ��ٶ�
const int Game::tntInitialDistance = 2;						//ը����ʼ��ը����
const int Game::tntInitialTime = 2000;						//ը����ʼ��ըʱ��
const int Game::tntBombAreaTime = 500;						//ը���ı�ը�������ʱ��
const int Game::mineInitialTime = 8000;						//���׺ʹ�����˹��ʼ����ʱ��
const int Game::grenadeMaxDistance = 2;						//�����ƶ�����
const int Game::grenadeInitialTime = 100;					//���񵯱�ը����ʱ��
const int Game::fireMaxDistance = 3;						//����ǹ��������
const int Game::fireInitialTime = 500;						//����ǹ�������ʱ��
const int Game::scoreOfDestroyObstacle = 1;					//�ݻ��ϰ���÷�
const int Game::scoreOfHitOthers = 20;						//ɱ��������ɫ�÷�
const int Game::scoreOfPickProp = 2;						//����ߵ÷�

//������ը���ƶ��ٶ�
const std::map<Prop::propType, obj_base::sigPosType> Game::propMoveSpeed
{
	std::make_pair(Prop::propType::lachrymator, 0), 
	std::make_pair(Prop::propType::mine, 0),
	std::make_pair(Prop::propType::fire, 0),
	std::make_pair(Prop::propType::ice, 0),
	std::make_pair(Prop::propType::grenade, 256),
	std::make_pair(Prop::propType::missile, 128),
}; 

//�������ߵĴ���
const std::vector<Prop::propType> Game::propNums
{
	Prop::propType::null, Prop::propType::null, Prop::propType::null, Prop::propType::null, Prop::propType::null, Prop::propType::null, //6��
	Prop::propType::glove, Prop::propType::glove,Prop::propType::glove,Prop::propType::glove,Prop::propType::glove, //5��
	Prop::propType::shield, Prop::propType::shield,	//2��
	Prop::propType::addtnt, Prop::propType::addtnt, Prop::propType::addtnt, Prop::propType::addtnt, Prop::propType::addtnt, Prop::propType::addtnt, Prop::propType::addtnt, Prop::propType::addtnt, Prop::propType::addtnt, Prop::propType::addtnt,	//10��
	Prop::propType::addlife,	//1��
	Prop::propType::shoe, Prop::propType::shoe,Prop::propType::shoe,Prop::propType::shoe,Prop::propType::shoe,Prop::propType::shoe,Prop::propType::shoe,Prop::propType::shoe,	//8��
	Prop::propType::jinKeLa, Prop::propType::jinKeLa, Prop::propType::jinKeLa, Prop::propType::jinKeLa, Prop::propType::jinKeLa, Prop::propType::jinKeLa, Prop::propType::jinKeLa, Prop::propType::jinKeLa, Prop::propType::jinKeLa, Prop::propType::jinKeLa,//10��
	Prop::propType::lachrymator, Prop::propType::lachrymator,	//2��
	Prop::propType::mine, Prop::propType::mine, Prop::propType::mine,	//3��
	Prop::propType::fire, Prop::propType::fire, Prop::propType::fire,	//3��
	////Prop::propType::ice,	//��ʱû��д�������߼�����������
	Prop::propType::grenade, Prop::propType::grenade, Prop::propType::grenade, //3��
	Prop::propType::missile, Prop::propType::missile, Prop::propType::missile, //3��
	Prop::propType::null, Prop::propType::null, Prop::propType::null, Prop::propType::null, Prop::propType::null, Prop::propType::null	//6��
}; 

//��Ϸ��ͼ��0�ǿգ�1�ǳ����㣬2�����ϰ���3��Ӳ�ϰ�
const std::vector<std::vector<std::vector<int>>> Game::gameMap
{
	{
		{2, 0, 0, 3, 0, 3, 2, 2, 3, 2, 0, 3, 0, 3, 2}, 
		{0, 3, 2, 0, 2, 0, 2, 0, 2, 2, 2, 0, 2, 2, 2}, 
		{0, 3, 3, 0, 3, 3, 2, 3, 3, 0, 3, 0, 3, 2, 2}, 
		{0, 0, 0, 1, 0, 0, 2, 0, 0, 2, 0, 1, 0, 0, 3}, 
		{2, 2, 3, 0, 3, 2, 0, 3, 3, 2, 3, 0, 3, 2, 2}, 
		{2, 3, 3, 2, 2, 0, 3, 3, 0, 2, 2, 2, 2, 2, 2}, 
		{2, 0, 3, 3, 2, 3, 3, 3, 0, 3, 3, 3, 2, 3, 2}, 
		{2, 2, 2, 0, 2, 2, 3, 2, 2, 0, 0, 2, 0, 3, 2}, 
		{2, 3, 3, 0, 3, 0, 0, 0, 3, 2, 3, 0, 3, 3, 2}, 
		{0, 2, 0, 1, 0, 2, 3, 2, 3, 0, 0, 1, 0, 2, 2}, 
		{2, 3, 3, 0, 3, 0, 3, 2, 0, 2, 3, 0, 3, 3, 0}, 
		{0, 2, 3, 0, 3, 2, 3, 0, 3, 2, 0, 2, 2, 2, 0}, 
		{2, 2, 2, 2, 2, 0, 2, 2, 0, 2, 2, 3, 0, 3, 2} 
	}, 
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
	}, 
	{
		{2, 3, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0}, 
		{2, 2, 0, 0, 3, 3, 3, 2, 3, 2, 3, 2, 3, 2, 3}, 
		{2, 3, 3, 0, 3, 0, 2, 2, 2, 0, 3, 0, 3, 0, 2}, 
		{2, 2, 0, 1, 0, 2, 3, 3, 2, 0, 0, 1, 0, 2, 2}, 
		{3, 2, 3, 0, 3, 0, 0, 2, 2, 3, 3, 0, 3, 2, 3}, 
		{2, 0, 2, 0, 2, 2, 3, 3, 0, 2, 0, 0, 2, 0, 2}, 
		{0, 2, 3, 3, 0, 0, 3, 3, 3, 0, 0, 2, 2, 3, 3}, 
		{0, 3, 3, 2, 2, 2, 2, 0, 2, 2, 3, 2, 2, 2, 2}, 
		{0, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 3, 2}, 
		{2, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 2}, 
		{0, 3, 3, 0, 3, 0, 3, 0, 3, 0, 3, 0, 3, 2, 3}, 
		{2, 2, 0, 2, 2, 2, 3, 2, 3, 0, 3, 0, 2, 2, 2}, 
		{0, 3, 0, 2, 3, 0, 0, 0, 2, 2, 0, 0, 3, 0, 0}
	}, 
	{
		{2, 2, 0, 2, 2, 2, 3, 0, 2, 2, 2, 2, 3, 0, 2}, 
		{2, 3, 2, 2, 3, 2, 3, 2, 3, 3, 3, 0, 2, 2, 0}, 
		{2, 3, 3, 0, 3, 0, 2, 2, 0, 0, 3, 0, 3, 0, 3}, 
		{0, 0, 0, 1, 0, 0, 3, 2, 3, 2, 0, 1, 0, 2, 2}, 
		{2, 3, 3, 0, 3, 2, 2, 0, 3, 0, 3, 0, 3, 0, 3}, 
		{0, 0, 0, 2, 0, 0, 3, 3, 0, 0, 2, 2, 3, 2, 0}, 
		{0, 3, 3, 3, 3, 3, 3, 3, 2, 3, 3, 2, 3, 2, 3}, 
		{2, 0, 2, 0, 0, 2, 2, 2, 0, 3, 2, 0, 2, 0, 2}, 
		{0, 3, 3, 0, 3, 2, 3, 3, 2, 3, 3, 0, 3, 0, 3}, 
		{2, 0, 0, 1, 0, 2, 0, 2, 3, 0, 0, 1, 0, 2, 2}, 
		{0, 2, 3, 0, 3, 0, 3, 2, 3, 2, 3, 0, 3, 3, 0}, 
		{2, 3, 3, 0, 3, 0, 2, 2, 2, 0, 3, 2, 2, 2, 2}, 
		{0, 2, 2, 2, 0, 2, 3, 2, 3, 0, 0, 0, 3, 2, 0}
	}, 
	{
		{2, 2, 2, 0, 2, 0, 0, 0, 2, 2, 3, 0, 3, 0, 0},
		{2, 3, 0, 2, 3, 0, 3, 3, 3, 2, 2, 2, 2, 2, 0},
		{0, 3, 3, 0, 3, 0, 3, 0, 3, 2, 3, 0, 3, 3, 2},
		{2, 2, 0, 1, 0, 2, 0, 0, 2, 0, 0, 1, 0, 2, 0},
		{3, 0, 3, 0, 3, 2, 3, 2, 3, 2, 3, 0, 3, 3, 0},
		{0, 0, 3, 2, 0, 2, 3, 3, 3, 0, 2, 0, 3, 0, 0},
		{3, 0, 0, 2, 3, 2, 3, 3, 2, 0, 3, 2, 2, 0,3 },
		{2, 0, 3, 2, 3, 0, 3, 0, 3, 2, 2, 2, 3, 0, 0},
		{3, 2, 3, 0, 3, 0, 0, 2, 2, 0, 3, 0, 3, 0, 3},
		{2, 2, 0, 1, 0, 2, 3, 3, 2, 0, 0, 1, 0, 2, 2},
		{2, 3, 3, 0, 3, 2, 2, 0, 0, 0, 3, 0, 3, 3, 2},
		{2, 0, 2, 0, 0, 2, 3, 2, 3, 2, 0, 0, 2, 3, 0},
		{2, 2, 3, 2, 3, 2, 2, 2, 0, 2, 3, 0, 0, 0, 2}
	}
}; 

const std::vector<std::vector<int>>& Game::GetGameMap(unsigned int num) const
{
	return gameMap[num % (int)gameMap.size()];
}

Game::Game(int numOfPlayer, int id1, int id2, Difficulty difficulty) : numOfPlayer(numOfPlayer), id1(id1), id2(id2), nowLevel(0), difficulty(difficulty), randNum((unsigned)time(nullptr))
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
	for (auto p : deletedObjs)
		delete p; 
}

void Game::InitNewLevel(int newLevel, bool mergeScore)
{

	//�����һ�صĻ���վ
	for (auto p : deletedObjs) delete p;
	deletedObjs.clear();
	//�����һ�ص��ϰ������
	for (auto p : obstacles) delete p; 
	obstacles.clear(); 
	for (auto p : otherGameObjs) delete p; 
	otherGameObjs.clear(); 

	newLevel %= (int)gameMap.size();
	nowLevel = newLevel; 
	for (int i = 1; i <= 4; ++i)
	{
		//�����
		if (i == id1 || numOfPlayer == 2 && i == id2)
		{
			if (mergeScore) roles[i]->MergeScore();
			roles[i]->Reset(false, false);
			if (difficulty == Difficulty::easy)
			{
				roles[i]->WearShield(); 
			}
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
				obstacles.push_back(new HardObstacle(CellToPos(x), CellToPos(y), randNum() % 2)); 
				break; 
			case mapItem::softObstacle:
				obstacles.push_back(new SoftObstacle(CellToPos(x), CellToPos(y), randNum() % 2));
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
	obstaclesMutex.lock(); 
	for (auto p : obstacles)
	{
		obj_base::posType pos = p->GetPos();
		if (PosToCell(pos.x) == x && PosToCell(pos.y) == y)
			res.push_back(static_cast<obj_base*>(p)); 
	}
	obstaclesMutex.unlock();
	otherGameObjsMutex.lock(); 
	for (auto p : otherGameObjs)
	{
		//���������ĵ����ǿ�������
		if (p->GetObjType() == obj_base::objType::prop && dynamic_cast<Prop*>(p)->IsPicked()) continue; 
		obj_base::posType pos = p->GetPos();
		if (PosToCell(pos.x) == x && PosToCell(pos.y) == y)
			res.push_back(static_cast<obj_base*>(p));
	}
	otherGameObjsMutex.unlock();
	return res; 
}

void Game::LayTnt(int roleID)
{
	Role* pRole = roles[roleID]; 
	auto [x, y] = pRole->GetPos(); 
	auto xc = PosToCell(x), yc = PosToCell(y); 
	std::list<obj_base*> mapObjList = GetMapObj(xc, yc); 
	for (auto pObj : mapObjList)
	{
		switch (pObj->GetObjType())
		{
		case obj_base::objType::tnt: return; 
		case obj_base::objType::prop: 
		{
			Prop* pProp = dynamic_cast<Prop*>(pObj); 
			switch (pProp->GetPropType())
			{
			case Prop::propType::mine: case Prop::propType::lachrymator: 
				if (pProp->IsLaid()) return;		//�Ѿ���������������ը����
			}
			break; 
		}
		}
	}

	pRole->GetMutex().lock();						//��ס��ɫ
	SpecialBomb* pWeapon = pRole->GetWeapon();		//��ȡ����
	pRole->GetMutex().unlock();						//������ɫ

	if (pWeapon)									//�������������
	{
		Prop::propType weaponType = pWeapon->GetPropType();
		//���񵯺ͻ���ǹ����Խ�������Ӳ�ϰ�����
		if (weaponType == Prop::propType::grenade || weaponType == Prop::propType::fire)
		{
			int xcDest = xc, ycDest = yc;
			int offset;
			if (weaponType == Prop::propType::grenade) offset = grenadeMaxDistance;
			else offset = 1;
			switch (pRole->GetDirect())
			{
			case obj_base::direction::Up: xcDest = xc - offset; break;
			case obj_base::direction::Down: xcDest = xc + offset; break;
			case obj_base::direction::Left: ycDest = yc - offset; break;
			case obj_base::direction::Right: ycDest = yc + offset; break;
			}
			if (xcDest < 0 || xcDest >= (int)gameMap[nowLevel].size() || ycDest < 0 || ycDest >= (int)gameMap[nowLevel][0].size()) return;	//Խ��
			auto tmpMapObjList = GetMapObj(xcDest, ycDest);
			for (auto pTmpMapObj : tmpMapObjList) if (pTmpMapObj->GetObjType() == obj_base::objType::hardObstacle) return;		//��Ӳ�ϰ�
		}
		pRole->GetMutex().lock();					//��ס��ɫ
		pRole->SetWeapon(nullptr); 
		pRole->GetMutex().unlock();					//������ɫ
		switch (pRole->GetDirect())
		{
		case obj_base::direction::Up: pWeapon->SetLaidUp(x, y, propMoveSpeed.at(pWeapon->GetPropType())); break; 
		case obj_base::direction::Down: pWeapon->SetLaidDown(x, y, propMoveSpeed.at(pWeapon->GetPropType())); break; 
		case obj_base::direction::Left: pWeapon->SetLaidLeft(x, y, propMoveSpeed.at(pWeapon->GetPropType())); break; 
		case obj_base::direction::Right: pWeapon->SetLaidRight(x, y, propMoveSpeed.at(pWeapon->GetPropType())); break; 
		}
		otherGameObjsMutex.lock(); otherGameObjs.push_back(pWeapon); otherGameObjsMutex.unlock();
	}
	else
	{
		pRole->GetMutex().lock();					//��ס��ɫ
		bool successLayTnt = pRole->LayTNT();
		pRole->GetMutex().unlock();					//������ɫ
		if (successLayTnt)
		{
			TNT* pTNT = new TNT(x, y, false, tntInitialMoveSpeed, roleID, pRole->GetDistance(), tntInitialTime);
			otherGameObjsMutex.lock(); otherGameObjs.push_back(static_cast<obj_base*>(pTNT)); otherGameObjsMutex.unlock();
		}
	}
}

void Game::WalkOneCell(int roleID, direction direct, int stepInterval)
{
	Role* pRole = roles[roleID]; 
	if (!pRole->IsLiving()) return;											//�����˲�������
	auto x0 = roles[roleID]->GetPos().x, y0 = roles[roleID]->GetPos().y;	//��ȡ��ɫλ��
	auto x = x0, y = y0; 
	int xc = PosToCell(x0), yc = PosToCell(y0); 
	std::function<int(void)> leftDistance = nullptr;						//����ʣ�����
	std::function<void(void)> walk = nullptr, walkHalf = nullptr;			//����
	std::list<obj_base*> mapObjList; 
	switch (direct)
	{
	case direction::Up:
	{
		//���ó���
		pRole->GetMutex().lock(); pRole->SetDirectUp(); pRole->GetMutex().unlock();
		if (xc <= 0) return;			//�Ѿ�����߽�
		mapObjList = GetMapObj(xc - 1, yc);
		leftDistance = [&x0, &y0, &x, &y]() { return x - (x0 - defPosUnitPerCell); };
		walk = [pRole]() { pRole->MoveUp(); };
		walkHalf = [pRole, &leftDistance] { pRole->MoveUpLessThanOneStep(leftDistance()); };
		break;
	}
	case direction::Down: 
	{
		//���ó���
		pRole->GetMutex().lock(); pRole->SetDirectDown(); pRole->GetMutex().unlock();
		if (xc >= (obj_base::sigPosType)gameMap[nowLevel].size() - 1) return;			//�Ѿ�����߽�
		mapObjList = GetMapObj(xc + 1, yc);
		leftDistance = [&x0, &y0, &x, &y]() { return x0 + defPosUnitPerCell - x; };
		walk = [pRole]() { pRole->MoveDown(); };
		walkHalf = [pRole, &leftDistance] { pRole->MoveDownLessThanOneStep(leftDistance()); };
		break;
	}
	case direction::Left:
	{
		//���ó���
		pRole->GetMutex().lock(); pRole->SetDirectLeft(); pRole->GetMutex().unlock();
		if (yc <= 0) return;			//�Ѿ�����߽�
		mapObjList = GetMapObj(xc, yc - 1);
		leftDistance = [&x0, &y0, &x, &y]() { return y - (y0 - defPosUnitPerCell); };
		walk = [pRole]() { pRole->MoveLeft(); };
		walkHalf = [pRole, &leftDistance] { pRole->MoveLeftLessThanOneStep(leftDistance()); };
		break;
	}
	case direction::Right:
	{
		//���ó���
		pRole->GetMutex().lock(); pRole->SetDirectRight(); pRole->GetMutex().unlock();
		if (yc >= (obj_base::sigPosType)gameMap[nowLevel][0].size() - 1) return;			//�Ѿ�����߽�
		mapObjList = GetMapObj(xc, yc + 1);
		leftDistance = [&x0, &y0, &x, &y]() { return y0 + defPosUnitPerCell - y; };
		walk = [pRole]() { pRole->MoveRight(); };
		walkHalf = [pRole, &leftDistance] { pRole->MoveRightLessThanOneStep(leftDistance()); };
		break;
	}
	}

	//����Ƿ����ϰ�
	for (auto pObj : mapObjList)
	{
		switch (pObj->GetObjType())
		{
		case obj_base::objType::hardObstacle:
		case obj_base::objType::softObstacle:
			return;							//�����ϰ�������
		case obj_base::objType::tnt:		//����ը��
		{
			TNT* pTnt = dynamic_cast<TNT*>(pObj);
			if (!pTnt->IsMoving())
			{
				if (pRole->CanPushTnt())
				{
					//�Ƿ����ƶ�ը��
					if (!MoveTnt(pTnt, direct)) return;	//�����ƶ�ը������������
				}
				else return; 
			}
			break;
		}
		}
	}

	//����һ��
	while (leftDistance() > 0)
	{
		pRole->GetMutex().lock();
		if (leftDistance() >= pRole->GetMoveSpeed()) walk();
		else walkHalf();
		pRole->GetMutex().unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(stepInterval));
		x = pRole->GetPos().x; y = pRole->GetPos().y; 
	}

}

bool Game::MoveTnt(TNT* pTnt, direction direct)
{
	auto [x0, y0] = pTnt->GetPos();				//��ȡը�����꣨C++17��
	int xc = PosToCell(x0), yc = PosToCell(y0); //ת���ɵ�ͼ���ڸ���
	std::list<obj_base*> mapObjList; 
	std::function<bool(void)> canMove = [&mapObjList]()	//����Ƿ�����
	{
		//����Ƿ����ϰ�
		for (auto pObj : mapObjList)
		{
			switch (pObj->GetObjType())
			{
			case obj_base::objType::hardObstacle:
			case obj_base::objType::softObstacle:
				return false;					//�����ϰ�������
			case obj_base::objType::tnt:		//����ը��
			{
				//�����ס��ը��û�����ƶ��������ƶ���ը��
				TNT* pTntNext = dynamic_cast<TNT*>(pObj);
				if (!pTntNext->IsMoving()) return false;
				break;
			}
			}
		}
		return true; 
	}; 
	switch (direct)
	{
	case direction::Up: 
		if (xc <= 0) return false;				//�ڵ�ͼ���ˣ��޷��ƶ�
		mapObjList = GetMapObj(xc - 1, yc);	
		if (canMove()) { pTnt->GetMutex().lock(); pTnt->SetMoveUp(); pTnt->GetMutex().unlock(); }
		else return false; 
		break; 
	case direction::Down: 
		//�ڵ�ͼ�׶�
		if (xc >= (obj_base::sigPosType)gameMap[nowLevel].size() - 1) return false; 
		mapObjList = GetMapObj(xc + 1, yc); 
		if (canMove()) { pTnt->GetMutex().lock(); pTnt->SetMoveDown(); pTnt->GetMutex().unlock(); }
		else return false; 
		break; 
	case direction::Left:
		if (yc <= 0) return false;				//�ڵ�ͼ��ˣ��޷��ƶ�
		mapObjList = GetMapObj(xc, yc - 1);
		if (canMove()) { pTnt->GetMutex().lock(); pTnt->SetMoveLeft(); pTnt->GetMutex().unlock(); }
		else return false; 
		break;
	case direction::Right:
		//�ڵ�ͼ�Ҷ�
		if (yc >= (obj_base::sigPosType)gameMap[nowLevel][0].size() - 1) return false;
		mapObjList = GetMapObj(xc, yc + 1);
		if (canMove()) { pTnt->GetMutex().lock(); pTnt->SetMoveRight(); pTnt->GetMutex().unlock(); }
		else return false; 
		break;
	default: 
		return false; 
	}
	return true; 
}

void Game::CheckRole()
{
	Role* pRole; 
	for (int i = 1; i <= 4; ++i)
	{
		//�����е��Ѷȵĵ��Բ���ʰȡ����
		if ((difficulty == Difficulty::easy || difficulty == Difficulty::medium)
			&& !(i == id1 || numOfPlayer == 2 && i == id2))	continue; 
		pRole = roles[i]; 
		if (!pRole->IsLiving()) continue;									//��ɫ�Ѿ�����
		auto [x, y] = pRole->GetPos();										//��ȡ��ɫλ�ã�C++17��
		int xc = PosToCell(x), yc = PosToCell(y); 
		std::list<obj_base*> mapObjList = GetMapObj(xc, yc);				//��ȡ��ͼ�ϵ���Ʒ
		for (auto pObj : mapObjList)										//������Ʒ
		{
			if (pObj->GetObjType() == obj_base::objType::prop)				//����е���
			{
				Prop* pProp = dynamic_cast<Prop*>(pObj); 
				if (pProp->IsUnpicked())									//���߻�δ���𣬼������
				{
					otherGameObjsMutex.lock();								//�ӵ�ͼ�м���õ��ߣ��ӵ�ͼ�б���ɾ����
					auto itr = std::find(otherGameObjs.begin(), otherGameObjs.end(), pObj); 
					if (itr != otherGameObjs.end()) otherGameObjs.erase(itr); 
					otherGameObjsMutex.unlock();
					Prop::propType thisPropType = pProp->GetPropType();		//��ȡ��������
					//�����buff�����
					if (thisPropType > Prop::propType::buffbegin && thisPropType < Prop::propType::buffend)
					{
						pRole->GetMutex().lock();
						switch (thisPropType)
						{
						case Prop::propType::glove:							//��������
							pRole->WearGlove(); 
							PickPropSound(); 
							break; 
						case Prop::propType::shield:						//�������
							pRole->WearShield(); 
							PickPropSound();
							break; 
						case Prop::propType::addtnt:						//��TNT����
							pRole->AddTntNum(); 
							PickPropSound();
							break; 
						case Prop::propType::addlife:						//������ֵ
							pRole->AddLife(); 
							PickPropSound();
							break; 
						case Prop::propType::shoe:							//���ƶ��ٶ�
							pRole->AddMoveSpeed(); 
							PickPropSound();
							break; 
						case Prop::propType::jinKeLa:						//����ը����ը��Χ
							pRole->AddDistance(); 
							PickPropSound();
							break; 
						}
						pRole->AddNowScore(scoreOfPickProp); 
						pRole->GetMutex().unlock(); 
						//buff�������ʧ
						deletedObjsMutex.lock(); deletedObjs.push_back(pProp); deletedObjsMutex.unlock();
					}

					//���������ը������ߣ��л���������
					else if (thisPropType > Prop::propType::specialbombbegin && thisPropType < Prop::propType::specialbombend)
					{
						pRole->GetMutex().lock();
						SpecialBomb* pPrevWeapon = pRole->GetWeapon(); 
						pRole->SetWeapon(dynamic_cast<SpecialBomb*>(pProp)); 
						pRole->GetMutex().unlock();
						dynamic_cast<SpecialBomb*>(pProp)->SetPicked(pRole->GetID());	//����Ϊ����״̬
						PickPropSound();
						if (pPrevWeapon) { deletedObjsMutex.lock(); deletedObjs.push_back(pPrevWeapon); deletedObjsMutex.unlock(); }
					}
					break;										//�����˵��ߣ��Ͳ���Ҫ����Ѱ����
				}
			}
		}
	}
}

void Game::CheckBomb(int dataScanInterval)
{
	otherGameObjsMutex.lock(); 
	std::list<obj_base*> bombList = otherGameObjs;					//����һ�ݣ����������������̸߳��Ķ����µĴ����Լ���������ɵ���������
	otherGameObjsMutex.unlock(); 
	for (auto pObj : bombList)
	{
		auto [x, y] = pObj->GetPos();								//��ȡλ�ã�C++17��
		auto xc = PosToCell(x), yc = PosToCell(y); 
		if (pObj->GetObjType() == obj_base::objType::tnt)			//�������ͨը��
		{
			TNT* pTnt = dynamic_cast<TNT*>(pObj); 
			if (pTnt->IsMoving())									//���TNT�����ƶ�
			{
				bool willBomb = false; 
				//�����ײ
				std::list<obj_base*> mapObjList; 
				switch (pTnt->GetDirect())
				{
				case obj_base::direction::Up: 
					if (xc <= 0) willBomb = true;					//��������˵�ͼ�߽磬ը
					else mapObjList = GetMapObj(xc - 1, yc); 
					break; 
				case obj_base::direction::Down:
					if (xc >= (int)gameMap[nowLevel].size() - 1) willBomb = true;	//��������˵�ͼ�߽磬ը
					else mapObjList = GetMapObj(xc + 1, yc);
					break;
				case obj_base::direction::Left:
					if (yc <= 0) willBomb = true;									//��������˵�ͼ�߽磬ը
					else mapObjList = GetMapObj(xc, yc - 1);
					break;
				case obj_base::direction::Right:
					if (yc >= (int)gameMap[nowLevel][0].size() - 1) willBomb = true;	//��������˵�ͼ�߽磬ը
					else mapObjList = GetMapObj(xc, yc + 1);
					break;
				}
				for (auto pMapObj : mapObjList)
				{
					switch (pMapObj->GetObjType())
					{
					case obj_base::objType::softObstacle: 
					case obj_base::objType::hardObstacle:
					case obj_base::objType::tnt: willBomb = true; break; 
					}
					if (willBomb) break; 
				}
				if (willBomb) BombTnt(pTnt);
				else pTnt->Move(); 
			}
			else
			{
				if (pTnt->AboutToDisappear()) BombTnt(pTnt); 
				else
				{
					pTnt->GetMutex().lock(); pTnt->SubTimeLeft(dataScanInterval); pTnt->GetMutex().unlock();
				}
			}
		}
		else if (pObj->GetObjType() == obj_base::objType::bombArea)	//��ը����
		{
			BombArea* pBombArea = dynamic_cast<BombArea*>(pObj); 
			if (pBombArea->AboutToDisappear())						//��ʧ��
			{
				otherGameObjsMutex.lock();
				auto itr = std::find(otherGameObjs.begin(), otherGameObjs.end(), pObj);
				if (itr != otherGameObjs.end()) otherGameObjs.erase(itr);
				otherGameObjsMutex.unlock();
				deletedObjsMutex.lock(); deletedObjs.push_back(pObj); deletedObjsMutex.unlock();
			}
			else { BombMapCell(pBombArea); pBombArea->SubTimeLeft(dataScanInterval); }
		}
		//����ը�����Ѿ�������
		else if (pObj->GetObjType() == obj_base::objType::prop && dynamic_cast<Prop*>(pObj)->IsLaid()
			&& dynamic_cast<Prop*>(pObj)->GetPropType() > Prop::propType::specialbombbegin
			&& dynamic_cast<Prop*>(pObj)->GetPropType() < Prop::propType::specialbombend)
		{
			SpecialBomb* pSpecialBomb = dynamic_cast<SpecialBomb*>(pObj); 
			switch (pSpecialBomb->GetPropType())
			{
			case Prop::propType::lachrymator: 
			{
				if (pSpecialBomb->AboutToDisappear())		//Ҫ��ʧ�ˣ�ɾ��
				{
					otherGameObjsMutex.lock(); 
					auto itr = std::find(otherGameObjs.begin(), otherGameObjs.end(), pSpecialBomb); 
					if (itr != otherGameObjs.end()) otherGameObjs.erase(itr); 
					otherGameObjsMutex.unlock(); 
					deletedObjsMutex.lock(); deletedObjs.push_back(pSpecialBomb); deletedObjsMutex.unlock(); 
				}
				else
				{
					if (dynamic_cast<Lachrymator*>(pSpecialBomb)->GetTimeLeft() < mineInitialTime / 2)	//��Ϊ���ɼ�
						dynamic_cast<Lachrymator*>(pSpecialBomb)->SetInvisible(); 
					std::list<obj_base*> mapObjList = GetMapObj(xc, yc);
					bool active = false;		//�Ƿ񴥷�����Ҫ���������˲��ܴ���
					for (auto pMapObj : mapObjList)
					{
						if (pMapObj->GetObjType() == obj_base::objType::role && dynamic_cast<Role*>(pMapObj)->GetID() != pSpecialBomb->GetOwnerID())
						{
							active = true; break;
						}
					}
					if (active)
					{
						for (auto pMapObj : mapObjList)
							if (pMapObj->GetObjType() == obj_base::objType::role)
							{
								Role* pRole = dynamic_cast<Role*>(pMapObj);
								pRole->GetMutex().lock();
								pRole->SubMoveSpeed();			//�ȵ�������˼���
								pRole->GetMutex().unlock();
							}
						//ɾ��������˹
						otherGameObjsMutex.lock(); 
						auto itr = std::find(otherGameObjs.begin(), otherGameObjs.end(), pSpecialBomb); 
						if (itr != otherGameObjs.end()) otherGameObjs.erase(itr); 
						otherGameObjsMutex.unlock();
						deletedObjsMutex.lock(); deletedObjs.push_back(pSpecialBomb); deletedObjsMutex.unlock();
					}
					else
					{
						Lachrymator* pLachrymator = dynamic_cast<Lachrymator*>(pSpecialBomb); pLachrymator->GetMutex().lock();
						pLachrymator->SubTimeLeft(dataScanInterval); pLachrymator->GetMutex().unlock();
					}
				}
				break; 
			}
			case Prop::propType::mine: 
			{
				if (pSpecialBomb->AboutToDisappear())		//Ҫ��ʧ�ˣ�ɾ��
				{
					otherGameObjsMutex.lock();
					auto itr = std::find(otherGameObjs.begin(), otherGameObjs.end(), pSpecialBomb);
					if (itr != otherGameObjs.end()) otherGameObjs.erase(itr);
					otherGameObjsMutex.unlock();
					deletedObjsMutex.lock(); deletedObjs.push_back(pSpecialBomb); deletedObjsMutex.unlock();
				}
				else
				{
					if (dynamic_cast<Mine*>(pSpecialBomb)->GetTimeLeft() < mineInitialTime / 2)	//��Ϊ���ɼ�
						dynamic_cast<Mine*>(pSpecialBomb)->SetInvisible();
					std::list<obj_base*> mapObjList = GetMapObj(xc, yc);
					bool active = false;		//�Ƿ񴥷�����Ҫ���������˲��ܴ���
					for (auto pMapObj : mapObjList)
					{
						if (pMapObj->GetObjType() == obj_base::objType::role && dynamic_cast<Role*>(pMapObj)->GetID() != pSpecialBomb->GetOwnerID())
						{
							active = true; 
							break; 
						}
					}
					if (active)
					{
						for (auto pMapObj : mapObjList)
							if (pMapObj->GetObjType() == obj_base::objType::role)
							{
								Role* pRole = dynamic_cast<Role*>(pMapObj);
								pRole->GetMutex().lock();
								bool hit = pRole->BeAttacked();								//�Ƿ����
								pRole->GetMutex().unlock();
								if (hit)
								{
									std::thread setRoleMiss(&Game::RoleMiss, this, pRole);	//���뱣��״̬
									setRoleMiss.detach();
									if (pSpecialBomb->GetOwnerID() != pRole->GetID())		//���ը���Ĳ����Լ����ӷ�
									{
										roles[pSpecialBomb->GetOwnerID()]->GetMutex().lock();
										roles[pSpecialBomb->GetOwnerID()]->AddNowScore(scoreOfHitOthers);
										roles[pSpecialBomb->GetOwnerID()]->GetMutex().unlock();
									}
								}
							}
						//ɾ������
						otherGameObjsMutex.lock();
						auto itr = std::find(otherGameObjs.begin(), otherGameObjs.end(), pSpecialBomb);
						if (itr != otherGameObjs.end()) otherGameObjs.erase(itr);
						otherGameObjsMutex.unlock();
						deletedObjsMutex.lock(); deletedObjs.push_back(pSpecialBomb); deletedObjsMutex.unlock();
					}
					else
					{
						Mine* pMine = dynamic_cast<Mine*>(pSpecialBomb); pMine->GetMutex().lock(); 
						pMine->SubTimeLeft(dataScanInterval); pMine->GetMutex().unlock();
					}
				}
				break;
			}
			case Prop::propType::fire:						//����ǹ��ֱ�ӱ�ը
			{
				BombFire(dynamic_cast<Fire*>(pSpecialBomb)); break;
			}
			case Prop::propType::grenade:					//����
			{
				if (pSpecialBomb->AboutToDisappear()) BombGrenade(dynamic_cast<Grenade*>(pSpecialBomb));
				else dynamic_cast<Grenade*>(pSpecialBomb)->Move(); 
				break; 
			}
			case Prop::propType::missile:					//����
			{
				//��鵽��߽�
				int rows = (int)gameMap[nowLevel].size(), cols = (int)gameMap[nowLevel][0].size(); 
				bool arriveBorder = false; 
				Missile* pMissile = dynamic_cast<Missile*>(pSpecialBomb); 
				int newXc = xc, newYc = yc; 
				switch (pMissile->GetDirection())
				{
				case obj_base::direction::Up:
					if (xc <= 0) arriveBorder = true; 
					newXc = xc - 1; 
					break; 
				case obj_base::direction::Down:
					if (xc >= rows - 1) arriveBorder = true; 
					newXc = xc + 1; 
					break;
				case obj_base::direction::Left:
					if (yc <= 0) arriveBorder = true; 
					newYc = yc - 1; 
					break;
				case obj_base::direction::Right:
					if (yc >= cols - 1) arriveBorder = true; 
					newYc = yc + 1; 
					break;
				}
				if (arriveBorder)
				{
					//ը����һ��
					BombArea* pBombArea = new BombArea(CellToPos(xc), CellToPos(yc), pMissile->GetOwnerID(), Prop::propType::missile, 500);
					BombMapCell(pBombArea); delete pBombArea;
					BombMissile(pMissile);
				}
				else
				{
					bool willBomb = false;
					std::list<obj_base*> mapObjList = GetMapObj(newXc, newYc);
					for (auto pMapObj : mapObjList)
					{
						switch (pMapObj->GetObjType())
						{
						case obj_base::objType::tnt:
						case obj_base::objType::role:
						case obj_base::objType::softObstacle:
						case obj_base::objType::hardObstacle:
						{
							willBomb = true;
							break;
						}
						}
						if (willBomb) break; 
					}
					if (willBomb)
					{
						//ը����һ��
						BombArea* pBombArea = new BombArea(CellToPos(newXc), CellToPos(newYc), pMissile->GetOwnerID(), Prop::propType::missile, 500);
						BombMapCell(pBombArea); delete pBombArea;
						BombMissile(pMissile);
					}
					else pMissile->Move();
				}
				break; 
			}
			}
		}
	}
}

int Game::CheckGameEnd() const
{

	bool playerWin = true; 
	//������Ӯ��
	for (int i = 1; i <= 4; ++i)
	{
		if (i == id1 || numOfPlayer == 2 && i == id2) continue; 
		if (roles[i]->IsLiving()) { playerWin = false; break; }
	}

	if (playerWin) return 1; 

	//�������û��Ӯ
	if (roles[id1]->IsLiving() || numOfPlayer == 2 && roles[id2]->IsLiving()) return 0; 
	return 2; 
}

void Game::BombTnt(TNT* pTnt)
{
	std::vector<int> xcMove{ 0, 0, -1, 1 }; 
	std::vector<int> ycMove{ -1, 1, 0, 0 }; 
	int rols = (int)gameMap[nowLevel].size(), cols = (int)gameMap[nowLevel][0].size(); 
	std::function<bool(int, int)> InRange = [rols, cols](int xc, int yc)
	{
		return (xc >= 0) && (yc >= 0) && (xc < rols) && (yc < cols); 
	}; 

	int distance = pTnt->GetDistance(); 
	auto [x, y] = pTnt->GetPos(); 
	auto xc = PosToCell(x), yc = PosToCell(y); 
	std::vector<BombArea*> pBombAreas; 
	pBombAreas.reserve((distance - 1) * 4 + 1); 
	pBombAreas.push_back(new BombArea(CellToPos(xc), CellToPos(yc), pTnt->GetOwnerID(), Prop::propType::null, tntBombAreaTime));	//�������ĵı�ը����
	
	//�����ܲ���
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 1; j < distance; ++j)
		{
			int newXc = xc + j * xcMove[i], newYc = yc + j * ycMove[i]; 
			if (!InRange(newXc, newYc)) break; 
			bool beBlocked = false; 
			std::list<obj_base*> mapObjList = GetMapObj(newXc, newYc); 
			for (auto pObj : mapObjList)
			{
				switch (pObj->GetObjType())
				{
				case obj_base::objType::softObstacle:				//���ϰ���ը����������
					pBombAreas.push_back(new BombArea(CellToPos(newXc), CellToPos(newYc), pTnt->GetOwnerID(), Prop::propType::null, tntBombAreaTime)); 
				case obj_base::objType::hardObstacle:				//Ӳ�ϰ�������
					beBlocked = true; break; 
				}
				if (beBlocked) break; 
			}
			if (beBlocked) break; 
			pBombAreas.push_back(new BombArea(CellToPos(newXc), CellToPos(newYc), pTnt->GetOwnerID(), Prop::propType::null, tntBombAreaTime)); 
		}
	}

	//�ѱ�ը���򶼷ŵ��б���
	otherGameObjsMutex.lock(); 
	for (auto pBombArea : pBombAreas)
		otherGameObjs.push_back(pBombArea); 
	otherGameObjsMutex.unlock();

	//�����˼ӵ�
	roles[pTnt->GetOwnerID()]->GetMutex().lock(); 
	roles[pTnt->GetOwnerID()]->TNTBomb(); 
	roles[pTnt->GetOwnerID()]->GetMutex().unlock();

	otherGameObjsMutex.lock();		//�����б����б����Ƴ�TNT
	auto itr = std::find(otherGameObjs.begin(), otherGameObjs.end(), pTnt);
	if (itr != otherGameObjs.end()) otherGameObjs.erase(itr);
	otherGameObjsMutex.unlock();	//�����б�

	//��TNT�������վ
	deletedObjsMutex.lock(); 
	deletedObjs.push_back(pTnt); 
	deletedObjsMutex.unlock(); 

	BombSound(); 
}

void Game::BombFire(Fire* pFire)
{
	auto [x, y] = pFire->GetPos(); 
	auto xc = PosToCell(x), yc = PosToCell(y); 
	int rols = (int)gameMap[nowLevel].size(), cols = (int)gameMap[nowLevel][0].size();
	std::function<bool(int, int)> InRange = [rols, cols](int xc, int yc)
	{
		return (xc >= 0) && (yc >= 0) && (xc < rols) && (yc < cols);
	};
	std::vector<int> xMove, yMove; 
	switch (pFire->GetDirection())
	{
	case obj_base::direction::Up: 
		for (int i = 0; i < fireMaxDistance; ++i)
		{
			xMove.push_back(-i - 1); 
			yMove.push_back(0); 
		}
		break; 
	case obj_base::direction::Down:
		for (int i = 0; i < fireMaxDistance; ++i)
		{
			xMove.push_back(i + 1);
			yMove.push_back(0);
		}
		break;
	case obj_base::direction::Left:
		for (int i = 0; i < fireMaxDistance; ++i)
		{
			xMove.push_back(0);
			yMove.push_back(-i - 1);
		}
		break;
	case obj_base::direction::Right:
		for (int i = 0; i < fireMaxDistance; ++i)
		{
			xMove.push_back(0);
			yMove.push_back(i + 1);
		}
		break;
	default: 
		for (int i = 0; i < fireMaxDistance; ++i)
		{
			xMove.push_back(0);
			yMove.push_back(0);
		}
		break;
	}
	std::vector<BombArea*> pBombAreas; 
	pBombAreas.reserve(fireMaxDistance);
	for (int i = 0; i < fireMaxDistance; ++i)
	{
		int nowXc = xc + xMove[i], nowYc = yc + yMove[i]; 
		if (!InRange(nowXc, nowYc)) break; 
		auto mapObjList = GetMapObj(nowXc, nowYc); 
		bool haveBarrier = false;
		for (auto pMapObj : mapObjList)
			if (pMapObj->GetObjType() == obj_base::objType::hardObstacle)	//��Ӳ�ϰ��赲
			{
				haveBarrier = true; 
				break; 
			}
		if (haveBarrier) break; 
		pBombAreas.push_back(new BombArea(CellToPos(nowXc), CellToPos(nowYc), pFire->GetOwnerID(), Prop::propType::fire, fireInitialTime)); 
	}
	//�ѱ�ը����ŵ��б���ȥ
	otherGameObjsMutex.lock();
	for (auto pBombArea : pBombAreas)
		otherGameObjs.push_back(pBombArea);
	otherGameObjsMutex.unlock(); 

	//���б����Ƴ�
	otherGameObjsMutex.lock();
	auto itr = std::find(otherGameObjs.begin(), otherGameObjs.end(), pFire);
	if (itr != otherGameObjs.end()) otherGameObjs.erase(itr);
	otherGameObjsMutex.unlock();

	//�ѻ���ǹ�������վ
	deletedObjsMutex.lock(); deletedObjs.push_back(pFire); deletedObjsMutex.unlock(); 
	BombSound();
}

void Game::BombGrenade(Grenade* pGrenade)
{
	int rols = (int)gameMap[nowLevel].size(), cols = (int)gameMap[nowLevel][0].size();
	std::function<bool(int, int)> InRange = [rols, cols](int xc, int yc)
	{
		return (xc >= 0) && (yc >= 0) && (xc < rols) && (yc < cols);
	};

	auto [x, y] = pGrenade->GetPos();
	auto xc = PosToCell(x), yc = PosToCell(y);
	std::vector<BombArea*> pBombAreas;
	pBombAreas.reserve(5);
	pBombAreas.push_back(new BombArea(x, y, pGrenade->GetOwnerID(), Prop::propType::grenade, grenadeInitialTime));	//�������ĵı�ը����
	
	std::function<void(int, int)> createGrenadeBombArea = [this, &pBombAreas, &InRange, pGrenade](int newXc, int newYc)
	{
		if (!InRange(newXc, newYc)) return; 
		bool beBlocked = false;
		std::list<obj_base*> mapObjList = GetMapObj(newXc, newYc);
		for (auto pObj : mapObjList)
		{
			if (pObj->GetObjType() == obj_base::objType::hardObstacle)	//Ӳ�ϰ�������ը
			{
				beBlocked = true; break; 
			}
		}
		if (!beBlocked) pBombAreas.push_back(new BombArea(CellToPos(newXc), CellToPos(newYc), pGrenade->GetOwnerID(), Prop::propType::grenade, grenadeInitialTime));
	}; 

	createGrenadeBombArea(xc - 1, yc); createGrenadeBombArea(xc + 1, yc); createGrenadeBombArea(xc, yc - 1); createGrenadeBombArea(xc, yc + 1);

	//�ѱ�ը���򶼷ŵ��б���
	otherGameObjsMutex.lock();
	for (auto pBombArea : pBombAreas)
		otherGameObjs.push_back(pBombArea);
	otherGameObjsMutex.unlock();


	otherGameObjsMutex.lock();		//�����б����б����Ƴ�����
	auto itr = std::find(otherGameObjs.begin(), otherGameObjs.end(), pGrenade);
	if (itr != otherGameObjs.end()) otherGameObjs.erase(itr);
	otherGameObjsMutex.unlock();	//�����б�
	
	//�����񵯷������վ
	deletedObjsMutex.lock();
	deletedObjs.push_back(pGrenade);
	deletedObjsMutex.unlock();
	BombSound();
}

void Game::BombMissile(Missile* pMissile)
{
	//���б���ɾ������
	otherGameObjsMutex.lock(); 
	auto itr = std::find(otherGameObjs.begin(), otherGameObjs.end(), pMissile); 
	if (itr != otherGameObjs.end()) otherGameObjs.erase(itr); 
	otherGameObjsMutex.unlock(); 

	//�ѵ����������վ
	deletedObjsMutex.lock(); deletedObjs.push_back(pMissile); deletedObjsMutex.unlock();
}

void Game::BombMapCell(BombArea* pBombArea)
{
	auto [x, y] = pBombArea->GetPos(); 
	auto xc = PosToCell(x), yc = PosToCell(y); 
	std::list<obj_base*> mapObjList = GetMapObj(xc, yc);
	for (auto pMapObj : mapObjList)
	{
		switch (pMapObj->GetObjType())
		{
		case obj_base::objType::role:									//��������ը��
		{
			Role* pRole = dynamic_cast<Role*>(pMapObj); 
			pRole->GetMutex().lock(); 
			bool hit = pRole->BeAttacked();								//�Ƿ����
			pRole->GetMutex().unlock();
			if (hit)
			{
				std::thread setRoleMiss(&Game::RoleMiss, this, pRole);	//���뱣��״̬
				setRoleMiss.detach(); 
				if (pBombArea->GetOwnerID() != pRole->GetID())			//���ը���Ĳ����Լ����ӷ�
				{
					roles[pBombArea->GetOwnerID()]->GetMutex().lock();
					roles[pBombArea->GetOwnerID()]->AddNowScore(scoreOfHitOthers);
					roles[pBombArea->GetOwnerID()]->GetMutex().unlock();
				}
			}
			break;
		}
		case obj_base::objType::softObstacle:							//��������ϰ���ը��
		{
			obstaclesMutex.lock(); 
			auto itr = std::find(obstacles.begin(), obstacles.end(), pMapObj); 
			if (itr != obstacles.end()) obstacles.erase(itr); 
			obstaclesMutex.unlock();
			//�������վ
			deletedObjsMutex.lock(); 
			deletedObjs.push_back(pMapObj); 
			deletedObjsMutex.unlock(); 
			//�ӷ�
			roles[pBombArea->GetOwnerID()]->GetMutex().lock();
			roles[pBombArea->GetOwnerID()]->AddNowScore(scoreOfDestroyObstacle);
			roles[pBombArea->GetOwnerID()]->GetMutex().unlock();
			//�����������
			CreateProp(xc, yc); 
			break; 
		}
		case obj_base::objType::tnt:									//�������ͨը��������
			BombTnt(dynamic_cast<TNT*>(pMapObj)); 
			break; 
		case obj_base::objType::prop: 
		{
			Prop* pProp = dynamic_cast<Prop*>(pMapObj); 
			switch (pProp->GetPropType())
			{
			case Prop::propType::mine: case Prop::propType::lachrymator: 
				if (pProp->IsLaid())									//������Ѿ������ŵĵ��׺ʹ�����˹��ը��
				{
					otherGameObjsMutex.lock(); 
					auto itr = find(otherGameObjs.begin(), otherGameObjs.end(), pProp); 
					if (itr != otherGameObjs.end()) otherGameObjs.erase(itr); 
					otherGameObjsMutex.unlock(); 
					//�������վ
					deletedObjsMutex.lock(); 
					deletedObjs.push_back(pProp); 
					deletedObjsMutex.unlock();
				}
				break; 
			}
			break; 
		}
		}
	}
}

void Game::RoleMiss(Role* pRole)
{
	pRole->GetMutex().lock(); pRole->SetMissing(true); pRole->GetMutex().unlock(); 
	std::this_thread::sleep_for(std::chrono::milliseconds(playerMissingTime)); 
	pRole->GetMutex().lock(); pRole->SetMissing(false); pRole->GetMutex().unlock();
}

void Game::CreateProp(int xc, int yc)
{
	auto x = CellToPos(xc), y = CellToPos(yc); 
	Prop::propType newPropType = propNums[randNum() % (propNums.size())]; 
	Prop* pNewProp = nullptr; 
	switch (newPropType)
	{
	case Prop::propType::glove: 
		pNewProp = new Glove(x, y); 
		break; 
	case Prop::propType::shield: 
		pNewProp = new Shield(x, y); 
		break; 
	case Prop::propType::addtnt: 
		pNewProp = new AddTNT(x, y); 
		break; 
	case Prop::propType::addlife: 
		pNewProp = new AddLife(x, y); 
		break; 
	case Prop::propType::shoe: 
		pNewProp = new Shoe(x, y); 
		break; 
	case Prop::propType::jinKeLa: 
		pNewProp = new JinKeLa(x, y); 
		break; 
	case Prop::propType::lachrymator: 
		pNewProp = new Lachrymator(x, y, mineInitialTime); 
		break; 
	case Prop::propType::mine: 
		pNewProp = new Mine(x, y, mineInitialTime); 
		break; 
	case Prop::propType::fire: 
		pNewProp = new Fire(x, y); 
		break; 
	case Prop::propType::ice: 
		pNewProp = new Ice(x, y); 
		break; 
	case Prop::propType::grenade: 
		pNewProp = new Grenade(x, y, grenadeMaxDistance * defPosUnitPerCell); 
		break; 
	case Prop::propType::missile: 
		pNewProp = new Missile(x, y);
		break; 
	}
	//���µ��߷ŵ��б���
	if (pNewProp)
	{
		otherGameObjsMutex.lock(); 
		otherGameObjs.push_back(pNewProp); 
		otherGameObjsMutex.unlock();
	}
}
