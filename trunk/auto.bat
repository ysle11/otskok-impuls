C:
C:\WINDOWS\system32\rasdial.exe IT
cd "C:\Program Files\MMCIS MetaTrader 4 Client Terminal\"
terminal.exe config\auto.ini
C:\WINDOWS\system32\rasdial.exe IT /disconnect
cd "c:\Program Files\MMCIS MetaTrader 4 Client Terminal\MQL4\Files\"
move /Y MMCIS-Real.1440 MMCIS-Real0.1440
D:
cd "D:\cd\otskok-impuls\"
php.exe sleep.php
patterns.exe /MMCIS-Real /opt /1440t /quit