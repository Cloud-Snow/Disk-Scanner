#include "scan.h"

//���ܣ����캯�����������
//���룺·����name���޸�ʱ��time����Сsize����ĸָ��parent
//�������ʼ����Ա����name,size,time,parent
//��������
node::node(string name, time_t time, int size, node* parent)
{
	this->name = name;
	this->size = size;
	this->time = time;
	this->parent = parent;
}

//���ܣ�==��������أ��ж����������ļ������Ƿ���ȣ����ڱȽϲ���
//���룺���������һ�����other
//�������ȷ���true�����򷵻�false
//��������
bool node::operator==(const node& other)
{
	if (this->name == other.name && this->size == other.size && this->time == other.time)
		return true;
	else
		return false;
}

//���ܣ���ӡ�����Ϣ
//���룺���������Ա����
//�����·�����޸�ʱ�䣬������ļ����������С
//������name��Ϊ�գ�������ж�Ӧ��Ŀ¼���ļ�
void node::print_node()
{
	if (name == "")
	{
		printf("������\n");
		return;
	}
	printf("%s\t", name.c_str());
	struct tm* t = localtime(&time);//��time_tת��Ϊ������
	printf("time: %s", timeStr(time).c_str());
	if (name.back() != '\\')//����Ŀ¼
		printf("\tsize: %d bytes", size);
	
	printf("\n");
}

//���ܣ���time_t timeת��Ϊ�ɶ���string
//���룺time_t time
//��������ؿɶ���string
//��������
string timeStr(time_t time)
{
	struct tm* t = localtime(&time);
	char s[30];
	sprintf(s, "%d/%02d/%02d %d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	string str(s);

	return str;
}
