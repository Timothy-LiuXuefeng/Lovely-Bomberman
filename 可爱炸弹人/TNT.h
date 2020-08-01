#ifndef TNT_H

#define TNT_H

#include "basic.h"

/////////////////////////////////////
//
//  ����ը��
//

//��ͨը����
class TNT : public obj_base
{
public: 
	TNT(sigPosType x, sigPosType y, bool isMoving, sigPosType moveSpeed, int ownerID, int distance, int timeLeft) 
		: obj_base(x, y, isMoving, moveSpeed), ownerID(ownerID), distance(distance), timeLeft(timeLeft), state(tntState::laid) {}

	int GetOwnerID() const { return ownerID; }
	int GetDistance() const { return distance; }

	virtual objType GetObjType() const override { return objType::tnt; }

	//��ȡʣ�౬ը�����ʱ��
	int GetTimeLeft() const { return timeLeft; }

	//���ٱ�ը�����ʱ��
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	//�Ƿ�Ҫ��ը����ʧ
	bool AboutToDisappear() const { return timeLeft <= 0; }

	//��ը
	void Bomb(int newTimeLeft) { timeLeft = newTimeLeft; state = tntState::bomb; }

	//�Ƿ��ڷ���״̬
	bool IsLaid() const { return state == tntState::laid; }

	//�Ƿ��ڱ�ը״̬
	bool IsBomb() const { return state == tntState::bomb; }

	virtual ~TNT() {}

private: 

	enum class tntState
	{
		laid,	//����״̬
		bomb	//��ը״̬
	};

	int ownerID;	//���˵�ID
	int distance;	//��ը��Χ
	int timeLeft;	//ʣ�౬ըʱ��
	tntState state; //����״̬
}; 


#endif  //#ifndef BOMB_H
