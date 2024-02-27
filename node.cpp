#include "scan.h"

//功能：构造函数，创建结点
//输入：路径名name，修改时间time，大小size，父母指针parent
//输出：初始化成员变量name,size,time,parent
//条件：无
node::node(string name, time_t time, int size, node* parent)
{
	this->name = name;
	this->size = size;
	this->time = time;
	this->parent = parent;
}

//功能：==运算符重载，判断两个结点间文件属性是否相等，便于比较差异
//输入：自身结点和另一个结点other
//输出：相等返回true，否则返回false
//条件：无
bool node::operator==(const node& other)
{
	if (this->name == other.name && this->size == other.size && this->time == other.time)
		return true;
	else
		return false;
}

//功能：打印结点信息
//输入：调用自身成员变量
//输出：路径，修改时间，如果是文件还会输出大小
//条件：name不为空，即结点有对应的目录或文件
void node::print_node()
{
	if (name == "")
	{
		printf("不存在\n");
		return;
	}
	printf("%s\t", name.c_str());
	struct tm* t = localtime(&time);//将time_t转换为年月日
	printf("time: %s", timeStr(time).c_str());
	if (name.back() != '\\')//不是目录
		printf("\tsize: %d bytes", size);
	
	printf("\n");
}

//功能：将time_t time转换为可读的string
//输入：time_t time
//输出：返回可读的string
//条件：无
string timeStr(time_t time)
{
	struct tm* t = localtime(&time);
	char s[30];
	sprintf(s, "%d/%02d/%02d %d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	string str(s);

	return str;
}
