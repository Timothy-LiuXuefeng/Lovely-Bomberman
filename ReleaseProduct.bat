@echo off
SETLOCAL
set returnval=0

mkdir �ɰ�ը����_Product

copy /Y Release\�ɰ�ը����.exe �ɰ�ը����_Product\�ɰ�ը����.exe || set returnval=1 && echo ���� �ɰ�ը����.exe ʧ��
copy /Y Release\GameSound.exe �ɰ�ը����_Product\GameSound.exe || set returnval=1 && echo ���� GameSound.exe ʧ��
copy /Y Release\Images.dll �ɰ�ը����_Product\Images.dll || set returnval=1 && echo ���� Images.dll ʧ��
copy /Y x64\Release\�ɰ�ը����(x64).exe �ɰ�ը����_Product\�ɰ�ը����(x64).exe || set returnval=1 && echo ���� �ɰ�ը����(x64).exe ʧ��
copy /Y x64\Release\GameSound(x64).exe �ɰ�ը����_Product\GameSound(x64).exe || set returnval=1 && echo ���� GameSound(x64).exe ʧ��
copy /Y x64\Release\Images(x64).dll �ɰ�ը����_Product\Images(x64).dll || set returnval=1 && echo ���� Images(x64).dll ʧ��

if %returnval% EQU 0 (
    echo �����ɹ���
)

pause

EXIT /B %returnval%
