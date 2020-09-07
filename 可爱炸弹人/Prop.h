#ifndef PROP_H

#define PROP_H

//////////////////////////////////////
//
//  ����
//

#include "obj_base.h"

//���߻���
class Prop : public obj_base
{
public:

	enum class propType
	{
		null = 0,				//�����κε���

		//buf�����
		buffbegin = 1,			//buff����߿�ʼ
		glove = 2,				//����
		shield = 3,				//����
		addtnt = 4,				//�����ڵ���
		addlife = 5,			//��������
		shoe = 6,				//��Ь
		jinKeLa = 7,			//�����
		buffend = 8,			//buff����߽���

		//����ը�������
		specialbombbegin = 9,	//������ը����ʼ
		lachrymator = 10,		//������˹
		mine = 11,				//����
		fire = 12,				//����ǹ
		ice = 13,				//��ǹ
		grenade = 14, 			//����
		missile = 15, 			//����
		specialbombend = 16		//������ը������
	};

	Prop(sigPosType x, sigPosType y) : obj_base(x, y, false, 0), state(propState::unpicked) {}
	virtual objType GetObjType() const override { return objType::prop; }
	//��ȡ��������
	virtual propType GetPropType() const = 0; 
	virtual bool IsBuff() const = 0; 
	virtual bool IsSpecialBomb() const = 0; 

	//��ȡ����״̬
	bool IsUnpicked() const { return state == propState::unpicked; }
	bool IsPicked() const { return state == propState::picked; }
	bool IsLaid() const { return state == propState::laid; }

	virtual ~Prop() {}

protected: 

	enum class propState		//���ߵ�״̬
	{
		unpicked,		//δ������
		picked,			//�Ѿ�������
		laid			//�Ѿ�������
	};

	propState state;	//����������״̬
}; 

//buff�����
class Buff : public Prop
{
public: 

	Buff(sigPosType x, sigPosType y) : Prop(x, y) {}
	virtual bool IsBuff() const { return true; }
	virtual bool IsSpecialBomb() const { return false; }
	virtual ~Buff() {}
};

//��������
class Glove : public Buff	//������
{
public: 
	Glove(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::glove; }
	virtual ~Glove() {}
};

//������
class Shield final : public Buff
{
public: 
	Shield(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::shield; }
	virtual ~Shield() {}
};

//�����ڵ���
class AddTNT final : public Buff
{
public:
	AddTNT(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::addtnt; }
	virtual ~AddTNT() {}
};

//��������
class AddLife final : public Buff
{
public:
	AddLife(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::addlife; }
	virtual ~AddLife() {}
};

//��Ь
class Shoe final : public Buff
{
public:
	Shoe(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::shoe; }
	virtual ~Shoe() {}
}; 

//�����
class JinKeLa final : public Buff
{
public:
	JinKeLa(sigPosType x, sigPosType y) : Buff(x, y) {}
	virtual propType GetPropType() const override { return propType::jinKeLa; }
	virtual ~JinKeLa() {}
};

//����ը�������

class SpecialBomb : public Prop
{
public:

	SpecialBomb(sigPosType x, sigPosType y) : Prop(x, y), ownerID(0), direct(direction::Null) {}

	virtual bool IsBuff() const { return false; }
	virtual bool IsSpecialBomb() const { return true; }

	//��ȡ����ID
	int GetOwnerID() const { return ownerID; }
	void SetOwnerID(int newOwnerID) { ownerID = newOwnerID; }

	//�������
	void SetPicked(int newOwnerID) { state = propState::picked; ownerID = newOwnerID; }

	//����ը��
	void SetLaid(sigPosType x, sigPosType y, direction laidDirect, sigPosType newMoveSpeed);

	//����ͬ�������ը��
	void SetLaidUp(sigPosType x, sigPosType y, sigPosType moveSpeed) { SetLaid(x, y, direction::Up, moveSpeed); }
	void SetLaidDown(sigPosType x, sigPosType y, sigPosType moveSpeed) { SetLaid(x, y, direction::Down, moveSpeed); }
	void SetLaidLeft(sigPosType x, sigPosType y, sigPosType moveSpeed) { SetLaid(x, y, direction::Left, moveSpeed); }
	void SetLaidRight(sigPosType x, sigPosType y, sigPosType moveSpeed) { SetLaid(x, y, direction::Right, moveSpeed); }

	//��ȡ����
	direction GetDirection() const { return direct; }

	//����Ƿ�Ҫ��ʧ��
	virtual bool AboutToDisappear() const = 0; 

	virtual ~SpecialBomb() {}

protected:

	int ownerID;		//����ID
	direction direct;	//���䳯��
}; 


//������˹
class Lachrymator final : public SpecialBomb
{
public:
	Lachrymator(sigPosType x, sigPosType y, int timeLeft) : SpecialBomb(x, y), timeLeft(timeLeft), visible(true) {}
	virtual propType GetPropType() const override { return propType::lachrymator; }

	int GetTimeLeft() const { return timeLeft; }
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	bool Visible() const { return visible; }
	//��Ϊ���ɼ�
	void SetInvisible() { visible = false; }

	//��������˹�Ƿ�Ҫ��ʧ��
	virtual bool AboutToDisappear() const override { return timeLeft <= 0; }

	virtual ~Lachrymator() {}

private:

	int timeLeft; 		//���������˹��ʧ��ʱ��
	bool visible;		//�Ƿ�ɼ�
};

//����
class Mine final : public SpecialBomb
{
public: 
	Mine(sigPosType x, sigPosType y, int timeLeft) : SpecialBomb(x, y), timeLeft(timeLeft), visible(true) {}
	virtual propType GetPropType() const override { return propType::mine; }

	int GetTimeLeft() const { return timeLeft; }
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	bool Visible() const { return visible; }
	//��Ϊ���ɼ�
	void SetInvisible() { visible = false; }

	//�������Ƿ�Ҫ��ʧ��
	virtual bool AboutToDisappear() const override { return timeLeft <= 0; }

	virtual ~Mine() {}

private: 

	int timeLeft; 		//���������ʧ��ʱ��
	bool visible;		//�Ƿ�ɼ�
}; 

//����ǹ
class Fire final : public SpecialBomb
{
public:
	Fire(sigPosType x, sigPosType y) : SpecialBomb(x, y) {}
	virtual propType GetPropType() const override { return propType::fire; }

	//�������Ƿ�Ҫ��ʧ��
	virtual bool AboutToDisappear() const override { return false; }

	virtual ~Fire() {}

};

//����ǹ
class Ice final : public SpecialBomb
{
public:
	Ice(sigPosType x, sigPosType y) : SpecialBomb(x, y) {}
	virtual propType GetPropType() const override { return propType::ice; }

	//�����Ƿ�Ҫ��ʧ��
	virtual bool AboutToDisappear() const override { return false; }

	virtual ~Ice() {}

};

//����
class Grenade final : public SpecialBomb
{
public: 
	Grenade(sigPosType x, sigPosType y, sigPosType maxDistance) : SpecialBomb(x, y), leftDistance(maxDistance) {}
	virtual propType GetPropType() const override { return propType::grenade; }
	
	sigPosType GetLeftDistance() const { return leftDistance; }

	//��������Ƿ�Ҫ��ը��
	virtual bool AboutToDisappear() const override { return leftDistance <= 0; }

	//�����ƶ�
	void Move(); 

	virtual ~Grenade() {}

private: 

	sigPosType leftDistance;		//ʣ������н��ľ���

}; 

//����
class Missile final : public SpecialBomb
{
public: 
	Missile(sigPosType x, sigPosType y) : SpecialBomb(x, y) {}
	virtual propType GetPropType() const override { return propType::missile; }
	//�����ƶ�
	void Move() { obj_base::Move(direct); }

	virtual bool AboutToDisappear() const override { return false; }

	virtual ~Missile() {}
};

#endif // #ifndef PROP_H
