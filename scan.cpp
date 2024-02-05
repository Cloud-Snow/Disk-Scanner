#include "scan.h"

void scan(string searchPath)
{
	if (searchPath.back() != '\\')//Ŀ¼���淶��
		searchPath += '\\';

	int fileNum = 0;//�ļ���
	int dirNum = 0;//��Ŀ¼��/�ļ�����
	int depth = 0;//Ŀ¼���
	size_t maxLen = 0;//�ȫ·���ļ�������
	string fileName;//�ȫ·���ļ���
	_finddata_t file;//�洢�����õ����ļ���Ŀ¼
	intptr_t handle;//_findfirst()���ص��ļ����
	string temp;
	queue<string> q;

	printf("����ɨ��Ŀ¼ %s\n", searchPath.c_str());
	printf("Ŀ¼��\t�ļ���\n");
	q.push(searchPath);
	while (!q.empty())
	{
		size_t cnt = 0;//��ʼ�����ĵ�cnt�����
		size_t num = q.size();//��һ������
		depth++;
		while (cnt < num)//�����ò㣬����һ��
		{
			cnt++;
			temp = q.front() +"*.*";
			handle = _findfirst(temp.c_str(), &file);//c_str()��stringתΪchar*
			if (handle == -1)//Ŀ¼Ϊ��
			{
				q.pop();
				continue;
			}

			do//�����ӽ��
			{
				if (file.name[0] != '.')//ȥ��.��..Ŀ¼
				{
					temp = q.front() + file.name;//��ȫ·����
					if (file.attrib & _A_SUBDIR)//����Ŀ¼��ѹ�����
					{
						temp += "\\";//Ŀ¼���淶��
						q.push(temp);
						dirNum++;
					}
					else//���ļ�����ѹ�����
					{
						size_t len = temp.length();
						if (len > maxLen)//Ѱ���ȫ·���ļ���
						{
							maxLen = len;
							fileName = temp;
						}
						fileNum++;
					}
					//print_file(temp, file);
					printf("%d\t%d\r", dirNum, fileNum);
				}
			} while (_findnext(handle, &file) == 0);//�ҵ���һ���ļ�/Ŀ¼��ѭ��
			q.pop();//q.front()�ӽ�������
		}
	}
	printf("\nɨ�����\n");
	printf("�ļ���Ŀ¼����Ϊ %d\n", dirNum + fileNum);
	printf("Ŀ¼�����Ϊ %d\n", depth);
	printf("�ȫ·���ļ���Ϊ %s\n����Ϊ %zd\n", fileName.c_str(), maxLen);
}

void print_file(string temp, _finddata_t file)//����ļ���ϸ��Ϣ
{
	printf("%s\t%d bytes\t", temp.c_str(), file.size); 
	struct tm* time = localtime(&file.time_create);//��time_tת��Ϊ������
	printf("%d-%d-%d %d:%d:%d\n", time->tm_year+1900, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
}