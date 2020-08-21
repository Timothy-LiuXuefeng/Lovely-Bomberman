#include "UI.h"

const int UI::objSize = 40;
const int UI::propSize = 30;
const POINT UI::mainWndPos = { 0, 0 };
const POINT UI::mainWndSize = { (objSize * 15) + 240, objSize * 13 }; 
const int UI::dataFps = 50; 
const int UI::paintFps = 50;

int UI::Begin(HINSTANCE hInstance, int nCmdShow)
{

    //����λͼ
    if (!LoadGameImg()) return 0; 

    //���崰����ʽ
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(MAINICON));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = MAKEINTRESOURCE(MAINMENU); 
    wcex.lpszClassName = c_lpszWndClassName;
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON));
    
    capMenuAppendCy = GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYMIN);

    Init(hInstance, nCmdShow, 0, 0, mainWndSize.x, mainWndSize.y + capMenuAppendCy, 
        WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX, 
        c_lpszWndTitle, wcex, MAKEINTRESOURCE(MAINMENUACCELERATOR));

    MSG msg;

    programState = programstate::starting;
    PlayMainMusic(); 

    // ����Ϣѭ��:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(m_hWnd, m_hAccel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam; 
}

//��Ϣ������
bool UI::MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: 
        CreateBuffer(hWnd);
        break; 
    case WM_PAINT:
    {
        Paint(hWnd, TRUE);
    }
    break;
    case WM_LBUTTONUP:
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_KEYDOWN: 
        switch (wParam)
        {
        case VK_SPACE: if (programState == programstate::gaming) playerLay[0] = true; break; 
        case VK_RETURN: if (programState == programstate::gaming) playerLay[1] = true; break;
        }
        break; 
    case WM_DESTROY: 
        if (hBmMem)
        {
            DeleteObject(hBmMem); 
            hBmMem = NULL; 
        }
        //KillTimer(hWnd, 500); 
        PostQuitMessage(0); 
        break; 
    case WM_COMMAND: 
    {
        switch (LOWORD(wParam))
        {
        case IDM_START: 
        {
            startGameDlg.Begin(m_hInst, hWnd);
            if (!startGameDlg.Choose()) break;
            pGame = new Game(startGameDlg.NumOfPlayer(), startGameDlg.Player1ID(),
                startGameDlg.Player2ID(), startGameDlg.GetDifficulty());
            pGame->InitNewLevel(0, true);
            programState = programstate::gaming;
            pScanDataTask = new std::future<void>(std::async(&UI::ScanData, this));
            pRefreshScreenTask = new std::future<void>(std::async(&UI::RefreshScreen, this));
            pRoleControlTasks[1] = new std::future<void>(std::async(&UI::RoleControl, this, startGameDlg.Player1ID()));    //��ɫ1
            if (startGameDlg.NumOfPlayer() == 2)
            {
                pRoleControlTasks[2] = new std::future<void>(std::async(&UI::RoleControl, this, startGameDlg.Player2ID()));    //��ɫ2
                int i = 3;
                for (int j = 1; j <= 4; ++j)
                {
                    if (j == startGameDlg.Player1ID() || j == startGameDlg.Player2ID()) continue;
                    pRoleControlTasks[i] = new std::future<void>(std::async(&UI::AI, this, j));
                    ++i;
                }
            }
            else if (startGameDlg.NumOfPlayer() == 1)
            {
                int i = 2;
                for (int j = 1; j <= 4; ++j)
                {
                    if (j == startGameDlg.Player1ID()) continue;
                    pRoleControlTasks[i] = new std::future<void>(std::async(&UI::AI, this, j));
                    ++i;
                }
            }
            HMENU hMenu = GetMenu(hWnd);
            EnableMenuItem(hMenu, IDM_START, TRUE);
            EnableMenuItem(hMenu, IDM_RESTART, FALSE);
            EnableMenuItem(hMenu, IDM_PAUSE, FALSE);
            EnableMenuItem(hMenu, IDM_END, FALSE);
            break;
        }
        case IDM_END: 
            if (MessageBox(hWnd, TEXT("�����Ҫ������Ϸ��\nDo you really want to end this game?"),
                TEXT("Confirm"), MB_YESNO | MB_ICONWARNING) == IDYES) EndGame(3); 
            break; 
        case IDM_RESTART: 
            if (MessageBox(hWnd,
                TEXT("�����Ҫ���¿�ʼ�����ı�����Ϸ���Ƚ����ᱣ����\nDo you really want to end this game? Your game will NOT be saved."),
                TEXT("Confirm"), MB_YESNO | MB_ICONWARNING) == IDYES)
            {
                EndGame(3); 
                SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDM_START, 0), 0); 
            }
            break; 
        case IDM_PAUSE: 
            programState = programstate::gamePulsing; 
            MessageBox(hWnd, TEXT("��ͣ�С���\nPausing..."), TEXT("��Ϸ��ͣ"), MB_OK); 
            programState = programstate::gaming;
            break; 
        case IDM_LIST:
        {
            auto scoreList = ReadHighScore(); 
            if (scoreList.size() != 6)
            {
                MessageBox(m_hWnd, TEXT("��ȡ����"), c_lpszError, MB_OK | MB_ICONERROR); 
                break; 
            }
            std::_tostringstream outStr; 
            if constexpr (std::is_same<std::_tostringstream, std::wostringstream>::value) outStr.imbue(std::locale("chs")); 
            outStr << TEXT("���ˣ�\n�򵥣�") << scoreList[0].second << TEXT("\t") << scoreList[0].first << TEXT("\n"); 
            outStr << TEXT("�еȣ�") << scoreList[1].second << TEXT("\t") << scoreList[1].first << TEXT("\n");
            outStr << TEXT("���ѣ�") << scoreList[2].second << TEXT("\t") << scoreList[2].first << TEXT("\n");
            outStr << TEXT("\n"); 
            outStr << TEXT("˫�ˣ�\n�򵥣�") << scoreList[3].second << TEXT("\t") << scoreList[3].first << TEXT("\n");
            outStr << TEXT("�еȣ�") << scoreList[4].second << TEXT("\t") << scoreList[4].first << TEXT("\n");
            outStr << TEXT("���ѣ�") << scoreList[5].second << TEXT("\t") << scoreList[5].first << TEXT("\n");
            MessageBox(hWnd, outStr.str().c_str(), TEXT("�߷ְ�"), MB_OK); 
            break; 
        }
        case IDM_HELP: 
        {
            MessageBox(m_hWnd, c_lpszHelp, TEXT("����"), MB_OK | MB_ICONQUESTION); 
            MessageBox(m_hWnd, c_lpszHelpProp, TEXT("����ϵͳ"), MB_OK | MB_ICONQUESTION);
            MessageBox(m_hWnd, c_lpszHelpScore, TEXT("����ϵͳ"), MB_OK | MB_ICONQUESTION);
            MessageBox(m_hWnd, c_lpszHelpMusic, TEXT("��ܰ��ʾ"), MB_OK | MB_ICONQUESTION); 
            break; 
        }
        case IDM_ABOUT: 
            MessageBox(hWnd, c_lpszAbout, TEXT("About"), MB_OK | MB_ICONINFORMATION); 
            break; 
        case IDM_EXIT:                          //�˳�
            SendMessage(hWnd, WM_CLOSE, 0, 0); 
            break; 
        }
        break; 
    }
    case WM_CLOSE: 
        if (MessageBox(hWnd, TEXT("��ȷ��Ҫ�˳���\nAre you sure to quit?"), TEXT("Exit"), MB_YESNO | MB_ICONQUESTION) == IDYES)
            SendMessage(hWnd, WM_DESTROY, 0, 0);
        break; 
    default: 
        return false; 
    }
    return true; 
}

bool UI::LoadGameImg()
{
loadImage: 

    hBmBkgnd = (HBITMAP)LoadImage(m_hInst, BKGND_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMain = (HBITMAP)LoadImage(m_hInst, MAIN_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmRole = (HBITMAP)LoadImage(m_hInst, ROLE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmTnt = (HBITMAP)LoadImage(m_hInst, TNT_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
    hBmObstacle = (HBITMAP)LoadImage(m_hInst, OBSTACLE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
    hBmGlove = (HBITMAP)LoadImage(m_hInst, GLOVE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmShield = (HBITMAP)LoadImage(m_hInst, SHIELD_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAddTnt = (HBITMAP)LoadImage(m_hInst, ADDTNT_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAddLife = (HBITMAP)LoadImage(m_hInst, ADDLIFE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmShoe = (HBITMAP)LoadImage(m_hInst, SHOE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmJinKeLa = (HBITMAP)LoadImage(m_hInst, JINKELA_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLachrymator = (HBITMAP)LoadImage(m_hInst, LACHRYMATOR_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMine = (HBITMAP)LoadImage(m_hInst, MINE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmFire = (HBITMAP)LoadImage(m_hInst, FIRE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmIce = (HBITMAP)LoadImage(m_hInst, ICE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmGrenade = (HBITMAP)LoadImage(m_hInst, GRENADE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMissile = (HBITMAP)LoadImage(m_hInst, MISSILE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (!hBmBkgnd || !hBmRole || !hBmTnt || !hBmObstacle || !hBmGlove || !hBmShield | !hBmAddTnt || !hBmAddLife || !hBmShoe 
        || !hBmJinKeLa || !hBmLachrymator || !hBmMine || !hBmFire || !hBmIce || !hBmGrenade || !hBmMissile)
    {

        if (!hBmBkgnd) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(BKGND_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmMain) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(MAIN_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmRole) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(ROLE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmTnt) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(TNT_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmObstacle) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(OBSTACLE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmGlove) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(GLOVE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmShield) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(SHIELD_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmAddTnt) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(ADDTNT_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmAddLife) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(ADDLIFE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmShoe) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(SHOE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmJinKeLa) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(JINKELA_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmLachrymator) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(LACHRYMATOR_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmMine) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(MINE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmFire) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(FIRE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmIce) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(ICE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmGrenade) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(GRENADE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmMissile) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(MISSILE_PATH), c_lpszError, MB_OK | MB_ICONERROR);

    choose: 
        switch (MessageBox(m_hWnd, IMAGE_LOAD_FAIL_RETRY, c_lpszError, MB_ABORTRETRYIGNORE | MB_ICONERROR))
        {
        case IDRETRY: goto loadImage; 
        default: case IDABORT: return false; 
        case IDIGNORE: 
            if (MessageBox(m_hWnd, c_lpszWarning, c_lpszWarningTitle, MB_YESNO | MB_ICONWARNING) != IDYES) goto choose; 
        }
    }

    if (hBmBkgnd) GetObject(hBmBkgnd, sizeof(BITMAP), &bmBkgnd); 
    if (hBmMain) GetObject(hBmMain, sizeof(BITMAP), &bmMain);
    if (hBmRole) GetObject(hBmRole, sizeof(BITMAP), &bmRole); 
    if (hBmTnt) GetObject(hBmTnt, sizeof(BITMAP), &bmTnt);
    if (hBmObstacle) GetObject(hBmObstacle, sizeof(BITMAP), &bmObstacle);
    if (hBmGlove) GetObject(hBmGlove, sizeof(BITMAP), &bmGlove);
    if (hBmShield) GetObject(hBmShield, sizeof(BITMAP), &bmShield);
    if (hBmAddTnt) GetObject(hBmAddTnt, sizeof(BITMAP), &bmAddTnt);
    if (hBmAddLife) GetObject(hBmAddLife, sizeof(BITMAP), &bmAddLife);
    if (hBmShoe) GetObject(hBmShoe, sizeof(BITMAP), &bmShoe);
    if (hBmJinKeLa) GetObject(hBmJinKeLa, sizeof(BITMAP), &bmJinKeLa);
    if (hBmLachrymator) GetObject(hBmLachrymator, sizeof(BITMAP), &bmLachrymator);
    if (hBmMine) GetObject(hBmMine, sizeof(BITMAP), &bmMine);
    if (hBmFire) GetObject(hBmFire, sizeof(BITMAP), &bmFire);
    if (hBmIce) GetObject(hBmIce, sizeof(BITMAP), &bmIce);
    if (hBmGrenade) GetObject(hBmGrenade, sizeof(BITMAP), &bmGrenade);
    if (hBmMissile) GetObject(hBmMissile, sizeof(BITMAP), &bmMissile);
	return true; 
}

void UI::CreateBuffer(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);
    hBmMem = CreateCompatibleBitmap(hdc, mainWndSize.x, mainWndSize.y); 
    ReleaseDC(hWnd, hdc); 
}

void UI::PlayMainMusic()
{
loadMainMusic: 
    WIN32_FIND_DATA wfd; 
    if (FindFirstFile(MAIN_MUSIC_PATH, &wfd) == INVALID_HANDLE_VALUE)
    {
        switch (MessageBox(m_hWnd, MAIN_MUSIC_LOAD_FAIL_STR, c_lpszError, MB_RETRYCANCEL | MB_ICONERROR))
        {
        case IDRETRY: goto loadMainMusic; break;
        }
    }
    else PlaySound(MAIN_MUSIC_PATH, NULL, SND_ASYNC | SND_LOOP); 

loadBombMusic: if (FindFirstFile(BOMB_MUSIC_PATH, &wfd) == INVALID_HANDLE_VALUE
        && MessageBox(m_hWnd, MUSIC_LOAD_FAIL_STR(BOMB_MUSIC_PATH), c_lpszError, MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY) goto loadBombMusic; 
loadPropMusic: if (FindFirstFile(PICK_PROP_MUSIC_PATH, &wfd) == INVALID_HANDLE_VALUE
        && MessageBox(m_hWnd, MUSIC_LOAD_FAIL_STR(PICK_PROP_MUSIC_PATH), c_lpszError, MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY) goto loadPropMusic;
loadFailMusic: if (FindFirstFile(FAIL_MUSIC_PATH, &wfd) == INVALID_HANDLE_VALUE
        && MessageBox(m_hWnd, MUSIC_LOAD_FAIL_STR(FAIL_MUSIC_PATH), c_lpszError, MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY) goto loadFailMusic;
loadSuccessMusic: if (FindFirstFile(SUCCESS_MUSIC_PATH, &wfd) == INVALID_HANDLE_VALUE
        && MessageBox(m_hWnd, MUSIC_LOAD_FAIL_STR(SUCCESS_MUSIC_PATH), c_lpszError, MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY) goto loadSuccessMusic;
}

void UI::ScanData()
{
    while (programState == programstate::gaming || programState == programstate::gamePulsing)
    {
        while (programState == programstate::gamePulsing) Sleep(1000 / dataFps); 
        pGame->CheckRole(); 
        pGame->CheckBomb(1000 / dataFps); 
        Sleep(1000 / dataFps); 
        int result = pGame->CheckGameEnd(); 
        if (result)
        {
            std::thread end(&UI::EndGame, this, result); 
            end.detach(); 
            break; 
        }
    }
}

void UI::RoleControl(int roleID)
{
    if (roleID == pGame->GetID1())
    {
        playerLay[0] = false; 
        while (programState == programstate::gaming || programState == programstate::gamePulsing)
        {
            while (programState == programstate::gamePulsing) Sleep(1000 / dataFps); 
            if (!pGame->GetRole(roleID)->IsLiving()) break; 
            if (GetKeyState('W') < 0) pGame->WalkUpOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('S') < 0) pGame->WalkDownOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('A') < 0) pGame->WalkLeftOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('D') < 0) pGame->WalkRightOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (playerLay[0]) pGame->LayTnt(pGame->GetID1()); 
            playerLay[0] = false; 
            Sleep(1000 / dataFps); 
        }
    }
    else if (roleID == pGame->GetID2())
    {
        playerLay[1] = false;
        while (programState == programstate::gaming || programState == programstate::gamePulsing)
        {
            while (programState == programstate::gamePulsing) Sleep(1000 / dataFps);
            if (!pGame->GetRole(roleID)->IsLiving()) break;
            if (GetKeyState(VK_UP) < 0) pGame->WalkUpOneCell(pGame->GetID2(), 1000 / dataFps);
            else if (GetKeyState(VK_DOWN) < 0) pGame->WalkDownOneCell(pGame->GetID2(), 1000 / dataFps);
            else if (GetKeyState(VK_LEFT) < 0) pGame->WalkLeftOneCell(pGame->GetID2(), 1000 / dataFps);
            else if (GetKeyState(VK_RIGHT) < 0) pGame->WalkRightOneCell(pGame->GetID2(), 1000 / dataFps);
            else if (playerLay[1]) pGame->LayTnt(pGame->GetID2()); 
            playerLay[1] = false;
            Sleep(1000 / dataFps);
        }
    }
}

void UI::RefreshScreen()
{
    while (programState == programstate::gaming || programState == programstate::gamePulsing)
    {
        InvalidateRect(m_hWnd, NULL, FALSE); 
        Sleep(1000 / paintFps); 
    }
}

void UI::EndGame(int result)
{
    programState = programstate::changeLevel; 

    std::function<void(void)> waitTask = [this]()
    {
        //�ȴ��첽����
        if (pScanDataTask)
        {
            pScanDataTask->wait(); delete pScanDataTask; pScanDataTask = nullptr;
        }
        if (pRefreshScreenTask)
        {
            pRefreshScreenTask->wait(); delete pRefreshScreenTask; pRefreshScreenTask = nullptr;
        }

        for (auto& pRoleControl : pRoleControlTasks)
        {
            if (pRoleControl)
            {
                pRoleControl->wait(); delete pRoleControl; pRoleControl = nullptr;
            }
        }
        Sleep(1000);
    }; 

    std::function<void(void)> beginGame = [this]()
    {
        programState = programstate::gaming;
        pScanDataTask = new std::future<void>(std::async(&UI::ScanData, this));
        pRefreshScreenTask = new std::future<void>(std::async(&UI::RefreshScreen, this));
        pRoleControlTasks[1] = new std::future<void>(std::async(&UI::RoleControl, this, pGame->GetID1()));    //��ɫ1
        if (startGameDlg.NumOfPlayer() == 2)
        {
            pRoleControlTasks[2] = new std::future<void>(std::async(&UI::RoleControl, this, pGame->GetID2()));    //��ɫ2
            int i = 3;
            for (int j = 1; j <= 4; ++j)
            {
                if (j == pGame->GetID1() || j == pGame->GetID2()) continue;
                pRoleControlTasks[i] = new std::future<void>(std::async(&UI::AI, this, j));
                ++i;
            }
        }
        else if (startGameDlg.NumOfPlayer() == 1)
        {
            int i = 2;
            for (int j = 1; j <= 4; ++j)
            {
                if (j == pGame->GetID1()) continue;
                pRoleControlTasks[i] = new std::future<void>(std::async(&UI::AI, this, j));
                ++i;
            }
        }
    }; 

    std::function<void(void)> endGame = [this]()
    {
        programState = programstate::starting; 

        delete pGame;
        pGame = nullptr;

        HMENU hMenu = GetMenu(m_hWnd);
        EnableMenuItem(hMenu, IDM_START, FALSE);
        EnableMenuItem(hMenu, IDM_RESTART, TRUE);
        EnableMenuItem(hMenu, IDM_PAUSE, TRUE);
        EnableMenuItem(hMenu, IDM_END, TRUE);
        InvalidateRect(m_hWnd, NULL, FALSE);
    }; 

    int totalScore = 0; 
    std::_tostringstream outStr; 
    if constexpr (std::is_same<std::_tostringstream, std::wostringstream>::value) outStr.imbue(std::locale("chs"));
    int nowLevel = pGame->GetNowLevel();

    if (result == 1)    //��һ�ʤ
    {
        SuccessSound(); 
        waitTask(); 
        pGame->InitNewLevel(nowLevel, true);
        totalScore += pGame->GetRole(pGame->GetID1())->GetTotalScore();
        if (pGame->GetNumOfPlayer() == 2) totalScore += pGame->GetRole(pGame->GetID2())->GetTotalScore();
        if (nowLevel == pGame->GetNumOfLevel() - 1)     //ͨ��
        {
            outStr << TEXT("��ϲͨ�أ��ܷ֣�") << totalScore << TEXT("\nCongratulations! Total score: ") << totalScore; 
            MessageBox(m_hWnd, outStr.str().c_str(), TEXT("Congratulations!"), MB_OK);
            newScore(pGame->GetNumOfPlayer(), pGame->GetDifficulty(), totalScore);
            endGame(); 
        }
        else
        {
            //������һ�أ�������future
            ++nowLevel; 
            outStr << TEXT("��Ӯ�ˣ�����������һ�ء���ǰ�ܷ֣�") << totalScore << TEXT("\nYou win! Total score: ") << totalScore;
            MessageBox(m_hWnd, outStr.str().c_str(), TEXT("You win!"), MB_OK); 
            pGame->InitNewLevel(nowLevel, true); 
            beginGame(); 
        }
    }
    else if (result == 2)  //���Ի�ʤ
    {
        FailSound(); 
        bool end = true; 
        if (pGame->GetRole(pGame->GetID1())->GetLife() > 0
            || pGame->GetNumOfPlayer() == 2 && pGame->GetRole(pGame->GetID2())->GetLife() > 0) end = false; 

        totalScore += pGame->GetRole(pGame->GetID1())->GetTotalScore();
        if (pGame->GetNumOfPlayer() == 2) totalScore += pGame->GetRole(pGame->GetID2())->GetTotalScore();

        if (end)        //���������
        {

            outStr << TEXT("��ʧ���ˣ����շ�����") << totalScore << TEXT("\nYou fail! Ultimate score: ") << totalScore; 
            MessageBox(m_hWnd, outStr.str().c_str(), TEXT("You fail!"), MB_OK); 
            newScore(pGame->GetNumOfPlayer(), pGame->GetDifficulty(), totalScore);
            waitTask(); 
            endGame(); 
        }
        else            //��û����
        {
            outStr << TEXT("�����ˣ�������¿�ʼ���ء���ǰ������") << totalScore << TEXT("\nYou lose! Click to restart this level. Total score: ") << totalScore;
            MessageBox(m_hWnd, outStr.str().c_str(), TEXT("You lose!"), MB_OK);
            waitTask();
            pGame->InitNewLevel(nowLevel, false); 
            beginGame(); 
        }
    }
    else if (result == 3)   //�������������Ϸ
    {
        FailSound(); 
        totalScore += pGame->GetRole(pGame->GetID1())->GetTotalScore();
        if (pGame->GetNumOfPlayer() == 2) totalScore += pGame->GetRole(pGame->GetID2())->GetTotalScore(); 
        outStr << TEXT("��Ϸ�������ܷ֣�") << totalScore << TEXT("\nGame over! Total score: ") << totalScore;
        MessageBox(m_hWnd, outStr.str().c_str(), TEXT("Game over!"), MB_OK);
        newScore(pGame->GetNumOfPlayer(), pGame->GetDifficulty(), totalScore); 
        waitTask();
        endGame(); 
    }
}

void UI::AI(int roleID)
{
    int rows, cols; 
    if (pGame) rows = pGame->GetGameMap(pGame->GetNowLevel()).size(), cols = pGame->GetGameMap(pGame->GetNowLevel())[0].size(); 
    else return; 

    int computerID1 = 0, computerID2 = 0, computerID3 = 0, computerID4 = 0; 
    for (int i = 1; i <= 4; ++i)
    {
        if (IsComputer(i))
        {
            if (computerID1 == 0) computerID1 = i;
            else if (computerID2 == 0) computerID2 = i;
            else if (computerID3 == 0) computerID3 = i; 
            else if (computerID4 == 0) computerID4 = i;
        }
    }
    
    std::vector<std::pair<std::pair<int, int>, bool>> laidMine;                                             //��¼�Լ����µĵ���
    std::mutex laidMineMutex;                                                                               //�������Ļ�����
    std::vector<std::thread*> laidMineThread; 

    //�ж��Ƿ�Խ��
    std::function<bool(int, int)> InRange = [rows, cols](int xc, int yc) { return xc >= 0 && yc >= 0 && xc < rows && yc < cols; }; 

    while (programState == programstate::gaming || programState == programstate::gamePulsing)               //��Ϸ��û����
    {
        while (programState == programstate::gamePulsing) Sleep(1000 / dataFps);                            //��Ϸ��ͣ״̬��ֹͣ
        
        if (!pGame->GetRole(roleID)->IsLiving()) break;                                                     //�����ˣ�ֹͣ

        //��ȡ��ɫλ��
        auto [x, y] = pGame->GetRole(roleID)->GetPos(); 
        int xc = pGame->PosToCell(x), yc = pGame->PosToCell(y); 
        
        //�����Χ��Σ��
        std::vector<std::vector<bool>> dangerous(rows, std::vector<bool>(cols, false));                     //��¼�Ƿ���Σ��
        std::vector<std::vector<bool>> obstacle(rows, std::vector<bool>(cols, false));                      //��¼�Ƿ����ϰ�
        std::initializer_list<int> xList{ -1, 1, 0, 0 }; 
        std::initializer_list<int> yList{ 0, 0, -1, 1 }; 
        if (roleID == computerID2)
        {
            xList = { 0, 0, -1, 1 }; 
            yList = { -1, 1, 0, 0 }; 
        }
        else if (roleID == computerID3)
        {
            xList = { 0, 0, 1, -1 };
            yList = { 1, -1, 0, 0 };
        }
        const std::vector<int> xMove(xList); 
        const std::vector<int> yMove(yList); 
        for (int tmpXc = 0; tmpXc < rows; ++tmpXc)
            for (int tmpYc = 0; tmpYc < cols; ++tmpYc)
            {
                auto mapObjList = pGame->GetMapObj(tmpXc, tmpYc);
                for (auto pObj : mapObjList)
                {
                    switch (pObj->GetObjType())
                    {
                    case obj_base::objType::tnt:                                                                //�����TNT
                    {

                        std::function<void(int, int)> bombTnt = [&dangerous, &obstacle, pObj, &InRange, &xMove, &yMove, this](int xc, int yc)
                        {
                            dangerous[xc][yc] = true;                                                           //���ΪΣ����
                            obstacle[xc][yc] = true;                                                            //���ܲ��ϸõ�
                            int distance = dynamic_cast<TNT*>(pObj)->GetDistance();
                            for (int i = 0; i < 4; ++i)                                                         //�����ܿ�̽
                            {
                                for (int j = 1; j < distance; ++j)
                                {
                                    int newXc = xc + j * xMove[i], newYc = yc + j * yMove[i];
                                    if (!InRange(newXc, newYc)) break;                                          //Խ��
                                    auto tmpMapObjList = pGame->GetMapObj(newXc, newYc);
                                    bool jumpOut = false;
                                    for (auto pTmpMapObj : tmpMapObjList)                                       //���ϰ�
                                        if (pTmpMapObj->GetObjType() == obj_base::objType::hardObstacle) { jumpOut = true; break; }
                                        else if (pTmpMapObj->GetObjType() == obj_base::objType::softObstacle)
                                        {
                                            dangerous[newXc][newYc] = true; jumpOut = true; break;
                                        }
                                    if (jumpOut) break;
                                    dangerous[newXc][newYc] = true;                                             //�˵�Σ��
                                }
                            }
                        }; 
                        if (!dynamic_cast<TNT*>(pObj)->IsMoving())                                              //��ֹ��ը��
                            bombTnt(tmpXc, tmpYc); 
                        else
                        {
                            int tmpXMove = 0, tmpYMove = 0; 
                            switch (dynamic_cast<TNT*>(pObj)->GetDirect())
                            {
                            case Prop::direction::Up: tmpXMove = -1; tmpYMove = 0; break; 
                            case Prop::direction::Down: tmpXMove = 1; tmpYMove = 0; break;
                            case Prop::direction::Left: tmpXMove = 0; tmpYMove = -1; break;
                            case Prop::direction::Right: tmpXMove = 0; tmpYMove = 1; break;
                            }
                            if (tmpXMove == 0 && tmpYMove == 0) break; 
                            for (int j = 1; ; ++j)
                            {
                                int nextXc = tmpXc + j * tmpXMove, nextYc = tmpYc + j * tmpYMove; 
                                bool hereBomb = false; 
                                if (!InRange(nextXc, nextYc)) hereBomb = true; 
                                if (!hereBomb)
                                {
                                    auto hereMapObjList = pGame->GetMapObj(nextXc, nextYc); 
                                    for (auto pHereMapObj : hereMapObjList)
                                    {
                                        switch (pHereMapObj->GetObjType())
                                        {
                                        case obj_base::objType::softObstacle: 
                                        case obj_base::objType::hardObstacle:
                                        case obj_base::objType::tnt:
                                            hereBomb = true; 
                                            break; 
                                        }
                                        if (hereBomb) break; 
                                    }
                                }
                                if (hereBomb)
                                {
                                    nextXc -= tmpXMove; nextYc -= tmpYMove; 
                                    if (InRange(nextXc, nextYc)) bombTnt(nextXc, nextYc); 
                                    break; 
                                }
                            }
                        }
                        break;
                    }
                    case obj_base::objType::bombArea:                                                       //���ڱ�ը��Σ��
                        dangerous[tmpXc][tmpYc] = obstacle[tmpXc][tmpYc] = true; 
                        break; 
                    case obj_base::objType::hardObstacle: case obj_base::objType::softObstacle:             //���ϰ�
                        obstacle[tmpXc][tmpYc] = true; 
                        break; 
                    case obj_base::objType::prop: 
                    {
                        Prop* pProp = dynamic_cast<Prop*>(pObj); 
                        if (!pProp->IsLaid()) break;                                                        //���Ƿ��õĵ��ߣ���Σ��
                        switch (pProp->GetPropType())
                        {
                        case Prop::propType::mine:                                                          //���׺ʹ�����˹������ɼ��Ҳ����Լ�������Բ��ܲ���
                            if (dynamic_cast<Mine*>(pProp)->GetOwnerID() != roleID && dynamic_cast<Mine*>(pProp)->Visible())
                                dangerous[tmpXc][tmpYc] = true, obstacle[tmpXc][tmpYc] = true;
                            break; 
                        case Prop::propType::lachrymator: 
                            if (dynamic_cast<Lachrymator*>(pProp)->GetOwnerID() != roleID && dynamic_cast<Lachrymator*>(pProp)->Visible())
                                dangerous[tmpXc][tmpYc] = true; 
                            break; 
                        case Prop::propType::missile:                                                       //����ը����ֱ��
                        {
                            dangerous[tmpXc][tmpYc] = obstacle[tmpXc][tmpYc] = true;
                            int direct = 0; 
                            switch (dynamic_cast<Missile*>(pProp)->GetDirection())
                            {
                            case obj_base::direction::Up: direct = 0; break; 
                            case obj_base::direction::Down: direct = 1; break; 
                            case obj_base::direction::Left: direct = 2; break;
                            case obj_base::direction::Right: direct = 3; break;
                            }
                            for (int i = 1;; ++i)
                            {
                                int dangerXc = tmpXc + i * xMove[direct], dangerYc = tmpYc + i * yMove[direct]; 
                                if (!InRange(dangerXc, dangerYc)) break; 
                                auto dangerMapObjList = pGame->GetMapObj(dangerXc, dangerYc); 
                                bool stop = false; 
                                for (auto pDangerMapObj : dangerMapObjList)
                                    if (pDangerMapObj->GetObjType() == obj_base::objType::hardObstacle) stop = true; 
                                    else if (pDangerMapObj->GetObjType() == obj_base::objType::softObstacle) { stop = true; dangerous[dangerXc][dangerYc] = true; }
                                if (stop) break; 
                                dangerous[dangerXc][dangerYc] = true; 
                            }
                            break; 
                        }
                        case Prop::propType::grenade:                                                       //���񵯵��յ�λ����Χ��ʮ�ֶ���Σ����
                        {
                            Grenade* pGrenade = dynamic_cast<Grenade*>(pProp); 
                            auto [tmpX, tmpY] = pGrenade->GetPos(); 
                            auto leftDistance = pGrenade->GetLeftDistance(); 
                            switch (pGrenade->GetDirection())
                            {
                            case Prop::direction::Up: tmpX -= leftDistance; break;  
                            case Prop::direction::Down: tmpX += leftDistance; break; 
                            case Prop::direction::Left: tmpY -= leftDistance; break; 
                            case Prop::direction::Right: tmpY += leftDistance; break; 
                            }
                            int tmpXc = pGame->PosToCell(tmpX), tmpYc = pGame->PosToCell(tmpY); 
                            if (InRange(tmpXc, tmpYc)) dangerous[tmpXc][tmpYc] = obstacle[tmpXc][tmpYc] = true; 
                            for (int i = 0; i < 4; ++i)
                            {
                                int nextXc = tmpXc + xMove[i], nextYc = tmpYc + yMove[i]; 
                                if (!InRange(nextXc, nextYc)) continue; 
                                dangerous[tmpXc][tmpYc] = obstacle[tmpXc][tmpYc] = true; 
                            }
                            break; 
                        }
                        }
                        break; 
                    }
                    }
                }
            }
        
        if (dangerous[xc][yc])                                                                              //��Σ�գ��Ͻ���
        {
            //���ȳ�����Σ�յ�·
            if (InRange(xc - 1, yc) && !dangerous[xc - 1][yc] && !obstacle[xc - 1][yc]) pGame->WalkUpOneCell(roleID, 1000 / dataFps);
            else if (InRange(xc + 1, yc) && !dangerous[xc + 1][yc] && !obstacle[xc + 1][yc]) pGame->WalkDownOneCell(roleID, 1000 / dataFps);
            else if (InRange(xc, yc - 1) && !dangerous[xc][yc - 1] && !obstacle[xc][yc - 1]) pGame->WalkLeftOneCell(roleID, 1000 / dataFps);
            else if (InRange(xc, yc + 1) && !dangerous[xc][yc + 1] && !obstacle[xc][yc + 1]) pGame->WalkRightOneCell(roleID, 1000 / dataFps);
            else                                                                                            //�޷��ҵ���Σ�յ�·�������������ʱ������Σ�յĵ�·
            {
                std::queue<std::pair<std::pair<int, int>, obj_base::direction>> q;                          //����Ѱ�ҳ�·����¼Ŀ���λ�ú͵�һ�������ķ���
                std::vector<std::vector<bool>> hasVisited(rows, std::vector<bool>(cols, false));            //��¼�Ƿ񱻷��ʹ�

                //��ʼ����
                if (InRange(xc - 1, yc) && !obstacle[xc - 1][yc])
                {
                    q.emplace(std::make_pair(xc - 1, yc), obj_base::direction::Up); hasVisited[xc - 1][yc] = true; 
                }
                if (InRange(xc + 1, yc) && !obstacle[xc + 1][yc])
                {
                    q.emplace(std::make_pair(xc + 1, yc), obj_base::direction::Down); hasVisited[xc + 1][yc] = true; 
                }
                if (InRange(xc, yc - 1) && !obstacle[xc][yc - 1]) 
                {
                    q.emplace(std::make_pair(xc, yc - 1), obj_base::direction::Left); hasVisited[xc][yc - 1] = true; 
                }
                if (InRange(xc, yc + 1) && !obstacle[xc][yc + 1])
                {
                    q.emplace(std::make_pair(xc, yc + 1), obj_base::direction::Right); hasVisited[xc][yc + 1] = true; 
                }
                while (!q.empty())
                {
                    auto [frontXc, frontYc] = q.front().first; 
                    auto direct = q.front().second; 
                    if (!dangerous[frontXc][frontYc]) break;                                                  //�ҵ��˰�ȫ�ĵط�������
                    q.pop(); 
                    for (int i = 0; i < 4; ++i)                                                               //�����ܱ���
                    {
                        int nextXc = frontXc + xMove[i], nextYc = frontYc + yMove[i]; 
                        if (InRange(nextXc, nextYc) && !hasVisited[nextXc][nextYc] && !obstacle[nextXc][nextYc])
                        {
                            q.emplace(std::make_pair(nextXc, nextYc), direct); 
                            hasVisited[nextXc][nextYc] = true; 
                        }
                    }
                }

                //������в��գ�˵���ҵ���·��
                if (!q.empty())
                {
                    switch (q.front().second)
                    {
                    case obj_base::direction::Up: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break; 
                    case obj_base::direction::Down: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                    case obj_base::direction::Left: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                    case obj_base::direction::Right: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                    }
                }
                else switch (pGame->GetRandom() % 4)                                                        //�޷����Σ�գ�����߶�
                {
                case 0: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break; 
                case 1: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                case 2: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                case 3: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break; 
                }
            }
        }
        else                                                                                                //û��Σ�գ���ʼ�����ж�
        {
            std::vector<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> unpickedProps; 
            std::vector<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> enemies;
            std::vector<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> softObstacles; 
            std::queue<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> q; 
            std::vector<std::vector<bool>> hasVisited(rows, std::vector<bool>(cols, false)); 

            //��¼�뱾��ĵ���
            auto localMapObjList = pGame->GetMapObj(xc, yc); 
            for (auto pLocalMapObj : localMapObjList)
                if (pLocalMapObj->GetObjType() == obj_base::objType::role && IsComputer(dynamic_cast<Role*>(pLocalMapObj)->GetID()))
                    enemies.emplace_back(std::make_pair(xc, yc), std::make_pair(obj_base::direction::Null, 0));
            
            //��ʼ����
            if (InRange(xc - 1, yc) && !dangerous[xc - 1][yc] && !obstacle[xc - 1][yc])
            {
                q.emplace(std::make_pair(xc - 1, yc), std::make_pair(obj_base::direction::Up, 1)); hasVisited[xc - 1][yc] = true;
            }
            if (InRange(xc + 1, yc) && !dangerous[xc + 1][yc] && !obstacle[xc + 1][yc])
            {
                q.emplace(std::make_pair(xc + 1, yc), std::make_pair(obj_base::direction::Down, 1)); hasVisited[xc + 1][yc] = true;
            }
            if (InRange(xc, yc - 1) && !dangerous[xc][yc - 1] && !obstacle[xc][yc - 1])
            {
                q.emplace(std::make_pair(xc, yc - 1), std::make_pair(obj_base::direction::Left, 1)); hasVisited[xc][yc - 1] = true;
            }
            if (InRange(xc, yc + 1) && !dangerous[xc][yc + 1] && !obstacle[xc][yc + 1])
            {
                q.emplace(std::make_pair(xc, yc + 1), std::make_pair(obj_base::direction::Right, 1)); hasVisited[xc][yc + 1] = true;
            }

            while (!q.empty())
            {
                auto [frontXc, frontYc] = q.front().first;
                auto direct = q.front().second.first; 
                int depth = q.front().second.second; 
                q.pop();
                auto mapObjList = pGame->GetMapObj(frontXc, frontYc);                                       //�鿴��һ�����Ʒ
                for (auto pMapObj : mapObjList)
                {
                    switch (pMapObj->GetObjType())
                    {
                    case obj_base::objType::role:                                                           //���������
                        if (IsComputer(dynamic_cast<Role*>(pMapObj)->GetID())) break;                       //������ǵ��ˣ�����ȥ��¼��
                        enemies.emplace_back(std::make_pair(frontXc, frontYc), std::make_pair(direct, depth)); 
                        break; 
                    case obj_base::objType::prop:                                                           //����
                        if (!dynamic_cast<Prop*>(pMapObj)->IsUnpicked()) break;                             //����δ����ĵ��ߣ�����ȥ��¼��
                        unpickedProps.emplace_back(std::make_pair(frontXc, frontYc), std::make_pair(direct, depth)); 
                        break; 
                    case obj_base::objType::softObstacle:                                                   //���ϰ���ֱ��¼��
                        softObstacles.emplace_back(std::make_pair(frontXc, frontYc), std::make_pair(direct, depth)); 
                        break; 
                    }
                }

                bool hasLaidMine = false;                                                                   //����Ƿ����Լ���صĵ���
                laidMineMutex.lock(); 
                for (int i = 0; i < (int)laidMine.size(); ++i)
                {
                    if (laidMine[i].second == false) continue;                                              //�Ѿ���ը��ϵĵ���
                    if (laidMine[i].first.first == frontXc && laidMine[i].first.second == frontYc)
                    {
                        hasLaidMine = true; break; 
                    }
                }
                laidMineMutex.unlock(); 

                if (!obstacle[frontXc][frontYc]) for (int i = 0; i < 4; ++i)                                //û���ϰ����Ϳ��������ܱ���
                {
                    int nextXc = frontXc + xMove[i], nextYc = frontYc + yMove[i]; 
                    if (InRange(nextXc, nextYc) && !hasVisited[nextXc][nextYc] && !dangerous[nextXc][nextYc])
                    {
                        bool isSoftObstacle = false; 
                        if (hasLaidMine)                                                                    //���������δ��ը�ĵ��ף���¼���Աߵ����ϰ�
                        {
                            for (auto pNextMapObj : pGame->GetMapObj(nextXc, nextYc))
                            {
                                if (pNextMapObj->GetObjType() == obj_base::objType::softObstacle) isSoftObstacle = true; 
                            }
                        }
                        if (!isSoftObstacle)
                        {
                            q.emplace(std::make_pair(nextXc, nextYc), std::make_pair(direct, depth + 1));
                            hasVisited[nextXc][nextYc] = true;
                        }
                    }
                }
            }

            //��ʱ�����������ж����������TNT�᲻��ɹ������������õ�
            std::function<obj_base::direction(int, int, int, int)> canRunAway = [this, &dangerous, &obstacle, &xMove, &yMove, &InRange, rows, cols, roleID](int roleXc, int roleYc, int tntXc, int tntYc)
            {
                std::vector<std::vector<bool>> avoid(rows, std::vector<bool>(cols, false));     //��¼�����η���ը����ը��������
                avoid[tntXc][tntYc] = true;
                int distance = pGame->GetRole(roleID)->GetDistance();
                for (int i = 0; i < 4; ++i)
                {
                    for (int j = 1; j < distance; ++j)
                    {
                        int tmpXc = tntXc + xMove[i] * j, tmpYc = tntYc + yMove[i] * j;
                        if (!InRange(tmpXc, tmpYc)) break;
                        auto mapObjList = pGame->GetMapObj(tmpXc, tmpYc);
                        bool stop = false;
                        for (auto pMapObjList : mapObjList)
                        {
                            switch (pMapObjList->GetObjType())
                            {
                            case obj_base::objType::softObstacle: case obj_base::objType::hardObstacle:
                                stop = true; break;
                            }
                            if (stop) break;
                        }
                        if (!stop) avoid[tmpXc][tmpYc] = true;
                    }
                }

                std::queue<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> q;              //����һ������ը�����Ƿ��г�·
                std::vector<std::vector<bool>> hasVisited(rows, std::vector<bool>(cols));
                if (InRange(roleXc - 1, roleYc) && !obstacle[roleXc - 1][roleYc] && !dangerous[roleXc - 1][roleYc])
                {
                    q.emplace(std::make_pair(roleXc - 1, roleYc), std::make_pair(obj_base::direction::Up, 1)); hasVisited[roleXc - 1][roleYc] = true;
                }
                if (InRange(roleXc + 1, roleYc) && !obstacle[roleXc + 1][roleYc] && !dangerous[roleXc + 1][roleYc])
                {
                    q.emplace(std::make_pair(roleXc + 1, roleYc), std::make_pair(obj_base::direction::Down, 1)); hasVisited[roleXc + 1][roleYc] = true;
                }
                if (InRange(roleXc, roleYc - 1) && !obstacle[roleXc][roleYc - 1] && !dangerous[roleXc][roleYc - 1])
                {
                    q.emplace(std::make_pair(roleXc, roleYc - 1), std::make_pair(obj_base::direction::Left, 1)); hasVisited[roleXc][roleYc - 1] = true;
                }
                if (InRange(roleXc, roleYc + 1) && !obstacle[roleXc][roleYc + 1] && !dangerous[roleXc][roleYc + 1])
                {
                    q.emplace(std::make_pair(roleXc, roleYc + 1), std::make_pair(obj_base::direction::Right, 1)); hasVisited[roleXc][roleYc + 1] = true;
                }

                while (!q.empty())
                {
                    auto [frontXc, frontYc] = q.front().first;
                    auto [direct, depth] = q.front().second;
                    q.pop();
                    if (!avoid[frontXc][frontYc])                                   //�ҵ�������·�ߣ��鿴�Լ��Ƿ���ʱ�䰲ȫ����
                    {
                        auto speed = pGame->GetRole(roleID)->GetMoveSpeed(); 
                        if (depth * Game::defPosUnitPerCell <= Game::tntInitialTime * speed * dataFps / 1000) return direct;   //������
                        else return obj_base::direction::Null;                                                                 //��ȫ�ش�̫Զ�ˣ���������
                    }
                    for (int i = 0; i < 4; ++i)
                    {
                        int nextXc = frontXc + xMove[i], nextYc = frontYc + yMove[i];
                        if (InRange(nextXc, nextYc) && !hasVisited[nextXc][nextYc] && !dangerous[nextXc][nextYc] && !obstacle[nextXc][nextYc])
                        {
                            q.emplace(std::make_pair(nextXc, nextYc), std::make_pair(direct, depth + 1)); hasVisited[nextXc][nextYc] = true;
                        }
                    }
                }

                return obj_base::direction::Null;                                   //û���ҵ�����·��
            };
            
            //�鿴��ǰ����
            SpecialBomb* pWeapon = pGame->GetRole(roleID)->GetWeapon(); 
            bool attack = false;
            if (pWeapon)
            {
                //���ȹ�������
                switch (pWeapon->GetPropType())
                {
                case Prop::propType::mine:                                  //�����е��׻������˹���ҵ��˲����Լ�ͬһ������Լ��л��ܣ��ҵ����ڲ�Զ���Ͱ���
                case Prop::propType::lachrymator:
                {
                    auto thisMapObjList = pGame->GetMapObj(xc, yc);
                    attack = true;
                    for (auto pThisMapObj : thisMapObjList)
                        if (pThisMapObj->GetObjType() == obj_base::objType::role && dynamic_cast<Role*>(pThisMapObj)->GetID() != roleID
                            && (!pGame->GetRole(roleID)->HaveShield() || IsComputer(dynamic_cast<Role*>(pThisMapObj)->GetID())))
                        {
                            attack = false; break;
                        }
                    if (attack)                                             //�������û��Σ��
                    {
                        if (!enemies.empty())                               //���û����ͨ���ˣ����˵��ף�����ͨ���˽��뿼��
                        {
                            if (enemies[0].second.second > 4 && enemies[0].second.second <= 7)   //���˲�Զ�������Ƚӽ�һ����˵����Զ���������
                            {
                                switch (enemies[0].second.first)
                                {
                                case obj_base::direction::Up: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Down: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Left: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Right: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Null: attack = false; break;
                                }
                                if (attack) break;                          //�Ѿ������ˣ�����������ֹ���������LayTnt
                            }
                        }
                    }

                    if (attack)                                             //���վ������õ���
                    {
                        pGame->LayTnt(roleID); 
                        if (pGame->GetRole(roleID)->GetWeapon() == nullptr)
                        {
                            int now = laidMine.size(); 
                            laidMineMutex.lock(); 
                            laidMine.emplace_back(std::make_pair(xc, yc), true); 
                            laidMineMutex.unlock(); 
                            laidMineThread.emplace_back(new std::thread
                            ([&laidMineMutex, &laidMine](int i)
                                {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(Game::mineInitialTime));
                                    laidMineMutex.lock(); laidMine[i].second = false; laidMineMutex.unlock();
                                }, now
                            )); 
                        }
                    }
                    break;
                }
                case Prop::propType::fire:                                  //����ǹ���жϵ����Ƿ���·��
                {
                    int xMove = 0, yMove = 0;
                    switch (pGame->GetRole(roleID)->GetDirect())            //��ȡ��ɫ��Եķ���
                    {
                    case obj_base::direction::Up: xMove = -1; break;
                    case obj_base::direction::Down: xMove = 1; break;
                    case obj_base::direction::Left: yMove = -1; break;
                    case obj_base::direction::Right: yMove = 1; break;
                    }
                    for (int i = 1; i <= Game::fireMaxDistance; ++i)        //�ڻ���ǹ��ը��Χ�����ҵ�����
                    {
                        int nowX = xc + xMove * i, nowY = yc + yMove * i;
                        if (!InRange(nowX, nowY)) break;
                        auto nextMapObjList = pGame->GetMapObj(nowX, nowY);
                        for (auto pNextMapObj : nextMapObjList)
                            if (pNextMapObj->GetObjType() == obj_base::objType::role && IsPlayer(dynamic_cast<Role*>(pNextMapObj)->GetID()))
                            {
                                attack = true; break;
                            }
                    }
                    if (attack) pGame->LayTnt(roleID);                      //��ը
                    else if (!enemies.empty())
                    {
                        for (int i = 0; i < (int)enemies.size(); ++i)       //���˲��գ�ը����
                        {
                            if (enemies[i].second.second > 2 * Game::fireMaxDistance) break;       //����̫Զ
                            if (enemies[i].first.first == xc)               //��ͬ�ŵ��ˣ�����Ƿ�׷��������м�û���ϰ���׷��
                            {
                                attack = true;
                                if (enemies[i].first.second > yc)
                                {
                                    for (int nextYc = yc + 1; nextYc <= enemies[i].first.second - Game::fireMaxDistance; ++nextYc)
                                        if (dangerous[xc][nextYc] || obstacle[xc][nextYc]) { attack = false; break; }
                                }
                                else
                                {
                                    for (int nextYc = yc - 1; nextYc >= enemies[i].first.second + Game::fireMaxDistance; --nextYc)
                                        if (dangerous[xc][nextYc] || obstacle[xc][nextYc]) { attack = false; break; }
                                }
                            }
                            else if (enemies[i].first.second == yc)
                            {
                                attack = true;
                                if (enemies[i].first.first > xc)
                                {
                                    for (int nextXc = xc + 1; nextXc <= enemies[i].first.first - Game::fireMaxDistance; ++nextXc)
                                        if (dangerous[nextXc][yc] || obstacle[nextXc][yc]) { attack = false; break; }
                                }
                                else
                                {
                                    for (int nextXc = xc - 1; nextXc >= enemies[i].first.first + Game::fireMaxDistance; --nextXc)
                                        if (dangerous[nextXc][yc] || obstacle[nextXc][yc]) { attack = false; break; }
                                }
                            }
                            if (attack) switch (enemies[i].second.first)            //��ͬ�ŵ��ˣ�׷��
                            {
                            case obj_base::direction::Up: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Down: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Left: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Right: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Null: attack = false; break;
                            }
                        }
                    }
                    break;
                }
                case Prop::propType::grenade:
                {
                    int destXc = xc, destYc = yc;
                    switch (pGame->GetRole(roleID)->GetDirect())                    //��ȡ��ɫ��Եķ���
                    {
                    case obj_base::direction::Up: destXc -= 2; break;
                    case obj_base::direction::Down: destXc += 2; break;
                    case obj_base::direction::Left: destYc -= 2; break;
                    case obj_base::direction::Right: destYc += 2; break;
                    }
                    if (InRange(destXc, destYc))
                    {
                        auto destMapObjList = pGame->GetMapObj(destXc, destYc);     //�������񵯱�ըλ�����޵���
                        for (auto pDestMapObj : destMapObjList)
                            if (pDestMapObj->GetObjType() == obj_base::objType::role && IsPlayer(dynamic_cast<Role*>(pDestMapObj)->GetID()))
                            {
                                attack = true; break;
                            }
                        if (!attack) for (int i = 0; i < 4; ++i)                    //�������񵯱�ը�ķ�Χ��û�е���
                        {
                            int aroundXc = destXc + xMove[i], aroundYc = destYc + yMove[i];
                            if (!InRange(aroundXc, aroundYc)) continue;
                            auto aroundMapObjList = pGame->GetMapObj(aroundXc, aroundYc);
                            for (auto pAroundMapObj : aroundMapObjList)
                                if (pAroundMapObj->GetObjType() == obj_base::objType::role && IsPlayer(dynamic_cast<Role*>(pAroundMapObj)->GetID()))
                                {
                                    attack = true; break;
                                }
                            if (attack) break;
                        }
                        if (attack) { pGame->LayTnt(roleID); Sleep(1000 / dataFps); } //����е��ˣ�ը��
                        else
                        {
                            for (int i = 0; i < 4; ++i)                             //û�е��ˣ���Ѱ����
                            {
                                int destXc = xc + xMove[i], destYc = yc + yMove[i];
                                if (!InRange(destXc, destYc)) continue; 
                                if (dangerous[destXc][destYc]) continue;            //���Ǳ��߻���Σ��
                                if (obstacle[destXc][destYc]) { destXc += xMove[i], destYc += yMove[i]; }
                                else { destXc += 2 * xMove[i], destYc += 2 * yMove[i]; }
                                if (!InRange(destXc, destYc)) continue; 
                                bool stop = false; 
                                for (auto pMapObj : pGame->GetMapObj(destXc, destYc))
                                {
                                    if (pMapObj->GetObjType() == obj_base::objType::role
                                        && IsPlayer(dynamic_cast<Role*>(pMapObj)->GetID()))
                                    {
                                        attack = true; break; 
                                    }
                                    else if (pMapObj->GetObjType() == obj_base::objType::hardObstacle)
                                    {
                                        stop = true; break; 
                                    }
                                }
                                if (stop) continue; 
                                if (!attack)                                        //��������ܲ���
                                {
                                    for (int j = 0; j < 4; ++j)
                                    {
                                        int aroundXc = destXc + xMove[j], aroundYc = destYc + yMove[j]; 
                                        if (!InRange(aroundXc, aroundYc)) continue; 
                                        for (auto pMapObj : pGame->GetMapObj(aroundXc, aroundYc))
                                        {
                                            if (pMapObj->GetObjType() == obj_base::objType::role
                                                && IsPlayer(dynamic_cast<Role*>(pMapObj)->GetID()))
                                            {
                                                attack = true; break; 
                                            }
                                        }
                                        if (attack) break; 
                                    }
                                }
                                if (attack)                                         //�������ը����
                                {
                                    if (xMove[i] == -1 && yMove[i] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 1 && yMove[i] == 0) pGame->WalkDownOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 0 && yMove[i] == -1) pGame->WalkLeftOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 0 && yMove[i] == 1) pGame->WalkRightOneCell(roleID, 1000 / dataFps); 
                                    pGame->LayTnt(roleID); Sleep(1000 / dataFps); 
                                }
                            }
                        }
                    }
                    break;
                }
                case Prop::propType::missile:                                       //�ǵ���
                {
                    int tmpXMove = 0, tmpYMove = 0;
                    switch (pGame->GetRole(roleID)->GetDirect())                    //�����һ����û�е���
                    {
                    case obj_base::direction::Up: tmpXMove = -1; break;
                    case obj_base::direction::Down: tmpXMove = 1; break;
                    case obj_base::direction::Left: tmpYMove = -1; break;
                    case obj_base::direction::Right: tmpYMove = 1; break;
                    }
                    int nextXc = xc + tmpXMove, nextYc = yc + tmpYMove;
                    if (InRange(nextXc, nextYc))
                    {
                        auto nextMapObjList = pGame->GetMapObj(nextXc, nextYc);
                        for (auto pNextMapObj : nextMapObjList)
                        {
                            bool stop = false;
                            switch (pNextMapObj->GetObjType())
                            {
                            case obj_base::objType::hardObstacle:                   //�����ϰ���û��������ɫ��������
                            case obj_base::objType::softObstacle:
                                stop = true; break;
                            case obj_base::objType::role:                           //������ɫ������
                                attack = true; stop = true; break;
                            }
                            if (stop) break;
                        }
                        if (attack) pGame->LayTnt(roleID);
                    }
                    break;
                }
                }
            }
            else if (pGame->GetRole(roleID)->GetLeftTntNum() > 0)                   //����û�е��ߣ�ֻ����ͨը��
            {
                bool canLayTnt = true; 
                auto mapObjList = pGame->GetMapObj(xc, yc); 
                for (auto pMapObj : mapObjList)
                {
                    switch (pMapObj->GetObjType())
                    {
                    case obj_base::objType::role: 
                        if (IsPlayer(dynamic_cast<Role*>(pMapObj)->GetID())) attack = true; 
                        break; 
                    case obj_base::objType::prop: 
                        switch (dynamic_cast<Prop*>(pMapObj)->GetPropType())
                        {
                        case Prop::propType::mine: case Prop::propType::lachrymator: 
                            if (dynamic_cast<Prop*>(pMapObj)->IsLaid()) canLayTnt = false; 
                            break; 
                        }
                        break; 
                    }
                    if (!canLayTnt) break; 
                }
                if (canLayTnt && !attack)
                {
                    int distance = pGame->GetRole(roleID)->GetDistance(); 
                    for (int i = 0; i < 4; ++i)
                    {
                        for (int j = 1; j < distance; ++j)
                        {
                            int tmpXc = xc + xMove[i] * j, tmpYc = yc + yMove[i] * j; 
                            if (!InRange(tmpXc, tmpYc)) break; 
                            auto tmpMapObjList = pGame->GetMapObj(tmpXc, tmpYc); 
                            bool stop = false; 
                            for (auto pTmpMapObj : tmpMapObjList)
                            {
                                switch (pTmpMapObj->GetObjType())
                                {
                                case obj_base::objType::softObstacle: case obj_base::objType::hardObstacle: 
                                    stop = true; 
                                    break; 
                                case obj_base::objType::role: 
                                    if (IsPlayer(dynamic_cast<Role*>(pTmpMapObj)->GetID())) attack = true; 
                                    break; 
                                }
                            }
                            if (stop || attack) break; 
                        }
                        if (attack) break; 
                    }
                }
                else if (!canLayTnt) attack = false; 

                

                if (attack)                                                             //������ֵ���
                {
                    obj_base::direction runAwayDirect = canRunAway(xc, yc, xc, yc); 
                    switch (runAwayDirect)
                    {
                    case obj_base::direction::Up: pGame->LayTnt(roleID); pGame->WalkUpOneCell(roleID, 1000 / dataFps); break; 
                    case obj_base::direction::Down: pGame->LayTnt(roleID); pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                    case obj_base::direction::Left: pGame->LayTnt(roleID); pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                    case obj_base::direction::Right: pGame->LayTnt(roleID); pGame->WalkRightOneCell(roleID, 1000 / dataFps); break; 
                    default: attack = false; break; 
                    }
                }
            }

            if (!attack)                                                            //���û�й�������
            {
                if (pWeapon)                                                        //����е���
                {
                    switch (pWeapon->GetPropType())
                    {
                    case Prop::propType::mine: 
                    case Prop::propType::lachrymator: 
                        break; 
                    case Prop::propType::missile: 
                    case Prop::propType::fire:                                      //�ȿ��Ա���û�����ϰ�
                    {
                        for (int i = 0; i < 4; ++i)                                 //�Ѳ�����
                        {
                            int nearXc = xc + xMove[i], nearYc = yc + yMove[i];
                            if (!InRange(nearXc, nearYc)) continue;
                            bool stop = false; 
                            for (auto pMapObj : pGame->GetMapObj(nearXc, nearYc))
                            {
                                if (pMapObj->GetObjType() == obj_base::objType::softObstacle)
                                {
                                    attack = true; break;
                                }
                                else if (pMapObj->GetObjType() == obj_base::objType::hardObstacle)
                                {
                                    stop = true; break; 
                                }
                                else if (pMapObj->GetObjType() == obj_base::objType::role && IsComputer(dynamic_cast<Role*>(pMapObj)->GetID()))
                                {
                                    stop = true; break; 
                                }
                            }
                            if (stop) { attack = false; continue; }

                            //�Ѳ����߼����
                            if (dangerous[nearXc][nearYc]) { attack = false; continue; }
                            for (int j = 2; j <= Game::fireMaxDistance; ++j)
                            {
                                int nextXc = xc + xMove[i] * j, nextYc = yc + yMove[i] * j;
                                if (!InRange(nextXc, nextYc)) break;
                                bool computerExists = false;
                                for (auto pMapObj : pGame->GetMapObj(nextXc, nextYc))
                                {
                                    if (pMapObj->GetObjType() == obj_base::objType::softObstacle)
                                    {
                                        attack = true; break;
                                    }
                                    else if (pMapObj->GetObjType() == obj_base::objType::hardObstacle)
                                    {
                                        stop = true; break;
                                    }
                                    else if (pMapObj->GetObjType() == obj_base::objType::role && IsComputer(dynamic_cast<Role*>(pMapObj)->GetID()))
                                    {
                                        computerExists = true; break;
                                    }
                                }
                                if (computerExists) { attack = false; break; }
                                if (stop || attack) break;
                            }
                            
                            if (attack)
                            {
                                if (xMove[i] == -1 && yMove[i] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps); 
                                else if (xMove[i] == 1 && yMove[i] == 0) pGame->WalkDownOneCell(roleID, 1000 / dataFps);
                                else if (xMove[i] == 0 && yMove[i] == -1) pGame->WalkLeftOneCell(roleID, 1000 / dataFps);
                                else if (xMove[i] == 0 && yMove[i] == 1) pGame->WalkRightOneCell(roleID, 1000 / dataFps);
                                pGame->LayTnt(roleID); 
                                break; 
                            }
                        }
                        break;
                    }
                    case Prop::propType::grenade: 
                    {

                        //�ж��Ƿ���ը�����ϰ�
                        std::function<bool(int, int)> canLayGrenadeToBombSoftArea = [this, &dangerous, &obstacle, &hasVisited, &InRange, &xMove, &yMove](int destXc, int destYc)
                        {
                            bool attack = false; 
                            bool stop = false; 
                            if (!InRange(destXc, destYc)) return attack; 
                            for (auto pMapObj : pGame->GetMapObj(destXc, destYc))   //������񵯱�ըλ����û�����ϰ�
                            {
                                switch (pMapObj->GetObjType())
                                {
                                case obj_base::objType::hardObstacle:
                                    stop = true; break;
                                case obj_base::objType::role:                       //��������
                                    if (IsComputer(dynamic_cast<Role*>(pMapObj)->GetID())) stop = true; break;
                                case obj_base::objType::softObstacle:
                                    if (hasVisited[destXc][destYc]) attack = true;
                                    break;
                                }
                                if (stop) { attack = false; break; }
                            }
                            if (!stop)
                            {
                                for (int i = 0; i < 4; ++i)                         //�������񵯱�ը�ķ�Χ��û�����ϰ�
                                {
                                    int aroundXc = destXc + xMove[i], aroundYc = destYc + yMove[i];
                                    if (!InRange(aroundXc, aroundYc)) continue;
                                    auto aroundMapObjList = pGame->GetMapObj(aroundXc, aroundYc);
                                    for (auto pAroundMapObj : aroundMapObjList)
                                        if (pAroundMapObj->GetObjType() == obj_base::objType::role && IsComputer(dynamic_cast<Role*>(pAroundMapObj)->GetID()))
                                        {
                                            stop = true; break;
                                        }
                                        else if (pAroundMapObj->GetObjType() == obj_base::objType::softObstacle) { if (hasVisited[aroundXc][aroundYc]) attack = true; }
                                    if (stop) { attack = false; break; }
                                }
                            }
                            return attack; 
                        }; 

                        int destXc = xc, destYc = yc; 
                        switch (pGame->GetRole(roleID)->GetDirect())                //�Ȳ鿴���Ե�λ����û�����ϰ�
                        {
                        case obj_base::direction::Up: destXc -= 2; break; 
                        case obj_base::direction::Down: destXc += 2; break; 
                        case obj_base::direction::Left: destYc -= 2; break; 
                        case obj_base::direction::Right: destYc += 2; break; 
                        }
                        if (InRange(destXc, destYc))
                        {
                            attack = canLayGrenadeToBombSoftArea(destXc, destYc); 
                            if (attack) { pGame->LayTnt(roleID); Sleep(1000 / dataFps); }
                        }
                        if (!attack)                                       //ûִ�й������鿴������û�����ϰ�
                        {
                            for (int i = 0; i < 4; ++i)
                            {
                                int destXc = xc + xMove[i], destYc = yc + yMove[i];
                                if (!InRange(destXc, destYc) || dangerous[destXc][destYc]) continue;
                                if (obstacle[destXc][destYc]) { destXc += xMove[i], destYc += yMove[i]; }
                                else { destXc += 2 * xMove[i], destYc += 2 * yMove[i]; }
                                if (!InRange(destXc, destYc)) continue;
                                attack = canLayGrenadeToBombSoftArea(destXc, destYc); 
                                if (attack)                                         //�������ը���ϰ�
                                {
                                    if (xMove[i] == -1 && yMove[i] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 1 && yMove[i] == 0) pGame->WalkDownOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 0 && yMove[i] == -1) pGame->WalkLeftOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 0 && yMove[i] == 1) pGame->WalkRightOneCell(roleID, 1000 / dataFps);
                                    pGame->LayTnt(roleID); Sleep(1000 / dataFps);
                                    break; 
                                }
                            }
                        }
                        break; 
                    }
                    }
                }
            }
            
            if (!attack && pGame->GetDifficulty() == Game::Difficulty::difficult)
            {
                for (int i = 0; i < (int)unpickedProps.size(); ++i)          //�����
                {
                    auto [propXc, propYc] = unpickedProps.front().first; 
                    auto [direct, depth] = unpickedProps.front().second; 
                    if (depth >= 7) break;                              //̫Զ�ĵ��߲���

                    bool find = false; 

                    for (auto pMapObj : pGame->GetMapObj(propXc, propYc))
                    {
                        if (pMapObj->GetObjType() == obj_base::objType::prop)
                        {
                            Prop* pProp = dynamic_cast<Prop*>(pMapObj); 
                            if (pProp->IsUnpicked())
                            {
                                if (pWeapon && pProp->GetPropType() >= Prop::propType::specialbombbegin
                                    && pProp->GetPropType() <= Prop::propType::specialbombend) break;       //�Ѿ��������ˣ����ټ�������
                                find = true; 
                            }
                        }
                    }

                    if (!find) continue; 

                    switch (direct)
                    {
                    case Prop::direction::Up: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break; 
                    case Prop::direction::Down: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                    case Prop::direction::Left: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                    case Prop::direction::Right: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                    }
                    attack = true; 
                    break; 
                }
            }

            if (!attack)                                                //�Ҹ��������ϰ�ը
            {
                if (!pWeapon && pGame->GetRole(roleID)->GetLeftTntNum() > 0) //û��������ߣ���TNT��������û�б�ը��Χ�ڵ����ϰ�
                {

                    bool haveLaidMine = false;                          //�����Ƿ���δ��ը�ĵ���
                    laidMineMutex.lock(); 
                    for (int i = 0; i < (int)laidMine.size(); ++i)
                    {
                        if (laidMine[i].second == false) continue; 
                        if (laidMine[i].first.first == xc && laidMine[i].first.second == yc)
                        {
                            haveLaidMine = true; break; 
                        }
                    }
                    laidMineMutex.unlock(); 

                    if (!haveLaidMine)                                      //�޵��׺ʹ�����˹�����Է���
                    {
                        int distance = pGame->GetRole(roleID)->GetDistance();     //��ȡ�Լ��ų�ը���ķ�Χ
                        for (int i = 0; i < 4; ++i)                         //�鿴����
                        {
                            for (int j = 1; j < distance; ++j)
                            {
                                int bombXc = xc + xMove[i] * j, bombYc = yc + yMove[i] * j;
                                int stop = false;
                                if (!InRange(bombXc, bombYc)) break;
                                for (auto pMapObj : pGame->GetMapObj(bombXc, bombYc))
                                {
                                    switch (pMapObj->GetObjType())
                                    {
                                    case obj_base::objType::softObstacle: attack = true; break;
                                    case obj_base::objType::hardObstacle: stop = true; break;
                                    }
                                    if (attack || stop) break;
                                }
                                if (attack || stop) break;
                            }
                        }

                        bool prevDangerous = dangerous[xc][yc], prevObstacle = obstacle[xc][yc];
                        dangerous[xc][yc] = obstacle[xc][yc] = true;

                        for (int i = 1; i <= 4; ++i)            //�Ҷ��ѣ��᲻����������
                        {
                            if (IsPlayer(i) || i == roleID) continue;
                            auto [teamMateX, teamMateY] = pGame->GetRole(i)->GetPos();
                            int teamMateXc = pGame->PosToCell(teamMateX), teamMateYc = pGame->PosToCell(teamMateY);
                            if (canRunAway(teamMateXc, teamMateYc, xc, yc) == obj_base::direction::Null)  //���������ѣ����ܰ���
                            {
                                attack = false; break;
                            }
                        }

                        dangerous[xc][yc] = prevDangerous; obstacle[xc][yc] = prevObstacle;

                        if (attack)
                        {
                            switch (canRunAway(xc, yc, xc, yc))
                            {
                            case obj_base::direction::Up: pGame->LayTnt(roleID); pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Down: pGame->LayTnt(roleID); pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Left: pGame->LayTnt(roleID); pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Right: pGame->LayTnt(roleID); pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                            default: attack = false; break;
                            }
                        }
                    }
                }

                if (!attack)
                {
                    //��������õĲ��ǵ��׻������˹������
                    if (!(pWeapon && (pWeapon->GetPropType() == Prop::propType::mine 
                        || pWeapon->GetPropType() == Prop::propType::lachrymator 
                        || pWeapon->GetPropType() == Prop::propType::grenade)))
                    {
                        if (!softObstacles.empty())             //ȥ�����ϰ�
                        {
                            attack = true;
                            for (int i = 0; i < (int)softObstacles.size(); ++i)
                            {
                                auto [obXc, obYc] = softObstacles[i].first; 
                                bool isBeside = false; 
                                for (int j = 0; j < 4; ++j)
                                {
                                    if (xc + xMove[j] == obXc && yc + yMove[j] == obYc)
                                    {
                                        isBeside = true; 
                                        break; 
                                    }
                                }

                                if (isBeside) continue; 

                                switch (softObstacles[i].second.first)
                                {
                                case obj_base::direction::Up: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Down: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Left: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Right: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                                default: attack = false; break;
                                }

                                if (attack) break; 
                            }
                        }
                    }
                }

                if (!attack)                                    //������¿���
                {
                    if (!pWeapon && pGame->GetRole(roleID)->GetLeftTntNum() > 0)
                    {
                        attack = true; 

                        bool prevDangerous = dangerous[xc][yc], prevObstacle = obstacle[xc][yc]; 
                        dangerous[xc][yc] = obstacle[xc][yc] = true; 
                        
                        for (int i = 1; i <= 4; ++i)            //�Ҷ��ѣ��᲻����������
                        {
                            if (IsPlayer(i) || i == roleID) continue;
                            auto [teamMateX, teamMateY] = pGame->GetRole(i)->GetPos();
                            int teamMateXc = pGame->PosToCell(teamMateX), teamMateYc = pGame->PosToCell(teamMateY);
                            if (canRunAway(teamMateXc, teamMateYc, xc, yc) == obj_base::direction::Null)  //���������ѣ����ܰ���
                            {
                                attack = false; break;
                            }
                        }

                        dangerous[xc][yc] = prevDangerous; obstacle[xc][yc] = prevObstacle; 

                        if (attack) switch (canRunAway(xc, yc, xc, yc))
                        {
                        case obj_base::direction::Up: pGame->LayTnt(roleID); pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Down: pGame->LayTnt(roleID); pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Left: pGame->LayTnt(roleID); pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Right: pGame->LayTnt(roleID); pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                        default: attack = false; break;
                        }
                    }

                    if (!attack)                            //�޷�����TNT�������һ������
                    {
                        int directInt = pGame->GetRandom() % 4; 
                        int destXc = xc + xMove[directInt], destYc = yc + yMove[directInt]; 
                        if (InRange(destXc, destYc) && !dangerous[destXc][destYc] && !obstacle[destXc][destYc])
                        {
                            if (xMove[directInt] == -1 && yMove[directInt] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps); 
                            else if (xMove[directInt] == 1 && yMove[directInt] == 0) pGame->WalkDownOneCell(roleID, 1000 / dataFps);
                            else if (xMove[directInt] == 0 && yMove[directInt] == -1) pGame->WalkLeftOneCell(roleID, 1000 / dataFps);
                            else if (xMove[directInt] == 0 && yMove[directInt] == 1) pGame->WalkRightOneCell(roleID, 1000 / dataFps);
                            attack = true; 
                        }
                    }
                }
            }
        }
    }

    for (auto& pLaidMineThread : laidMineThread)
    {
        if (pLaidMineThread)
        {
            if (pLaidMineThread->joinable()) pLaidMineThread->join(); 
            delete pLaidMineThread; pLaidMineThread = nullptr; 
        }
    }

}

std::vector<std::pair<std::_tstring, int>> UI::ReadHighScore() const
{
    WIN32_FIND_DATA wfd; 
    HANDLE hFile; 
    std::vector<std::pair<std::_tstring, int>> res;
    std::_tstring name;
    int score;
    std::_tifstream fin; 
    if constexpr (std::is_same<std::_tifstream, std::wifstream>::value) fin.imbue(std::locale("chs"));
    if ((hFile = FindFirstFile(HIGH_SCORE_DIRECTORY, &wfd)) == INVALID_HANDLE_VALUE)
    {
        CreateDirectory(HIGH_SCORE_DIRECTORY, NULL);    //�ļ��в�����
        goto endRead; 
    }

    //���ļ��������ļ���
    if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        if (MessageBox(m_hWnd, TEXT("�ļ���data������ȷ���Ƿ���ɾ����\nIt is a wrong that the file \"") HIGH_SCORE_DIRECTORY TEXT("\" exists. Delete it?"),
            c_lpszError, MB_YESNO | MB_ICONERROR) == IDYES)
        {
            DeleteFile(HIGH_SCORE_DIRECTORY); 
            CreateDirectory(HIGH_SCORE_DIRECTORY, NULL);
        }
        goto endRead;
    }
    
    //�ļ�������
    if (FindFirstFile(HIGH_SCORE_PATH, &wfd) == INVALID_HANDLE_VALUE) goto endRead;
    fin.open(HIGH_SCORE_PATH, std::ios::in); 
    if (!fin) goto endRead;     //��ȡʧ��

    //��ȡ
    for (int i = 0; i < 6; ++i)
    {
        std::getline(fin, name); 
        fin >> score; 
        while (fin.get() != TEXT('\n') && fin); 
        if (!fin) { fin.close(); goto endRead; }   //��ȡʧ��
        res.emplace_back(name, score); 
    }
    fin.close(); 

    endRead: 

    if (res.size() != 6 )          //û�д浵
    {
        res.clear(); 
        for (int i = 0; i < 6; ++i)
            res.emplace_back(TEXT("δ����"), 0);
    }

    return res; 
}

void UI::SaveHighScore(const std::vector<std::pair<std::_tstring, int>>& scoreList) const
{
    WIN32_FIND_DATA wfd;
    HANDLE hFile;

    std::function<void(void)> saveError = [this]()
    {
        MessageBox(m_hWnd, TEXT("д��浵����"), c_lpszError, MB_ICONERROR | MB_OK);
    }; 

    if (scoreList.size() != 6)
    {
        saveError(); return; 
    }
    if ((hFile = FindFirstFile(HIGH_SCORE_DIRECTORY, &wfd)) == INVALID_HANDLE_VALUE)
    {
        CreateDirectory(HIGH_SCORE_DIRECTORY, NULL);    //�ļ��в�����
    }

    //���ļ��������ļ���
    if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        if (MessageBox(m_hWnd, TEXT("�ļ���data������ȷ���Ƿ���ɾ����\nIt is a wrong that the file \"") HIGH_SCORE_DIRECTORY TEXT("\" exists. Delete it?"),
            c_lpszError, MB_YESNO | MB_ICONERROR) == IDYES)
        {
            DeleteFile(HIGH_SCORE_DIRECTORY);
            CreateDirectory(HIGH_SCORE_DIRECTORY, NULL);
        }
        else 
        {
            saveError(); return;
        }
    }

    std::_tofstream fout(HIGH_SCORE_PATH, std::ios::out); 
    if constexpr (std::is_same<std::wofstream, std::_tofstream>::value) fout.imbue(std::locale("chs")); 
    if (!fout)
    {
        saveError(); return;
    }
    for (int i = 0; i < 6; ++i)
        fout << scoreList[i].first << std::endl << scoreList[i].second << std::endl; 
    fout.close(); 
}

void UI::newScore(int numOfPlayer, Game::Difficulty difficulty, int score)
{
    auto highScoreInfo = ReadHighScore(); 
    int pos = 0; 
    switch (difficulty)
    {
    case Game::Difficulty::easy: pos = 0 + (numOfPlayer - 1) * 3; break;
    case Game::Difficulty::medium: pos = 1 + (numOfPlayer - 1) * 3; break;
    case Game::Difficulty::difficult: pos = 2 + (numOfPlayer - 1) * 3; break;
    }
    if (highScoreInfo[pos].second < score)
    {
        inputName.Begin(m_hInst, m_hWnd); 
        highScoreInfo[pos] = std::make_pair(inputName.GetName(), score); 
        SaveHighScore(highScoreInfo); 
    }
}

void UI::Paint(HWND hWnd, const BOOL calledByPaintMessage)
{
    HDC hdc = NULL; 
    PAINTSTRUCT ps = { 0 }; 

    if (calledByPaintMessage)
        hdc = BeginPaint(hWnd, &ps);
    else hdc = GetDC(hWnd); 

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBmMemOld = (HBITMAP)SelectObject(hdcMem, hBmMem);

    HDC hdcObj = CreateCompatibleDC(hdc);
    HBITMAP hBmBkgndOld = (HBITMAP)SelectObject(hdcObj, hBmBkgnd);

    switch (programState)
    {
    case programstate::starting: 
    {
        SelectObject(hdcMem, hBmMain); 
        break; 
    }
    case programstate::gaming: 
    case programstate::gamePulsing: 
    case programstate::changeLevel: 
    {
        BitBlt(hdcMem, 0, 0, objSize * 15, objSize * 13, hdcObj, 0, 0, SRCCOPY); 
        if (pGame == nullptr) break; 
        int rows = pGame->GetGameMap(pGame->GetNowLevel()).size(), cols = pGame->GetGameMap(pGame->GetNowLevel())[0].size();
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                auto mapObjList = pGame->GetMapObj(i, j);
                for (auto pMapObj : mapObjList)
                {
                    auto [x, y] = pMapObj->GetPos();
                    int xp = PosToPaint(x), yp = PosToPaint(y);
                    switch (pMapObj->GetObjType())
                    {
                    case obj_base::objType::role:
                    {
                        Role* pRole = dynamic_cast<Role*>(pMapObj);
                        int yGrid = 0;
                        switch (pRole->GetDirect())
                        {
                        case obj_base::direction::Down: yGrid = 0; break;
                        case obj_base::direction::Up: yGrid = 1; break;
                        case obj_base::direction::Left: yGrid = 2; break;
                        case obj_base::direction::Right: yGrid = 3; break;
                        }
                        SelectObject(hdcObj, hBmRole);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, objSize * (pRole->GetID() - 1), yGrid * objSize, SRCCOPY);
                        break;
                    }
                    case obj_base::objType::tnt:
                    {
                        SelectObject(hdcObj, hBmTnt);
                        BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                        break;
                    }
                    case obj_base::objType::softObstacle:
                    {
                        int xGrid = 0;
                        if (dynamic_cast<SoftObstacle*>(pMapObj)->GetStyle()) xGrid = 1;
                        else xGrid = 0;
                        SelectObject(hdcObj, hBmObstacle);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, 0, xGrid * objSize, SRCCOPY);
                        break;
                    }
                    case obj_base::objType::hardObstacle:
                    {
                        int xGrid = 0;
                        if (dynamic_cast<HardObstacle*>(pMapObj)->GetStyle()) xGrid = 1;
                        else xGrid = 0;
                        SelectObject(hdcObj, hBmObstacle);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, objSize, xGrid * objSize, SRCCOPY);
                        break;
                    }
                    case obj_base::objType::bombArea:
                    {
                        BombArea* pBombArea = dynamic_cast<BombArea*>(pMapObj);
                        switch (pBombArea->GetBomb())
                        {
                        case Prop::propType::null:
                            SelectObject(hdcObj, hBmTnt);
                            BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            break;
                        case Prop::propType::fire:
                            SelectObject(hdcObj, hBmFire);
                            BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            break;
                        case Prop::propType::ice:
                            SelectObject(hdcObj, hBmIce);
                            BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            break;
                        case Prop::propType::grenade:
                            SelectObject(hdcObj, hBmGrenade);
                            BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize * 2, 0, SRCCOPY);
                            break;
                        }
                        break;
                    }
                    case obj_base::objType::prop:
                    {
                        Prop* pProp = dynamic_cast<Prop*>(pMapObj);
                        switch (pProp->GetPropType())
                        {
                        case Prop::propType::glove: SelectObject(hdcObj, hBmGlove); goto paintUnpickedProp;
                        case Prop::propType::shield: SelectObject(hdcObj, hBmShield); goto paintUnpickedProp;
                        case Prop::propType::addtnt: SelectObject(hdcObj, hBmAddTnt); goto paintUnpickedProp;
                        case Prop::propType::addlife: SelectObject(hdcObj, hBmAddLife); goto paintUnpickedProp;
                        case Prop::propType::shoe: SelectObject(hdcObj, hBmShoe); goto paintUnpickedProp;
                        case Prop::propType::jinKeLa: SelectObject(hdcObj, hBmJinKeLa); goto paintUnpickedProp;
                        case Prop::propType::fire: SelectObject(hdcObj, hBmFire); goto paintUnpickedProp;
                        case Prop::propType::ice: SelectObject(hdcObj, hBmIce); goto paintUnpickedProp;
                        paintUnpickedProp:
                            if (pProp->IsUnpicked())
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                            break;
                        case Prop::propType::mine:
                            if (pProp->IsUnpicked())
                            {
                                SelectObject(hdcObj, hBmMine);
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                            }
                            else if (pProp->IsLaid() && dynamic_cast<Mine*>(pMapObj)->Visible())
                            {
                                SelectObject(hdcObj, hBmMine);
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            }
                            break;
                        case Prop::propType::lachrymator:
                            if (pProp->IsUnpicked())
                            {
                                SelectObject(hdcObj, hBmLachrymator);
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                            }
                            else if (pProp->IsLaid() && dynamic_cast<Lachrymator*>(pMapObj)->Visible())
                            {
                                SelectObject(hdcObj, hBmLachrymator);
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            }
                            break;
                        case Prop::propType::grenade: SelectObject(hdcObj, hBmGrenade); goto printFlyingProp;
                        case Prop::propType::missile: SelectObject(hdcObj, hBmMissile); goto printFlyingProp;
                        printFlyingProp:
                            if (pProp->IsUnpicked())
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                            else if (pProp->IsLaid())
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            break;
                        }
                        break;
                    }
                    }
                }
            }
        }

        SelectObject(hdcObj, hBmObstacle); 
        for (int i = cols; i * objSize < mainWndSize.x; ++i)
            for (int j = 0; j < rows; ++j)
                BitBlt(hdcMem, i * objSize, j * objSize, objSize, objSize, hdcObj, objSize, 0, SRCCOPY); 
        
        int fontHeight = 40; 

        //��ӡ����
        HFONT hF = CreateFont
        (
            fontHeight,
            0,
            0,
            0,
            FW_HEAVY,
            0,
            0,
            0,
            GB2312_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH,
            TEXT("����")
        ); 
        HFONT hFOld = (HFONT)SelectObject(hdcMem, hF); 
        SetTextColor(hdcMem, RGB(255, 255, 255)); 
        SetBkColor(hdcMem, RGB(16, 11, 26)); 
        int nowLevel = pGame->GetNowLevel() + 1; 
        std::_tostringstream levelStr; 
        if constexpr (std::is_same<std::_tostringstream, std::wostringstream>::value) levelStr.imbue(std::locale("chs"));
        levelStr << TEXT("�� ") << nowLevel << TEXT(" ��"); 
        RECT rc = { 15 * objSize, 0, mainWndSize.x, mainWndSize.y };
        DrawText(hdcMem, levelStr.str().c_str(), levelStr.str().length(), &rc, DT_CENTER);

        //��ӡ��ɫ��Ϣ
        for (int i = 0; i < 4; ++i)
        {
            const Role* pRole = pGame->GetRole(i + 1); 
            POINT rolePaintPos = { 15 * objSize, fontHeight + i * (mainWndSize.y - fontHeight) / 4 };
            SelectObject(hdcObj, hBmRole); 
            BitBlt(hdcMem, rolePaintPos.x, rolePaintPos.y,
                objSize, objSize, hdcObj, i * objSize, 4 * objSize, SRCCOPY); 
            //ӵ������
            SpecialBomb* pWeapon = pRole->GetWeapon();
            if (pWeapon)
            {
                switch (pWeapon->GetPropType())
                {
                case Prop::propType::lachrymator: SelectObject(hdcObj, hBmLachrymator); goto paintWeapon;
                case Prop::propType::mine: SelectObject(hdcObj, hBmMine); goto paintWeapon;
                case Prop::propType::fire: SelectObject(hdcObj, hBmFire); goto paintWeapon;
                case Prop::propType::ice: SelectObject(hdcObj, hBmIce); goto paintWeapon;
                case Prop::propType::missile: SelectObject(hdcObj, hBmMissile); goto paintWeapon;
                case Prop::propType::grenade: SelectObject(hdcObj, hBmGrenade); goto paintWeapon;
                paintWeapon: BitBlt(hdcMem, rolePaintPos.x + 1 * objSize + (objSize - propSize) / 2, rolePaintPos.y + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                }
            }
            //����
            if (pRole->CanPushTnt())
            {
                SelectObject(hdcObj, hBmGlove); 
                BitBlt(hdcMem, rolePaintPos.x + 2 * objSize + (objSize - propSize) / 2, rolePaintPos.y + (objSize - propSize) / 2, 
                propSize, propSize, hdcObj, 0, 0, SRCCOPY); 
            }
            //����
            if (pRole->HaveShield())
            {
                SelectObject(hdcObj, hBmShield);
                BitBlt(hdcMem, rolePaintPos.x + 3 * objSize + (objSize - propSize) / 2, rolePaintPos.y + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, 0, 0, SRCCOPY);
            }
            
            hF = CreateFont
            (
                fontHeight / 2,
                0,
                0,
                0,
                FW_HEAVY,
                0,
                0,
                0,
                GB2312_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH,
                TEXT("����")
            );

            DeleteObject(SelectObject(hdcMem, hF)); 

            //������
            std::_tostringstream lifeStr;

            if constexpr (std::is_same<std::_tostringstream, std::wostringstream>::value) lifeStr.imbue(std::locale("chs"));
            
            lifeStr << TEXT("������") << pRole->GetLife(); 
            rc.left = rolePaintPos.x + objSize; rc.right = mainWndSize.x; rc.top = rolePaintPos.y + objSize; rc.bottom = rolePaintPos.y + objSize * 2; 
            DrawText(hdcMem, lifeStr.str().c_str(), lifeStr.str().length(), &rc, DT_LEFT); 
            //����
            std::_tostringstream scoreStr;
            if constexpr (std::is_same<std::_tostringstream, std::wostringstream>::value) scoreStr.imbue(std::locale("chs"));
            scoreStr << TEXT("������") << pRole->GetNowScore(); 
            rc.left = rolePaintPos.x + objSize; rc.right = mainWndSize.x; rc.top = rolePaintPos.y + 2 * objSize; rc.bottom = rolePaintPos.y + objSize * 3;
            DrawText(hdcMem, scoreStr.str().c_str(), scoreStr.str().length(), &rc, DT_LEFT);
        }
        SelectObject(hdcMem, hFOld);
        DeleteObject(hF);
        break; 
    }
    }

    SelectObject(hdcObj, hBmBkgndOld);
    DeleteDC(hdcObj);

    BitBlt(hdc, 0, 0, mainWndSize.x, mainWndSize.y, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hBmMemOld);
    DeleteDC(hdcMem);

    if (calledByPaintMessage)
        EndPaint(hWnd, &ps);
    else ReleaseDC(hWnd, hdc); 
}

UI::~UI()
{
    if(hBmMem) DeleteObject(hBmMem); 
    
    programState = programstate::starting;

    //�ȴ��첽����
    if (pScanDataTask)
    {
        delete pScanDataTask; pScanDataTask = nullptr;
    }
    if (pRefreshScreenTask)
    {
        delete pRefreshScreenTask; pRefreshScreenTask = nullptr;
    }

    for (auto& pRoleControl : pRoleControlTasks)
    {
        if (pRoleControl)
        {
            delete pRoleControl; pRoleControl = nullptr;
        }
    }

    if (pGame)
    {
        Sleep(800);
        delete pGame;
        pGame = nullptr;
    }
}

bool UI::StartGameDlg::Begin(HINSTANCE hInstance, HWND hWndParent)
{
    Init(hInstance, MAKEINTRESOURCE(STARTGAME_DIALOG), hWndParent); 
    return choose; 
}

void UI::StartGameDlg::MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG: 
        DisableButton(IDC_PLAYER2_RED); 
        DisableButton(IDC_PLAYER2_YELLOW);
        DisableButton(IDC_PLAYER2_GREEN);
        DisableButton(IDC_PLAYER2_BLUE);
        choose = false; 
        break; 
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_SINGLE:
            if (IsButtonCheck(IDC_SINGLE))
            {
                DisableButton(IDC_PLAYER2_RED); DisableButton(IDC_PLAYER2_YELLOW);
                DisableButton(IDC_PLAYER2_GREEN); DisableButton(IDC_PLAYER2_BLUE);
            }
            break;
        case IDC_DOUBLE:
            if (IsButtonCheck(IDC_DOUBLE))
            {
                EnableButton(IDC_PLAYER2_RED); EnableButton(IDC_PLAYER2_YELLOW);
                EnableButton(IDC_PLAYER2_GREEN); EnableButton(IDC_PLAYER2_BLUE);
            }
            break;
        case IDC_PLAYER1_RED: SetUnChecked(IDC_PLAYER2_RED); break; 
        case IDC_PLAYER1_YELLOW: SetUnChecked(IDC_PLAYER2_YELLOW); break;
        case IDC_PLAYER1_GREEN: SetUnChecked(IDC_PLAYER2_GREEN); break;
        case IDC_PLAYER1_BLUE: SetUnChecked(IDC_PLAYER2_BLUE); break;
        case IDC_PLAYER2_RED: SetUnChecked(IDC_PLAYER1_RED); break;
        case IDC_PLAYER2_YELLOW: SetUnChecked(IDC_PLAYER1_YELLOW); break;
        case IDC_PLAYER2_GREEN: SetUnChecked(IDC_PLAYER1_GREEN); break;
        case IDC_PLAYER2_BLUE: SetUnChecked(IDC_PLAYER1_BLUE); break;
        case IDCANCEL: choose = false; EndDialog(hDlg, 0); break; 
        case IDOK: 
            if (IsButtonCheck(IDC_EASY)) difficulty = Difficulty::easy;
            else if (IsButtonCheck(IDC_MEDIUM)) difficulty = Difficulty::medium;
            else if (IsButtonCheck(IDC_DIFFICULT)) difficulty = Difficulty::difficult;
            else { MessageBox(hDlg, TEXT("��ѡ���Ѷȣ�\nPlease choose difficulty!"), TEXT("Warning"), MB_OK | MB_ICONWARNING); break; }
            if (IsButtonCheck(IDC_SINGLE)) numOfPlayer = 1;
            else if (IsButtonCheck(IDC_DOUBLE)) numOfPlayer = 2; 
            else { MessageBox(hDlg, TEXT("��ѡ����Ϸ������\nPlease choose the number of players"), TEXT("Warning"), MB_OK | MB_ICONWARNING); break; }

            if (IsButtonCheck(IDC_PLAYER1_RED)) player1ID = 1;
            else if (IsButtonCheck(IDC_PLAYER1_YELLOW)) player1ID = 2;
            else if (IsButtonCheck(IDC_PLAYER1_GREEN)) player1ID = 3;
            else if (IsButtonCheck(IDC_PLAYER1_BLUE)) player1ID = 4;
            else { MessageBox(hDlg, TEXT("��ѡ��Player1��\nPlease choose Player1"), TEXT("Warning"), MB_OK | MB_ICONWARNING); break; }

            if (numOfPlayer == 2)
            {
                if (IsButtonCheck(IDC_PLAYER2_RED)) player2ID = 1;
                else if (IsButtonCheck(IDC_PLAYER2_YELLOW)) player2ID = 2;
                else if (IsButtonCheck(IDC_PLAYER2_GREEN)) player2ID = 3;
                else if (IsButtonCheck(IDC_PLAYER2_BLUE)) player2ID = 4;
                else { MessageBox(hDlg, TEXT("��ѡ��Player2��\nPlease choose Player2"), TEXT("Warning"), MB_OK | MB_ICONWARNING); break; }
            }
             
            choose = true; 
            EndDialog(hDlg, 0); 
            break;
        }
        break; 
    }
}

void UI::InputNameDlg::Begin(HINSTANCE hInstance, HWND hWndParent)
{
    Init(hInstance, MAKEINTRESOURCE(INPUTNAME_DIALOG), hWndParent); 
}

void UI::InputNameDlg::MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND: 
        switch (LOWORD(wParam))
        {
        case IDOK: 
        {
            TCHAR nameBuf[51]; 
            GetDlgItemText(hDlg, IDC_NAMEEDIT, nameBuf, 50); 
            name = nameBuf; 
            if (name == TEXT(""))
            {
                MessageBox(hDlg, TEXT("���ֲ���Ϊ�գ�"), TEXT("Null name"), MB_OK | MB_ICONWARNING); 
                break; 
            }
            EndDialog(hDlg, 0);
            break;
        }
        }
        break; 
    }
}
