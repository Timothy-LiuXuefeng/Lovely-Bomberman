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

	Role(sigPosType x, sigPosType y, int moveSpeed, int id)
		: obj_base(x, y, false, moveSpeed),
		id(id), 
		distance(2), 
		tntNum(1), 
		nowTntNum(0), 
		score(0),
		canPushTnt(false), 
		haveDefence(false), 
		isMissing(false), 
		weapon(NULL) {}

	virtual objType GetObjType() const override { return objType::role; }

	//�����ɫ���Ƿ����������
	void Reset(bool resetScore); 
	
	//�ƶ�
	void MoveUp() { Move(direction::Up); }
	void MoveDown() { Move(direction::Down); }
	void MoveLeft() { Move(direction::Left); }
	void MoveRight() { Move(direction::Right); }

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
	int GetScore() const { return score; }
	//���ӷ���
	void AddScore(int add) { score += add; }

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
	
	~Role() {}

private: 
	
	int id;					//��ɫID: 1~4
	int distance;			//ը���ı�ը��Χ
	int tntNum;				//������ͬʱ���õ�ը����
	int nowTntNum;			//Ŀǰ�Ѿ����õ�ը����
	int score;				//����
	bool canPushTnt;		//�ܷ��ƶ�TNT
	bool haveDefence;		//�Ƿ�ӵ�ж��Ʒ���
	bool isMissing;			//�Ƿ����ڴ��ڸձ�ը�ı���״̬
	SpecialBomb* weapon;	//���г��е�����
};


#endif  // #ifndef ROLE_H
