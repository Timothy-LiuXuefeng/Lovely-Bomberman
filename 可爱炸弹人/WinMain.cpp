///////////////////////////////////
//
//  �������ļ�
//

#include "framework.h"
#include "resource.h"
#include "str.h"
#include "globals.h"
#include "MsgProc.h"

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    

    if (!MyRegisterClass(hInstance)) 
    {
        MessageBox(NULL, TEXT("Register failed!"), c_lpszError, MB_OK | MB_ICONERROR); 
        return FALSE; 
    }

    // ִ��Ӧ�ó����ʼ��:
    if (!InitInstance(hInstance, nCmdShow))
    {
        MessageBox(NULL, TEXT("Window creating failed!"), c_lpszError, MB_OK | MB_ICONERROR);
        return FALSE;
    }


    MSG msg; 

    //����λͼ

    hBmBkgnd = (HBITMAP)LoadImage(hInst, TEXT("image\\bkgnd.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
    hBmRole = (HBITMAP)LoadImage(hInst, TEXT("image\\role.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 

    if (!hBmBkgnd || !hBmRole)
    {
        MessageBox(hMainWnd, TEXT("����ͼƬʧ�ܣ�"), c_lpszError, MB_OK | MB_ICONERROR); 
    }

    GetObject(hBmBkgnd, sizeof(BITMAP), &bmBkgnd); 
    GetObject(hBmRole, sizeof(BITMAP), &bmRole); 

    // ����Ϣѭ��:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = c_lpszWndClassName;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ������������ھ��������������
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;  //��ʵ������洢��ȫ�ֱ�����

    capMenuAppendCy = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU); 

    HWND hWnd = CreateWindow(c_lpszWndClassName, c_lpszMainWndTitle, WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX,
        mainWndPos.x, mainWndPos.y, mainWndSize.x, mainWndSize.y + capMenuAppendCy, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    hMainWnd = hWnd;    //�������ھ���洢��ȫ�ֱ�����

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

