@echo off
set VSCMD_DEBUG=not_empty
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\Tools\VsDevCmd.bat" -arch=amd64
rem /Ox maximum optimizations
rem /GX[-] enable C++ EH (same as /EHsc)
for %%c in (*.cpp) do cl /Ox /EHsc %%c zzxcf_tokenizer.lib /link /MACHINE:X64
pause