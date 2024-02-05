#include "scan.h"

void sql(string searchPath, string sqlPath)
{
	if (searchPath.back() != '\\')
		searchPath += '\\';
	if (sqlPath.back() != '\\')
		sqlPath += '\\';

	int fileNum = 0;//�ļ���
	int dirNum = 0;//��Ŀ¼��/�ļ�����
	_finddata_t file;//�洢�����õ����ļ���Ŀ¼
	intptr_t handle;//_findfirst()���ص��ļ����
	string temp;
	queue<string> q;
	ofstream sqlFile;

	printf("��ʼ����sql�ļ���ÿ��sql�ļ�������� %d ��sql���\n", MAXSQL);
	printf("����ɨ��Ŀ¼ %s\n", searchPath.c_str());
	printf("Ŀ¼��\t�ļ���\n");
	q.push(searchPath);
	while (!q.empty())
	{	
		temp = q.front() + "*.*";
		handle = _findfirst(temp.c_str(), &file);
		if (handle == -1)
		{
			q.pop();
			continue;
		}

		do//�����ӽ��
		{
			if (file.name[0] != '.')
			{
				if ((fileNum + dirNum) % MAXSQL == 0)//�տ�ʼɨ��(sql���Ϊ0)�򵥸��ļ�sql�����10000�����������ļ�
				{
					if (fileNum + dirNum != 0)//�ʼδ���ļ���������Ҫ�ر�
					{
						sqlFile.close();
					}
					sqlFile.open(sqlPath + to_string((fileNum + dirNum) / MAXSQL + 1) + ".sql", ios::out);
				}
				temp = q.front() + file.name;
				if (file.attrib & _A_SUBDIR)//����Ŀ¼��ѹ�����
				{
					temp += "\\";
					q.push(temp);
					dirNum++;
					sqlFile << "insert into Ŀ¼ values('" + temp + "'," + to_string(file.time_create) + "); " << endl;
				}
				else//���ļ�
				{
					fileNum++;
					sqlFile << "insert into �ļ� values('" + temp + "'," + to_string(file.size) + "," + to_string(file.time_create) + "); " << endl;
				}
				printf("%d\t%d\r", dirNum, fileNum);
			}
		} while (_findnext(handle, &file) == 0);
		q.pop();
	}
	if (fileNum + dirNum != 0)
		sqlFile.close();
	printf("\nɨ�����\n");
	printf("������ %d ��sql�ļ����洢�� %s\n", (fileNum + dirNum) / MAXSQL + 1, sqlPath.c_str());
	printf("�ܼ� %d ��sql�������\n", dirNum + fileNum);
}