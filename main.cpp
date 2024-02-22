#include "scan.h"

int main()
{
	string searchPath;//扫描目录路径
	string sqlPath;//sql文件生成路径
	string statPath;//批量统计文件路径
	string opfilePath;//批量文件操作文件路径
	string opdirPath;//批量目录操作文件路径

	int choice;
	int cnt = 0;//统计次数
	dir_tree tree;
	vector<vector<statInfo> >Sets;//批量统计信息的集合

	printf("Welcome To The Disk Scanner!\n");
	do
	{
		printf("--------------------------------\n");
		printf("请选择功能\n");
		printf("0 退出\n");
		printf("1 扫描(生成sql文件和目录树)\n");
		printf("2 统计目录下文件信息\n");
		printf("3 模拟文件操作\n");
		printf("4 模拟目录操作\n");
		printf("5 删除目录树\n");
		printf("6 比较统计信息\n");
		printf("7 查询统计信息\n");
		printf("8 查询文件信息\n");
		printf("--------------------------------\n");
		if (cin.fail())//cin读取错误，不处理将无限循环
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');//清空缓冲区
		}
		cin >> choice;
		cin.ignore();//忽略换行符

		if (cin.fail())//cin读取错误，跳过后续操作
			continue;

		if (choice == 1)//扫描
		{
			if (tree.root)
			{
				cout << "目录树已创建，其根目录为 " << tree.root->name << endl;
				continue;
			}
			printf("请输入需要扫描的目录（如C:\\Windows\\）\n");
			getline(cin, searchPath);
			printf("请输入sql文件的生成位置(不需要文件名)\n");
			getline(cin, sqlPath);

			tree.create(searchPath, sqlPath);//创建目录树并生成sql文件
		}
		else if (choice == 2)//统计文件信息
		{
			if (tree.root == NULL)
			{
				printf("目录树尚未创建\n");
				continue;
			}

			int choice2 = 0;
			printf("1 单个目录统计（不计入统计次数）\t2批量统计\n");
			cin >> choice2;
			if (choice2 == 1)
			{
				string path;
				printf("请输入需要统计的目录\n");
				getline(cin, path);
				if (path.back() != '\\')
					path += '\\';

				statInfo info(path);
				node* p = tree.find(path);
				tree.statNode(p, info);
				info.print_info();

				if (p != NULL && p->child != NULL)
				{
					printf("---目录内容如下---\n");
					for (p = p->child; p != NULL; p = p->brother)
					{
						p->print_node();
					}
				}

			}
			else if(choice2 == 2)
			{
				cnt++;
				printf("第%d次统计\n", cnt);
				printf("请输入统计文件路径\n");
				getline(cin, statPath);

				vector<statInfo> infoSet;
				if (!tree.mul_stat(statPath, infoSet))//开始统计
				{
					cnt--;
					continue;//统计失败
				}
				Sets.push_back(infoSet);//存储结果
				print_set(Sets[cnt - 1]);
			}

		}
		else if (choice == 3)//模拟文件操作
		{
			if (tree.root == NULL)
			{
				printf("目录树尚未创建\n");
				continue;
			}

			int choice2 = 0;
			printf("1 单条指令\t2 批量操作\n");
			cin >> choice2;

			if (choice2 == 1)
			{
				string opStatement;
				printf("请输入指令（格式：文件名,A/M/D,time,size）\n");
				cin >> opStatement;
				tree.op_file(opStatement);
			}
			else if (choice2 == 2)
			{
				printf("请输入批量操作文件路径\n");
				getline(cin, opfilePath);
				tree.mul_op_file(opfilePath);
			}
		}
		else if (choice == 4)//模拟目录操作
		{
			if (tree.root == NULL)
			{
				printf("目录树尚未创建\n");
				continue;
			}

			int choice2 = 0;
			printf("1 单条指令\t2 批量操作\n");
			cin >> choice2;

			if (choice2 == 1)
			{
				string opStatement;
				printf("请输入指令（格式：目录,D,time,size）\n");
				cin >> opStatement;
				tree.op_dir(opStatement);
			}
			else if (choice2 == 2)
			{
				printf("请输入批量操作文件路径\n");
				getline(cin, opdirPath);
				tree.mul_op_dir(opdirPath);
			}
		}
		else if (choice == 5)//删除目录树
		{
			if (tree.root == NULL)
			{
				printf("目录树尚未创建\n");
				continue;
			}
			string choice2;
			printf("目录树根目录为 %s ,是否删除？(y/n)\n", tree.root->name.c_str());
			cin >> choice2;
			if (choice2 == "y")
			{
				printf("正在删除目录树\n");
				int n = tree.destroy(tree.root);
				tree.root = NULL;
				printf("删除成功，删除结点数 %d\n", n);
			}
		}
		else if (choice == 6)//比较统计信息
		{
			printf("共有 %d 次统计信息\n", cnt);
			if (cnt == 0)
			{
				printf("请进行一次批量统计\n");
				continue;
			}

			printf("请指定需要进行比较的统计信息（如输入1 2）\n");
			int i = 0, j = 0;
			cin >> i >> j;
			if (i > cnt || j > cnt|| i <= 0 || j <= 0)
			{
				if(i > cnt || i <= 0)
					printf("不存在第 %d 次统计信息\n", i);
				if (j > cnt || i <= 0)
					printf("不存在第 %d 次统计信息\n", j);
				continue;
			}
			

			tree.cmpStat(Sets[i - 1], Sets[j - 1]);
		}
		else if (choice == 7)//查询统计信息
		{
			printf("共有 %d 次统计信息\n", cnt);
			if (cnt == 0)
			{
				printf("请进行一次批量统计\n");
				continue;
			}

			printf("请指定需要查询的统计信息\n");
			int i = 0;
			cin >> i;
			if (i > cnt || i <= 0)
			{
				printf("不存在第 %d 次统计信息\n", i);
				continue;
			}

			print_set(Sets[cnt - 1]);
		}
		else if(choice == 8)//查询文件信息
		{
			if (tree.root == NULL)
			{
				printf("目录树尚未创建\n");
				continue;
			}

			printf("请输入全路径文件名\n");
			string name;
			getline(cin, name);
			node* p = tree.find(name);
			if (p == NULL)
			{
				printf("文件不存在\n");
			}
			else
			{
				p->print_node();
			}
		}
		else if(choice == 0)//退出
		{
			printf("Bye~\n");
			break;
		}
	} while (true);

	return 0;
}