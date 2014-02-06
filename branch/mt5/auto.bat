F:
f:\WINDOWS\system32\rasdial.exe IT
cd "f:\Program Files\MMCIS MetaTrader 4 Client Terminal\"
terminal.exe config\auto.ini
f:\WINDOWS\system32\rasdial.exe IT /disconnect
cd "F:\Dev-Cpp\realInsta\"
move /Y MMCIS-Real.1440 MMCIS-Real0.1440
php.exe sleep.php
patterns.exe /MMCIS-Real /opt /1440t /quit