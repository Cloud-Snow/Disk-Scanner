#include "scan.h"

statInfo::statInfo(string path)
{
	this->path = path;
}

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
		printf("---%s\n\t文件总数 %d\n\t总文件大小 %d bytes\n", path.c_str(), num_total, size_total);
		printf("\t最早时间文件 ");
		time_earliest.print_node();
		printf("\t最晚时间文件 ");
		time_latest.print_node();
	}
}

node* dir_tree::statNode(node* p, statInfo &info)
{
	info.num_total = 0;
	info.size_total = 0;
	if (p == NULL)//结点为空，无法统计
	{
		info.status = FAILED;
		return NULL;
	}
	
	for (p = p->child; p != NULL; p = p->brother)//遍历目录下文件
	{
		if (p->name.back() != '\\')//是文件，不是目录
		{
			if (info.time_earliest.name == "" && info.time_latest.name == "")//用第一个文件作为初值
			{
				info.time_earliest = node(p->name, p->time, p->size, NULL);//生成新结点，避免原结点被删除后丢失统计信息
				info.time_latest = node(p->name, p->time, p->size, NULL);
			}

			if (p->time < info.time_earliest.time)//寻找最早时间和最晚时间文件
				info.time_earliest = node(p->name, p->time, p->size, NULL);
			if (p->time > info.time_latest.time)
				info.time_latest = node(p->name, p->time, p->size, NULL);

			//p->print_node();
			info.num_total++;
			info.size_total += p->size;

		}
	}
	if (info.num_total == 0)//查找成功但目录中没有文件
		info.status = NOFILE;
	else//查找成功且有文件
		info.status = SUCCEED;

	return p;
}

node* dir_tree::find(string path)//查找指定目录或文件
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

bool dir_tree::mul_stat(string filePath, vector<statInfo> &infoSet)//依据文件内容批量统计目录
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

node* dir_tree::rstat(string path)//统计指定目录及其子目录
{
	if (path.back() != '\\')
		path += '\\';

	statInfo info(path);
	node* p = find(path);
	statNode(p, info);//统计指定目录
	info.print_info();

	if(p)//目录存在
		for (node* q = p->child; q != NULL; q = q->brother)//统计子目录
		{
			if (q->name.back() == '\\')//是目录
			{
				statNode(q, info);
				info.print_info();
			}
		}

	return p;
}

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
			printf("比较提前终止\n");
			break;
		}

		if (infoSet1[i].status == infoSet2[i].status && infoSet1[i].num_total == infoSet2[i].num_total && infoSet1[i].size_total == infoSet2[i].size_total &&
			infoSet1[i].time_earliest == infoSet2[i].time_earliest && infoSet1[i].time_latest == infoSet2[i].time_latest)//无差异
		{
			continue;
		}
		
		cnt++;
		printf("---%s\n", infoSet1[i].path.c_str());

		if (infoSet1[i].status != infoSet2[i].status)
			printf("统计状态 %s -> %s\n", infoSet1[i].statusInfo().c_str(), infoSet2[i].statusInfo().c_str());
		if (infoSet1[i].num_total != infoSet2[i].num_total)
			printf("文件总数 %d -> %d\n", infoSet1[i].num_total, infoSet2[i].num_total);
		if (infoSet1[i].size_total != infoSet2[i].size_total)
			printf("文件总大小 %d -> %d\n", infoSet1[i].size_total, infoSet2[i].size_total);
		if (!(infoSet1[i].time_earliest == infoSet2[i].time_earliest))
		{
			printf("最早时间文件\n");
			infoSet1[i].time_earliest.print_node();
			printf("-> ");
			infoSet2[i].time_earliest.print_node();
		}
		if (!(infoSet1[i].time_latest == infoSet2[i].time_latest))
		{
			printf("最晚时间文件\n");
			infoSet1[i].time_latest.print_node();
			printf("-> ");
			infoSet2[i].time_latest.print_node();
		}

	}

	printf("共比较 %d 个目录，有 %d 个差异\n", i, cnt);

	return;
}
