////////////////////////////////
//
//  һ����Ϸ����Ļ���
//

#ifndef BASIC_H

#define BASIC_H

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

	enum objType		//��������
	{
		role,			//��ɫ
		tnt,			//��ͨը��
		softObstacle,	//���ϰ�
		hardObstacle,	//Ӳ�ϰ�
		bombArea,		//��ը����
		prop			//����
	}; 

	obj_base(sigPosType x, sigPosType y, bool isMoving, sigPosType moveSpeed) 
		: pos(x, y), isMoving(isMoving), moveSpeed(moveSpeed) 
	{
		CorrectPos(); 
	}

	//��ȡ����
	posType GetPos() const { return pos; }

	//���õ�ͼ��Χ���������������Լ�ÿ����ı߳�
	static void SetMapBound(int rows, int cols, int posUnitPerCell);  
	
	//�ж��Ƿ����ƶ�
	bool IsMoving() const { return isMoving; }

	//����Ϊ�ƶ�״̬
	void SetMoving() { isMoving = true; }
	//����Ϊ���ƶ�״̬
	void SetNoMoving() { isMoving = false; }

	//��ȡ�ƶ��ٶ�
	sigPosType GetMoveSpeed() const { return moveSpeed; }

	//��ȡ���������麯��
	virtual objType GetObjType() const = 0;

	virtual ~obj_base() {}

protected:

	enum direction		//����
	{
		Null, 
		Up, 
		Down, 
		Left, 
		Right
	}; 

	void Move(direction direc); 

	//��¼��ͼ���ϽǺ����½ǵ�����
	static posType minPos; 
	static posType maxPos; 
	static const sigPosType EPS; //���꾫��
	static int posUnitPerCell;	//ÿ�����ӵı߳�
	void CorrectPos();		//�������꣬����Խ��


	posType pos;			//λ������
	bool isMoving;			//�Ƿ����ƶ�
	sigPosType moveSpeed;	//�ƶ��ٶ�

};

#endif // #ifndef OBJBASE_H
