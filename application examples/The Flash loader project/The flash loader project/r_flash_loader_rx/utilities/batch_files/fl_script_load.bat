@echo OFF
CLS

SET PY=C:\Python27\python.exe
SET WS=..\python
SET PROJ_NAME=FL_UserApp_RSKRX63N

%PY% -u "%WS%\r_fl_mot_converter.py" -i "..\..\..\%PROJ_NAME%\Debug\Exe\%PROJ_NAME%.mot"
%PY% -u "%WS%\r_fl_serial_flash_loader.py" -p0 -f "..\..\..\%PROJ_NAME%\Debug\Exe\%PROJ_NAME%.bch" -cload

ECHO DONE
pause
