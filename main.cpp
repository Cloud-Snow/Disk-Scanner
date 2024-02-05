#include "scan.h"

int main()
{
	string searchPath;//ɨ��Ŀ¼·��
	string sqlPath;//sql�ļ�����·��
	string statPath;//����ͳ���ļ�·��
	string opfilePath;//�����ļ������ļ�·��
	string opdirPath;//����Ŀ¼�����ļ�·��

	string choice;
	int cnt = 0;//ͳ�ƴ���
	dir_tree tree;
	vector<vector<statInfo> >Sets;

	printf("Welcome To The Disk Scanner!\n");
	do
	{
		printf("--------------------------------\n");
		printf("��ѡ����\n");
		printf("0 �˳�\n");
		printf("1 ɨ��(����sql�ļ���Ŀ¼��)\n");
		printf("2 ͳ���ļ���Ϣ\n");
		printf("3 ģ���ļ�����\n");
		printf("4 ģ��Ŀ¼����\n");
		printf("5 ɾ��Ŀ¼��\n");
		printf("6 �Ƚ�ͳ����Ϣ\n");
		printf("7 ��ѯͳ����Ϣ\n");
		printf("--------------------------------\n");
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');//��ջ�����
		}
		getline(cin, choice);
		if (choice == "1")//ɨ��
		{
			if (tree.root)
			{
				cout << "Ŀ¼���Ѵ��������Ŀ¼Ϊ " << tree.root->name << endl;
				continue;
			}
			printf("��������Ҫɨ���Ŀ¼����C:\\Windows\\��\n");
			getline(cin, searchPath);
			printf("������sql�ļ�������λ��(����Ҫ�ļ���)\n");
			getline(cin, sqlPath);
			tree.create(searchPath, sqlPath);
		}
		else if (choice == "2")
		{
			if (tree.root == NULL)
			{
				printf("Ŀ¼����δ����\n");
				continue;
			}

			int choice2 = 0;
			printf("1 ����Ŀ¼ͳ�ƣ�������ͳ�ƴ�����\t2����ͳ��\n");
			cin >> choice2;
			if (choice2 == 1)
			{
				string path;
				printf("��������Ҫͳ�Ƶ�Ŀ¼\n");
				cin >> path;
				statInfo info(path);
				tree.statNode(tree.find(path), info);
				info.print_info();
			}
			else if(choice2 == 2)
			{
				cnt++;
				printf("��%d��ͳ��\n", cnt);
				printf("������ͳ���ļ�·��\n");
				cin >> statPath;

				vector<statInfo> infoSet;
				if (!tree.mul_stat(statPath, infoSet))//��ʼͳ��
				{
					cnt--;
					continue;//ͳ��ʧ��
				}
				Sets.push_back(infoSet);//�洢���

				for (int i = 0; i < Sets[cnt - 1].size(); i++)//��ӡ��Ϣ
					Sets[cnt - 1][i].print_info();
				printf("ͳ��Ŀ¼�� %zd\n", Sets[cnt - 1].size());
			}

		}
		else if (choice == "3")
		{
			if (tree.root == NULL)
			{
				printf("Ŀ¼����δ����\n");
				continue;
			}

			int choice2 = 0;
			printf("1 ����ָ��\t2 ��������\n");
			cin >> choice2;

			if (choice2 == 1)
			{
				string opStatement;
				printf("������ָ���ʽ���ļ���,A/M/D,time,size��\n");
				cin >> opStatement;
				tree.op_file(opStatement);
			}
			else if (choice2 == 2)
			{
				printf("���������������ļ�·��\n");
				cin >> opfilePath;
				tree.mul_op_file(opfilePath);
			}
		}
		else if (choice == "4")
		{
			if (tree.root == NULL)
			{
				printf("Ŀ¼����δ����\n");
				continue;
			}

			int choice2 = 0;
			printf("1 ����ָ��\t2 ��������\n");
			cin >> choice2;

			if (choice2 == 1)
			{
				string opStatement;
				printf("������ָ���ʽ��Ŀ¼,D,time,size��\n");
				cin >> opStatement;
				tree.op_dir(opStatement);
			}
			else if (choice2 == 2)
			{
				printf("���������������ļ�·��\n");
				cin >> opdirPath;
				tree.mul_op_dir(opdirPath);
			}
		}
		else if (choice == "5")
		{
			if (tree.root == NULL)
			{
				printf("Ŀ¼����δ����\n");
				continue;
			}
			printf("����ɾ��Ŀ¼�������Ŀ¼Ϊ %s\n", tree.root->name.c_str());
			int n = tree.destroy(tree.root);
			tree.root = NULL;
			printf("ɾ���ɹ���ɾ������� %d\n", n);
		}
		else if (choice == "6")
		{
			printf("���� %d ��ͳ����Ϣ\n", cnt);
			printf("��ָ����Ҫ���бȽϵ�ͳ����Ϣ��������1 2��\n");
			int i = 0, j = 0;
			cin >> i >> j;
			if (i > cnt || j > cnt|| i <= 0 || j <= 0)
			{
				if(i > cnt || i <= 0)
					printf("�����ڵ� %d ��ͳ����Ϣ\n", i);
				if (j > cnt || i <= 0)
					printf("�����ڵ� %d ��ͳ����Ϣ\n", j);
				continue;
			}
			

			tree.cmpStat(Sets[i - 1], Sets[j - 1]);
		}
		else if (choice == "7")
		{
			printf("���� %d ��ͳ����Ϣ", cnt);
			printf("��ָ����Ҫ��ѯ��ͳ����Ϣ\n");
			int i = 0;
			cin >> i;
			if (i > cnt || i <= 0)
			{
				printf("�����ڵ� %d ��ͳ����Ϣ\n", i);
				continue;
			}

			for (int i = 0; i < Sets[cnt - 1].size(); i++)
			{
				Sets[cnt - 1][i].print_info();
			}
			printf("ͳ��Ŀ¼�� %zd\n", Sets[cnt - 1].size());
		}
		else if(choice == "0")
		{
			printf("Bye~\n");
			break;
		}
	} while (true);

	return 0;
}