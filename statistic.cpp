#include "scan.h"

//���ܣ����캯��������ͳ����Ϣ
//���룺·����path
//�������ʼ����Ա����path
//��������
statInfo::statInfo(string path)
{
	this->path = path;
}

//���ܣ�����statinfo��Ա����status���ض�Ӧ�ַ�����Ϣ
//���룺���ó�Աstatus
//���������״̬��Ϣ�ַ���
//��������
string statInfo::statusInfo()
{
	string result;
	if (status == FAILED)
		result = "Ŀ¼������";
	else if (status == NOFILE)
		result = "Ŀ¼���ڵ�û���ļ�";
	else if (status == SUCCEED)
		result = "Ŀ¼���������ļ�";

	return result;
}

//���ܣ���ӡstatInfo��Ϣ
//���룺����statInfo���Գ�Ա
//�������Ա��Ϣ������ͳ��״̬���ļ��������ļ��ܴ�С�����������ʱ���ļ���
//��������
void statInfo::print_info()
{
	if (status == FAILED)//����ʧ��
	{
		printf("---%s\n\t������\n", path.c_str());
	}
	else if (status == NOFILE)//���ҳɹ���Ŀ¼��û���ļ�
	{
		printf("---%s\n\t���ļ�\n", path.c_str());
	}
	else if (status == SUCCEED)//���ҳɹ���Ŀ¼�����ļ�
	{
		printf("---%s\n\t�ļ����� %d\n\t�ļ��ܴ�С %d bytes\n", path.c_str(), num_total, size_total);
		printf("\t����ʱ���ļ� ");
		time_earliest.print_node();
		printf("\t����ʱ���ļ� ");
		time_latest.print_node();
	}
}

//���ܣ���ӡĳһ������ͳ����Ϣ
//���룺vector<statInfo>���ͣ����ô���
//�������ӡvector�и���statInfoԪ����Ϣ����ͳ��û���ļ���Ŀ¼���Ͳ����ڵ�Ŀ¼��
//��������
void print_set(vector<statInfo>& set)
{
	int nofile = 0;//û���ļ���Ŀ¼����
	int notExist = 0;//�����ڵ�Ŀ¼����
	for (int i = 0; i < set.size(); i++)//��ӡ��Ϣ
	{
		set[i].print_info();
		if (set[i].status == FAILED)
			notExist++;
		if (set[i].status == NOFILE)
			nofile++;
	}
	printf("ͳ��Ŀ¼�� %zd\n", set.size());
	printf("û���ļ���Ŀ¼�� %d\n", nofile);
	printf("�����ڵ�Ŀ¼�� %d\n", notExist);
	return;
}

//���ܣ�ͳ�ƽ��ָ��pָ��Ľ�����ļ���Ϣ����ͳ����Ϣ����statInfo���͵�info��
//���룺nodeָ��p��statInfo info���ô���
//�������ͳ�ƵĽ��ָ��p��ͳ����Ϣ����info��
//������p��ΪNULL
node* dir_tree::statNode(node* p, statInfo &info)
{
	if (p == NULL)//���Ϊ�գ��޷�ͳ��
	{
		info.status = FAILED;
		return NULL;
	}

	for (node*q  = p->child; q != NULL; q = q->brother)//����Ŀ¼���ļ�
	{
		if (q->name.back() != '\\')//���ļ�������Ŀ¼
		{
			if (info.time_earliest.name == "" && info.time_latest.name == "")//�õ�һ���ļ���Ϊ��ֵ
			{
				info.time_earliest = node(q->name, q->time, q->size, NULL);//�����½�㣬����ԭ��㱻ɾ����ʧͳ����Ϣ
				info.time_latest = node(q->name, q->time, q->size, NULL);
			}

			if (q->time < info.time_earliest.time)//Ѱ������ʱ�������ʱ���ļ�
				info.time_earliest = node(q->name, q->time, q->size, NULL);
			if (q->time > info.time_latest.time)
				info.time_latest = node(q->name, q->time, q->size, NULL);

			//q->print_node();
			info.num_total++;
			info.size_total += q->size;

		}
	}
	if (info.num_total == 0)//���ҳɹ���Ŀ¼��û���ļ�
		info.status = NOFILE;
	else//���ҳɹ������ļ�
		info.status = SUCCEED;

	return p;
}

//���ܣ�ͳ��ָ��Ŀ¼������Ŀ¼�ļ���Ϣ����ӡ
//���룺��Ҫͳ�Ƶ�Ŀ¼·��string path
//�������Ӧ�Ľ��ָ�룬�����估��Ŀ¼���ļ���Ϣ��ӡ
//��������
node* dir_tree::rstat(string path)
{
	if (path.back() != '\\')
		path += '\\';

	statInfo info(path);
	node* p = find(path);
	statNode(p, info);//ͳ��ָ��Ŀ¼
	info.print_info();

	if (p)//Ŀ¼����
		for (node* q = p->child; q != NULL; q = q->brother)//ͳ����Ŀ¼
		{
			if (q->name.back() == '\\')//ֻ��Ŀ¼����ͳ��
			{
				statInfo info2(q->name);
				statNode(q, info2);
				info2.print_info();
			}
		}

	return p;
}

//���ܣ���Ŀ¼���в���ָ��Ŀ¼���ļ�
//���룺���Ҷ���·��path
//��������Ҳ�������NULL���ҵ��򷵻ض�Ӧ���ָ��
//��������
node* dir_tree::find(string path)
{

	string path_lower, dir_lower;
	node* p = root;

	path_lower = path;
	transform(path_lower.begin(), path_lower.end(), path_lower.begin(), ::tolower);//��ת��ΪСд�����ڲ��ִ�Сд�Ƚ�

	while (p != NULL)//����
	{
		dir_lower = p->name;
		transform(dir_lower.begin(), dir_lower.end(), dir_lower.begin(), ::tolower);
		if (path_lower == dir_lower)//���ҳɹ�
		{
			break;
		}
		else if (dir_lower.back() == '\\' && path_lower.find(dir_lower) == 0)//����һ��Ŀ¼��
		{
			p = p->child;
		}
		else//ƥ����һ��ͬ��Ŀ¼���ļ�
		{
			p = p->brother;
		}
	}

	return p;
}

//���ܣ�����ͳ��Ŀ¼���ļ���Ϣ
//���룺����ͳ���ļ�·�������ڴ洢ͳ����Ϣ��infoSet����
//�������ͳ����Ϣ����infoSet
//������root��ΪNULL������ͳ���ļ�����
bool dir_tree::mul_stat(string filePath, vector<statInfo> &infoSet)
{
	if (root == NULL)
	{
		printf("Ŀ¼��δ����\n");
		return false;
	}

	printf("���������ļ� %s ����ͳ��Ŀ¼\n", filePath.c_str());
	ifstream f(filePath);//��ȡ�ļ�
	if (!f)
	{
		printf("��ȡ�ļ� %s ʧ��\n", filePath.c_str());
		return false;
	}
	
	string buf;
	getline(f, buf);
	if (buf != "stat dirs")
	{
		printf("�ļ���ʽ��������ӦΪ\"stat dirs\"\n");
		return false;
	}

	infoSet.clear();//���
	while (getline(f, buf) && buf != "end of dirs")
	{
		if (buf.empty())//��������
			continue;

		if (buf.back() != '\\')
			buf += '\\';

		statInfo info(buf);
		node* p = find(buf);//����Ŀ¼
		statNode(p, info);
		//info.print_info();//��ӡ��Ϣ
		infoSet.push_back(info);//��������
	}
	f.close();

	return true;
}

//���ܣ��Ƚ�ͳ����Ϣ����ʾ����
//���룺����ͳ����Ϣ����infoSet1��infoSet2
//������Ƚϵõ��Ĳ����Լ���������
//����������ͳ����Ϣ����ͬһ������ͳ���ļ�ͳ�ƶ���
void dir_tree::cmpStat(vector<statInfo> infoSet1, vector<statInfo> infoSet2)
{
	if (infoSet1.size() != infoSet2.size())
	{
		printf("ͳ�ƶ���ͬ�� �޷��Ƚ�\n");
		return;
	}

	int cnt = 0;//������
	int i = 0;
	for (i = 0; i < infoSet1.size(); i++)
	{
		if (infoSet1[i].path != infoSet2[i].path)
		{
			printf("ͳ�ƶ���ͬ�� �޷��Ƚ�\n");
			printf("�Ƚ���ǰ��ֹ������λ�ã��� %d ��ͳ����Ϣ\n", i + 1);
			break;
		}

		if (infoSet1[i].status == infoSet2[i].status && infoSet1[i].num_total == infoSet2[i].num_total && infoSet1[i].size_total == infoSet2[i].size_total &&
			infoSet1[i].time_earliest == infoSet2[i].time_earliest && infoSet1[i].time_latest == infoSet2[i].time_latest)
		{
			continue;//�޲��죬�Ƚ���һͳ�ƶ���
		}
		
		//�в��죬����������
		cnt++;

		//����Ƚ���Ϣ
		printf("---%s\n", infoSet1[i].path.c_str());
		if (infoSet1[i].status != infoSet2[i].status)
			printf("\tͳ��״̬: %s -> %s\n", infoSet1[i].statusInfo().c_str(), infoSet2[i].statusInfo().c_str());
		if (infoSet1[i].num_total != infoSet2[i].num_total)
			printf("\t�ļ�����: %d -> %d\n", infoSet1[i].num_total, infoSet2[i].num_total);
		if (infoSet1[i].size_total != infoSet2[i].size_total)
			printf("\t�ļ��ܴ�С: %d bytes -> %d bytes\n", infoSet1[i].size_total, infoSet2[i].size_total);
		if (!(infoSet1[i].time_earliest == infoSet2[i].time_earliest))
		{
			printf("\t����ʱ���ļ�:\n\t");
			infoSet1[i].time_earliest.print_node();
			printf("\t-> ");
			infoSet2[i].time_earliest.print_node();
		}
		if (!(infoSet1[i].time_latest == infoSet2[i].time_latest))
		{
			printf("\t����ʱ���ļ�:\n\t");
			infoSet1[i].time_latest.print_node();
			printf("\t-> ");
			infoSet2[i].time_latest.print_node();
		}

	}

	printf("���Ƚ� %d ��Ŀ¼���� %d ������\n", i, cnt);

	return;
}
