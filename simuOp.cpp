#include "scan.h"

void dir_tree::op_file(string opStatement)//对指定文件进行操作A,M,D
{
	if (root == NULL)
	{
		printf("目录树未创建\n");
		return;
	}
	
	string path, opcode;//分离操作参数
	time_t time = 0;
	_fsize_t size = 0;
	opsplit(opStatement, path, opcode, time, size);

	printf("-------执行操作 %s\n", opStatement.c_str());
	printf("操作前：");
	node* q = find(path);//查找文件
	if (q == NULL)
		printf("文件不存在\n");
	else
		q->print_node();

	printf("操作结果：");
	if (opcode == "A")
	{
		size_t pos = path.find_last_of("\\");
		string dirPath = path.substr(0, pos + 1);//获取文件所处目录
		node* p = find(dirPath);
		if (p == NULL)//文件所处目录不存在
		{
			printf("文件所在目录不存在, 无法添加\n");
			return;
		}

		if (p->child == NULL)//目录下没有文件/子目录
		{
			p->child = new node(path, time, size, p);//添加文件
			printf("添加成功\n");
		}
		else
		{
			for (p = p->child; p->brother != NULL; p = p->brother);//遍历至最后一个兄弟结点
			p->brother = new node(path, time, size, p);
			printf("添加成功\n");
		}
	}
	else if (opcode == "M")
	{
		node* p = find(path);

		if (p == NULL)
		{
			printf("文件不存在，无法修改\n");
		}
		else//找到文件
		{
			p->size = size;
			p->time = time;
			printf("修改成功\n");
		}
	}
	else if (opcode == "D")
	{
		node* p = find(path);

		if (p == NULL)
		{
			printf("文件不存在，无法删除\n");
		}
		else//找到文件
		{
			if(destroy(p))
				printf("删除成功\n");
		}
	}
	else
		printf("操作不明\n");

	printf("操作后：");
	q = find(path);
	if (q == NULL)
		printf("文件不存在\n");
	else
		q->print_node();
	
	return;
}

void dir_tree::op_dir(string opStatement)//对指定目录进行操作, 仅进行删除操作
{
	if (root == NULL)
	{
		printf("目录树未创建\n");
		return;
	}
	string path, opcode;//分离操作参数
	time_t time = 0;
	_fsize_t size = 0;
	opsplit(opStatement, path, opcode, time, size);

	printf("------执行操作 %s\n", opStatement.c_str());
	if (path.back() != '\\')//目录规范化
		path += "\\";

	printf("操作前\n");
	node* p = rstat(path);//统计目录及子目录信息，返回目录指针

	if (opcode == "D")
	{
		printf("操作结果:");
		if (p == NULL)
		{
			printf("目录不存在，无法删除\n");
		}
		else
		{
			int n = destroy(p);
			if (n)
				printf("删除成功, 删除结点数 %d\n", n);
			else
				printf("结点为空，无法删除\n");
		}
	}
	else
		printf("操作不明\n");

	printf("操作后\n");
	rstat(path);
	
	return;
}

void dir_tree::mul_op_file(string filePath)
{
	if (root == NULL)
	{
		printf("目录树未创建\n");
		return;
	}

	printf("正在依据文件 %s 批量进行文件操作\n", filePath.c_str());
	ifstream f;
	f.open(filePath);
	if (!f)
	{
		printf("读取文件 %s 失败\n", filePath.c_str());
		return;
	}

	string buf;
	getline(f, buf);
	if (buf != "selected files")
	{
		printf("文件格式错误，首行应为\"selected files\"\n");
		return;
	}

	int cnt = 0;
	string path, opcode, size, time;
	while (getline(f, buf) && buf != "end of files")
	{
		cnt++;
		op_file(buf);
	}
	printf("执行操作次数 %d\n", cnt);
	return;
}

void dir_tree::mul_op_dir(string filePath)
{
	if (root == NULL)
	{
		printf("目录树未创建\n");
		return;
	}

	printf("正在依据文件 %s 批量进行目录操作\n", filePath.c_str());
	ifstream f;
	f.open(filePath);
	if (!f)
	{
		printf("读取文件 %s 失败\n", filePath.c_str());
		return;
	}

	string buf;
	getline(f, buf);
	if (buf != "selected dirs")
	{
		printf("文件格式错误，首行应为\"selected dirs\"\n");
		return;
	}

	int cnt = 0;
	string path, opcode, size, time;
	while (getline(f, buf) && buf != "end of dirs")
	{
		cnt++;
		op_dir(buf);
	}
	printf("执行操作次数 %d\n", cnt);

	return;
}

void dir_tree::opsplit(string opStatement, string& path, string& opcode, time_t& time, _fsize_t& size)//分离操作参数
{
	string ssize, stime;
	stringstream iss(opStatement);
	getline(iss, path, ',');
	getline(iss, opcode, ',');
	getline(iss, stime, ',');
	getline(iss, ssize, '\n');
	if (stime != "time")
		time = (time_t)stoi(stime);
	if (ssize != "size")
		size = (_fsize_t)stoi(ssize);

	return;
}