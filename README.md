# simple-chatroom
Computer Network Homework:
網路程式作業: 聊天室程式
		資工2蕭勝興
410285034
	系統環境
在windows中執行cygwin。
Cygwin是 Windows 下可以執行 bash、dash、csh 的一個方法，即在 Windows 下可以使用 Linux 終端機的指令。
	編譯方法
首先要先安裝cygwin
x86安裝
https://mega.co.nz/#!5hAjFJCQ!mNeSVBIPjHSCFfQmu3Zj4DWmh1SEJMub6YdL8ShmFio
x64安裝
https://mega.co.nz/#!VxxxSIqb!B-swbxvGckfvM4DEkOQ3AsKAiPlL3YZCwIhPSF6H-QY
下載後補上附檔名 .zip
解壓縮後執行cygwin_install資料夾內的setup-x86.exe (x86)或                                                                      setup-x86_64.exe (x64)
[Choose A Download Source] 選擇 [Install from Local Directory]
Root Directory不變
[Local Package Directory] 選擇此安裝檔所在的資料夾 [cygwin_install]
[Select Packages] 將 [Devel] 的狀態改為"Install"
詳情請見附件 cygwin 安裝說明.pdf
再來，執行cygwin，將server.c與client.c還有test.jpg放入
C:\cygwin\home\使用者名字  資料夾中

在cygwin中打入指令
gcc –o ﹝自己取的server.exe檔名﹞ server.c
《代表去編譯server.c這個程式。》
 
再來開一個新的cygwin
gcc –o ﹝自己取的client.exe檔名﹞ client.c
《代表去編譯client.c這個程式。》
 
《那些字是警告，不是錯誤》
	程式流程圖










	使用說明
	Server端
在cygwin中打
./﹝自己取的server.exe檔名﹞.exe
 
就會等待連接
	Client端
在cygwin中打
./﹝自己取的server.exe檔名﹞.exe localhost
或
./﹝自己取的server.exe檔名﹞.exe 【ip位址ivp6】
 
pic recv代表有收到圖片《在cygwin有一個get.jpg》
接著輸入名字
	執行結果
   可以多人連接
再開一個cygwin也打./c localhost
 
開始聊天！！！
 
Server與client端都可以看到完整對話。(sever端不能參與對話)

