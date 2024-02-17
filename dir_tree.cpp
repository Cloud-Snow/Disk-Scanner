#include "scan.h"


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

	int fileNum = 0;//�ļ���
	int dirNum = 0;//��Ŀ¼��/�ļ�����
	int depth = 0;//Ŀ¼���
	size_t maxLen = 0;//�ȫ·���ļ�������
	string fileName;//�ȫ·���ļ���

	_finddata_t file;//�洢�����õ����ļ���Ŀ¼
	intptr_t handle;//_findfirst()���ص��ļ����
	string temp;
	queue<node*> node_q;//��Ž��ָ�룬���ڹ����ӽ��                            
	ofstream sqlFile;//�ļ���
	clock_t start = 0, end = 0;

	printf("����ɨ��Ŀ¼ %s\n", searchPath.c_str());
	printf("Ŀ¼��\t�ļ���\n");
	printf("0\t0\r");

	start = clock();
	root = new node(searchPath, 0, 0, NULL);//����Ŀ¼�������
	node* p = root;//ָ�����ָ��
	node_q.push(p);

	while (!node_q.empty())
	{
		size_t cnt = 0;//��ʼ�����ĵ�cnt�����
		size_t num = node_q.size();//��һ������
		depth++;
		while (cnt < num)//�����ò㣬����һ��
		{
			cnt++;
			temp = node_q.front()->name + "*.*";
			handle = _findfirst(temp.c_str(), &file);
			if (handle == -1)//Ŀ¼Ϊ��
			{
				node_q.pop();
				continue;
			}
			p = node_q.front();

			bool isfirst = true;
			do//�����ӽ��
			{
				if (strcmp(file.name, ".") && strcmp(file.name, ".."))//����Ŀ¼.��..
				{
					if ((fileNum + dirNum) % MAXSQL == 0)//�տ�ʼɨ��(sql���Ϊ0)�򵥸��ļ�sql�����10000�����������ļ�
					{
						if (fileNum + dirNum != 0)//�ʼδ���ļ���������Ҫ�ر�
						{
							sqlFile.close();
						}
						sqlFile.open(sqlPath + to_string((fileNum + dirNum) / MAXSQL + 1) + ".sql", ios::out);
						if (!sqlFile)//��ʧ��
						{
							cout << sqlPath + to_string((fileNum + dirNum) / MAXSQL + 1) + ".sql" << " ��ʧ��" << endl;
							exit(-1);
						}

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
						sqlFile << "insert into Ŀ¼ values('" + temp + "'," + to_string(file.time_write) + "); " << endl;
					}
					else//���ļ�
					{
						size_t len = temp.length();
						if (len > maxLen)//Ѱ���ȫ·���ļ���
						{
							maxLen = len;
							fileName = temp;
						}

						fileNum++;
						sqlFile << "insert into �ļ� values('" + temp + "'," + to_string(file.time_write) + "," + to_string(file.size) + "); " << endl;
					}
			  		printf("%d\t%d\r", dirNum, fileNum);
					//p->print_node();
					isfirst = false;
				}
			} while (_findnext(handle, &file) == 0);
			node_q.pop();
		}
	}
	if (fileNum + dirNum == 0)
	{
		printf("\nĿ¼�����ڻ�Ϊ��Ŀ¼\n");
		delete root;
		root = NULL;
		return;
	}
		

	sqlFile.close();
	end = clock();
	printf("\nɨ����ɣ���ʱ%.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
	printf("�ļ���Ŀ¼���� %d\n", dirNum + fileNum);
	printf("Ŀ¼��� %d\n", depth);
	printf("�ȫ·���ļ��� %s\n", fileName.c_str());
	printf("�ȫ·���ļ������� % zd\n", maxLen);
	printf("Ŀ¼�������ɹ�\n");
	printf("Ŀ¼����� %d\n", cal_depth());
	printf("��������� %d\n", dirNum + fileNum + 1);
	printf("������ %d ��sql�ļ����洢�� %s\n", (fileNum + dirNum) / MAXSQL + 1, sqlPath.c_str());
	printf("�ܼ� %d ��sql�������\n", dirNum + fileNum);
}

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

	int cnt = 0;//ɾ���ڵ���
	node* r = p;
	p = p->child;
	delete r;
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
		cnt++;
	}

	return cnt;
}