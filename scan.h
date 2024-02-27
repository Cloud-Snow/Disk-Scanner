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

const int MAXSQL = 100000;//生成的sql文件所能包含的sql语句数量上限
//统计结果状态
const int FAILED = -1;//目录不存在
const int NOFILE = 0;//目录中没有文件
const int SUCCEED = 1;//目录中有文件

class node//目录树结点
{
public:
	string name;//名称
	time_t time = 0;//创建时间
	_fsize_t size = 0;//大小
	node* child = NULL;//第一个子节点
	node* brother = NULL;//兄弟结点
	node* parent = NULL;//父母结点

	node() = default;//默认构造函数
	node(string name, time_t time, int size, node* parent);//初始化结点，输入路径、文件修改时间、文件大、父母结点
	void print_node();//打印结点信息
	bool operator== (const node& other);//重载运算符==，方便结点间进行比较
};
string timeStr(time_t time);//将时间转换为可读的字符串

class statInfo//对某个目录进行统计分析的结果
{
public:
	string path;//统计对象
	int status = -1;//统计状态
	node time_earliest;//最早时间结点
	node time_latest;//最晚时间结点
	int num_total = 0;//文件总数
	_fsize_t size_total = 0;//文件总大小

	statInfo(string path);//构造函数，输入路径初始化类成员
	void print_info();//打印信息
	string statusInfo();//将统计状态转换为文字，调用status，返回字符串
};
void print_set(vector<statInfo>& set);//打印信息集

class dir_tree//目录树
{
public:
	node* root = NULL;//根目录结点指针
	int depth = 0;//树的深度

	void create(string searchpath, string sqlPath);//创建树，输入扫描目录和sql文件生成路径
	int destroy(node* p);//输入指向结点的指针p，删除结点及其子结点, 返回删除结点数
	int cal_depth();//计算并返回树的深度

	node* statNode(node* p, statInfo& info);//对指定结点进行统计，输入结点指针p和统计信息，将统计结果保存在info中，返回p
	node* rstat(string path);//统计指定目录及其子目录，并打印信息，输入路径，返回对应结点指针，不存在则返回NULL
	bool mul_stat(string filePath, vector<statInfo>& infoSet);//依据指定文件，批量统计目录，结果存于infoSet，统计成功返回true，失败返回false
	node* find(string path);//寻找目录或文件，输入路径，返回对应结点指针，不存在则返回NULL
	void cmpStat(vector<statInfo> infoSet1, vector<statInfo> infoSet2);//输入两个统计信息，对其进行比较，输出差异

	bool op_file(string opStatement);//对指定文件进行操作，输入操作指令
	bool op_dir(string opStatement);//对指定目录进行操作，输入操作指令
	void mul_op_file(string filePath);//批量进行文件操作，输入批量操作文件路径
	void mul_op_dir(string filePath);//批量进行目录操作，输入批量操作文件路径
	void opsplit(string opStatement, string& path, string& opcode, time_t& time, _fsize_t& size);//分离操作参数，输入操作指令，各部分存储于path、opcode、time、size
};