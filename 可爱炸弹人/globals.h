//////////////////////////////
//
//  ���ȫ�ֱ���
//

#ifndef GLOBALS_H

#define GLOBALS_H

#include "framework.h"

//��ʱ��ID

#define TIMER_ID_START 9999					//��ʼˢ�´��ڼ�ʱ��

//���ڴ���

extern HINSTANCE hInst;						//��ǰʵ��
extern HWND hMainWnd;						//�����ھ��
extern CONST LONG objSize;					//ÿ��ı߳�
extern CONST POINT mainWndPos;				//���������Ͻ�����
extern CONST POINT mainWndSize;				//�����ڴ�С
extern LONG capMenuAppendCy;				//�������Ͳ˵���ռ�õĸ߶�

//λͼ

extern HBITMAP hBmMem;						//���ڻ���
extern HBITMAP hBmBkgnd;					//����λͼ���
extern HBITMAP hBmRole;						//��ɫλͼ���

extern BITMAP bmBkgnd;						//����λͼ
extern BITMAP bmRole;						//��ɫλͼ

#endif	// #ifndef GLOBALS_H
