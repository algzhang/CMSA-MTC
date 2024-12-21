#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <sys/stat.h>//为了判断文件夹是否存在的头文件
#include <string>
#include <map>
#include <unordered_map>
#include <ctime>
#include <fstream>

#define Max 1003
using namespace std;

const int MAX_V = 1001;
const int INF = 0x3f3f3f3f;//INF
int n, depot, Max_A;;


int w_lower = 10;//图中边权的下界
int w_upper = 100;//图中边权的上界
int g1[MAX_V][MAX_V];
int g[MAX_V][MAX_V];//存储两个点的距离
typedef struct AMGraph {							//定义图
    int vex;
    int arcs[Max][Max];//邻接矩阵
    AMGraph()=default;
}AMGGraph;
AMGraph G;


struct edge{
    int a, b;
    double w;
    edge() = default;
    edge(int _a, int _b, double _w) {
        a = _a;
        b = _b;
        w = _w;
    }
};

vector<edge> Graph;
vector<edge> RandomGraph;
vector<int> ver3;
vector<int> ver2;
vector<int> weight;

void readdat() {
    Graph.clear();
    ver3.clear();
    ver2.clear();
    weight.clear();
    string fileName = " ";//测试文件地址 
    ifstream in;           //ifstream读文件（ofstream写文件）
    in.open(fileName);
    if (!in)
    {
        cout << "打开文件出错！" << endl;
    }
    double temp2;
    int temp;
    int index = 0;
    int ding = 0;

    while (in)
    {
        in >> temp;             //第一个序号不用
        ver3.push_back(temp);
        in >> temp;
        ver2.push_back(temp);
        in >> temp2;
        weight.push_back(temp2);
    }


    for (int i = 0; i < MAX_V; ++i) {
        for (int j = 0; j < MAX_V; ++j) {
            g[i][j] = INF;
        }
    }
    for (int i = 0; i < ver3.size()-1; ++i) {
        g[ver3[i]-1][ver2[i]-1] = weight[i];
        g[ver2[i]-1][ver3[i]-1] = weight[i];
    }

    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
            if (g[i][j] < INF) {
                if (g[i][j] >= Max_A) {
                    Max_A = g[i][j];
                }
            }
        };
    }

}

vector<edge> BST;
int dis1[Max], path1[Max];							//dis保存最短路径总权值、path通过保存路径的前驱结点来保存路径
bool book1[Max];
void Prime(int B,int vex)							//基于Prime的贪心算法 
{
    double sum_weights = 0;
    for (int i = 0; i < vex; ++i) {
        book1[i]= false;
    }
    for (int i = 0; i < vex; i++)
    {
        dis1[i] = g[depot][i];						//初始化dis数组
        path1[i] = dis1[i] < INF ? depot : -1;			//初始化路径数组
    }
    book1[depot] = true;
    dis1[depot] = 0;//起点初始化
    for (int i = 0; i < vex; i++)				//遍历G.vex-1次
    {
        if (i!=depot){
            int mins = INF, u = depot;
            for (int j = 0; j < vex; j++)			//找到当前没加入集合的最短路的后驱点
            {
                if (!book1[j] && mins > dis1[j]) {
                    mins = dis1[j];
                    u = j;
                }
            }
            book1[u] = true;
            BST.emplace_back(path1[u],u,g[path1[u]][u]);//将该点加入集合
            sum_weights+=g[path1[u]][u];
            if (sum_weights>B){
                break;
            }
            for (int j = 0; j <  vex; j++)			//遍历所有其他点对其最短路进行更新（松弛操作）
            {
                if (!book1[j] && dis1[j] > g[u][j]) {
                    dis1[j] = g[u][j];		//更新最短路径值
                    path1[j] = u;						//修改j的前驱为u
                }
            }
        }
    }
}

int main() { 
    n=125;// 测试图的顶点数 
    int B=750;//预算大小 
    srand((unsigned) time(NULL));//随机数种子，用当前时间保证随机,只定义一次就OK //NOLINT
    readdat();
    depot=randInt(0, n - 1);;
    printf("depot=%d\n", depot);
    Prime(750,125);
    cout << "基于Prime算法的贪心算法的解为";
    cout << BST.size() << endl;
    return 0;
}

