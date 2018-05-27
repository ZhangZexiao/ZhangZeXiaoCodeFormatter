call build_all_standalone_modifiers.bat
@echo off
set shell_c=.\sqlite-amalgamation-3200000\shell.c
set shell_c_backup=.\sqlite-amalgamation-3200000\shell.c.backup
set sqlite3_c=.\sqlite-amalgamation-3200000\sqlite3.c
set sqlite3_c_backup=.\sqlite-amalgamation-3200000\sqlite3.c.backup
if exist shell.exe del /f shell.exe
for %%c in (*.exe) do (
echo testing %%c
if exist %shell_c_backup% (
del /f %shell_c%
copy %shell_c_backup% %shell_c%
)
if exist %sqlite3_c_backup% (
del /f %sqlite3_c%
copy %sqlite3_c_backup% %sqlite3_c%
)
%%c %shell_c%
%%c %sqlite3_c%
cl %shell_c% %sqlite3_c%
pause
if exist shell.exe del /f shell.exe
)
pause