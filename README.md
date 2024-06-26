# Disk Scanner使用说明

## 本程序共有8个功能  
0 退出  
1 扫描（生成sql文件和目录树）  
2 统计文件信息   
3 模拟文件操作  
4 模拟目录操作  
5 删除目录树  
6 比较统计信息  
7 查询统计信息  
8 查找文件信息

## 输入格式
输入功能前序号即可选择相应功能  

本程序对用户输入的目录和文件大小写不敏感  

输入的目录格式为 C:\Windows\ 或 C:\Windows 两种方式均可

## 扫描
在进行2 3 4 5 8操作前，请先进行1操作，生成目录树

扫描的同时会生成目录树（孩子兄弟书）和包含文件/目录信息的insert语句的sql文件，每个sql文件最多包含100000条sql语句  

扫描信息如下
- 扫描时间
- 文件数和目录数
- 目录深度
- 最长全路径文件名及其长度
- 目录树深度
- 目录树结点数（由于包含了根目录，所以会比文件和目录总数多1）
- 生成sql文件数量
- 生成sql语句总数

## 统计文件信息
可选择单个目录统计或批量统计

程序会保留本次运行期间的所有批量统计结果，并进行计数，可依此进行比较和查询

## 模拟文件和目录操作
可选择单条操作或批量操作  

### 单条文件操作格式  
全路径文件名,A/M/D,time,size  
其中A为添加文件，M为修改文件，D为删除文件，三者选择其一，time和size应输入数字，但当进行D操作时，time和size应直接写为time和size

### 单条目录操作格式  
目录路径,D,time,size  
本程序仅支持对目录进行D操作

### 批量操作时，文件格式如下
#### 批量操作目录  
selected dirs  
……  
c:\windows\SysWOW64\XPSViewer\zh-CN\,D,time,size  
……  
end of dirs 

#### 批量操作文件
selected files  
……  
c:\windows\SystemApps\MicrosoftWindows.Client.CBS_cw5n1h2txyewy\SearchUx.MiniUI.dll,M,1704466243,810496  
……  
end of files  	

#### 批量统计文件信息
stat dirs  
……  
c:\windows\UUS\x86\   
……  
end of dirs   

## 删除目录树
为防止误操作，在删除目录树前会进行一次确认(y/n)  
删除成功后会显示删除的结点数

## 比较统计信息
输入两次统计信息的序号，如1 2，即可比较第一次和第二次统计的信息，并显示二者差异

## 查询统计信息
输入某一次统计信息的序号，如输入1，即可显示出第一次统计的信息

## 查找文件信息
*本操作需要在扫描完目录后进行*  
输入全路径文件名，即可查询该文件的修改时间，及其大小

