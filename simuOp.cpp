#include "scan.h"

//功能：对指定文件进行操作
//输入：操作指令，格式为："path,opcode(A/M/D),time,size"。其中A为增加，M为修改，D为删除
//输出：操作前文件信息，操作结果，操作后文件信息
//条件：目录树root不为NULL
bool dir_tree::op_file(string opStatement)//对指定文件进行操作A,M,D
{
	if (root == NULL)
	{
		printf("目录树未创建\n");
		return false;
	}
	
	string path, opcode;//分离操作参数
	time_t time = 0;
	_fsize_t size = 0;
	opsplit(opStatement, path, opcode, time, size);

	printf(">>>执行操作 %s\n", opStatement.c_str());
	printf("\t操作前:");
	node* q = find(path);//查找文件
	if (q == NULL)
		printf("文件不存在\n");
	else
		q->print_node();

	printf("\t操作结果:");
	if (opcode == "A")
	{
		size_t pos = path.find_last_of("\\");
		string dirPath = path.substr(0, pos + 1);//获取文件所处目录
		node* p = find(dirPath);
		if (p == NULL)//文件所处目录不存在
		{
			printf("文件所在目录不存在, 无法添加\n");
			return false;
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
			return false;
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
			return false;
		}
		else//找到文件
		{
			if (destroy(p))
			{
				printf("删除成功\n");
			}
		}
	}
	else
	{
		printf("操作不明\n");
		return false;
	}

	printf("\t操作后:");
	q = find(path);
	if (q == NULL)
		printf("文件不存在\n");
	else
		q->print_node();
	
	return true;
}

//功能：对指定目录进行操作,仅支持删除操作
//输入：操作指令，格式为："path,opcode(M),time,size"
//输出：操作前目录及子目录信息，操作结果，操作后目录及子目录信息
//条件：目录树root不为NULL
bool dir_tree::op_dir(string opStatement)
{
	if (root == NULL)
	{
		printf("目录树未创建\n");
		return false;
	}
	string path, opcode;//分离操作参数
	time_t time = 0;
	_fsize_t size = 0;
	opsplit(opStatement, path, opcode, time, size);

	printf(">>>执行操作 %s\n", opStatement.c_str());
	if (path.back() != '\\')//目录规范化
		path += "\\";

	printf("操作前:\n");
	node* p = rstat(path);//统计目录及子目录信息，返回目录指针

	if (opcode == "D")
	{
		printf("操作结果:");
		if (p == NULL)
		{
			printf("目录不存在，无法删除\n");
			return false;
		}
		else
		{
			int n = destroy(p);
			printf("删除成功, 删除结点数 %d\n", n);
		}
	}
	else
	{
		printf("操作不明\n");
		return false;
	}
		
	printf("操作后:\n");
	rstat(path);

	return true;
}

//功能：依据文件批量进行文件操作
//输入：批量操作文件路径
//输出：1.每次操作的输出信息，见op_file()；2.操作总数，成功次数，失败次数
//条件：目录树root不为NULL
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
	int succed = 0;
	int failed = 0;
	while (getline(f, buf) && buf != "end of files")
	{
		if (buf.empty())//读到空行
			continue;

		cnt++;
		if (op_file(buf))
			succed++;
		else
			failed++;
	}
	printf("执行操作次数: %d\n", cnt);
	printf("成功次数: %d\t失败次数: %d\n", succed, failed);

	return;
}

//功能：依据文件批量进行目录操作
//输入：批量操作文件路径
//输出：1.每次操作的输出信息，见op_dir()；2.操作总数，成功次数，失败次数
//条件：目录树root不为NULL
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
	int succed = 0;
	int failed = 0;
	while (getline(f, buf) && buf != "end of dirs")
	{
		if (buf.empty())//读到空行
			continue;
		
		cnt++;
		if (op_dir(buf))
			succed++;
		else
			failed++;
	}
	printf("执行操作次数: %d\n", cnt);
	printf("成功次数: %d\t失败次数: %d\n", succed, failed);

	return;
}

//功能：分离操作操作语句中各项参数
//输入：操作语句，各种参数引用传入，包括路径、操作方式、时间、大小
//输出：通过引用传参返回结果
//条件：无
void dir_tree::opsplit(string opStatement, string& path, string& opcode, time_t& time, _fsize_t& size)
{
	string ssize, stime;
	stringstream iss(opStatement);
	getline(iss, path, ',');
	getline(iss, opcode, ',');
	getline(iss, stime, ',');
	getline(iss, ssize, '\n');
	//当进行删除操作时，size和time分别为"size"和"time"，此时不用为time和size赋值；
	//进行其他操作时，time和size应为数字，要将string类型的stime和ssize转换成相应类型
	if (stime != "time")
		time = stoll(stime);
	if (ssize != "size")
		size = (_fsize_t)stol(ssize);

	return;
}