@echo off
echo=
echo 	++++++++++++++++++++
echo 	+     ������ʦ     +
echo 	+           by:hhy +
echo 	++++++++++++++++++++
:start
echo=

::��������
set /p birthday=���������������(����20010521),��q�˳�:
:: /p���ڽ�������
REM echo %birthday%	
:: %%�������ñ���
if /i %birthday%==q exit
:: /i��ʾ�����ִ�Сд

::����������
set /a year=%birthday%/10000
:: set /a ��ʾ������ʽ
set /a month=%birthday%/100%%100
set /a day=(%birthday%)%%100
REM echo %year%, %month%, %day%

::�ж�����Ϸ���
if %year% equ 0 goto error
if %month% equ 0 goto error
if %day% equ 0 goto error
if %month% gtr 12 goto error

::�ж�����
set leap=0
set /a tmp1=(%year%)%%400
set /a tmp2=(%year%)%%100
set /a tmp3=(%year%)%%4
if %tmp1% equ 0 (
	set leap=1
) else if %tmp2% neq 0 if %tmp3% equ 0 (
	set leap=1
)

::�ж����ںϷ���
if %month% equ 1 goto checkDay31
if %month% equ 3 goto checkDay31
if %month% equ 5 goto checkDay31
if %month% equ 7 goto checkDay31
if %month% equ 8 goto checkDay31
if %month% equ 10 goto checkDay31
if %month% equ 12 goto checkDay31
if %month% equ 4 goto checkDay30
if %month% equ 6 goto checkDay30
if %month% equ 9 goto checkDay30
if %month% equ 11 goto checkDay30
if %month% equ 2 (
	if %leap% equ 0 (
		if %day% gtr 28 goto error
	) else if %day% gtr 29 goto error
)

:next
::������Ф
set /a animal=(%year%)%%12
if %animal% equ 0 (
	echo ��Ф: ��
) else if %animal% equ 1 (
	echo ��Ф: ��
) else if %animal% equ 2 (
	echo ��Ф: ��
) else if %animal% equ 3 (
	echo ��Ф: ��
) else if %animal% equ 4 (
	echo ��Ф: ��
) else if %animal% equ 5 (
	echo ��Ф: ţ
) else if %animal% equ 6 (
	echo ��Ф: ��
) else if %animal% equ 7 (
	echo ��Ф: ��
) else if %animal% equ 8 (
	echo ��Ф: ��
) else if %animal% equ 9 (
	echo ��Ф: ��
) else if %animal% equ 10 (
	echo ��Ф: ��
) else if %animal% equ 11 (
	echo ��Ф: ��
)
::��������
set /a star=(%birthday%)%%10000
REM echo %star%
if %star% leq 119 (
	echo ����: Ħ����
) else if %star% leq 218 (
	echo ����: ˮƿ��
) else if %star% leq 320 (
	echo ����: ˫����
) else if %star% leq 419 (
	echo ����: ������
) else if %star% leq 520 (
	echo ����: ��ţ��
) else if %star% leq 621 (
	echo ����: ˫����
) else if %star% leq 722 (
	echo ����: ��з��
) else if %star% leq 822 (
	echo ����: ʨ����
) else if %star% leq 922 (
	echo ����: ��Ů��
) else if %star% leq 1023 (
	echo ����: �����
) else if %star% leq 1122 (
	echo ����: ��Ы��
) else if %star% leq 1221 (
	echo ����: ������
) else (
	echo ����: Ħ����
)
goto start

::���ںϷ��Լ��
:checkDay31
if %day% gtr 31 goto error
goto next
:checkDay30
if %day% gtr 30 goto error
goto next

::������
:error
echo ��������,����������
goto start
