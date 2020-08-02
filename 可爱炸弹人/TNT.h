#ifndef TNT_H

#define TNT_H

#include "obj_base.h"

/////////////////////////////////////
//
//  ����ը��
//

//��ͨը����
class TNT : public obj_base
{
public: 
	TNT(sigPosType x, sigPosType y, bool isMoving, sigPosType moveSpeed, int ownerID, int distance, int timeLeft) 
		: obj_base(x, y, isMoving, moveSpeed), ownerID(ownerID), distance(distance), timeLeft(timeLeft) {}

	int GetOwnerID() const { return ownerID; }
	int GetDistance() const { return distance; }

	virtual objType GetObjType() const override { return objType::tnt; }

	//��ȡʣ�౬ըʱ��
	int GetTimeLeft() const { return timeLeft; }

	//���ٱ�ը�����ʱ��
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	//�Ƿ�Ҫ��ը
	bool AboutToDisappear() const { return timeLeft <= 0; }

	virtual ~TNT() {}

private: 

	int ownerID;	//���˵�ID
	int distance;	//��ը��Χ
	int timeLeft;	//ʣ�౬ըʱ��
}; 


#endif  //#ifndef BOMB_H
