@echo off
SETLOCAL
set returnval=0

copy /Y Release\GameSound.exe �ɰ�ը����\GameSound.exe || set returnval=1 && echo �ƶ� GameSound.exe ʧ��
copy /Y Release\Images.dll �ɰ�ը����\Images.dll || set returnval=1 && echo �ƶ� Images.dll ʧ��
copy /Y x64\Release\GameSound(x64).exe �ɰ�ը����\GameSound(x64).exe || set returnval=1 && echo �ƶ� GameSound(x64).exe ʧ��
copy /Y x64\Release\Images(x64).dll �ɰ�ը����\Images(x64).dll || set returnval=1 && echo �ƶ� Images(x64).dll ʧ��

if %returnval% EQU 0 (
    echo �ƶ��ɹ���
)

pause

EXIT /B %returnval%
