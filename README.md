# 4d-plugin-folder-watch
Monitor a folder path and receive events when items are added, deleted or changed.

##Platform

| carbon | cocoa | win32 | win64 |
|:------:|:-----:|:---------:|:---------:|
|🆗|🆗|🚫|🚫|

###Remarks

On Mac, [Grand Central Dispatach](https://developer.apple.com/library/mac/documentation/Performance/Reference/GCD_libdispatch_Ref/index.html) is used, therefore 10.7+ is required. 

Commands
---

```
// --- Settings
FOLDER_Set_watch_path
FOLDER_Get_watch_path
FOLDER_Set_watch_method
FOLDER_Get_watch_method
```
