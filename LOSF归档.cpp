#include<iostream>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<Windows.h>

using namespace std;
typedef vector<vector<int>> Mat;

class SortIntoStorage
{
	public:
		void FirstClassArchive(unsigned int, Mat&);
		void SecondClassArchive(Mat&);
	private:
		int thresholdValue = 32767;
};

//用随机数模拟不断加入的各种不同的文件，暂时不考虑文件相同的情况，即随机数有可能重复
//申请动态数组
void SortIntoStorage::FirstClassArchive(unsigned int n, Mat& first)
{	
	int *ary = NULL;
	vector<int> temp,totalFiles,A, B, C, D, E; //假设共有5个分布式存储容器
	for (unsigned int p = 0; p < first.size(); p++)
	{
		for (unsigned int q = 0; q < first[p].size(); q++) //传进来的first[p].size()一定是1
		{
			temp.push_back(first[p][q]); //中间变量用于ary[i]
		}
	}
	ary = new int[n + first.size()];
	if (ary == NULL)
		cout << "分配给数组的存储空间失败！" << endl;	
	srand(unsigned(time(NULL))); //用系统当前时间的无符号整数值初始化随机数生成器的启动值
	for (unsigned int i = 0; i < (n+first.size()); i++)
	{
		vector<int> choices;
		if (i < first.size())		
			ary[i] = temp[i];		
		else	ary[i] = rand(); //返回0~32767的随机值		
		for (int j = 0; j < 3; j++) //生成两个副本，随机存放在存储容器里
		{
			choices.push_back(rand() % 5+1); //在1~5内产生3个不同的随机数,选存储容器
			for (int k = 0; k < j; k++)
			{
				if (choices[j] == choices[k]) //如果与之前产生的随机数相同就重新生成再比较
				{
					choices.pop_back(); //删除最后一个元素
					choices.push_back(rand() % 5+1);
					k = -1;
				}
			}	
			for (int p = 0; p < 1; p++) //配合break，跳出循环仍停在j循环里
			{
				switch (choices[j])
				{
					case 1:A.push_back(ary[i]); 
						break;
					case 2:B.push_back(ary[i]); 
						break;
					case 3:C.push_back(ary[i]); 
						break;
					case 4:D.push_back(ary[i]); 
						break;
					case 5:E.push_back(ary[i]); 
						break;
					default:cout << "存放异常！" << endl;
				}
			}			
		}
		totalFiles.push_back(ary[i]);
	}		
	first.clear();
	first.push_back(totalFiles); //存放所有文件,包括从first进来的已归档文件
	first.push_back(A);
	first.push_back(B);
	first.push_back(C);
	first.push_back(D);
	first.push_back(E);
	totalFiles.clear();
	A.clear();
	B.clear();
	C.clear();
	D.clear();
	E.clear();
	delete[]ary;
	ary = NULL;
}

void SortIntoStorage::SecondClassArchive(Mat &first)
{
	Mat second; //临时存储系统
	vector<int> temp;
	vector <int>::iterator Iter;
	bool flag = true; //标志跳出所有循环
	for (int i = 1; i < 6; i++)
	{
		for (unsigned int j = 0; j < first[i].size(); j++)
		{
			for (unsigned int k = 0; k < first[0].size(); k++)
			{
				if (first[0][k] == first[i][j]) //从所有文件中剔除文件直至为空
				{
					temp.push_back(first[0][k]);					
					for (Iter = first[0].begin(); Iter != first[0].end(); Iter++)
					{
						if (*Iter == first[0][k]) //删除特定位置的元素
						{
							Iter = first[0].erase(Iter);
						}
						if (Iter == first[0].end()) //控制迭代器不能超过整个容器						
							break;			
					}
					break;
				}							
			}
			if (first[0].size() == 0)
			{
				flag = false;
				break;
			}			
		}
		second.push_back(temp); //有可能为空
		temp.clear();
		if (flag == false)
			break;
	}
	first.clear();
	for (unsigned int m = 0; m < second.size(); m++)
	{
		if (second[m].size() != 0)
		{
			first.push_back(second[m]);
		}
		second[m].clear();
	}
	for (unsigned int p = 0; p < first.size(); p++)
	{
		cout <<p+1<<"号归档文件" << endl;
		for (unsigned int q = 0; q < first[p].size(); q++)
		{
			cout << first[p][q] << endl;
		}
	}
	unsigned int sizeOfFirst = first.size();
	first.clear();

	//删繁就简，将first[m]简化成int，将int赋值在rand()阈值之外，确保它与一般文件(随机数)的区别
	//降维，多维全都简化成一维
	for (unsigned int p = 0; p < sizeOfFirst; p++)
	{
		temp.push_back(++thresholdValue);
		first.push_back(temp);
		temp.clear(); //记得中间变量在使用过后清理存储，否则会导致数据重复存储
	}
}

void main()
{	
	SortIntoStorage obj1;
	Mat obj2;
	unsigned int n;
	int m=0 ,fileNumbers=0;
	int answer=1;
	unsigned int fileArchiveNumbers=0;
	int timeStart = 0;
	ofstream out("D://FOR5.txt", ios::app); //文件建立后不能随意插入数据，但可以在尾部追加数据
	if (!out)
	{
		cerr << "文件不能被打开！" << endl;
	}
	out << "#小文件归档模拟#" << endl;
	out << "A=第几次归档" << endl;
	out << "B=该次归档的文件数目" << endl;
	out << "C=归档前的文件总数目" << endl;
	out << "D=归档后的文件总数目" << endl;
	out << "A   " << "B   " << "C   " << "D   " << endl;
	while (answer<11)
	{
		int timeEnd = clock();
		if (timeEnd-timeStart>0)
		{
			n = rand() % 100 + 1; //每次产生的文件数
			cout << "第" << ++m << "次需要归档的文件数目：" << n << endl;
			fileNumbers = n + fileArchiveNumbers;
			cout << "归档前的文件总数目：" << fileNumbers << endl;
			obj1.FirstClassArchive(n, obj2);
			obj1.SecondClassArchive(obj2); //迭代		
			fileArchiveNumbers = obj2.size();
			cout << "归档后的文件总数目：" << fileArchiveNumbers << endl;
			cout << endl;
			timeStart = clock();
			out << m << "   " << n << "   " << fileNumbers << "   " << fileArchiveNumbers << "   " << endl;
			answer++;
		}
	}
	out.close();
	system("pause");
}
