////////////////////////////////
//
//  һ����Ϸ����Ļ���
//

#ifndef OBJ_BASE_H

#define OBJ_BASE_H

#include <mutex>

class obj_base
{
public:

	typedef int sigPosType;
	typedef struct tagPosType
	{
		sigPosType x;
		sigPosType y;
		tagPosType(sigPosType x, sigPosType y) : x(x), y(y) {}
	} posType;

	enum class objType		//��������
	{
		null,			//�����κ�����
		role,			//��ɫ
		tnt,			//��ͨը��
		softObstacle,	//���ϰ�
		hardObstacle,	//Ӳ�ϰ�
		bombArea,		//��ը����
		prop			//����
	}; 

	enum class direction		//����
	{
		Null,
		Up,
		Down,
		Left,
		Right
	};

	obj_base(sigPosType x, sigPosType y, bool isMoving, sigPosType moveSpeed) 
		: pos(x, y), isMoving(isMoving), moveSpeed(moveSpeed) 
	{
		CorrectPos(); 
	}

	
	//���õ�ͼ��Χ���������������Լ�ÿ����ı߳�
	static void SetMapBound(int rows, int cols, obj_base::sigPosType posUnitPerCell);
	
	//��ȡ����
	posType GetPos() const { return pos; }

	//��ȡ��Χ
	posType GetMinPos() const { return minPos; }
	posType GetMaxPos() const { return maxPos; }

	//�ж��Ƿ����ƶ�
	bool IsMoving() const { return isMoving; }

	//��ȡ�ƶ��ٶ�
	sigPosType GetMoveSpeed() const { return moveSpeed; }

	//��ȡ������
	std::mutex& GetMutex() { return objMutex; }
	
	//��ȡ���������麯��
	virtual objType GetObjType() const = 0; 

	virtual ~obj_base() {}

protected:

	void Move(direction direc);										//�ƶ�һ��
	void MoveLessThanOneStep(direction direc, sigPosType moveDistance);	//�ƶ�����1��

	//��¼��ͼ���ϽǺ����½ǵ�����
	static posType minPos; 
	static posType maxPos; 
	static const sigPosType EPS;				//���꾫��
	static obj_base::sigPosType posUnitPerCell;	//ÿ�����ӵı߳�
	void CorrectPos();						//�������꣬����Խ��


	posType pos;			//λ������
	bool isMoving;			//�Ƿ����ƶ�
	sigPosType moveSpeed;	//�ƶ��ٶ�
	std::mutex objMutex;	//�������������߳�ʹ��
};

#endif // #ifndef OBJBASE_H
