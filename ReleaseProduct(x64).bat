@echo off
SETLOCAL
set returnval=0

mkdir �ɰ�ը����_Product(x64)

copy /Y x64\Release\�ɰ�ը����(x64).exe �ɰ�ը����_Product(x64)\�ɰ�ը����(x64).exe || set returnval=1 && echo ���� �ɰ�ը����(x64).exe ʧ��
copy /Y x64\Release\GameSound(x64).exe �ɰ�ը����_Product(x64)\GameSound(x64).exe || set returnval=1 && echo ���� GameSound(x64).exe ʧ��
copy /Y x64\Release\Images(x64).dll �ɰ�ը����_Product(x64)\Images(x64).dll || set returnval=1 && echo ���� Images(x64).dll ʧ��

if %returnval% EQU 0 (
    echo �����ɹ���
)

pause

EXIT /B %returnval%
