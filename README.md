一个简单的命令行背单词小程序。在 C++11的标准下编译 `reciteword.cpp` 即可得到可执行文件。

目前仅支持 Windows 平台。



| 命令              | 备注                                                         |
| ----------------- | ------------------------------------------------------------ |
| import [文件主名] | 导入 `.\reciteword\[文件主名].txt` 中记录的信息。            |
| save0 [主文件名]  | 将记录的信息导出到 `.\reciteword\[文件主名].txt`             |
| save [主文件名]   | 将当前的数据保存到 `.\reciteword\[文件主名].dat`，推荐使用这种方式保存 |
| load [主文件名]   | 导入`.\reciteword\[文件主名].dat` 中记录的数据               |
|                   | 向程序加入一个单词和它的一种含义                             |
|                   | 删除一个单词和它对应的一种含义                               |
| recite            | 开始背单词，每次给出一个单词的含义，你需要回答出符合这个含义的一种单词。<br />由于目前暂时没有处理包含相同含义的单词，因此建议相同含义的单词建议在含义中多加入一些信息进行区分。 |




xxx.txt 的保存信息的格式：

```plain 
单词1
含义1
单词2
含义2
……
```