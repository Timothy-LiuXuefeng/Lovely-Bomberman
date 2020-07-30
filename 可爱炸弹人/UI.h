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

	HBITMAP hBmMem = NULL;					//���ڻ���
	HBITMAP hBmBkgnd = NULL;				//����λͼ���
	HBITMAP hBmRole = NULL;					//��ɫλͼ���
	HBITMAP hBmTnt = NULL;					//ը��λͼ���

	BITMAP bmBkgnd;						//����λͼ
	BITMAP bmRole;						//��ɫλͼ
	BITMAP bmTnt;						//ը��λͼ
};

extern UI mainGameUI; 

#endif	// #ifndef GLOBALS_H
