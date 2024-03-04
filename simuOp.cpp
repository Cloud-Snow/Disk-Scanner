#include "scan.h"

//���ܣ���ָ���ļ����в���
//���룺����ָ���ʽΪ��"path,opcode(A/M/D),time,size"������AΪ���ӣ�MΪ�޸ģ�DΪɾ��
//���������ǰ�ļ���Ϣ������������������ļ���Ϣ
//������Ŀ¼��root��ΪNULL
bool dir_tree::op_file(string opStatement)//��ָ���ļ����в���A,M,D
{
	if (root == NULL)
	{
		printf("Ŀ¼��δ����\n");
		return false;
	}
	
	string path, opcode;//�����������
	time_t time = 0;
	_fsize_t size = 0;
	opsplit(opStatement, path, opcode, time, size);

	printf(">>>ִ�в��� %s\n", opStatement.c_str());
	printf("\t����ǰ:");
	node* q = find(path);//�����ļ�
	if (q == NULL)
		printf("�ļ�������\n");
	else
		q->print_node();

	printf("\t�������:");
	if (opcode == "A")
	{
		size_t pos = path.find_last_of("\\");
		string dirPath = path.substr(0, pos + 1);//��ȡ�ļ�����Ŀ¼
		node* p = find(dirPath);
		if (p == NULL)//�ļ�����Ŀ¼������
		{
			printf("�ļ�����Ŀ¼������, �޷����\n");
			return false;
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
			return false;
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
			return false;
		}
		else//�ҵ��ļ�
		{
			if (destroy(p))
			{
				printf("ɾ���ɹ�\n");
			}
		}
	}
	else
	{
		printf("��������\n");
		return false;
	}

	printf("\t������:");
	q = find(path);
	if (q == NULL)
		printf("�ļ�������\n");
	else
		q->print_node();
	
	return true;
}

//���ܣ���ָ��Ŀ¼���в���,��֧��ɾ������
//���룺����ָ���ʽΪ��"path,opcode(M),time,size"
//���������ǰĿ¼����Ŀ¼��Ϣ�����������������Ŀ¼����Ŀ¼��Ϣ
//������Ŀ¼��root��ΪNULL
bool dir_tree::op_dir(string opStatement)
{
	if (root == NULL)
	{
		printf("Ŀ¼��δ����\n");
		return false;
	}
	string path, opcode;//�����������
	time_t time = 0;
	_fsize_t size = 0;
	opsplit(opStatement, path, opcode, time, size);

	printf(">>>ִ�в��� %s\n", opStatement.c_str());
	if (path.back() != '\\')//Ŀ¼�淶��
		path += "\\";

	printf("����ǰ:\n");
	node* p = rstat(path);//ͳ��Ŀ¼����Ŀ¼��Ϣ������Ŀ¼ָ��

	if (opcode == "D")
	{
		printf("�������:");
		if (p == NULL)
		{
			printf("Ŀ¼�����ڣ��޷�ɾ��\n");
			return false;
		}
		else
		{
			int n = destroy(p);
			printf("ɾ���ɹ�, ɾ������� %d\n", n);
		}
	}
	else
	{
		printf("��������\n");
		return false;
	}
		
	printf("������:\n");
	rstat(path);

	return true;
}

//���ܣ������ļ����������ļ�����
//���룺���������ļ�·��
//�����1.ÿ�β����������Ϣ����op_file()��2.�����������ɹ�������ʧ�ܴ���
//������Ŀ¼��root��ΪNULL
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
	int succed = 0;
	int failed = 0;
	while (getline(f, buf) && buf != "end of files")
	{
		if (buf.empty())//��������
			continue;

		cnt++;
		if (op_file(buf))
			succed++;
		else
			failed++;
	}
	printf("ִ�в�������: %d\n", cnt);
	printf("�ɹ�����: %d\tʧ�ܴ���: %d\n", succed, failed);

	return;
}

//���ܣ������ļ���������Ŀ¼����
//���룺���������ļ�·��
//�����1.ÿ�β����������Ϣ����op_dir()��2.�����������ɹ�������ʧ�ܴ���
//������Ŀ¼��root��ΪNULL
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
	int succed = 0;
	int failed = 0;
	while (getline(f, buf) && buf != "end of dirs")
	{
		if (buf.empty())//��������
			continue;
		
		cnt++;
		if (op_dir(buf))
			succed++;
		else
			failed++;
	}
	printf("ִ�в�������: %d\n", cnt);
	printf("�ɹ�����: %d\tʧ�ܴ���: %d\n", succed, failed);

	return;
}

//���ܣ����������������и������
//���룺������䣬���ֲ������ô��룬����·����������ʽ��ʱ�䡢��С
//�����ͨ�����ô��η��ؽ��
//��������
void dir_tree::opsplit(string opStatement, string& path, string& opcode, time_t& time, _fsize_t& size)
{
	string ssize, stime;
	stringstream iss(opStatement);
	getline(iss, path, ',');
	getline(iss, opcode, ',');
	getline(iss, stime, ',');
	getline(iss, ssize, '\n');
	//������ɾ������ʱ��size��time�ֱ�Ϊ"size"��"time"����ʱ����Ϊtime��size��ֵ��
	//������������ʱ��time��sizeӦΪ���֣�Ҫ��string���͵�stime��ssizeת������Ӧ����
	if (stime != "time")
		time = stoll(stime);
	if (ssize != "size")
		size = (_fsize_t)stol(ssize);

	return;
}