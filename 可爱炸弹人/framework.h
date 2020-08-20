////////////////////////////////////////
//
//  windows���������Ҫ��ͷ�ļ��ͺ�
//


#ifndef FRAMEWORK_H

#define FRAMEWORK_H

// // ���� SDKDDKVer.h �ɶ�����õ���߰汾�� Windows ƽ̨��
// ���ϣ��Ϊ֮ǰ�� Windows ƽ̨����Ӧ�ó����ڰ��� SDKDDKVer.h ֮ǰ���Ȱ��� WinSDKVer.h ��
// �� _WIN32_WINNT ������Ϊ��Ҫ֧�ֵ�ƽ̨��
#include <SDKDDKVer.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ�����
#endif	// #ifndef WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <type_traits>

#ifdef _UNICODE
#define _tstring wstring
#define _tistringstream wistringstream
#define _tostringstream wostringstream
#define _tifstream wifstream
#define _tofstream wofstream
#else					//#ifdef _UNICODE
#define _tstring string
#define _tistringstream istringstream
#define _tostringstream ostringstream
#define _tifstream ifstream
#define _tofstream ofstream
#endif					//#ifdef _UNICODE

#endif // #ifndef FRAMEWORK_H

