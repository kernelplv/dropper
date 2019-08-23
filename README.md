# Dropper
Dropper - dropbox download/upload command-line tool
  - C++14
  - POCO 1.9.1
  - MinGw

for In4Tech with love..


# droppex /help
droppex put/get /src_path /dst_path
```
/help = h          Display help information on command line arguments.
/src_path = s  Send/Receive file path.
/dst_path = d  Receive/Send file path.
put           Send file(native path format) to dst_path(unix path format).
get           Download file(unix path format) to dst_path(native path format).
```
Example(Windows-PowerShell): 
```
.\dropper.exe get /s /netfolder/fest.png /d localfolder\fest.png
```
