@echo off
SETLOCAL
set returnval=0

mkdir 可爱炸弹人_Product

copy /Y Release\可爱炸弹人.exe 可爱炸弹人_Product\可爱炸弹人.exe || set returnval=1 && echo 发布 可爱炸弹人.exe 失败
copy /Y Release\GameSound.exe 可爱炸弹人_Product\GameSound.exe || set returnval=1 && echo 发布 GameSound.exe 失败
copy /Y Release\Images.dll 可爱炸弹人_Product\Images.dll || set returnval=1 && echo 发布 Images.dll 失败

if %returnval% EQU 0 (
    echo 发布成功！
)

pause

EXIT /B %returnval%
