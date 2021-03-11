

#ifndef UI_H

#define UI_H

#include "Game.h"
#include "framework.h"
#include "resource.h"
#include "str.h"
#include "BasicWindow.h"
#include "BasicDialog.h"
#include <queue>
#include <future>
#include <exception>
#include "..\Images\Images.h"
#include "..\Images\resource.h"

//��ʱ��ID

#define TIMER_ID_START 9999					//��ʼˢ�´��ڼ�ʱ��

class UI final : public BasicWindow
{
public: 

	UI();

	//��ʼ��������ִ��
	int Begin(HINSTANCE hInstance, int nCmdShow); 
	

	~UI();

	class FailToLoadGameImage : std::exception
	{
	public:
		virtual const char* what() const { return "Fail to load images!"; }
	};

	class FailToExecuteGameSound : std::exception
	{
	public:
		virtual const char* what() const { return "Fail to execute the application for playing the sound of the game!"; }
	};

protected: 

	//��Ϣ������
	virtual bool MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override; 

private: 

	enum class programstate	//����״̬
	{
		starting = 0,		//��ʼ����
		gaming = 1,			//������Ϸ
		gamePulsing = 2,	//��Ϸ��ͣ
		changeLevel = 3		//����
	}; 
	programstate programState;			//����״̬

	static const int objSize;
	static const int propSize;
	static const POINT mainWndPos;
	static const POINT mainWndSize;
	static const int dataFps;			//����֡
	static const int paintFps;			//����֡

	Game* pGame = nullptr;				//��Ϸ�ڲ��߼�
	bool IsPlayer(int roleID) const		//�ж��Ƿ������
	{
		return pGame ? (roleID == pGame->GetID1() || (pGame->GetNumOfPlayer() == 2 && roleID == pGame->GetID2())) : false; 
	}
	bool IsComputer(int roleID) const { return pGame ? !IsPlayer(roleID) : false; }

	void ScanData();					//ɨ����Ϸ����
	void RoleControl(int roleID);		//��ɫ����
	void RefreshScreen();				//ˢ����Ļ
	void EndGame(int result);			//������Ϸ
	void AI(int roleID);				//����AI

	//��ȡ�߷�
	std::vector<std::pair<std::_tstring, int>> ReadHighScore() const;

	//д��߷�
	void SaveHighScore(const std::vector<std::pair<std::_tstring, int>>& scoreList) const;
	
	//�·���
	void newScore(int numOfPlayer, Game::Difficulty difficulty, int score); 

	std::future<void>* pScanDataTask = nullptr; 
	std::future<void>* pRefreshScreenTask = nullptr;
	std::vector<std::future<void>*> pRoleControlTasks;

	//��¼��ɫ����״̬
	std::vector<bool> playerLay{ false, false }; 
	
	//λͼ�����ȱʡֵ��ΪNULL��C++11�����ϣ�
	HBITMAP hBmMem = NULL;				//���ڻ���
	HBITMAP hBmMain = NULL;				//��ʼ����λͼ����
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
	BITMAP bmMain;						//��ʼ����λͼ
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
	bool TryExecutingGameSound();		//���Լ������ֲ����ļ�
	const SoundPlay soundPlay;			//����/��Ч������

	//���ű�������

	class PlayMainMusic
	{
	public:
		void operator()() const;
	};

	//��ʼ��ͼ
	void Paint(HWND hWnd, BOOL calledByPaintMessage); 

	//��������תͼ������
	int PosToPaint(int p) { return (int)(((double)p / Game::GetPosUnitPerCell() - 0.5) * objSize); }

	////////////////////////////////////////////////////
	//
	// �ӶԻ���
	//

	//��ʼ��Ϸ�Ի�����
	class StartGameDlg : public BasicModalDialog
	{
	private: using Difficulty = Game::Difficulty; 

	protected: 
		virtual void MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) override; 

	public: 
		StartGameDlg() : choose(false), difficulty(Difficulty::easy), numOfPlayer(0), player1ID(0), player2ID(0) {}
		bool Begin(HINSTANCE hInstance, HWND hWndParent);
		bool Choose() const { return choose; }
		int NumOfPlayer() const { return numOfPlayer; }
		int Player1ID() const { return player1ID; }
		int Player2ID() const { return player2ID; }
		Difficulty GetDifficulty() const { return difficulty; }

	private: 
		bool choose;					//�Ƿ�ѡ��ɹ�
		Difficulty difficulty;			//�Ѷ�
		int numOfPlayer;				//�����
		int player1ID;					//1P��id
		int player2ID;					//2P��ID
	};

	//������������Ի���
	class InputNameDlg : public BasicModalDialog
	{
	protected: 
		virtual void MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) override;
	public: 
		void Begin(HINSTANCE hInstance, HWND hWndParent);
		std::_tstring GetName() const { return name; }
	private: 
		std::_tstring name; 
	};

	StartGameDlg startGameDlg; 
	InputNameDlg inputNameDlg; 
};

#endif	// #ifndef GLOBALS_H
