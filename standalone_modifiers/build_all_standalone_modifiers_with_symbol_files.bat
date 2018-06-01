@echo off
set VSCMD_DEBUG=not_empty
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\Tools\VsDevCmd.bat" -arch=amd64
rem /Zi enable debugging information
rem /Fd[file] name .PDB file
rem /permissive- set standard-conformance mode (feature set subject to change)
rem /std:<c++14|c++latest> C++ standard version
rem     c++14 – ISO/IEC 14882:2014 (default)
rem     c++latest – latest draft standard (feature set subject to change)
rem /showIncludes show include file names
rem /nologo suppress copyright message
rem /WX treat warnings as errors
rem /analyze[-] Enable native analysis
for %%c in (*.cpp) do cl /analyze /WX /nologo /permissive- /std:c++latest /Zi /Fd /EHsc %%c zzxcf_tokenizer.lib /link /MACHINE:X64
pause