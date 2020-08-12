

#ifndef UI_H

#define UI_H

#include "Game.h"
#include "framework.h"
#include "resource.h"
#include "str.h"
#include "BasicWindow.h"

//��ʱ��ID

#define TIMER_ID_START 9999					//��ʼˢ�´��ڼ�ʱ��

class UI final : public BasicWindow
{
public: 

	UI() = default; 

	//��ʼ��������ִ��
	int Begin(HINSTANCE hInstance, int nCmdShow); 
	

	~UI(); 

protected: 

	//��Ϣ������
	virtual bool messageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override; 

private: 

	enum class programstate	//����״̬
	{
		starting = 0,		//��ʼ����
		gaming = 1,			//������Ϸ
		gamePulsing = 2,	//��Ϸ��ͣ
	}; 
	programstate programState;			//����״̬

	static const int objSize;
	static const int propSize;
	static const POINT mainWndPos;
	static const POINT mainWndSize;
	static const int dataFps;			//����֡
	static const int paintFps;			//����֡

	Game* pGame = nullptr;				//��Ϸ�ڲ��߼�

	void ScanData();					//ɨ����Ϸ����
	void RoleControl(int player);		//��ɫ����
	void RefreshScreen();				//ˢ����Ļ

	void EndGame(); 

	//λͼ�����ȱʡֵ��ΪNULL��C++11�����ϣ�
	HBITMAP hBmMem = NULL;				//���ڻ���
	HBITMAP hBmBkgnd = NULL;			//����λͼ���
	HBITMAP hBmRole = NULL;				//��ɫλͼ���
	HBITMAP hBmTnt = NULL;				//ը��λͼ���
	HBITMAP hBmObstacle = NULL;			//�ϰ���λͼ���
	HBITMAP hBmGlove = NULL;			//����λͼ���
	HBITMAP hBmShield = NULL;			//����λͼ���
	HBITMAP hBmAddTnt = NULL;			//���ڵ�λͼ���
	HBITMAP hBmAddLife = NULL;			//������λͼ���
	HBITMAP hBmShoe = NULL;				//��Ьλͼ���
	HBITMAP hBmJinKeLa = NULL;			//�����λͼ���
	HBITMAP hBmLachrymator = NULL;		//������˹λͼ���
	HBITMAP hBmMine = NULL;				//����λͼ���
	HBITMAP hBmFire = NULL;				//����ǹλͼ���
	HBITMAP hBmIce = NULL;				//��ǹλͼ���
	HBITMAP hBmGrenade = NULL;			//����λͼ���
	HBITMAP hBmMissile = NULL;			//����λͼ���

	//��λͼ��Ϣ��
	BITMAP bmBkgnd;						//����λͼ��Ϣ
	BITMAP bmRole;						//��ɫλͼ��Ϣ
	BITMAP bmTnt;						//ը��λͼ��Ϣ
	BITMAP bmObstacle;					//�ϰ���λͼ��Ϣ
	BITMAP bmGlove;						//����λͼ��Ϣ
	BITMAP bmShield;					//����λͼ��Ϣ
	BITMAP bmAddTnt;					//���ڵ�λͼ��Ϣ
	BITMAP bmAddLife;					//������λͼ��Ϣ
	BITMAP bmShoe;						//��Ьλͼ��Ϣ
	BITMAP bmJinKeLa;					//�����λͼ��Ϣ
	BITMAP bmLachrymator;				//������˹λͼ��Ϣ
	BITMAP bmMine;						//����λͼ��Ϣ
	BITMAP bmFire;						//����ǹλͼ�����Ϣ
	BITMAP bmIce;						//��ǹλͼ��Ϣ
	BITMAP bmGrenade;					//����λͼ��Ϣ
	BITMAP bmMissile;					//����λͼ��Ϣ

	LONG capMenuAppendCy;				//�������Ͳ˵���ռ�õĸ߶�

	bool LoadGameImg();					//����λͼ
	void CreateBuffer(HWND hWnd);		//��������λͼ

	//��ʼ��ͼ
	void Paint(HWND hWnd, BOOL calledByPaintMessage); 

	//��������תͼ������
	int PosToPaint(int p) { return (int)(((double)p / Game::GetPosUnitPerCell() - 0.5) * objSize); }
};

#endif	// #ifndef GLOBALS_H
