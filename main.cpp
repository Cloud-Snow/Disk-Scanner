#include "scan.h"

int main()
{
	string searchPath;//ɨ��Ŀ¼·��
	string sqlPath;//sql�ļ�����·��
	string statPath;//����ͳ���ļ�·��
	string opfilePath;//�����ļ������ļ�·��
	string opdirPath;//����Ŀ¼�����ļ�·��

	int choice;
	int cnt = 0;//ͳ�ƴ���
	dir_tree tree;
	vector<vector<statInfo> >Sets;//����ͳ����Ϣ�ļ���

	printf("Welcome To The Disk Scanner!\n");
	do
	{
		printf("--------------------------------\n");
		printf("��ѡ����\n");
		printf("0 �˳�\n");
		printf("1 ɨ��(����sql�ļ���Ŀ¼��)\n");
		printf("2 ͳ��Ŀ¼���ļ���Ϣ\n");
		printf("3 ģ���ļ�����\n");
		printf("4 ģ��Ŀ¼����\n");
		printf("5 ɾ��Ŀ¼��\n");
		printf("6 �Ƚ�ͳ����Ϣ\n");
		printf("7 ��ѯͳ����Ϣ\n");
		printf("8 ��ѯ�ļ���Ϣ\n");
		printf("--------------------------------\n");
		if (cin.fail())//cin��ȡ���󣬲���������ѭ��
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');//��ջ�����
		}
		cin >> choice;
		cin.ignore();//���Ի��з�

		if (cin.fail())//cin��ȡ����������������
			continue;

		if (choice == 1)//ɨ��
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

			tree.create(searchPath, sqlPath);//����Ŀ¼��������sql�ļ�
		}
		else if (choice == 2)//ͳ���ļ���Ϣ
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
				getline(cin, path);
				if (path.back() != '\\')
					path += '\\';

				statInfo info(path);
				node* p = tree.find(path);
				tree.statNode(p, info);
				info.print_info();

				if (p != NULL && p->child != NULL)
				{
					printf("---Ŀ¼��������---\n");
					for (p = p->child; p != NULL; p = p->brother)
					{
						p->print_node();
					}
				}

			}
			else if(choice2 == 2)
			{
				cnt++;
				printf("��%d��ͳ��\n", cnt);
				printf("������ͳ���ļ�·��\n");
				getline(cin, statPath);

				vector<statInfo> infoSet;
				if (!tree.mul_stat(statPath, infoSet))//��ʼͳ��
				{
					cnt--;
					continue;//ͳ��ʧ��
				}
				Sets.push_back(infoSet);//�洢���
				print_set(Sets[cnt - 1]);
			}

		}
		else if (choice == 3)//ģ���ļ�����
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
				getline(cin, opfilePath);
				tree.mul_op_file(opfilePath);
			}
		}
		else if (choice == 4)//ģ��Ŀ¼����
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
				getline(cin, opdirPath);
				tree.mul_op_dir(opdirPath);
			}
		}
		else if (choice == 5)//ɾ��Ŀ¼��
		{
			if (tree.root == NULL)
			{
				printf("Ŀ¼����δ����\n");
				continue;
			}
			string choice2;
			printf("Ŀ¼����Ŀ¼Ϊ %s ,�Ƿ�ɾ����(y/n)\n", tree.root->name.c_str());
			cin >> choice2;
			if (choice2 == "y")
			{
				printf("����ɾ��Ŀ¼��\n");
				int n = tree.destroy(tree.root);
				tree.root = NULL;
				printf("ɾ���ɹ���ɾ������� %d\n", n);
			}
		}
		else if (choice == 6)//�Ƚ�ͳ����Ϣ
		{
			printf("���� %d ��ͳ����Ϣ\n", cnt);
			if (cnt == 0)
			{
				printf("�����һ������ͳ��\n");
				continue;
			}

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
		else if (choice == 7)//��ѯͳ����Ϣ
		{
			printf("���� %d ��ͳ����Ϣ\n", cnt);
			if (cnt == 0)
			{
				printf("�����һ������ͳ��\n");
				continue;
			}

			printf("��ָ����Ҫ��ѯ��ͳ����Ϣ\n");
			int i = 0;
			cin >> i;
			if (i > cnt || i <= 0)
			{
				printf("�����ڵ� %d ��ͳ����Ϣ\n", i);
				continue;
			}

			print_set(Sets[cnt - 1]);
		}
		else if(choice == 8)//��ѯ�ļ���Ϣ
		{
			if (tree.root == NULL)
			{
				printf("Ŀ¼����δ����\n");
				continue;
			}

			printf("������ȫ·���ļ���\n");
			string name;
			getline(cin, name);
			node* p = tree.find(name);
			if (p == NULL)
			{
				printf("�ļ�������\n");
			}
			else
			{
				p->print_node();
			}
		}
		else if(choice == 0)//�˳�
		{
			printf("Bye~\n");
			break;
		}
	} while (true);

	return 0;
}