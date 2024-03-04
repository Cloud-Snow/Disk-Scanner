#include "scan.h"

//���ܣ�1.ɨ��ָ��Ŀ¼searchPath��2.��ɨ����Ϣ��sql����������ʽд��sql�ļ��У��洢��sqlPathĿ¼�£�3.���ڴ��д�����searchPathΪ��Ŀ¼��Ŀ¼��
//���룺ɨ��Ŀ¼string searchPath��sql�ļ��洢·��string sqlPath
//�����1.ɨ��õ�����Ϣ�����ļ�������Ŀ¼��ȵȣ�2.sql�ļ���3.��rootΪ������Ŀ¼��
//������dir_tree��Աroot��ҪΪNULL
void dir_tree::create(string searchPath, string sqlPath)//��������ļ�Ŀ¼�����������ֵ����ṹ��Ŀ¼��
{
	if (root != NULL)//�����ظ�����Ŀ¼��
	{
		printf("Ŀ¼���Ѵ��������Ŀ¼Ϊ %s\n", root->name.c_str());
		return;
	}
	
	if (searchPath.back() != '\\')//Ŀ¼��ʽ�淶��
		searchPath += '\\';
	if (sqlPath.back() != '\\')
		sqlPath += '\\';


	int fileNum = 0;//���ļ���
	int dirNum = 0;//��Ŀ¼��
	int depth = 0;//Ŀ¼���
	size_t maxLen = 0;//�ȫ·���ļ�������
	string fileName;//�ȫ·���ļ���

	_finddata_t file;//�洢�����õ����ļ���Ŀ¼
	intptr_t handle =  0;//_findfirst()���ص��ļ����
	string temp;//��ʱ����������ƴ��string
	queue<node*> node_q;//��Ž��ָ�룬���ڹ����ӽ��                            
	ofstream sqlFile;//�ļ���
	int sqlcnt = 0;//����sql���
	clock_t start = 0, end = 0;

	printf("����ɨ��Ŀ¼ %s\n", searchPath.c_str());

	start = clock();
	root = new node(searchPath, 0, 0, NULL);//����Ŀ¼�������
	node* p = root;//ָ�����ָ��
	node_q.push(p);

	time_t now = time(NULL);
	logFile << "-----------------------------------\n"
		<< timeStr(now) << "\n"
		<< "ɨ��Ŀ¼: " << root->name << endl;

	int notAccessDir = 0;//û��Ȩ�޷��ʵ�Ŀ¼��
	char s[MAXSQLLEN];//sql��仺����
	int num_total = 0;//ĳĿ¼���ļ�����
	_fsize_t size_total = 0;//ĳĿ¼���ļ��ܴ�С
	while (!node_q.empty())
	{
		size_t cnt = 0;//��ʼ�����ĵ�cnt�����
		size_t num = node_q.size();//��һ������
		depth++;
		while (cnt < num)//�����ò㣬����һ��
		{
			cnt++;
			p = node_q.front();
			temp = p->name + "*.*";
			handle = _findfirst(temp.c_str(), &file);
			if (handle == -1)//Ŀ¼�����ڻ�û��Ȩ��
			{
				if (p == root)//��ɨ��ĸ�Ŀ¼�����ڻ�û��Ȩ��ʱ���˳�ɨ��
				{
					node_q.pop();
					break;
				}

				if (!notAccessDir)
					logFile << "û��Ȩ�޴򿪵�Ŀ¼:" << endl;
				logFile << p->name << endl;
				notAccessDir++;	
			}
			else//�ҵ�Ŀ¼�µ�һ���ļ���Ŀ¼��������.��..
			{
				num_total = 0;//����Ŀ¼���ļ�����
				size_total = 0;//����Ŀ¼���ļ��ܴ�С
				bool isfirst = true;//�����ӽ����ֵܽ�㽨��

				do//ɨ��Ŀ¼���ļ�����Ŀ¼
				{
					if (strcmp(file.name, ".") && strcmp(file.name, ".."))//����Ŀ¼.��..
					{
						if ((dirNum + fileNum) == 0)//�����ͷ
						{
							printf("Ŀ¼��\t�ļ���\n");
							printf("0\t0\r");
						}

						temp = node_q.front()->name + file.name;

						if (isfirst == true)//�ǵ�һ���ӽ��
						{
							p->child = new node(temp, file.time_write, file.size, p);
							p = p->child;
						}
						else//�ǵ�һ���ӽڵ���ֵܽ��
						{
							p->brother = new node(temp, file.time_write, file.size, p);
							p = p->brother;
						}

						if (file.attrib & _A_SUBDIR)//����Ŀ¼��ѹ�����
						{
							p->name += "\\";
							dirNum++;

							node_q.push(p);

						}
						else//���ļ�
						{
							size_t len = temp.length();
							if (len > maxLen)//Ѱ���ȫ·���ļ���
							{
								maxLen = len;
								fileName = temp;
							}

							if (sqlcnt % MAXSQL == 0)//�տ�ʼɨ��(sql���Ϊ0)�򵥸��ļ�sql�����MAXSQL�����������ļ�
							{
								if (sqlFile.is_open())//����ļ��Ѿ��򿪣��ر�
								{
									sqlFile.flush();
									sqlFile.close();
								}
								sqlFile.open(sqlPath + to_string(sqlcnt / MAXSQL + 1) + ".sql", ios::out);
								if (!sqlFile)//��ʧ��
								{
									cout << "\n" << sqlPath + to_string(sqlcnt / MAXSQL + 1) + ".sql" << " ��ʧ��" << endl;
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
						
						if ((dirNum + fileNum) % 1000 == 0)//���1000�����ɨ�������������ɨ���ٶ�
							printf("%d\t%d\r", dirNum, fileNum);
						//p->print_node();
						isfirst = false;
					}
				} while (_findnext(handle, &file) == 0);//_findnext�ɹ�����0��ʧ�ܷ���-1��ʧ��ԭ����û�и����ļ�
			}
			//������Ҫ����Ŀ¼���ļ��������ܴ�С�����ڱ������Ŀ¼���ʱ���Ŀ¼��Ϣ
			node * q = node_q.front();
			if (q != root)//���Ը�Ŀ¼
			{
				if (sqlcnt % MAXSQL == 0)//�տ�ʼɨ��(sql���Ϊ0)�򵥸��ļ�sql�����MAXSQL�����������ļ�
				{
					if (sqlFile.is_open())//����ļ��Ѿ��򿪣��ر�
					{
						sqlFile.flush();
						sqlFile.close();
					}
					sqlFile.open(sqlPath + to_string(sqlcnt / MAXSQL + 1) + ".sql", ios::out);
					if (!sqlFile)//��ʧ��
					{
						cout << "\n" << sqlPath + to_string(sqlcnt / MAXSQL + 1) + ".sql" << " ��ʧ��" << endl;
						exit(-1);
					}

				}

				//����·����Ŀ¼��
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
	if (sqlcnt == 0)//�и�Ŀ¼�����ڡ�û��Ȩ�޻�Ϊ��Ŀ¼3�ֿ���
	{

		if (handle == -1)
		{
			if (errno == EINVAL)//û��Ȩ��
			{
				logFile << "û��Ȩ�޴�Ŀ¼" << endl;
				printf("\nû��Ȩ�޴�\n");
			}
			else if(errno == ENOENT) //������
			{
				logFile << "Ŀ¼������" << endl;
				printf("\nĿ¼������\n");
			}
		}
		else//Ϊ��Ŀ¼
		{
			logFile << "Ϊ��Ŀ¼" << endl;
			printf("\nΪ��Ŀ¼\n");
		}
		
		delete root;
		root = NULL;
		return;
	}
		
	sqlFile.close();
	end = clock();
	printf("%d\t%d\n", dirNum, fileNum);
	double t = (double)(end - start) / CLOCKS_PER_SEC;
	printf("ɨ����ɣ���ʱ%.3lfs\n", t);
	//printf("Ŀ¼��\t�ļ���\n");
	//printf("%d\t%d\n", dirNum, fileNum);
	printf("�ļ���Ŀ¼����: %d\n", dirNum + fileNum);
	printf("û��Ȩ�޴򿪵�Ŀ¼����Ϊ:%d\n", notAccessDir);
	printf("Ŀ¼���: %d\n", depth);
	printf("�ȫ·���ļ���: %s\n", fileName.c_str());
	printf("�ȫ·���ļ������� %zd\n", maxLen);
	printf("Ŀ¼�������ɹ�\n");
	printf("Ŀ¼�����: %d\n", cal_depth());
	printf("�����������%d\n", sqlcnt + 1);
	printf("������ %d ��sql�ļ����洢�� %s\n", sqlcnt / MAXSQL + 1, sqlPath.c_str());
	printf("�ܼ� %d ��sql�������\n", sqlcnt);

	//д����־
	logFile << "û��Ȩ�޴򿪵�Ŀ¼����: " << notAccessDir << "\n"
			<< "ɨ����ʱ:" << fixed << setprecision(3) << t << "s\n"
			<< "�ļ���: " << fileNum << "\tĿ¼��: " << dirNum << "\n"
			<< "Ŀ¼���: " << depth << "\n"
			<< "�ȫ·���ļ�������: " << maxLen << "\n"
			<< "Ŀ¼�������ɹ�\n"
			<< "Ŀ¼�����: " << cal_depth() << "\n"
			<< "�����������" << sqlcnt + 1 << "\n"
			<< "������ " << sqlcnt / MAXSQL + 1 << " ��sql�ļ����洢�� " << sqlPath << "\n"
			<< "�ܼ� " << sqlcnt << " ��sql�������" << endl;
}

//���ܣ�����Ŀ¼�����
//���룺����dir_tree��Ա����root
//�����Ŀ¼�����
//������root��ΪNULL
int dir_tree::cal_depth()//�������Ŀ¼�������������
{
	if (root == NULL)
	{
		printf("Ŀ¼��δ����\n");
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

//���ܣ�ɾ����㼰��child������brother���ͷſռ�
//���룺���ָ��node* p
//�����ɾ�������
//������p��ΪNULL
int dir_tree::destroy(node* p)
{
	if (p == NULL)//���Ϊ�գ��޷�ɾ��
		return 0;

	if (p != root)
	{
		if (p == p->parent->child)//�������ø�ĸ����ӽڵ�
			p->parent->child = p->brother;
		else if (p == p->parent->brother)
			p->parent->brother = p->brother;
	}
	else
	{
		root = NULL;
	}

	int cnt = 0;//ɾ���ڵ���
	node* r = p;
	p = p->child;
	delete r;
	r = NULL;
	cnt++;

	if (p == NULL)//Ŀ��û����Ŀ¼/�ļ�
		return cnt;

	queue<node*> q;
	q.push(p);
	while (!q.empty())//�������ɾ��
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