#include "scan.h"

void dir_tree::op_file(string opStatement)//��ָ���ļ����в���A,M,D
{
	if (root == NULL)
	{
		printf("Ŀ¼��δ����\n");
		return;
	}
	
	string path, opcode;//�����������
	time_t time = 0;
	_fsize_t size = 0;
	opsplit(opStatement, path, opcode, time, size);

	printf("-------ִ�в��� %s\n", opStatement.c_str());
	printf("����ǰ��");
	node* q = find(path);//�����ļ�
	if (q == NULL)
		printf("�ļ�������\n");
	else
		q->print_node();

	printf("���������");
	if (opcode == "A")
	{
		size_t pos = path.find_last_of("\\");
		string dirPath = path.substr(0, pos + 1);//��ȡ�ļ�����Ŀ¼
		node* p = find(dirPath);
		if (p == NULL)//�ļ�����Ŀ¼������
		{
			printf("�ļ�����Ŀ¼������, �޷����\n");
			return;
		}

		if (p->child == NULL)//Ŀ¼��û���ļ�/��Ŀ¼
		{
			p->child = new node(path, time, size, p);//����ļ�
			printf("��ӳɹ�\n");
		}
		else
		{
			for (p = p->child; p->brother != NULL; p = p->brother);//���������һ���ֵܽ��
			p->brother = new node(path, time, size, p);
			printf("��ӳɹ�\n");
		}
	}
	else if (opcode == "M")
	{
		node* p = find(path);

		if (p == NULL)
		{
			printf("�ļ������ڣ��޷��޸�\n");
		}
		else//�ҵ��ļ�
		{
			p->size = size;
			p->time = time;
			printf("�޸ĳɹ�\n");
		}
	}
	else if (opcode == "D")
	{
		node* p = find(path);

		if (p == NULL)
		{
			printf("�ļ������ڣ��޷�ɾ��\n");
		}
		else//�ҵ��ļ�
		{
			if(destroy(p))
				printf("ɾ���ɹ�\n");
		}
	}
	else
		printf("��������\n");

	printf("������");
	q = find(path);
	if (q == NULL)
		printf("�ļ�������\n");
	else
		q->print_node();
	
	return;
}

void dir_tree::op_dir(string opStatement)//��ָ��Ŀ¼���в���, ������ɾ������
{
	if (root == NULL)
	{
		printf("Ŀ¼��δ����\n");
		return;
	}
	string path, opcode;//�����������
	time_t time = 0;
	_fsize_t size = 0;
	opsplit(opStatement, path, opcode, time, size);

	printf("------ִ�в��� %s\n", opStatement.c_str());
	if (path.back() != '\\')//Ŀ¼�淶��
		path += "\\";

	printf("����ǰ\n");
	node* p = rstat(path);//ͳ��Ŀ¼����Ŀ¼��Ϣ������Ŀ¼ָ��

	if (opcode == "D")
	{
		printf("�������:");
		if (p == NULL)
		{
			printf("Ŀ¼�����ڣ��޷�ɾ��\n");
		}
		else
		{
			int n = destroy(p);
			if (n)
				printf("ɾ���ɹ�, ɾ������� %d\n", n);
			else
				printf("���Ϊ�գ��޷�ɾ��\n");
		}
	}
	else
		printf("��������\n");

	printf("������\n");
	rstat(path);
	
	return;
}

void dir_tree::mul_op_file(string filePath)
{
	if (root == NULL)
	{
		printf("Ŀ¼��δ����\n");
		return;
	}

	printf("���������ļ� %s ���������ļ�����\n", filePath.c_str());
	ifstream f;
	f.open(filePath);
	if (!f)
	{
		printf("��ȡ�ļ� %s ʧ��\n", filePath.c_str());
		return;
	}

	string buf;
	getline(f, buf);
	if (buf != "selected files")
	{
		printf("�ļ���ʽ��������ӦΪ\"selected files\"\n");
		return;
	}

	int cnt = 0;
	string path, opcode, size, time;
	while (getline(f, buf) && buf != "end of files")
	{
		cnt++;
		op_file(buf);
	}
	printf("ִ�в������� %d\n", cnt);
	return;
}

void dir_tree::mul_op_dir(string filePath)
{
	if (root == NULL)
	{
		printf("Ŀ¼��δ����\n");
		return;
	}

	printf("���������ļ� %s ��������Ŀ¼����\n", filePath.c_str());
	ifstream f;
	f.open(filePath);
	if (!f)
	{
		printf("��ȡ�ļ� %s ʧ��\n", filePath.c_str());
		return;
	}

	string buf;
	getline(f, buf);
	if (buf != "selected dirs")
	{
		printf("�ļ���ʽ��������ӦΪ\"selected dirs\"\n");
		return;
	}

	int cnt = 0;
	string path, opcode, size, time;
	while (getline(f, buf) && buf != "end of dirs")
	{
		cnt++;
		op_dir(buf);
	}
	printf("ִ�в������� %d\n", cnt);

	return;
}

void dir_tree::opsplit(string opStatement, string& path, string& opcode, time_t& time, _fsize_t& size)//�����������
{
	string ssize, stime;
	stringstream iss(opStatement);
	getline(iss, path, ',');
	getline(iss, opcode, ',');
	getline(iss, stime, ',');
	getline(iss, ssize, '\n');
	if (stime != "time")
		time = (time_t)stoi(stime);
	if (ssize != "size")
		size = (_fsize_t)stoi(ssize);

	return;
}