# Beleg im Modul ELT04930 Augmented Reality Hardware
## Thema: Kochanleitung
----------
## Voraussetzungen
Das Projekt wurde unter:
- Visual Studio 2019 Comunity Version erstellt und gestestet.  
- ARtoolkit in der Version 3

## Installation
Um das Programm kompilierne zu können ist das Opensource Bibliothek Assimp nötig. Diese kann als pre compiled binary über den Paketmanager VCPKG 

Das Batch-file `install.bat` ermöglicht die Installaion von GitSCM, VCPKG und der Bibliothek. Manuell kann ein git Tool installiert werden und die folgenden Zeilen in ein Konsolen fenster eingegeben werden
 
 ```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install assimp
```
