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

	Role(sigPosType x, sigPosType y, int moveSpeed, int id, int initialLife)
		: obj_base(x, y, false, moveSpeed),
		id(id), 
		distance(2), 
		tntNum(1), 
		nowTntNum(0), 
		nowScore(0),
		totalScore(0), 
		canPushTnt(false), 
		haveDefence(false), 
		isMissing(false), 
		weapon(nullptr), 
		isLiving(true), 
		life(initialLife), 
		initialLife(initialLife), 
		direct(direction::Down) {}

	virtual objType GetObjType() const override { return objType::role; }

	//�����ɫ���Ƿ������ֺܷ�������
	void Reset(bool resetTotalScore, bool resetLife); 

	//�Ƿ���
	bool IsLiving() const { return isLiving; }

	//�ܵ�����
	bool BeAttacked(); 

	//��ȡ����ֵ
	int GetLife() const { return life; }
	//������һ
	void AddLife() { ++life; }
	
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

	//����TNT
	bool LayTNT(); 
	//���õ�TNT��ը
	void TNTBomb(); 

	//���������õ�ը����
	void AddTntNum() { ++tntNum; }
	//��ȡ�����õ�ը����
	int GetTntNum() const { return tntNum; }
	//��ȡʣ��ը����
	int GetLeftTntNum() const { return tntNum - nowTntNum; }

	//��ȡը����ը��Χ
	int GetDistance() const { return distance; }
	//����ը����ը��Χ
	void AddDistance() { ++distance; }

	//��ȡ����
	int GetNowScore() const { return nowScore; }
	//���ӷ���
	void AddNowScore(int add) { nowScore += add; }
	//��ȡ�ܷ�
	int GetTotalScore() const { return totalScore; }
	//�ѵ�ǰ�����ӵ��ܷ��в���յ�ǰ����
	void MergeScore(); 

	//�����ƶ��ٶ�
	void AddMoveSpeed(); 
	//�����ƶ��ٶ�
	void SubMoveSpeed(); 
	
	//�ܷ��ƶ�ը��
	bool CanPushTnt() const { return canPushTnt; }
	//������ס����ƶ�Tnt������
	void GetGlove() { canPushTnt = true; }

	//�Ƿ���ж��Ʒ���
	bool HaveDefence() const { return haveDefence; }
	//��ö���
	bool GetDefence() { haveDefence = true; }

	//�Ƿ��ڱ�ը��ı���״̬
	bool IsMissing() const { return isMissing; }
	//���ñ���״̬
	void SetMissing(bool missing) { isMissing = missing; }

	//��ȡ����
	SpecialBomb* GetWeapon() const { return weapon; }
	//��������
	void SetWeapon(SpecialBomb* newWeapon) { weapon = newWeapon; }

	//��ȡ����
	direction GetDirect() const { return direct; }
	//�ı䷽��
	void SetDirectUp() { SetDirect(direction::Up); }
	void SetDirectDown() { SetDirect(direction::Down); }
	void SetDirectLeft() { SetDirect(direction::Left); }
	void SetDirectRight() { SetDirect(direction::Right); }
	
	~Role() {}

private: 
	
	int id;					//��ɫID: 1~4
	int distance;			//ը���ı�ը��Χ
	int tntNum;				//������ͬʱ���õ�ը����
	int nowTntNum;			//Ŀǰ�Ѿ����õ�ը����
	int nowScore;			//��ǰ����
	int totalScore;			//�ܷ���
	bool canPushTnt;		//�ܷ��ƶ�TNT
	bool haveDefence;		//�Ƿ�ӵ�ж��Ʒ���
	bool isMissing;			//�Ƿ����ڴ��ڸձ�ը�ı���״̬
	bool isLiving;			//�Ƿ����
	int life;				//������
	const int initialLife;	//��ʼ������
	SpecialBomb* weapon;	//���г��е�����
	direction direct;		//����

	void SetDirect(direction newDirect) { direct = newDirect; }

	//������һ
	void SubLife() { if (life > 0) --life; }
};


#endif  // #ifndef ROLE_H
