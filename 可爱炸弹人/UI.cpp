#include "UI.h"

const int UI::objSize = 40;
const int UI::propSize = 30;
const POINT UI::mainWndPos = { 0, 0 };
const POINT UI::mainWndSize = { (objSize * 15) + 200, objSize * 13 }; 
const int UI::dataFps = 50; 
const int UI::paintFps = 50;

int UI::Begin(HINSTANCE hInstance, int nCmdShow)
{

    //����λͼ
    LoadGameImg();

    //���崰����ʽ
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = c_lpszWndClassName;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    capMenuAppendCy = GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYCAPTION); 

    Init(hInstance, nCmdShow, 0, 0, mainWndSize.x, mainWndSize.y + capMenuAppendCy, 
        WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX, c_lpszWndTitle, wcex);

    MSG msg;

    programState = programstate::gaming;
    pGame = new Game(2, 1, 2);
    pGame->InitNewLevel(0, true);
    std::thread thr1(&UI::ScanData, this); 
    std::thread thr2(&UI::RoleControl, this, 1);
    std::thread thr3(&UI::RefreshScreen, this); 
    std::thread thr4(&UI::RoleControl, this, 2);
    std::thread thr5(&UI::AI, this, 3); 
    std::thread thr6(&UI::AI, this, 4);
    thr1.detach(); thr2.detach(); 
    thr3.detach(); thr4.detach(); 
    thr5.detach(); thr6.detach();

    // ����Ϣѭ��:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
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
        SetTimer(hWnd, TIMER_ID_START, 500, NULL);
        SetTimer(hWnd, 500, 20, NULL);
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
    case WM_TIMER:
        if (wParam == TIMER_ID_START)
        {
            InvalidateRect(hWnd, NULL, TRUE);
            KillTimer(hWnd, TIMER_ID_START);
        }
        else
        {
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break; 
    case WM_DESTROY: 
        if (hBmMem)
        {
            DeleteObject(hBmMem); 
            hBmMem = NULL; 
        }
        KillTimer(hWnd, 500); 
        PostQuitMessage(0); 
        break; 
    default: 
        return false; 
    }
    return true; 
}

bool UI::LoadGameImg()
{
    hBmBkgnd = (HBITMAP)LoadImage(m_hInst, TEXT("image\\bkgnd.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmRole = (HBITMAP)LoadImage(m_hInst, TEXT("image\\role.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmTnt = (HBITMAP)LoadImage(m_hInst, TEXT("image\\tnt.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
    hBmObstacle = (HBITMAP)LoadImage(m_hInst, TEXT("image\\obstacle.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
    hBmGlove = (HBITMAP)LoadImage(m_hInst, TEXT("image\\glove.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmShield = (HBITMAP)LoadImage(m_hInst, TEXT("image\\shield.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAddTnt = (HBITMAP)LoadImage(m_hInst, TEXT("image\\addtnt.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAddLife = (HBITMAP)LoadImage(m_hInst, TEXT("image\\addLife.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmShoe = (HBITMAP)LoadImage(m_hInst, TEXT("image\\shoe.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmJinKeLa = (HBITMAP)LoadImage(m_hInst, TEXT("image\\jinkela.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLachrymator = (HBITMAP)LoadImage(m_hInst, TEXT("image\\lachrymator.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMine = (HBITMAP)LoadImage(m_hInst, TEXT("image\\mine.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmFire = (HBITMAP)LoadImage(m_hInst, TEXT("image\\fire.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmIce = (HBITMAP)LoadImage(m_hInst, TEXT("image\\ice.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmGrenade = (HBITMAP)LoadImage(m_hInst, TEXT("image\\grenade.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMissile = (HBITMAP)LoadImage(m_hInst, TEXT("image\\missil.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (!hBmBkgnd || !hBmRole || !hBmTnt || !hBmObstacle || !hBmGlove || !hBmShield | !hBmAddTnt || !hBmAddLife || !hBmShoe 
        || !hBmJinKeLa || !hBmLachrymator || !hBmMine || !hBmFire || !hBmIce || !hBmGrenade || !hBmMissile)
    {
        MessageBox(m_hWnd, TEXT("����ͼƬʧ�ܣ�"), c_lpszError, MB_OK | MB_ICONERROR);
        return false; 
    }

    GetObject(hBmBkgnd, sizeof(BITMAP), &bmBkgnd); 
    GetObject(hBmRole, sizeof(BITMAP), &bmRole); 
    GetObject(hBmTnt, sizeof(BITMAP), &bmTnt); 
    GetObject(hBmObstacle, sizeof(BITMAP), &bmObstacle); 
    GetObject(hBmGlove, sizeof(BITMAP), &bmGlove);
    GetObject(hBmShield, sizeof(BITMAP), &bmShield);
    GetObject(hBmAddTnt, sizeof(BITMAP), &bmAddTnt);
    GetObject(hBmAddLife, sizeof(BITMAP), &bmAddLife);
    GetObject(hBmShoe, sizeof(BITMAP), &bmShoe);
    GetObject(hBmJinKeLa, sizeof(BITMAP), &bmJinKeLa);
    GetObject(hBmLachrymator, sizeof(BITMAP), &bmLachrymator);
    GetObject(hBmMine, sizeof(BITMAP), &bmMine);
    GetObject(hBmFire, sizeof(BITMAP), &bmFire);
    GetObject(hBmIce, sizeof(BITMAP), &bmIce);
    GetObject(hBmGrenade, sizeof(BITMAP), &bmGrenade);
    GetObject(hBmMissile, sizeof(BITMAP), &bmMissile);
	return true; 
}

void UI::CreateBuffer(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);
    hBmMem = CreateCompatibleBitmap(hdc, mainWndSize.x, mainWndSize.y); 
    ReleaseDC(hWnd, hdc); 
}

void UI::ScanData()
{
    while (programState == programstate::gaming)
    {
        pGame->CheckRole(); 
        pGame->CheckBomb(1000 / dataFps); 
        Sleep(1000 / dataFps); 
        if (pGame->CheckGameEnd()) EndGame(); 
    }
}

void UI::RoleControl(int player)
{
    if (player == 1)
    {
        playerLay[0] = false; 
        while (programState == programstate::gaming)
        {
            if (GetKeyState('W') < 0) pGame->WalkUpOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('S') < 0) pGame->WalkDownOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('A') < 0) pGame->WalkLeftOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('D') < 0) pGame->WalkRightOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (playerLay[0]) pGame->LayTnt(pGame->GetID1()); 
            playerLay[0] = false; 
            Sleep(1000 / dataFps); 
        }
    }
    else if (player == 2)
    {
        playerLay[1] = false;
        while (programState == programstate::gaming)
        {
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
    while (programState == programstate::gaming)
    {
        InvalidateRect(m_hWnd, NULL, FALSE); 
        Sleep(1000 / paintFps); 
    }
}

void UI::EndGame()
{
    programState = programstate::starting; 
    MessageBox(m_hWnd, TEXT("��Ϸ����"), NULL, MB_OK); 
    Sleep(800); 
    delete pGame; 
    pGame = nullptr; 
}

void UI::AI(int roleID)
{
    int rows, cols; 
    if (pGame) rows = pGame->GetGameMap(pGame->GetNowLevel()).size(), cols = pGame->GetGameMap(pGame->GetNowLevel())[0].size(); 
    else return; 

    //�ж��Ƿ�Խ��
    std::function<bool(int, int)> InRange = [rows, cols](int xc, int yc) { return xc >= 0 && yc >= 0 && xc < rows && yc < cols; }; 

    while (programState == programstate::gaming || programState == programstate::gamePulsing)               //��Ϸ��û����
    {
        while (programState == programstate::gamePulsing) Sleep(dataFps);                                   //��Ϸ��ͣ״̬��ֹͣ
        if (!(programState == programstate::gaming || programState == programstate::gamePulsing)) break;    //��Ϸ�����ˣ�ֹͣ
        
        if (!pGame->GetRole(roleID)->IsLiving()) break;                                                     //�����ˣ�ֹͣ

        //��ȡ��ɫλ��
        auto [x, y] = pGame->GetRole(roleID)->GetPos(); 
        int xc = pGame->PosToCell(x), yc = pGame->PosToCell(y); 
        
        //�����Χ��Σ��
        std::vector<std::vector<bool>> dangerous(rows, std::vector<bool>(cols, false));                     //��¼�Ƿ���Σ��
        std::vector<std::vector<bool>> obstacle(rows, std::vector<bool>(cols, false));                      //��¼�Ƿ����ϰ�
        const std::vector<int> xMove{ -1, 1, 0, 0 }; 
        const std::vector<int> yMove{ 0, 0, -1, 1 }; 
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
            if (InRange(xc - 1, yc) && !dangerous[xc - 1][yc])
            {
                q.emplace(std::make_pair(xc - 1, yc), std::make_pair(obj_base::direction::Up, 1)); hasVisited[xc - 1][yc] = true;
            }
            if (InRange(xc + 1, yc) && !dangerous[xc + 1][yc])
            {
                q.emplace(std::make_pair(xc + 1, yc), std::make_pair(obj_base::direction::Down, 1)); hasVisited[xc + 1][yc] = true;
            }
            if (InRange(xc, yc - 1) && !dangerous[xc][yc - 1])
            {
                q.emplace(std::make_pair(xc, yc - 1), std::make_pair(obj_base::direction::Left, 1)); hasVisited[xc][yc - 1] = true;
            }
            if (InRange(xc, yc + 1) && !dangerous[xc][yc + 1])
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
                if (!obstacle[frontXc][frontYc]) for (int i = 0; i < 4; ++i)                                //û���ϰ����Ϳ��������ܱ���
                {
                    int nextXc = frontXc + xMove[i], nextYc = frontYc + yMove[i]; 
                    if (InRange(nextXc, nextYc) && !hasVisited[nextXc][nextYc] && !dangerous[nextXc][nextYc])
                    {
                        q.emplace(std::make_pair(nextXc, nextYc), std::make_pair(direct, depth + 1));
                        hasVisited[nextXc][nextYc] = true; 
                    }
                }
            }

            //��ʱ�����������ж����������TNT�᲻��ɹ������������õ�
            std::function<obj_base::direction(void)> canRunAway = [this, &dangerous, &obstacle, &xMove, &yMove, &InRange, xc, yc, rows, cols, roleID]()
            {
                std::vector<std::vector<bool>> avoid(rows, std::vector<bool>(cols, false));     //��¼�����η���ը����ը��������
                avoid[xc][yc] = true;
                int distance = pGame->GetRole(roleID)->GetDistance();
                for (int i = 0; i < 4; ++i)
                {
                    for (int j = 1; j < distance; ++j)
                    {
                        int tmpXc = xc + xMove[i] * j, tmpYc = yc + yMove[i] * j;
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
                if (InRange(xc - 1, yc) && !obstacle[xc - 1][yc] && !dangerous[xc - 1][yc])
                {
                    q.emplace(std::make_pair(xc - 1, yc), std::make_pair(obj_base::direction::Up, 1)); hasVisited[xc - 1][yc] = true;
                }
                if (InRange(xc + 1, yc) && !obstacle[xc + 1][yc] && !dangerous[xc + 1][yc])
                {
                    q.emplace(std::make_pair(xc + 1, yc), std::make_pair(obj_base::direction::Down, 1)); hasVisited[xc + 1][yc] = true;
                }
                if (InRange(xc, yc - 1) && !obstacle[xc][yc - 1] && !dangerous[xc][yc - 1])
                {
                    q.emplace(std::make_pair(xc, yc - 1), std::make_pair(obj_base::direction::Left, 1)); hasVisited[xc][yc - 1] = true;
                }
                if (InRange(xc, yc + 1) && !obstacle[xc][yc + 1] && !dangerous[xc][yc + 1])
                {
                    q.emplace(std::make_pair(xc, yc + 1), std::make_pair(obj_base::direction::Right, 1)); hasVisited[xc][yc + 1] = true;
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

                    if (attack) pGame->LayTnt(roleID);                      //���վ������õ���
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
                        if (attack) pGame->LayTnt(roleID);                          //����е��ˣ�ը��
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
                                    pGame->LayTnt(roleID); 
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
                    obj_base::direction runAwayDirect = canRunAway(); 
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
                            int nearXc = xc + xMove[i], nearYc = yc + xMove[i];
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
                            if (!attack)                                            //�Ѳ����߼����
                            {
                                if (dangerous[nearXc][nearYc]) continue; 
                                for (int j = 2; j <= Game::fireMaxDistance; ++j)
                                {
                                    int nextXc = xc + xMove[i] * j, nextYc = xc + xMove[i] * j; 
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
                                        else if (pAroundMapObj->GetObjType() == obj_base::objType::softObstacle) { if (hasVisited[destXc][destYc]) attack = true; }
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
                            if (attack) pGame->LayTnt(roleID); 
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
                                    pGame->LayTnt(roleID);
                                    break; 
                                }
                            }
                        }
                        break; 
                    }
                    }
                }
            }
            
            if (!attack)
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
                    int distance = pGame->GetRole(roleID)->GetDistance();     //��ȡ�Լ��ų�ը���ķ�Χ
                    bool computerExists = false; 
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
                                case obj_base::objType::role: 
                                    if (IsComputer(dynamic_cast<Role*>(pMapObj)->GetID())) computerExists = true; 
                                    break; 
                                case obj_base::objType::softObstacle: attack = true; break; 
                                case obj_base::objType::hardObstacle: stop = true; break;
                                }
                                if (computerExists || attack || stop) break; 
                            }
                            if (computerExists || attack || stop) break;
                        }
                        if (computerExists) break;
                    }

                    if (computerExists) attack = false; 
                    else if (attack)
                    {
                        switch (canRunAway())
                        {
                        case obj_base::direction::Up: pGame->LayTnt(roleID); pGame->WalkUpOneCell(roleID, 1000 / dataFps); break; 
                        case obj_base::direction::Down: pGame->LayTnt(roleID); pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Left: pGame->LayTnt(roleID); pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Right: pGame->LayTnt(roleID); pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                        default: attack = false; break; 
                        }
                    }

                }

                if (!attack)
                {
                    //��������õĲ��ǵ��׻������˹
                    if (!(pWeapon && (pWeapon->GetPropType() == Prop::propType::mine || pWeapon->GetPropType() == Prop::propType::lachrymator)))
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
                        switch (canRunAway())
                        {
                        case obj_base::direction::Up: pGame->LayTnt(roleID); pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Down: pGame->LayTnt(roleID); pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Left: pGame->LayTnt(roleID); pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Right: pGame->LayTnt(roleID); pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                        default: attack = false; break;
                        }
                    }

                    if (!attack)                            //�޷���ֹTNT�������һ������
                    {
                        int directInt = pGame->GetRandom() % 4; 
                        int destXc = xc + xMove[directInt], destYc = yc + yMove[directInt]; 
                        if (InRange(destXc, destYc) && !dangerous[destXc][destYc] && !obstacle[destXc][destYc])
                        {
                            if (xMove[directInt] == -1 && yMove[directInt] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps); 
                            else if (xMove[directInt] == -1 && yMove[directInt] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps);
                            else if (xMove[directInt] == -1 && yMove[directInt] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps);
                            else if (xMove[directInt] == -1 && yMove[directInt] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps);
                            attack = true; 
                        }
                    }
                }
            }
        }
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
    case programstate::gaming: 
    case programstate::gamePulsing: 
    {
        BitBlt(hdcMem, 0, 0, objSize * 15, objSize * 13, hdcObj, 0, 0, SRCCOPY); 
        if (pGame == nullptr) break; 
        int rows = pGame->GetGameMap(pGame->GetNowLevel()).size(), cols = pGame->GetGameMap(pGame->GetNowLevel())[0].size();
        for (int i = 0; i < rows; ++i)
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
                        SelectObject(hdcObj, hBmRole);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, objSize * (pRole->GetID() - 1), 0, SRCCOPY);
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
                        SelectObject(hdcObj, hBmObstacle);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, 0, 0, SRCCOPY);
                        break;
                    }
                    case obj_base::objType::hardObstacle:
                    {
                        SelectObject(hdcObj, hBmObstacle);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, objSize, 0, SRCCOPY);
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
    if (pGame)
    {
        programState = programstate::starting;
        Sleep(800);
        delete pGame;
        pGame = nullptr;
    }
}
