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
  - Download & install MSYS2 x64 https://github.com/msys2/msys2/wiki/MSYS2-installation
    
    NOTICE: You have to install the MSYS2 in the C:\building\ directory because there are some problems with static linking:
    https://github.com/msys2/MINGW-packages/issues/1670
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
  
  ![alt text](https://github.com/kernelplv/dropper/blob/master/wikipics/2019-08-25_18-53-50.png)
  ## Linux install
  ### Manjaro(Arch)
  - Open Terminal
    ```
      # su
      # pacman -Syo
      # pacman -S gcc
      # pacman -S cmake
      # pacman -S poco
      # cd /home/%user%/%folder_contain_Dropper%
      # mkdir build
      # cd build
      # cmake ../ 
      # make install
    ```
![alt text](https://github.com/kernelplv/dropper/blob/master/wikipics/2019-08-25_23-27-40.png)

Enjoy!
