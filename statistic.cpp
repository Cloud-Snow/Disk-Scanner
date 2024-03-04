#include "scan.h"

//功能：构造函数，创建统计信息
//输入：路径名path
//输出：初始化成员变量path
//条件：无
statInfo::statInfo(string path)
{
	this->path = path;
}

//功能：依据statinfo成员变量status返回对应字符串信息
//输入：调用成员status
//输出：返回状态信息字符串
//条件：无
string statInfo::statusInfo()
{
	string result;
	if (status == FAILED)
		result = "目录不存在";
	else if (status == NOFILE)
		result = "目录存在但没有文件";
	else if (status == SUCCEED)
		result = "目录存在且有文件";

	return result;
}

//功能：打印statInfo信息
//输入：调用statInfo属性成员
//输出：成员信息，包括统计状态、文件总数、文件总大小、最早和最晚时间文件啊
//条件：无
void statInfo::print_info()
{
	if (status == FAILED)//查找失败
	{
		printf("---%s\n\t不存在\n", path.c_str());
	}
	else if (status == NOFILE)//查找成功但目录中没有文件
	{
		printf("---%s\n\t无文件\n", path.c_str());
	}
	else if (status == SUCCEED)//查找成功且目录中有文件
	{
		printf("---%s\n\t文件总数 %d\n\t文件总大小 %d bytes\n", path.c_str(), num_total, size_total);
		printf("\t最早时间文件 ");
		time_earliest.print_node();
		printf("\t最晚时间文件 ");
		time_latest.print_node();
	}
}

//功能：打印某一次批量统计信息
//输入：vector<statInfo>类型，引用传参
//输出：打印vector中各个statInfo元素信息，并统计没有文件的目录数和不存在的目录数
//条件：无
void print_set(vector<statInfo>& set)
{
	int nofile = 0;//没有文件的目录数量
	int notExist = 0;//不存在的目录数量
	for (int i = 0; i < set.size(); i++)//打印信息
	{
		set[i].print_info();
		if (set[i].status == FAILED)
			notExist++;
		if (set[i].status == NOFILE)
			nofile++;
	}
	printf("统计目录数 %zd\n", set.size());
	printf("没有文件的目录数 %d\n", nofile);
	printf("不存在的目录数 %d\n", notExist);
	return;
}

//功能：统计结点指针p指向的结点下文件信息，将统计信息存入statInfo类型的info中
//输入：node指针p，statInfo info引用传参
//输出：所统计的结点指针p，统计信息存入info中
//条件：p不为NULL
node* dir_tree::statNode(node* p, statInfo &info)
{
	if (p == NULL)//结点为空，无法统计
	{
		info.status = FAILED;
		return NULL;
	}

	for (node*q  = p->child; q != NULL; q = q->brother)//遍历目录下文件
	{
		if (q->name.back() != '\\')//是文件，不是目录
		{
			if (info.time_earliest.name == "" && info.time_latest.name == "")//用第一个文件作为初值
			{
				info.time_earliest = node(q->name, q->time, q->size, NULL);//生成新结点，避免原结点被删除后丢失统计信息
				info.time_latest = node(q->name, q->time, q->size, NULL);
			}

			if (q->time < info.time_earliest.time)//寻找最早时间和最晚时间文件
				info.time_earliest = node(q->name, q->time, q->size, NULL);
			if (q->time > info.time_latest.time)
				info.time_latest = node(q->name, q->time, q->size, NULL);

			//q->print_node();
			info.num_total++;
			info.size_total += q->size;

		}
	}
	if (info.num_total == 0)//查找成功但目录中没有文件
		info.status = NOFILE;
	else//查找成功且有文件
		info.status = SUCCEED;

	return p;
}

//功能：统计指定目录及其子目录文件信息并打印
//输入：所要统计的目录路径string path
//输出：对应的结点指针，并将其及子目录的文件信息打印
//条件：无
node* dir_tree::rstat(string path)
{
	if (path.back() != '\\')
		path += '\\';

	statInfo info(path);
	node* p = find(path);
	statNode(p, info);//统计指定目录
	info.print_info();

	if (p)//目录存在
		for (node* q = p->child; q != NULL; q = q->brother)//统计子目录
		{
			if (q->name.back() == '\\')//只对目录进行统计
			{
				statInfo info2(q->name);
				statNode(q, info2);
				info2.print_info();
			}
		}

	return p;
}

//功能：在目录树中查找指定目录或文件
//输入：查找对象路径path
//输出：查找不到返回NULL，找到则返回对应结点指针
//条件：无
node* dir_tree::find(string path)
{

	string path_lower, dir_lower;
	node* p = root;

	path_lower = path;
	transform(path_lower.begin(), path_lower.end(), path_lower.begin(), ::tolower);//先转换为小写，便于不分大小写比较

	while (p != NULL)//查找
	{
		dir_lower = p->name;
		transform(dir_lower.begin(), dir_lower.end(), dir_lower.begin(), ::tolower);
		if (path_lower == dir_lower)//查找成功
		{
			break;
		}
		else if (dir_lower.back() == '\\' && path_lower.find(dir_lower) == 0)//在下一级目录中
		{
			p = p->child;
		}
		else//匹配下一个同级目录或文件
		{
			p = p->brother;
		}
	}

	return p;
}

//功能：批量统计目录下文件信息
//输入：批量统计文件路径，用于存储统计信息的infoSet容器
//输出：将统计信息存入infoSet
//条件：root不为NULL，批量统计文件存在
bool dir_tree::mul_stat(string filePath, vector<statInfo> &infoSet)
{
	if (root == NULL)
	{
		printf("目录树未创建\n");
		return false;
	}

	printf("正在依据文件 %s 批量统计目录\n", filePath.c_str());
	ifstream f(filePath);//读取文件
	if (!f)
	{
		printf("读取文件 %s 失败\n", filePath.c_str());
		return false;
	}
	
	string buf;
	getline(f, buf);
	if (buf != "stat dirs")
	{
		printf("文件格式错误，首行应为\"stat dirs\"\n");
		return false;
	}

	infoSet.clear();//清空
	while (getline(f, buf) && buf != "end of dirs")
	{
		if (buf.empty())//读到空行
			continue;

		if (buf.back() != '\\')
			buf += '\\';

		statInfo info(buf);
		node* p = find(buf);//查找目录
		statNode(p, info);
		//info.print_info();//打印信息
		infoSet.push_back(info);//存入结果集
	}
	f.close();

	return true;
}

//功能：比较统计信息，显示差异
//输入：两个统计信息集，infoSet1和infoSet2
//输出：比较得到的差异以及差异数量
//条件：两个统计信息均由同一个批量统计文件统计而来
void dir_tree::cmpStat(vector<statInfo> infoSet1, vector<statInfo> infoSet2)
{
	if (infoSet1.size() != infoSet2.size())
	{
		printf("统计对象不同， 无法比较\n");
		return;
	}

	int cnt = 0;//差异数
	int i = 0;
	for (i = 0; i < infoSet1.size(); i++)
	{
		if (infoSet1[i].path != infoSet2[i].path)
		{
			printf("统计对象不同， 无法比较\n");
			printf("比较提前终止，出错位置：第 %d 个统计信息\n", i + 1);
			break;
		}

		if (infoSet1[i].status == infoSet2[i].status && infoSet1[i].num_total == infoSet2[i].num_total && infoSet1[i].size_total == infoSet2[i].size_total &&
			infoSet1[i].time_earliest == infoSet2[i].time_earliest && infoSet1[i].time_latest == infoSet2[i].time_latest)
		{
			continue;//无差异，比较下一统计对象
		}
		
		//有差异，差异数增加
		cnt++;

		//输出比较信息
		printf("---%s\n", infoSet1[i].path.c_str());
		if (infoSet1[i].status != infoSet2[i].status)
			printf("\t统计状态: %s -> %s\n", infoSet1[i].statusInfo().c_str(), infoSet2[i].statusInfo().c_str());
		if (infoSet1[i].num_total != infoSet2[i].num_total)
			printf("\t文件总数: %d -> %d\n", infoSet1[i].num_total, infoSet2[i].num_total);
		if (infoSet1[i].size_total != infoSet2[i].size_total)
			printf("\t文件总大小: %d bytes -> %d bytes\n", infoSet1[i].size_total, infoSet2[i].size_total);
		if (!(infoSet1[i].time_earliest == infoSet2[i].time_earliest))
		{
			printf("\t最早时间文件:\n\t");
			infoSet1[i].time_earliest.print_node();
			printf("\t-> ");
			infoSet2[i].time_earliest.print_node();
		}
		if (!(infoSet1[i].time_latest == infoSet2[i].time_latest))
		{
			printf("\t最晚时间文件:\n\t");
			infoSet1[i].time_latest.print_node();
			printf("\t-> ");
			infoSet2[i].time_latest.print_node();
		}

	}

	printf("共比较 %d 个目录，有 %d 个差异\n", i, cnt);

	return;
}
