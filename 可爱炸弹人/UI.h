//////////////////////////////
//
//  ���ȫ�ֱ���
//

#ifndef UI_H

#define UI_H

#include "framework.h"
#include "str.h"

//��ʱ��ID

#define TIMER_ID_START 9999					//��ʼˢ�´��ڼ�ʱ��

//���ڴ���

extern HINSTANCE hInst;						//��ǰʵ��
extern HWND hMainWnd;						//�����ھ��
extern CONST LONG objSize;					//ÿ��ı߳�
extern CONST LONG propSize;					//���ߵı߳�
extern CONST POINT mainWndPos;				//���������Ͻ�����
extern CONST POINT mainWndSize;				//�����ڴ�С
extern LONG capMenuAppendCy;				//�������Ͳ˵���ռ�õĸ߶�

class UI
{
public: 

	bool LoadGameImg();						//����λͼ
	void CreateBuffer(HWND hWnd);			//��������λͼ

	//��ʼ��ͼ
	void Paint(HWND hWnd, BOOL calledByPaintMessage); 	

	~UI(); 

private: 

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
	HBITMAP hBmMissil = NULL;			//����λͼ���

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
	BITMAP bmMissil;					//����λͼ��Ϣ
};

extern UI mainGameUI; 

#endif	// #ifndef GLOBALS_H
