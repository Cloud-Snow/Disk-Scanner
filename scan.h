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

class node//���
{
public:
	string name;//����
	time_t time = 0;//����ʱ��
	_fsize_t size = 0;//��С
	node* child = NULL;//��һ���ӽڵ�
	node* brother = NULL;//�ֵܽ��
	node* parent = NULL;//��ĸ���

	node() = default;//Ĭ�Ϲ��캯��
	node(string name, time_t time, int size, node* parent);//��ʼ�����
	void print_node();//��ӡ��Ϣ
	bool operator== (const node& other);
};


class statInfo//��ĳ��Ŀ¼����ͳ�Ʒ����Ľ��
{
public:
	string path;//ͳ�ƶ���
	int status = -1;//ͳ��״̬
	node time_earliest;//����ʱ����
	node time_latest;//����ʱ����
	int num_total = 0;//�ļ�����
	_fsize_t size_total = 0;//�ļ��ܴ�С

	statInfo(string path);
	void print_info();//��ӡ��Ϣ
	string statusInfo();//��ͳ��״̬ת��Ϊ����
};

class dir_tree//Ŀ¼��
{
public:
	node* root = NULL;//��Ŀ¼
	int depth = 0;//�������

	void create(string searchpath, string sqlPath);//������
	int destroy(node* p);//ɾ�����p�����ӽ��, ����ɾ�������
	int cal_depth();//�����������

	node* statNode(node* p, statInfo& info);//��ָ��������ͳ��
	node* rstat(string path);//ͳ��ָ��Ŀ¼������Ŀ¼������ӡ��Ϣ������ͳ�ƶ���ָ��
	bool mul_stat(string filePath, vector<statInfo>& infoSet);//����ָ���ļ�������ͳ��Ŀ¼���������infoSet
	node* find(string path);//Ѱ��Ŀ¼���ļ�
	void cmpStat(vector<statInfo> infoSet1, vector<statInfo> infoSet2);//��ͳ����Ϣ���бȽ�

	void op_file(string opStatement);//��ָ���ļ����в���
	void op_dir(string opStatement);//��ָ��Ŀ¼���в��������ز����Ľ����
	void mul_op_file(string filePath);//���������ļ�����
	void mul_op_dir(string filePath);//��������Ŀ¼����
	void opsplit(string opStatement, string& path, string& opcode, time_t& time, _fsize_t& size);//�����������
};