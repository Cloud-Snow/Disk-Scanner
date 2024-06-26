#include "scan.h"

//功能：1.扫描指定目录searchPath；2.将扫描信息以sql插入语句的形式写入sql文件中，存储与sqlPath目录下；3.在内存中创建以searchPath为根目录的目录树
//输入：扫描目录string searchPath，sql文件存储路径string sqlPath
//输出：1.扫描得到的信息，如文件数量，目录深度等；2.sql文件；3.以root为根结点的目录树
//条件：dir_tree成员root需要为NULL
void dir_tree::create(string searchPath, string sqlPath)//层序遍历文件目录，建立孩子兄弟树结构的目录树
{
	if (root != NULL)//避免重复创建目录树
	{
		printf("目录树已创建，其根目录为 %s\n", root->name.c_str());
		return;
	}
	
	if (searchPath.back() != '\\')//目录格式规范化
		searchPath += '\\';
	if (sqlPath.back() != '\\')
		sqlPath += '\\';


	int fileNum = 0;//总文件数
	int dirNum = 0;//总目录数
	int depth = 0;//目录深度
	size_t maxLen = 0;//最长全路径文件名长度
	string fileName;//最长全路径文件名

	_finddata_t file;//存储遍历得到的文件或目录
	intptr_t handle =  0;//_findfirst()返回的文件句柄
	string temp;//临时变量，用于拼接string
	queue<node*> node_q;//存放结点指针，用于构建子结点                            
	ofstream sqlFile;//文件流
	int sqlcnt = 0;//计数sql语句
	clock_t start = 0, end = 0;

	printf("正在扫描目录 %s\n", searchPath.c_str());

	start = clock();
	root = new node(searchPath, 0, 0, NULL);//设置目录树根结点
	node* p = root;//指向结点的指针
	node_q.push(p);

	time_t now = time(NULL);
	logFile << "-----------------------------------\n"
		<< timeStr(now) << "\n"
		<< "扫描目录: " << root->name << endl;

	int notAccessDir = 0;//没有权限访问的目录数
	char s[MAXSQLLEN];//sql语句缓冲区
	int num_total = 0;//某目录下文件总数
	_fsize_t size_total = 0;//某目录下文件总大小
	while (!node_q.empty())
	{
		size_t cnt = 0;//开始遍历的第cnt个结点
		size_t num = node_q.size();//上一层结点数
		depth++;
		while (cnt < num)//遍历该层，得下一层
		{
			cnt++;
			p = node_q.front();
			temp = p->name + "*.*";
			handle = _findfirst(temp.c_str(), &file);
			if (handle == -1)//目录不存在或没有权限
			{
				if (p == root)//所扫描的根目录不存在或没有权限时，退出扫描
				{
					node_q.pop();
					break;
				}

				if (!notAccessDir)
					logFile << "没有权限打开的目录:" << endl;
				logFile << p->name << endl;
				notAccessDir++;	
			}
			else//找到目录下第一个文件或目录，可能是.和..
			{
				num_total = 0;//重置目录下文件总数
				size_total = 0;//重置目录下文件总大小
				bool isfirst = true;//区别孩子结点和兄弟结点建立

				do//扫描目录下文件和子目录
				{
					if (strcmp(file.name, ".") && strcmp(file.name, ".."))//忽略目录.和..
					{
						if ((dirNum + fileNum) == 0)//输出表头
						{
							printf("目录数\t文件数\n");
							printf("0\t0\r");
						}

						temp = node_q.front()->name + file.name;

						if (isfirst == true)//是第一个子结点
						{
							p->child = new node(temp, file.time_write, file.size, p);
							p = p->child;
						}
						else//是第一个子节点的兄弟结点
						{
							p->brother = new node(temp, file.time_write, file.size, p);
							p = p->brother;
						}

						if (file.attrib & _A_SUBDIR)//是子目录，压入队列
						{
							p->name += "\\";
							dirNum++;

							node_q.push(p);

						}
						else//是文件
						{
							size_t len = temp.length();
							if (len > maxLen)//寻找最长全路径文件名
							{
								maxLen = len;
								fileName = temp;
							}

							if (sqlcnt % MAXSQL == 0)//刚开始扫描(sql语句为0)或单个文件sql语句满MAXSQL条，创建新文件
							{
								if (sqlFile.is_open())//如果文件已经打开，关闭
								{
									sqlFile.flush();
									sqlFile.close();
								}
								sqlFile.open(sqlPath + to_string(sqlcnt / MAXSQL + 1) + ".sql", ios::out);
								if (!sqlFile)//打开失败
								{
									cout << "\n" << sqlPath + to_string(sqlcnt / MAXSQL + 1) + ".sql" << " 打开失败" << endl;
									exit(-1);
								}

							}

							fileNum++;
							num_total++;
							size_total += file.size;
							sprintf(s, "insert into fileInfo(path,name,size,time_w_6 4,time_w) values(\'%s\',\'%s\',%lu, %lld,\'%s\');\n", p->name.c_str(), file.name, p->size, p->time, timeStr(p->time).c_str());

							sqlFile << s;
							sqlcnt++;

						}
						
						if ((dirNum + fileNum) % 1000 == 0)//间隔1000条输出扫描数量，以提高扫描速度
							printf("%d\t%d\r", dirNum, fileNum);
						//p->print_node();
						isfirst = false;
					}
				} while (_findnext(handle, &file) == 0);//_findnext成功返回0，失败返回-1，失败原因是没有更多文件
			}
			//由于需要计算目录下文件总数和总大小，故在遍历完该目录结点时输出目录信息
			node * q = node_q.front();
			if (q != root)//忽略根目录
			{
				if (sqlcnt % MAXSQL == 0)//刚开始扫描(sql语句为0)或单个文件sql语句满MAXSQL条，创建新文件
				{
					if (sqlFile.is_open())//如果文件已经打开，关闭
					{
						sqlFile.flush();
						sqlFile.close();
					}
					sqlFile.open(sqlPath + to_string(sqlcnt / MAXSQL + 1) + ".sql", ios::out);
					if (!sqlFile)//打开失败
					{
						cout << "\n" << sqlPath + to_string(sqlcnt / MAXSQL + 1) + ".sql" << " 打开失败" << endl;
						exit(-1);
					}

				}

				//分离路径得目录名
				string dirName = q->name;
				dirName.pop_back();
				size_t pos = dirName.find_last_of('\\');
				dirName = dirName.substr(pos+1, string::npos);

				sprintf(s, "insert into dirInfo(path,name,time_w_64,time_w,fileNum,fileSize) values(\'%s\',\'%s\',%lld,\'%s\',%d,%lu);\n", q->name.c_str(), dirName.c_str(), q->time, timeStr(q->time).c_str(), num_total, size_total);
				sqlFile << s;
				sqlcnt++;
			}
			node_q.pop();
		}
	}
	if (sqlcnt == 0)//有根目录不存在、没有权限或为空目录3种可能
	{

		if (handle == -1)
		{
			if (errno == EINVAL)//没有权限
			{
				logFile << "没有权限打开目录" << endl;
				printf("\n没有权限打开\n");
			}
			else if(errno == ENOENT) //不存在
			{
				logFile << "目录不存在" << endl;
				printf("\n目录不存在\n");
			}
		}
		else//为空目录
		{
			logFile << "为空目录" << endl;
			printf("\n为空目录\n");
		}
		
		delete root;
		root = NULL;
		return;
	}
		
	sqlFile.close();
	end = clock();
	printf("%d\t%d\n", dirNum, fileNum);
	double t = (double)(end - start) / CLOCKS_PER_SEC;
	printf("扫描完成，耗时%.3lfs\n", t);
	//printf("目录数\t文件数\n");
	//printf("%d\t%d\n", dirNum, fileNum);
	printf("文件和目录总数: %d\n", dirNum + fileNum);
	printf("没有权限打开的目录个数为:%d\n", notAccessDir);
	printf("目录深度: %d\n", depth);
	printf("最长全路径文件名: %s\n", fileName.c_str());
	printf("最长全路径文件名长度 %zd\n", maxLen);
	printf("目录树创建成功\n");
	printf("目录树深度: %d\n", cal_depth());
	printf("创建结点数：%d\n", sqlcnt + 1);
	printf("共生成 %d 个sql文件，存储于 %s\n", sqlcnt / MAXSQL + 1, sqlPath.c_str());
	printf("总计 %d 条sql插入语句\n", sqlcnt);

	//写入日志
	logFile << "没有权限打开的目录总数: " << notAccessDir << "\n"
			<< "扫描用时:" << fixed << setprecision(3) << t << "s\n"
			<< "文件数: " << fileNum << "\t目录数: " << dirNum << "\n"
			<< "目录深度: " << depth << "\n"
			<< "最长全路径文件名长度: " << maxLen << "\n"
			<< "目录树创建成功\n"
			<< "目录树深度: " << cal_depth() << "\n"
			<< "创建结点数：" << sqlcnt + 1 << "\n"
			<< "共生成 " << sqlcnt / MAXSQL + 1 << " 个sql文件，存储于 " << sqlPath << "\n"
			<< "总计 " << sqlcnt << " 条sql插入语句" << endl;
}

//功能：计算目录树深度
//输入：调用dir_tree成员属性root
//输出：目录树深度
//条件：root不为NULL
int dir_tree::cal_depth()//层序遍历目录树，计算树深度
{
	if (root == NULL)
	{
		printf("目录树未创立\n");
		return 0;
	}

	queue<node*> q;
	q.push(root);
	node* p = root;

	depth = 0;
	while (!q.empty())
	{
		size_t cnt = 0;
		size_t num = q.size();
		depth++;
		while (cnt < num)
		{
			p = q.front();
			//p->print_node();
			cnt++;
			if (p->child)
				q.push(p->child);
			if (p->brother)
				q.push(p->brother);
			q.pop();
		}
	}

	return depth;
}

//功能：删除结点及其child，保留brother，释放空间
//输入：结点指针node* p
//输出：删除结点数
//条件：p不为NULL
int dir_tree::destroy(node* p)
{
	if (p == NULL)//结点为空，无法删除
		return 0;

	if (p != root)
	{
		if (p == p->parent->child)//重新设置父母结点子节点
			p->parent->child = p->brother;
		else if (p == p->parent->brother)
			p->parent->brother = p->brother;
	}
	else
	{
		root = NULL;
	}

	int cnt = 0;//删除节点数
	node* r = p;
	p = p->child;
	delete r;
	r = NULL;
	cnt++;

	if (p == NULL)//目标没有子目录/文件
		return cnt;

	queue<node*> q;
	q.push(p);
	while (!q.empty())//层序遍历删除
	{
		p = q.front();
		if (p->child)
			q.push(p->child);
		if (p->brother)
			q.push(p->brother);

		q.pop();
		delete p;
		p = NULL;
		cnt++;
	}

	return cnt;
}