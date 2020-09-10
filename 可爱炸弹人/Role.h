#ifndef ROLE_H

#define ROLE_H

////////////////////////////////////////////
//
//  ��ɫ��
//

#include "Prop.h"
#include <list>

//��ɫ��
class Role : public obj_base
{
public: 

	Role(sigPosType x, sigPosType y, sigPosType moveSpeed, int id, int initialLife)
		: obj_base(x, y, false, moveSpeed),
		id(id), 
		distance(2), 
		tntNum(1), 
		nowTntNum(0), 
		nowScore(0),
		totalScore(0), 
		canPushTnt(false), 
		haveShield(false), 
		isMissing(false), 
		weapon(nullptr), 
		isLiving(true), 
		life(initialLife), 
		initialLife(initialLife), 
		direct(direction::Down), 
		orgPos(x, y), 
		orgMoveSpeed(moveSpeed) {}

	virtual objType GetObjType() const override { return objType::role; }

	//�����ɫ���Ƿ������ֺܷ�������
	void Reset(bool resetTotalScore, bool resetLife); 

	//�ѵ�ǰ�����ӵ��ܷ��в���յ�ǰ����
	void MergeScore();

	////////////////////////
	//
	//��ɫ��Ϊ
	//

	//�ܵ�����
	bool BeAttacked();

	//�ƶ�
	void MoveUp() { Move(direction::Up); }
	void MoveDown() { Move(direction::Down); }
	void MoveLeft() { Move(direction::Left); }
	void MoveRight() { Move(direction::Right); }
	//�ƶ�����һ��
	void MoveUpLessThanOneStep(sigPosType moveDistance) { MoveLessThanOneStep(direction::Up, moveDistance); }
	void MoveDownLessThanOneStep(sigPosType moveDistance) { MoveLessThanOneStep(direction::Down, moveDistance); }
	void MoveLeftLessThanOneStep(sigPosType moveDistance) { MoveLessThanOneStep(direction::Left, moveDistance); }
	void MoveRightLessThanOneStep(sigPosType moveDistance) { MoveLessThanOneStep(direction::Right, moveDistance); }

	//�ı䷽��
	void SetDirectUp() { SetDirect(direction::Up); }
	void SetDirectDown() { SetDirect(direction::Down); }
	void SetDirectLeft() { SetDirect(direction::Left); }
	void SetDirectRight() { SetDirect(direction::Right); }

	//����TNT
	bool LayTNT();
	//���õ�TNT��ը
	void TNTBomb();

	//���ӷ���
	void AddNowScore(int add) { nowScore += add; }

	////////////////////////
	//
	//���buff
	//

	//���������õ�ը����
	void AddTntNum() { ++tntNum; }

	//����ը����ը��Χ
	void AddDistance() { ++distance; }

	//������һ
	void AddLife() { ++life; }

	//�����ƶ��ٶ�
	void AddMoveSpeed();
	//�����ƶ��ٶ�
	void SubMoveSpeed();

	//������ס����ƶ�Tnt������
	void WearGlove() { canPushTnt = true; }

	//װ�϶���
	void WearShield() { haveShield = true; }

	//���ñ���״̬
	void SetMissing(bool missing) { isMissing = missing; }

	//��������
	void SetWeapon(SpecialBomb* newWeapon) { weapon = newWeapon; }

	/////////////////////////
	//
	//��ȡ��ɫ����
	//

	//��ȡID
	int GetID() const { return id; }

	//�Ƿ���
	bool IsLiving() const { return isLiving; }

	//��ȡ����ֵ
	int GetLife() const { return life; }
	
	//��ȡ�����õ�ը����
	int GetTntNum() const { return tntNum; }
	//��ȡʣ��ը����
	int GetLeftTntNum() const { return tntNum - nowTntNum; }

	//��ȡը����ը��Χ
	int GetDistance() const { return distance; }
	
	//��ȡ����
	int GetNowScore() const { return nowScore; }
	
	//��ȡ�ܷ�
	int GetTotalScore() const { return totalScore; }

	//�ܷ��ƶ�ը��
	bool CanPushTnt() const { return canPushTnt; }
	
	//�Ƿ���ж��Ʒ���
	bool HaveShield() const { return haveShield; }
	
	//�Ƿ��ڱ�ը��ı���״̬
	bool IsMissing() const { return isMissing; }
	
	//��ȡ����
	SpecialBomb* GetWeapon() const { return weapon; }
	
	//��ȡ����
	direction GetDirect() const { return direct; }
	
	virtual ~Role(); 

private: 
	
	int id;					//��ɫID: 1~4
	int distance;			//ը���ı�ը��Χ
	int tntNum;				//������ͬʱ���õ�ը����
	int nowTntNum;			//Ŀǰ�Ѿ����õ�ը����
	int nowScore;			//��ǰ����
	int totalScore;			//�ܷ���
	bool canPushTnt;		//�ܷ��ƶ�TNT
	bool haveShield;		//�Ƿ�ӵ�ж��Ʒ���
	bool isMissing;			//�Ƿ����ڴ��ڸձ�ը�ı���״̬
	bool isLiving;			//�Ƿ����
	int life;				//������
	const int initialLife;	//��ʼ������
	SpecialBomb* weapon;	//���г��е�����
	direction direct;		//����
	const posType orgPos;	//ԭ�ȵ�λ��
	const sigPosType orgMoveSpeed; //ԭ�ȵ��ƶ��ٶ�

	void SetDirect(direction newDirect) { direct = newDirect; }

	//������һ
	void SubLife() { if (life > 0) --life; }
};


#endif  // #ifndef ROLE_H
