@echo off
REM �������壺
REM ����1��������ں�С��10, ��ֱ��д��COMX������COM1���������10����Ҫд\\\\.\\COMXX
REM ����2�����ڲ����ʣ�MT7682Ϊ115200��AC7916Ϊ460800
REM ����3���������̵�log����ʾ�豸����������
REM ����4����Ҫд���env�ļ�

REM ./wincom.exe \\\\.\\COM187 460800 print_version Lock_sdcard_upgrade_env.txt

.\wincom.exe COM3 115200 print_version Lock_sdcard_upgrade_env.txt


pause