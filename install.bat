if not exist "install" mkdir install
cd install
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass "& {& '%~dp0install\vcpkg\scripts\bootstrap.ps1' %*}"
@echo on
vcpkg integrate install
vcpkg install assimp
cd ..\..
