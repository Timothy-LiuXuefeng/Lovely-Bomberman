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
		: obj_base(x, y, isMoving, moveSpeed), ownerID(ownerID), distance(distance), timeLeft(timeLeft), direct(direction::Null) {}

	int GetOwnerID() const { return ownerID; }
	int GetDistance() const { return distance; }

	virtual objType GetObjType() const override { return objType::tnt; }
	
	//��ȡʣ�౬ըʱ��
	int GetTimeLeft() const { return timeLeft; }

	//���ٱ�ը�����ʱ��
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	//�Ƿ�Ҫ��ը
	bool AboutToDisappear() const { return timeLeft <= 0; }

	//���ƶ�
	void SetMoveUp() { SetMove(direction::Up); }
	void SetMoveDown() { SetMove(direction::Down); }
	void SetMoveLeft() { SetMove(direction::Left); }
	void SetMoveRight() { SetMove(direction::Right); }

	//�ƶ�
	void Move() { obj_base::Move(direct); }

	//��ȡ�ƶ�����
	direction GetDirect() const { return direct; }

	virtual ~TNT() {}

private: 

	int ownerID;		//���˵�ID
	int distance;		//��ը��Χ
	int timeLeft;		//ʣ�౬ըʱ��
	direction direct;	//�ƶ�����

	//���ƶ�
	void SetMove(direction newDirect) { isMoving = true; direct = newDirect; }

}; 


#endif  //#ifndef BOMB_H
