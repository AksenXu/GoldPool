@echo off
REM 参数含义：
REM 参数1：如果串口号小于10, 则直接写入COMX，例如COM1，如果大于10，需要写\\\\.\\COMXX
REM 参数2：串口波特率，MT7682为115200，AC7916为460800
REM 参数3：启动过程的log，表示设备有运行起来
REM 参数4：需要写入的env文件

REM ./wincom.exe \\\\.\\COM187 460800 print_version Lock_sdcard_upgrade_env.txt

.\wincom.exe COM3 115200 print_version Lock_sdcard_upgrade_env.txt


pause