# Beleg im Modul ELT04930 Augmented Reality Hardware
## Thema: Kochanleitung
----------
## Voraussetzungen
Das Projekt wurde unter:
- Visual Studio 2019 Comunity Version erstellt und gestestet.  
- ARtoolkit in der Version 3
- Die Hiro Pattern
- GitSCM oder ähnliches Tool muss installiert sein. <http://git-scm.com/downloads>
## Installation
Um das Programm kompilierne zu können ist das Opensource Bibliothek Assimp nötig. Diese kann als pre compiled binary über den Paketmanager VCPKG installiert werden.

Das Batch-file `install.bat` ermöglicht die Installaion von VCPKG und der Bibliothek. Manuell kann ein git Tool installiert werden und die folgenden Zeilen in ein Konsolen fenster eingegeben werden
 
 ```
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
```
## Empfehlung 
Die Videoauflösung sollte mindestents 960x720 gewählt werden, dass eine ruhige Dastellung ermöglicht wird. 

