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
bool eql(edge A, edge B){
    if(A.a==B.a&&A.b==B.b){
        return true;
    }
    else if(A.a==B.b&&A.b==B.a){
        return true;
    }
    else{
        return false;
    }
}
bool exist(vector<edge> &Array, edge a){
    for (int i = 0; i < Array.size(); ++i) {
        if(eql(Array[i],a)){
            return false;//��ʾ����
        }
    }
    return true;
}
bool exist1(vector<int>& Array, int a) {
    for (int i = 0; i < Array.size(); ++i) {
        if (a == Array[i]) {
            return false;//��ʾ����
        }
    }
    return true;
}
vector<edge> ValidSolution;
vector<edge> ValidSolution2;//������ŵõ��Ŀ��н�


int randInt(int a, int b) {
    return rand() % (b - a + 1) + a;//NOLINT
}

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
    string fileName = "";
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
    cout << Max_A << endl;

    for (size_t i = 0; i < ver3.size()-1; i++)
    {
        RandomGraph.emplace_back(ver3[i]-1, ver2[i]-1, weight[i]);
    }

}


vector<int>dis;
vector<int>path;
vector<bool>book;								//���ҵ����·����

void Dijkstra(AMGraph &G)							//�Ͻ�˹�����㷨
{
    book.clear();
    dis.clear();
    path.clear();
    for (int i = 0; i <= G.vex; ++i) {
        book.push_back(true);
        dis.push_back(INF);
        path.push_back(-1);
    }

    for (int i = 1; i <= G.vex; ++i) {
        book[i]= false;
    }
    for (int i = 1; i <= G.vex; i++)
    {
        dis[i] = G.arcs[1][i];						//��ʼ��dis����
        path[i] = dis[i] < INF ? 1 : -1;			//��ʼ��·������
    }
    book[1] = true;
    dis[1] = 0;										//����ʼ��
    for (int i = 2; i <= G.vex; i++)				//����G.vex-1��
    {
        int mins = INF, u = 1;
        for (int j = 1; j <= G.vex; j++)			//�ҵ���ǰû���뼯�ϵ����·�ĺ�����
        {
            if (!book[j] && mins > dis[j]) {
                mins = dis[j];
                u = j;
            }
        }
        book[u] = true;								//���õ���뼯��
        for (int j = 1; j <= G.vex; j++)			//��������������������·���и��£��ɳڲ�����
        {
            if (!book[j] && dis[j] > dis[u] + G.arcs[u][j]) {
                dis[j] = dis[u] + G.arcs[u][j];		//�������·��ֵ
                path[j] = u;						//�޸�j��ǰ��Ϊu
            }
        }
    }
}

vector<int> ver;
double find(int x,int distance,int sumweight,vector<int>& Array)									//�ݹ�������·��
{
    int w=0;
    int m=0;
    vector<int> A;
    double avw=0.0;
    for (int i = 0; i < Array.size(); ++i) {
        A.push_back(Array[i]);
    }
    distance +=1;
    sumweight+= g1[Array[x-1]][Array[path[x]-1]];
    w=distance;
    m=sumweight;
    double W=w;
    double M=m;
    avw = M/W;
    if (path[x] == 1) {
        ver.push_back(1);
    }
    else {
        m=find(path[x],w,m, A);
    }
    ver.push_back(x);
    return m;
}

int pre[Max]

int main() {
    n=125;//����ͼ�Ķ�����Ŀ 
    vector<int> vertex;
    vector<edge>MST2;
    vector<edge>MST3;
    vector<edge>opts;
    double mm=0.0;
    double mins=10000;
    vector<int>min;
    int B=750;//Ԥ���С 
    int yu=0;//��ǰ�ҵ���Ҫ���뵽��������·���ķ��� 
    double v=0.0;
    double weigh=0.0;
    double average=0.0;
    for (int i = 0; i < 1001; i++){
    	for (int j = 0; j < 1001; j++){
            g1[i][j] = INF;
        }
	}
    vertex.clear();
    srand((unsigned) time(NULL));//��������ӣ��õ�ǰʱ�䱣֤���,ֻ����һ�ξ�OK //NOLINT
    readdat();//��ȡҪ���Ե����� 
    depot=randInt(0, n - 1);;
    Graph.clear();
    for (int i = 0; i < RandomGraph.size(); ++i) {
            if(RandomGraph[i].a==depot){
                Graph.push_back(RandomGraph[i]);
            } else if(RandomGraph[i].b==depot){
                Graph.emplace_back(RandomGraph[i].b,RandomGraph[i].a,RandomGraph[i].w);
            }
        }
    for (int i = 0; i < RandomGraph.size(); ++i) {
            if (RandomGraph[i].a!=depot&&RandomGraph[i].b!=depot){
                Graph.push_back(RandomGraph[i]);
            }
        }
    for (int i = 0; i < RandomGraph.size(); ++i) {
            g1[RandomGraph[i].a][RandomGraph[i].b]=RandomGraph[i].w;
            g1[RandomGraph[i].b][RandomGraph[i].a]=RandomGraph[i].w;
        }
    for (int i = 0; i < n; ++i) {
            if (g1[depot][i]<INF){
                pre[i]=depot;
            }
        }
    vertex.push_back(depot);
    for (int i = 0; i < n; ++i) {
            if (i != depot) {
                vertex.push_back(i);
            }
        }

    while (Graph.size() > 0) {

            G.vex = vertex.size();
            for (int i = 1; i <= G.vex; i++)                //��ʼ���ڽӾ���
                for (int j = 1; j <= G.vex; j++)
                    G.arcs[i][j] = INF;
            for (int i = 0; i < Graph.size(); ++i) {
                int a = 0;
                int b = 0;
                for (int j = 0; j < vertex.size(); ++j) {
                    if (vertex[j] == Graph[i].a) {
                        a = j + 1;
                    }
                }
                for (int j = 0; j < vertex.size(); ++j) {
                    if (vertex[j] == Graph[i].b) {
                        b = j + 1;
                    }
                }
                G.arcs[a][b] = Graph[i].w;
                G.arcs[b][a] = Graph[i].w;
            }

            Dijkstra(G);
            mins = INF;
            yu=0;
            for (int i = 2; i <= G.vex; i++)
            {
                weigh=0;

                ver.clear();
                mm = find(i,0,0,vertex);
                weigh=g1[pre[vertex[ver[1]-1]]][vertex[ver[1]-1]];
                for (int j = 1; j < ver.size()-1; ++j) {
                    weigh+=g1[vertex[ver[j]-1]][vertex[ver[j+1]-1]];
                }
                v=ver.size()-1;
                average=weigh/v;
                if(average<mins && weigh<=B){
                    min.clear();
                    mins=average;
                    yu=weigh;
                    for (int j = 0; j < ver.size(); ++j) {
                        min.push_back(vertex[ver[j]-1]);
                    }
                }
            }

            B=B-yu;//��ȥ���뵽��������·���ķ��� 
            cout<<endl;
            if(min.size()==0){
                break;//���û���ҵ���Ҳ��·��������ѭ������ 
            }
            MST3.clear();
            MST2.clear();
            opts.emplace_back(pre[min[1]],min[1],g1[pre[min[1]]][min[1]]);
            for (int i = 1; i < min.size()-1; ++i) {
                opts.emplace_back(min[i],min[i+1],g1[min[i]][min[i+1]]);
            }
            int Mins=INF;
            int vx=0;
            for (int i = 0; i < vertex.size(); ++i) {
                Mins=INF;
                if (exist1(min,vertex[i])){
                    for (int j = 0; j < min.size(); ++j) {
                        if(g1[min[j]][vertex[i]]<Mins){
                            Mins=g1[min[j]][vertex[i]];
                            vx=min[j];
                        }
                    }

                    if (Mins<INF){
                        pre[vertex[i]]=vx;
                        MST3.emplace_back(depot,vertex[i],Mins);
                    }
                }
            }

            for (int i = 0; i < Graph.size(); ++i) {
                if(exist1(min,Graph[i].a)&&exist1(min,Graph[i].b)){
                    MST2.push_back(Graph[i]);
                }
            }

            Graph.clear();
            
            for (int i = 0; i < MST3.size(); ++i) {
                Graph.push_back(MST3[i]);
            }
            for (int i = 0; i < MST2.size(); ++i) {
                Graph.push_back(MST2[i]);
            }
            vertex.clear();
            for (size_t i = 0; i < Graph.size(); i++)
            {
                if (exist1(vertex, Graph[i].a)) {
                    vertex.push_back(Graph[i].a);
                }
                if (exist1(vertex, Graph[i].b)) {
                    vertex.push_back(Graph[i].b);
                }
            }
            min.clear();

        }
    cout << "����ƽ�����õ�̰���㷨�Ľ�Ϊ";
    cout << opts.size() << endl;
    return 0;
}

