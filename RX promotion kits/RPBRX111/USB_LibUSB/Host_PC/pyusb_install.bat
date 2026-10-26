REM batch file to install pyusb to python installation directory
xcopy /s pyusb %HOMEDRIVE%\Python27\Lib\site-packages
cd \Python27\Lib\site-packages\usb\pyusb-1.0.0a2
%HOMEDRIVE%\Python27\python.exe setup.py install

