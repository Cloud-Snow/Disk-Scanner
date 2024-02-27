#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <io.h>
#include <stack>
#include <queue>
#include <string>
#include <time.h>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

const int MAXSQL = 100000;//���ɵ�sql�ļ����ܰ�����sql�����������
//ͳ�ƽ��״̬
const int FAILED = -1;//Ŀ¼������
const int NOFILE = 0;//Ŀ¼��û���ļ�
const int SUCCEED = 1;//Ŀ¼�����ļ�

class node//Ŀ¼�����
{
public:
	string name;//����
	time_t time = 0;//����ʱ��
	_fsize_t size = 0;//��С
	node* child = NULL;//��һ���ӽڵ�
	node* brother = NULL;//�ֵܽ��
	node* parent = NULL;//��ĸ���

	node() = default;//Ĭ�Ϲ��캯��
	node(string name, time_t time, int size, node* parent);//��ʼ����㣬����·�����ļ��޸�ʱ�䡢�ļ��󡢸�ĸ���
	void print_node();//��ӡ�����Ϣ
	bool operator== (const node& other);//���������==�����������бȽ�
};
string timeStr(time_t time);//��ʱ��ת��Ϊ�ɶ����ַ���

class statInfo//��ĳ��Ŀ¼����ͳ�Ʒ����Ľ��
{
public:
	string path;//ͳ�ƶ���
	int status = -1;//ͳ��״̬
	node time_earliest;//����ʱ����
	node time_latest;//����ʱ����
	int num_total = 0;//�ļ�����
	_fsize_t size_total = 0;//�ļ��ܴ�С

	statInfo(string path);//���캯��������·����ʼ�����Ա
	void print_info();//��ӡ��Ϣ
	string statusInfo();//��ͳ��״̬ת��Ϊ���֣�����status�������ַ���
};
void print_set(vector<statInfo>& set);//��ӡ��Ϣ��

class dir_tree//Ŀ¼��
{
public:
	node* root = NULL;//��Ŀ¼���ָ��
	int depth = 0;//�������

	void create(string searchpath, string sqlPath);//������������ɨ��Ŀ¼��sql�ļ�����·��
	int destroy(node* p);//����ָ�����ָ��p��ɾ����㼰���ӽ��, ����ɾ�������
	int cal_depth();//���㲢�����������

	node* statNode(node* p, statInfo& info);//��ָ��������ͳ�ƣ�������ָ��p��ͳ����Ϣ����ͳ�ƽ��������info�У�����p
	node* rstat(string path);//ͳ��ָ��Ŀ¼������Ŀ¼������ӡ��Ϣ������·�������ض�Ӧ���ָ�룬�������򷵻�NULL
	bool mul_stat(string filePath, vector<statInfo>& infoSet);//����ָ���ļ�������ͳ��Ŀ¼���������infoSet��ͳ�Ƴɹ�����true��ʧ�ܷ���false
	node* find(string path);//Ѱ��Ŀ¼���ļ�������·�������ض�Ӧ���ָ�룬�������򷵻�NULL
	void cmpStat(vector<statInfo> infoSet1, vector<statInfo> infoSet2);//��������ͳ����Ϣ��������бȽϣ��������

	bool op_file(string opStatement);//��ָ���ļ����в������������ָ��
	bool op_dir(string opStatement);//��ָ��Ŀ¼���в������������ָ��
	void mul_op_file(string filePath);//���������ļ��������������������ļ�·��
	void mul_op_dir(string filePath);//��������Ŀ¼�������������������ļ�·��
	void opsplit(string opStatement, string& path, string& opcode, time_t& time, _fsize_t& size);//��������������������ָ������ִ洢��path��opcode��time��size
};