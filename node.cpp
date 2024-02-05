#include "scan.h"

node::node(string name, time_t time, int size, node* parent)//�������
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

void node::print_node()//��ӡ�����Ϣ
{
	if (name == "")
	{
		printf("������\n");
		return;
	}
	printf("%s\t", name.c_str());
	struct tm* t = localtime(&time);//��time_tת��Ϊ������
	printf("time: %d-%d-%d %d:%d:%d\t", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	if (name.back() != '\\')//����Ŀ¼
		printf("size: %d bytes\n", size);
} 