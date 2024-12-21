#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <sys/stat.h>//Ϊ���ж��ļ����Ƿ���ڵ�ͷ�ļ�
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


int w_lower = 10;//ͼ�б�Ȩ���½�
int w_upper = 100;//ͼ�б�Ȩ���Ͻ�
int g1[MAX_V][MAX_V];
int g[MAX_V][MAX_V];//�洢������ľ���
typedef struct AMGraph {							//����ͼ
    int vex;
    int arcs[Max][Max];//�ڽӾ���
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
    string fileName = " ";//�����ļ���ַ 
    ifstream in;           //ifstream���ļ���ofstreamд�ļ���
    in.open(fileName);
    if (!in)
    {
        cout << "���ļ�����" << endl;
    }
    double temp2;
    int temp;
    int index = 0;
    int ding = 0;

    while (in)
    {
        in >> temp;             //��һ����Ų���
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
int dis1[Max], path1[Max];							//dis�������·����Ȩֵ��pathͨ������·����ǰ�����������·��
bool book1[Max];
void Prime(int B,int vex)							//����Prime��̰���㷨 
{
    double sum_weights = 0;
    for (int i = 0; i < vex; ++i) {
        book1[i]= false;
    }
    for (int i = 0; i < vex; i++)
    {
        dis1[i] = g[depot][i];						//��ʼ��dis����
        path1[i] = dis1[i] < INF ? depot : -1;			//��ʼ��·������
    }
    book1[depot] = true;
    dis1[depot] = 0;//����ʼ��
    for (int i = 0; i < vex; i++)				//����G.vex-1��
    {
        if (i!=depot){
            int mins = INF, u = depot;
            for (int j = 0; j < vex; j++)			//�ҵ���ǰû���뼯�ϵ����·�ĺ�����
            {
                if (!book1[j] && mins > dis1[j]) {
                    mins = dis1[j];
                    u = j;
                }
            }
            book1[u] = true;
            BST.emplace_back(path1[u],u,g[path1[u]][u]);//���õ���뼯��
            sum_weights+=g[path1[u]][u];
            if (sum_weights>B){
                break;
            }
            for (int j = 0; j <  vex; j++)			//��������������������·���и��£��ɳڲ�����
            {
                if (!book1[j] && dis1[j] > g[u][j]) {
                    dis1[j] = g[u][j];		//�������·��ֵ
                    path1[j] = u;						//�޸�j��ǰ��Ϊu
                }
            }
        }
    }
}

int main() { 
    n=125;// ����ͼ�Ķ����� 
    int B=750;//Ԥ���С 
    srand((unsigned) time(NULL));//��������ӣ��õ�ǰʱ�䱣֤���,ֻ����һ�ξ�OK //NOLINT
    readdat();
    depot=randInt(0, n - 1);;
    printf("depot=%d\n", depot);
    Prime(750,125);
    cout << "����Prime�㷨��̰���㷨�Ľ�Ϊ";
    cout << BST.size() << endl;
    return 0;
}

