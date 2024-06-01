#include "scan.h"

ofstream logFile;//��־�ļ�������¼ɨ����Ϣ
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
	printf("Here are the options:\n");
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
		printf("9 �������ͳ����Ϣ\n");
		if (tree.root != NULL)
			printf("---��ǰĿ¼����Ŀ¼: %s\n", tree.root->name.c_str());
		else
			printf("---Ŀ¼��δ����\n");
		printf("---����ͳ�ƴ���: %d\n", cnt);
		printf("--------------------------------\n");
		if (cin.fail())//cin��ȡ���󣬲�����������ѭ��
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');//��ջ�����
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

			do
			{
				getline(cin, searchPath);
			} while (searchPath.empty());
			printf("������sql�ļ�������λ��(����Ҫ�ļ���)\n");
			do
			{
				getline(cin, sqlPath);
			} while (sqlPath.empty());

			logFile.open("LOG.log", ios::out | ios::app);
			tree.create(searchPath, sqlPath);//����Ŀ¼��������sql�ļ�
			logFile.close();
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
			cin.ignore();
			if (choice2 == 1)
			{
				string path;
				printf("��������Ҫͳ�Ƶ�Ŀ¼\n");
				do
				{
					getline(cin, path);
				} while (path.empty());
				if (path.back() != '\\')//ͳһĿ¼��ʽ
					path += '\\';

				statInfo info(path);
				node* p = tree.find(path);//����Ŀ¼���
				tree.statNode(p, info);//ͳ��Ŀ¼��Ϣ
				info.print_info();//���ͳ����Ϣ

				if (p != NULL && p->child != NULL)//���Ŀ¼������
				{
					printf("---Ŀ¼��������---\n");
					for (p = p->child; p != NULL; p = p->brother)
					{
						p->print_node();
					}
				}

			}
			else if (choice2 == 2)
			{
				cnt++;
				printf("��%d��ͳ��\n", cnt);
				printf("������ͳ���ļ�·��\n");
				do
				{
					getline(cin, statPath);
				} while (statPath.empty());

				vector<statInfo> infoSet;
				if (!tree.mul_stat(statPath, infoSet))//��ʼͳ��
				{
					cnt--;
					continue;//ͳ��ʧ��
				}
				Sets.push_back(infoSet);//�洢���
				print_set(Sets[cnt - 1]);//���ͳ����Ϣ
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
			cin.ignore();

			if (choice2 == 1)
			{
				string opStatement;
				printf("������ָ���ʽ���ļ���,A/M/D,time,size��\n");
				do
				{
					getline(cin, opStatement);
				} while (opStatement.empty());
				tree.op_file(opStatement);//�ļ�����
			}
			else if (choice2 == 2)
			{
				printf("�����������ļ������ļ�·��\n");
				do
				{
					getline(cin, opfilePath);
				} while (opfilePath.empty());
				tree.mul_op_file(opfilePath);//�����ļ�����
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
			cin.ignore();

			if (choice2 == 1)
			{
				string opStatement;
				printf("������ָ���ʽ��Ŀ¼,D,time,size��\n");
				do
				{
					getline(cin, opStatement);
				} while (opStatement.empty());
				tree.op_dir(opStatement);//Ŀ¼����
			}
			else if (choice2 == 2)
			{
				printf("����������Ŀ¼�����ļ�·��\n");
				do
				{
					getline(cin, opdirPath);
				} while (opdirPath.empty());
				tree.mul_op_dir(opdirPath);//����Ŀ¼����
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
			cin.ignore();
			if (choice2 == "y")
			{
				printf("����ɾ��Ŀ¼��\n");
				int n = tree.destroy(tree.root);//ɾ��Ŀ¼��
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
			cin.ignore();
			if (i > cnt || j > cnt || i <= 0 || j <= 0)
			{
				if (i > cnt || i <= 0)
					printf("�����ڵ� %d ��ͳ����Ϣ\n", i);
				if (j > cnt || i <= 0)
					printf("�����ڵ� %d ��ͳ����Ϣ\n", j);
				continue;
			}


			tree.cmpStat(Sets[i - 1], Sets[j - 1]);//�Ƚ�ͳ����Ϣ����
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
			cin.ignore();
			if (i > cnt || i <= 0)
			{
				printf("�����ڵ� %d ��ͳ����Ϣ\n", i);
				continue;
			}

			print_set(Sets[cnt - 1]);//���ͳ����Ϣ
		}
		else if (choice == 8)//��ѯ�ļ���Ϣ
		{
			if (tree.root == NULL)
			{
				printf("Ŀ¼����δ����\n");
				continue;
			}

			printf("������ȫ·���ļ���\n");
			string name;
			do
			{
				getline(cin, name);
			} while (name.empty());
			node* p = tree.find(name);//�����ļ����
			if (p == NULL)
			{
				printf("�ļ�������\n");
			}
			else
			{
				p->print_node();//����ļ���Ϣ
			}
		}
		else if (choice == 9)//���ͳ����Ϣ
		{
			printf("���� %d ��ͳ����Ϣ\n", cnt);
			if (cnt == 0)
			{
				printf("�����һ������ͳ��\n");
				continue;
			}

			printf("�Ƿ�Ҫ�������ͳ����Ϣ��(y/n)\n");
			string choice2;
			cin >> choice2;
			cin.ignore();
			if (choice2 == "y")
			{
				Sets.clear();
				cnt = 0;
				printf("���������ͳ����Ϣ\n");
			}
		}
		else if (choice == 0)//�˳�
		{
			printf("Bye~\n");
			break;
		}
	} while (true);

	return 0;
}