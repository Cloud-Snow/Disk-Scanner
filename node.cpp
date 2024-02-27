#include "scan.h"

node::node(string name, time_t time, int size, node* parent)//创建结点
{
	this->name = name;
	this->size = size;
	this->time = time;
	this->parent = parent;
}

bool node::operator==(const node& other)
{
	if (this->name == other.name && this->size == other.size && this->time == other.time)
		return true;
	else
		return false;
}

void node::print_node()//打印结点信息
{
	if (name == "")
	{
		printf("不存在\n");
		return;
	}
	printf("%s\t", name.c_str());
	struct tm* t = localtime(&time);//将time_t转换为年月日
	printf("time: %s", timeStr(time).c_str());
	//printf("time: %d-%d-%d %d:%d:%d\t", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	if (name.back() != '\\')//不是目录
		printf("\tsize: %d bytes", size);
	
	printf("\n");
}

string timeStr(time_t time)
{
	struct tm* t = localtime(&time);
	char s[30];
	sprintf(s, "%d/%02d/%02d %d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	string str(s);

	return str;
}
