#ifndef BOMBAREA_H

#define BOMBAREA_H

#include "TNT.h"
#include "Prop.h"

//��ը����
class BombArea : public obj_base
{
public: 
	BombArea(sigPosType x, sigPosType y, int ownerID, Prop::propType bomb) 
		: obj_base(x, y, false, 0), ownerID(ownerID), bomb(bomb) {}

	virtual objType GetObjType() const override { return objType::bombArea; }

	virtual ~BombArea() {}
private: 
	
	int ownerID; 
	Prop::propType bomb;	//���������ը�������������
};

#endif	// #ifndef BOMBAREA_H
