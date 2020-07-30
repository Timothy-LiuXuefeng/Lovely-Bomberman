#ifndef PROP_H

#define PROP_H

//////////////////////////////////////
//
//  ����
//

#include "basic.h"

//���߻���
class Prop : public obj_base
{
public:

	enum propType
	{
		//buf�����
		glove,		//����
		shield,		//����
		addtnt,		//�����ڵ���
		addlife,	//��������
		shoe,		//��Ь
		jinKeLa,	//�����
		
		//����ը�������
		lachrymator,//������˹
		mine,		//����
		fire,		//����ǹ
		ice,		//��ǹ
		grenade, 	//����
		missile		//����
	};

	Prop(sigPosType x, sigPosType y) : obj_base(x, y, false, 0), state(propState::unpicked) {}
	virtual objType GetObjType() const override { return objType::prop; }
	//��ȡ��������
	virtual propType GetPropType() const = 0; 

	//��ȡ����״̬
	bool isUnpicked() const { return state == propState::unpicked; }
	bool ispicked() const { return state == propState::picked; }
	bool isLaid() const { return state == propState::laid; }

	//�������
	virtual void SetPicked() { state = propState::picked; }
	
	virtual ~Prop() {}

protected: 

	enum propState		//���ߵ�״̬
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

	//��ȡ����ID
	int GetOwnerID() const { return ownerID; }
	void SetOwnerID(int newOwnerID) { ownerID = newOwnerID; }

	//����ͬ�������ը��
	void SetLaidUp(sigPosType x, sigPosType y, int moveSpeed) { SetLaid(x, y, direction::Up, moveSpeed); }
	void SetLaidDown(sigPosType x, sigPosType y, int moveSpeed) { SetLaid(x, y, direction::Down, moveSpeed); }
	void SetLaidLeft(sigPosType x, sigPosType y, int moveSpeed) { SetLaid(x, y, direction::Left, moveSpeed); }
	void SetLaidRight(sigPosType x, sigPosType y, int moveSpeed) { SetLaid(x, y, direction::Right, moveSpeed); }

	//�����ƶ�
	void Move() { obj_base::Move(direct); }

	//����Ƿ�Ҫ��ʧ/��ը��
	virtual bool AboutToDisappear() const = 0; 

	virtual ~SpecialBomb() {}

protected:

	//����ը��
	virtual void SetLaid(sigPosType x, sigPosType y, direction laidDirect, int newMoveSpeed); 

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
	Fire(sigPosType x, sigPosType y, int timeLeft) : SpecialBomb(x, y), timeLeft(timeLeft) {}
	virtual propType GetPropType() const override { return propType::fire; }

	int GetTimeLeft() const { return timeLeft; }
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	//�������Ƿ�Ҫ��ʧ��
	virtual bool AboutToDisappear() const override { return timeLeft <= 0; }

	virtual ~Fire() {}
private:
	int timeLeft;	//����ʣ��Ĵ���ʱ��
};

//����ǹ
class Ice final : public SpecialBomb
{
public:
	Ice(sigPosType x, sigPosType y, int timeLeft) : SpecialBomb(x, y), timeLeft(timeLeft) {}
	virtual propType GetPropType() const override { return propType::mine; }

	int GetTimeLeft() const { return timeLeft; }
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	//�����Ƿ�Ҫ��ʧ��
	virtual bool AboutToDisappear() const override { return timeLeft <= 0; }

	virtual ~Ice() {}

private:

	int timeLeft; 		//���뺮����ʧ��ʱ��
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

	virtual ~Grenade() {}
private: 
	sigPosType leftDistance;		//ʣ������н��ľ���
}; 

//����
class Missil final : public SpecialBomb
{
public: 
	Missil(sigPosType x, sigPosType y) : SpecialBomb(x, y) {}

	virtual ~Missil() {}
};

#endif // #ifndef PROP_H
