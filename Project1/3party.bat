@echo off
cd include
git clone https://github.com/bkaradzic/bx.git
git clone https://github.com/bkaradzic/bimg.git
git clone https://github.com/bkaradzic/bgfx.git
cd bgfx
..\bx\tools\bin\windows\genie.exe --with-examples --with-tools --with-shared-lib vs2022
pause