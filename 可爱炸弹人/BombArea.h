#ifndef BOMBAREA_H

#define BOMBAREA_H

#include "TNT.h"
#include "Prop.h"

//��ը����
class BombArea : public obj_base
{
public: 
	BombArea(sigPosType x, sigPosType y, int ownerID, Prop::propType bomb, int timeLeft) 
		: obj_base(x, y, false, 0), ownerID(ownerID), bomb(bomb), timeLeft(timeLeft) {}

	virtual objType GetObjType() const override { return objType::bombArea; }
	int GetTimeLeft() const { return timeLeft; }
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }
	bool AboutToDisappear() const { return timeLeft <= 0; }
	int GetOwnerID() const { return ownerID; }
	Prop::propType GetBomb() const { return bomb; }

	virtual ~BombArea() {}
private: 
	
	int ownerID; 
	Prop::propType bomb;	//���������ը�������������
	int timeLeft;			//ʣ��ʱ��
};

#endif	// #ifndef BOMBAREA_H
