# Dropper
Dropper - dropbox download/upload command-line tool
  - C++14
  - POCO 1.9.2
  - MinGW-W64
  - Cmake 3.15

for In4Tech with love..


## dropper /help
dropper put/get /src_path /dst_path
```
/help = h      Display help information on command line arguments.
/src_path = s  Send/Receive file path.
/dst_path = d  Receive/Send file path.
put            Send file(native path format) to dst_path(unix path format).
get            Download file(unix path format) to dst_path(native path format).
```
Example(Windows-PowerShell): 
```
.\dropper.exe get /s /netfolder/fest.png /d localfolder\fest.png
```

## Windows install
### You will need:
  - MSYS2 x64
  - Open "Environment Variables > System Variables > Path"
  - At the end, add msys64\mingw64\bin
  ```
    $ pacman -Syu
    $ pacman -S mingw-w64-x86_64-gcc
    $ pacman -S mingw64/mingw-w64-x86_64-poco
    $ pacman -S mingw64/mingw-w64-x86_64-extra-cmake-modules
    $ pacman -S mingw64/mingw-w64-x86_64-make 
  ```
  - Close MSYS2 x64
  - Open CMD
  ```
  cd /d %folder_contain_Dropper%
  md build
  cd build
  cmake ..\ -G"MinGW Makefiles"
  mingw32-make
        or
  mingw32-make install
  ```
  enjoy!
