////////////////////////////////////
//
//  ������Ҫ�õ����ַ���
//

#ifndef STR_H

#define STR_H

#include "framework.h"


//���ڶ���

static LPCTSTR c_lpszWndClassName = TEXT("LovelyBombermanMainWindow");	//��������
static LPCTSTR c_lpszWndTitle = TEXT("�ɰ�ը����");						//�����ڱ���

//����
static LPCTSTR c_lpszWarning = TEXT("���������ܻ�������������Ƿ������\nThis may cause fatal errors, continue?");
static LPCTSTR c_lpszWarningTitle = TEXT("warning");

//����
static LPCTSTR c_lpszAbout = TEXT("���ƣ��ɰ�ը����\n\n�汾��v1.0.0.0\n\nCopyRight (C) 2020\n\n�����������ƻ���(2) All Rights Reverved."); 

//ͼƬ·��
#define BKGND_PATH TEXT("image\\bkgnd.bmp")
#define MAIN_PATH TEXT("image\\main.bmp")
#define ROLE_PATH TEXT("image\\role.bmp")
#define TNT_PATH TEXT("image\\tnt.bmp")
#define OBSTACLE_PATH TEXT("image\\obstacle.bmp")
#define GLOVE_PATH TEXT("image\\glove.bmp")
#define SHIELD_PATH TEXT("image\\shield.bmp")
#define ADDTNT_PATH TEXT("image\\addtnt.bmp")
#define ADDLIFE_PATH TEXT("image\\addLife.bmp")
#define SHOE_PATH TEXT("image\\shoe.bmp")
#define JINKELA_PATH TEXT("image\\jinkela.bmp")
#define LACHRYMATOR_PATH TEXT("image\\lachrymator.bmp")
#define MINE_PATH TEXT("image\\mine.bmp")
#define FIRE_PATH TEXT("image\\fire.bmp")
#define ICE_PATH TEXT("image\\ice.bmp")
#define GRENADE_PATH TEXT("image\\grenade.bmp")
#define MISSILE_PATH TEXT("image\\missile.bmp")

#define IMAGE_LOAD_FAIL_STR(path) TEXT("�Ҳ���ͼƬ��") path TEXT("\nCannot find image: ") path
#define IMAGE_LOAD_FAIL_RETRY TEXT("ͼƬ����ʧ�ܣ�\nFailing to load images!")

#endif	// #ifndef STR_H
