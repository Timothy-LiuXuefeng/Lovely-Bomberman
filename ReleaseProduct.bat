@echo off
SETLOCAL
set returnval=0

mkdir �ɰ�ը����_Product

copy /Y Release\�ɰ�ը����.exe �ɰ�ը����_Product\�ɰ�ը����.exe || set returnval=1 && echo ���� �ɰ�ը����.exe ʧ��
copy /Y Release\GameSound.exe �ɰ�ը����_Product\GameSound.exe || set returnval=1 && echo ���� GameSound.exe ʧ��
copy /Y Release\Images.dll �ɰ�ը����_Product\Images.dll || set returnval=1 && echo ���� Images.dll ʧ��

if %returnval% EQU 0 (
    echo �����ɹ���
)

pause

EXIT /B %returnval%
