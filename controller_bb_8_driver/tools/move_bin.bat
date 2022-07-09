set MY_PATH=%cd%\..
set PI_SHARE=\\10.139.1.139\PiShare

copy /y %MY_PATH%\build\ch.bin %PI_SHARE%

:pause
sleep 2
