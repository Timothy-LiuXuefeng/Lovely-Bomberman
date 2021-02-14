////////////////////////////////////
//
//  str.h : ������Ҫ�õ����ַ���
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

//����
static LPCTSTR c_lpszHelp = TEXT("����Ϸ������ģʽ������ģʽ��˫��ģʽ�������Ѷȣ��򵥡�һ������ѡ�\n\
1.��Ҳ�����1P��WASD���ߣ��ո����ը����2P�������������ߣ��س�����ը��\n\
2.��ͼ����ͼ����Ӳ�ϰ������ϰ���Ӳ�ϰ����ܹ���ը�٣������ϰ����Ա�ը��\n\
3.���ߣ�ը�����ϰ�ʱ�м��ʲ���õ��ߣ���ͬ�ĵ����в�ͬ��Ч��\n\
4.����������Ϸ��5�أ�ÿһ�ػ�õ�һ���ķ���\n\
5.����ֵ����ɫ������ֵ����ҿ��Ը���Ĵ�����������ֵ����֮����޷������Ϸ����\n\
6.�����Ѷȣ�\n\
  ���Ѷȣ���ҳ�ʼ���л��ܣ����Բ��ܼ�����ߣ�\n\
  �е��Ѷȣ���ҳ�ʼ�����л��ܣ����Բ��ܼ�����ߣ�\n\
  �����Ѷȣ���ҳ�ʼ�����л��ܣ�����Ҳ���Լ����\n\
7.��ʾ��һ����ɫ���õ�ը������ͬʱ�ڵ�ͼ�ϴ��ڵ����������޵ģ����ﵽ����ʱ�㲻�ܷ���ը������Ҫ�ȴ���ǰ���õ�ը����ը����ܼ�������"); 
static LPCTSTR c_lpszHelpProp = TEXT("����ϵͳ��\n\
1.���ף�ʹ�������ƶ�ը��������\n\
2.���ƣ��ܹ��ֵ�һ�ι���\n\
3.����ը���������������ڵ�ͼ�Ϸ���ը������Ŀ\n\
4.��������������ֵ+1\n\
5.��Ь�������ƶ��ٶ�\n\
6.�����������ը����ը��Χ\n\
7.������˹�����ú�һ��ʱ��֮�ڿɼ���֮���Ϊ���ɼ�����ʹ�ȵ�����Ľ�ɫ���٣����ᱻը��ը�٣����ұ�Ϊ���ɼ�֮��һ��ʱ��Ҳ����ʧ\n\
8.���ף����ú�һ��ʱ��֮���Ϊ���ɼ�����ʹ�ŵ�����Ľ�ɫ�ܵ����������ᱻը��ը�٣����ұ�Ϊ���ɼ�֮��һ��ʱ��Ҳ����ʧ\n\
9.����ǹ��ʹ�ú������������ǰ����ĵ���\n\
10.���񵯣��ܹ�Խ���ϰ���������\n\
11.�������������ߵĵ���"); 
static LPCTSTR c_lpszHelpScore = TEXT("����ϵͳ����Ϸ���ض��¼���ʹ�ý�ɫ��÷���\n\
1.ÿը��һ�����ϰ���1��\n\
2.ÿ��һ�����ߵ�2��\n\
3.ÿ������һ����ɫ������ɱ������20��\n\
ֵ��ע����ǣ�����������һ���л�ʤ���������һ�صõ��ķ�����������ʧ�ܣ��򲻻�����һ�صõ��ķ���"); 
static LPCTSTR c_lpszHelpMusic = TEXT("��������ϷЧ������"); 

//�߷ִ浵·��
#define HIGH_SCORE_DIRECTORY TEXT("data")
#define HIGH_SCORE_PATH HIGH_SCORE_DIRECTORY TEXT("\\HighScore.sav")

#define IMAGE_LOAD_FAIL_STR(path) TEXT("�Ҳ���ͼƬ��") path TEXT("\nCannot find image: ") path
#define IMAGE_LOAD_FAIL_RETRY TEXT("ͼƬ����ʧ�ܣ�\nFailing to load images!")

#ifdef _WIN64
#define SOUND_EXE_PATH TEXT("GameSound(x64).exe")
#else	//ifdef _WIN64
#define SOUND_EXE_PATH TEXT("GameSound.exe")
#endif	//ifdef _WIN64
#define SOUND_EXE_NOT_FOUND TEXT("�Ҳ�����Ч���ų���") SOUND_EXE_PATH TEXT("\nCannot find the file for playing sound: ") SOUND_EXE_PATH

#define MUSIC_LOAD_FAIL_STR(path) TEXT("�Ҳ���������") path TEXT("\nCannot find sound: ") path

#endif	// #ifndef STR_H
