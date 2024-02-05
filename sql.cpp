#include "scan.h"

void sql(string searchPath, string sqlPath)
{
	if (searchPath.back() != '\\')
		searchPath += '\\';
	if (sqlPath.back() != '\\')
		sqlPath += '\\';

	int fileNum = 0;//文件数
	int dirNum = 0;//子目录数/文件夹数
	_finddata_t file;//存储遍历得到的文件或目录
	intptr_t handle;//_findfirst()返回的文件句柄
	string temp;
	queue<string> q;
	ofstream sqlFile;

	printf("开始生成sql文件，每个sql文件中最多有 %d 条sql语句\n", MAXSQL);
	printf("正在扫描目录 %s\n", searchPath.c_str());
	printf("目录数\t文件数\n");
	q.push(searchPath);
	while (!q.empty())
	{	
		temp = q.front() + "*.*";
		handle = _findfirst(temp.c_str(), &file);
		if (handle == -1)
		{
			q.pop();
			continue;
		}

		do//遍历子结点
		{
			if (file.name[0] != '.')
			{
				if ((fileNum + dirNum) % MAXSQL == 0)//刚开始扫描(sql语句为0)或单个文件sql语句满10000条，创建新文件
				{
					if (fileNum + dirNum != 0)//最开始未打开文件流，不需要关闭
					{
						sqlFile.close();
					}
					sqlFile.open(sqlPath + to_string((fileNum + dirNum) / MAXSQL + 1) + ".sql", ios::out);
				}
				temp = q.front() + file.name;
				if (file.attrib & _A_SUBDIR)//是子目录，压入队列
				{
					temp += "\\";
					q.push(temp);
					dirNum++;
					sqlFile << "insert into 目录 values('" + temp + "'," + to_string(file.time_create) + "); " << endl;
				}
				else//是文件
				{
					fileNum++;
					sqlFile << "insert into 文件 values('" + temp + "'," + to_string(file.size) + "," + to_string(file.time_create) + "); " << endl;
				}
				printf("%d\t%d\r", dirNum, fileNum);
			}
		} while (_findnext(handle, &file) == 0);
		q.pop();
	}
	if (fileNum + dirNum != 0)
		sqlFile.close();
	printf("\n扫描完成\n");
	printf("共生成 %d 个sql文件，存储于 %s\n", (fileNum + dirNum) / MAXSQL + 1, sqlPath.c_str());
	printf("总计 %d 条sql插入语句\n", dirNum + fileNum);
}