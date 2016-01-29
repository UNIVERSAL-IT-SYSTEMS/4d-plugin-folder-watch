# 4d-plugin-folder-watch
Monitor a folder path and receive events when items are added, deleted or changed.

##Platform

| carbon | cocoa | win32 | win64 |
|:------:|:-----:|:---------:|:---------:|
|🆗|🆗|🚫|🚫|

###Remarks

On Mac, [Grand Central Dispatach](https://developer.apple.com/library/mac/documentation/Performance/Reference/GCD_libdispatch_Ref/index.html) is used, therefore 10.7+ is required. 

**Note**: Changes to subfolders are not reported. 

Commands
---

```c
// --- Settings
FOLDER_Set_watch_path
FOLDER_Get_watch_path
FOLDER_Set_watch_method
FOLDER_Get_watch_method
```

Example
---

```
  //FOLDER_METHOD_CALLED_ON_EVENT

C_TEXT($1)
C_REAL($2)

$watchpath:=$1
$unixtime:=$2

ARRAY TEXT(<>DOCUMENTS;0)
DOCUMENT LIST($watchpath;<>DOCUMENTS;Recursive parsing | Ignore invisible | Absolute path)

CALL PROCESS(-1)
```

```
$success:=FOLDER Set watch path ($folder)
$success:=FOLDER Set watch method ("FOLDER_METHOD_CALLED_ON_EVENT")
```
