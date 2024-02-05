#include "scan.h"

void scan(string searchPath)
{
	if (searchPath.back() != '\\')//目录名规范化
		searchPath += '\\';

	int fileNum = 0;//文件数
	int dirNum = 0;//子目录数/文件夹数
	int depth = 0;//目录深度
	size_t maxLen = 0;//最长全路径文件名长度
	string fileName;//最长全路径文件名
	_finddata_t file;//存储遍历得到的文件或目录
	intptr_t handle;//_findfirst()返回的文件句柄
	string temp;
	queue<string> q;

	printf("正在扫描目录 %s\n", searchPath.c_str());
	printf("目录数\t文件数\n");
	q.push(searchPath);
	while (!q.empty())
	{
		size_t cnt = 0;//开始遍历的第cnt个结点
		size_t num = q.size();//上一层结点数
		depth++;
		while (cnt < num)//遍历该层，得下一层
		{
			cnt++;
			temp = q.front() +"*.*";
			handle = _findfirst(temp.c_str(), &file);//c_str()将string转为char*
			if (handle == -1)//目录为空
			{
				q.pop();
				continue;
			}

			do//遍历子结点
			{
				if (file.name[0] != '.')//去除.和..目录
				{
					temp = q.front() + file.name;//得全路径名
					if (file.attrib & _A_SUBDIR)//是子目录，压入队列
					{
						temp += "\\";//目录名规范化
						q.push(temp);
						dirNum++;
					}
					else//是文件，不压入队列
					{
						size_t len = temp.length();
						if (len > maxLen)//寻找最长全路径文件名
						{
							maxLen = len;
							fileName = temp;
						}
						fileNum++;
					}
					//print_file(temp, file);
					printf("%d\t%d\r", dirNum, fileNum);
				}
			} while (_findnext(handle, &file) == 0);//找到下一个文件/目录，循环
			q.pop();//q.front()子结点遍历完
		}
	}
	printf("\n扫描完成\n");
	printf("文件和目录总数为 %d\n", dirNum + fileNum);
	printf("目录的深度为 %d\n", depth);
	printf("最长全路径文件名为 %s\n长度为 %zd\n", fileName.c_str(), maxLen);
}

void print_file(string temp, _finddata_t file)//输出文件详细信息
{
	printf("%s\t%d bytes\t", temp.c_str(), file.size); 
	struct tm* time = localtime(&file.time_create);//将time_t转换为年月日
	printf("%d-%d-%d %d:%d:%d\n", time->tm_year+1900, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
}