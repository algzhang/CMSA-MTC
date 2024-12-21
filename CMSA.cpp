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
#include<ilcplex/ilocplex.h>
#include <stdio.h>
#include<fstream>
#include<Windows.h>
#pragma warning( disable : 4996 )
using namespace std;
ILOSTLBEGIN

const int MAX_V = 10001;
const int INF = 0x3f3f3f3f;//INF
int n, depot, Max_A;//顶点数，根，最大权重


int w_lower = 10;//图中边权的下界
int w_upper = 100;//图中边权的上界


int g[MAX_V][MAX_V];//存储两个点的距离


struct edge {
    int a, b;
    double w;
    edge() = default;
    edge(int _a, int _b, double _w) {
        a = _a;
        b = _b;
        w = _w;
    }
};
bool eql(edge A, edge B) {
    if (A.a == B.a && A.b == B.b) {
        return true;
    }
    else if (A.a == B.b && A.b == B.a) {
        return true;
    }
    else {
        return false;
    }
}
bool exist(vector<edge>& Array, edge a) {
    for (int i = 0; i < Array.size(); ++i) {
        if (eql(Array[i], a)) {
            return false;
        }
    }
    return true;
}

bool exist1(vector<int>& Array, int a) {
    for (int i = 0; i < Array.size(); ++i) {
        if (a == Array[i]) {
            return false;//表示存在
        }
    }
    return true;
}

vector<edge>Graph;//生成的完全图
vector<edge> ValidSolution;
vector<edge> ValidSolution2;//真正存放得到的可行解
vector<edge>DGraph;

int randInt(int a, int b) {
    return rand() % (b - a + 1) + a;//NOLINT
}


void generateMetricCompleteGraph() {
    printf("====generateMetricCompleteGraph()====\n");
    Graph.clear();
    Max_A = 0;
    //将g初始化
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i == j) g[i][j] = 0;
            else g[i][j] = INF;
    //随机生成完全图的权重, w_lower<=w<=w_upper;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            g[i][j] = randInt(w_lower, w_upper);
            g[j][i] = g[i][j];
        }
    }
    printf("n=%d, w_lower=%d, w_upper=%d\n", n, w_lower, w_upper);
    //随机生成depot
    depot = randInt(0, n - 1);
    printf("depot=%d\n", depot);
    //使用floyd方法把随机生成的图变成metric space下的完全图
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                g[i][j] = min(g[i][j], g[i][k] + g[k][j]);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g[i][j] >= Max_A) {
                Max_A = g[i][j];
            }
        };

    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            Graph.emplace_back(i, j, g[i][j]);
        };

    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            DGraph.emplace_back(i, j, g[i][j]);
        };

    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            DGraph.emplace_back(j, i, g[i][j]);
        };

    }
    cout << Graph.size() << endl;
    cout << Max_A << endl;
}

vector<int> ver;
vector<int> ver2;
vector<int> weight;

void readdat() {
    Graph.clear();
    ver.clear();
    ver2.clear();
    weight.clear();
    string fileName = "d://testdata/g25-4-04.dat";
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
    in >> temp;
    cout << temp << endl;
    in >> ding;
    cout << ding << endl;
    in >> index;
    cout << index << endl;

    while (in)
    {
        in >> temp;             //第一个序号不用
        ver.push_back(temp);
        in >> temp;
        ver2.push_back(temp);
        in >> temp2;
        weight.push_back(temp2);

        if (ver.size() == index) {
            break;
        }
    }


    for (int i = 0; i < MAX_V; ++i) {
        for (int j = 0; j < MAX_V; ++j) {
            g[i][j] = INF;
        }
    }
    for (int i = 0; i < ver.size(); ++i) {
        g[ver[i] - 1][ver2[i] - 1] = weight[i];
        g[ver2[i] - 1][ver[i] - 1] = weight[i];
    }
    for (int i = 0; i < ver.size(); ++i) {
        if (g[17][i] < INF) {
            cout << g[17][i] << endl;
        }
    }
    for (int i = 0; i < index; i++) {
        for (int j = 0; j < index; j++) {
            if (g[i][j] < INF) {
                if (g[i][j] >= Max_A) {
                    Max_A = g[i][j];
                }
            }
        };
    }
    cout << Max_A << endl;
    depot = randInt(0, ding - 1);
    printf("depot=%d\n", depot);
    for (size_t i = 0; i < ver.size(); i++)
    {
        Graph.emplace_back(ver[i]-1, ver2[i]-1, weight[i]);
    }

    for (size_t i = 0; i < ver.size(); i++)
    {
        DGraph.emplace_back(ver[i] - 1, ver2[i] - 1, weight[i]);
    }
    for (size_t i = 0; i < ver.size(); i++)
    {
        DGraph.emplace_back(ver2[i] - 1, ver[i] - 1, weight[i]);
    }
}

vector<edge> A;//存放与可行解中顶点相邻的边
vector<edge> Pro;//存放A中每天边的选择概率
vector<int> vertex;//存放当前可行解包含的顶点
void probabilisticSolutionGeneration(int graph[MAX_V][MAX_V], int root, int num, int Budget, double dmin, double dmax) {
    ValidSolution.clear();
    ValidSolution2.clear();
    vertex.clear();
    A.clear();
    Pro.clear();

    vertex.push_back(root);
    int aa = 0;

    int c = 0;
    for (int j = 0; j < num; j++) {
        if (root != j) {
            if (g[root][j] < INF) {
                A.emplace_back(root, j, g[root][j]);//初始化A
            }
        }
    }
    while (A.size() >= 0) {
        int sumweight = 0;

        ValidSolution2.clear();
        for (int i = 0; i < ValidSolution.size(); ++i) {
            ValidSolution2.emplace_back(ValidSolution[i].a, ValidSolution[i].b, ValidSolution[i].w);
        }
        Pro.clear();
        for (int i = 0; i < A.size(); i++) {
            double c = Max_A + 1 - A[i].w;
            Pro.emplace_back(A[i].a, A[i].b, c);
        }

        int sum = 0;
        for (int i = 0; i < Pro.size(); ++i) {
            sum += Pro[i].w;
        }
        for (int i = 0; i < Pro.size(); i++) {
            Pro[i].w = (double)Pro[i].w / sum;//将A中边的边权改为了选择概率存放在新的边集中
        }
        double to = 0;
        for (int i = 0; i < Pro.size(); ++i) {
            to += Pro[i].w;
        }
        //        cout<<to<<endl;
        int edges = 0;//存放具有最大选择概率的边的下标
        for (int i = 0; i < Pro.size(); ++i) {
            if (Pro[i].w >= Pro[edges].w) {
                edges = i;
            }
        }
        
        
        double r = (double)randInt(0, 1000) / 1000;
        int dmi = dmin * 1000;
        int dma = dmax * 1000;
        double sigma = (double)randInt(dmi, dma) / 1000;
        //        cout<<r<<endl;
        //        cout<<sigma<<endl;
        if (r <= sigma) {
            //            cout<<"xioayu"<<endl;
            ValidSolution.emplace_back(A[edges].a, A[edges].b, A[edges].w);

        }
        else {
            double possible = (double)randInt(dmi, dma) / 1000;
            double pos = 0;
            //            cout<<"posssibke is";
            //            cout<<possible<<endl;

            for (int i = 0; i < A.size(); ++i) {
                pos += Pro[i].w;
                if (possible <= pos) {
                    //                    cout<<"pos is";
                    //                    cout<<pos<<endl;
                    ValidSolution.emplace_back(A[i].a, A[i].b, A[i].w);
                    break;
                }
            }
        }

        for (int i = 0; i < ValidSolution.size(); ++i) {
            sumweight += ValidSolution[i].w;
        }
        aa = sumweight;
        //        cout<<sumweight<<endl;
        if (sumweight > Budget) {
            break;
        }//保证解的费用小于等于预算
        //重新给A赋值
        A.clear();
        for (int i = 0; i < ValidSolution.size(); ++i) {
            if (find(vertex.begin(), vertex.end(), ValidSolution[i].a) == vertex.end()) {
                vertex.push_back(ValidSolution[i].a);
            }
            if (find(vertex.begin(), vertex.end(), ValidSolution[i].b) == vertex.end()) {
                vertex.push_back(ValidSolution[i].b);
            }
        }//将当前可行解包含的顶点放入数组中
       

        int weighets = INF;
        int ver = 0;
        for (int i = 0; i < num; ++i) {
            if (find(vertex.begin(), vertex.end(), i) == vertex.end()) {
                weighets = INF;
                for (int j = 0; j < vertex.size(); ++j) {
                    if (g[i][vertex[j]] < weighets) {
                        weighets = g[i][vertex[j]];
                        ver = vertex[j];
                    }
                }
                if (weighets < INF) {
                    A.emplace_back(i, ver, g[i][ver]);
                }
            }
        }//

    }
    
}
vector<edge> C;//存放缩减的子实例
vector<edge> age;
vector<edge> OPT;
vector<edge> bsf;
vector<edge> DoubleC;
void CMSA(int na, int agemax, double drate, int B, int tmax) {
    int root = 0;
    vector<int> vertex1;
    vector<int> mark;
    vector<int> in;
    vector<int> out;
    for (size_t i = 0; i < 30; i++)
    {
        for (int i = 0; i < na; ++i) {
            probabilisticSolutionGeneration(g, depot, n, B, drate, drate);
            for (int j = 0; j < ValidSolution2.size(); ++j) {
                if (exist(C, ValidSolution2[j])) {
                    C.emplace_back(ValidSolution2[j].a, ValidSolution2[j].b, ValidSolution2[j].w);
                    age.emplace_back(ValidSolution2[j].a, ValidSolution2[j].b, 0);
                }
            }
        }//生成子实例
        
        vertex1.clear();
        for (size_t i = 0; i < C.size(); i++)
        {
            if (exist1(vertex1, C[i].a)) {
                vertex1.push_back(C[i].a);
            }
            if (exist1(vertex1, C[i].b)) {
                vertex1.push_back(C[i].b);
            }
        }
        for (size_t i = 0; i < vertex1.size(); i++)
        {
            if (vertex1[i] == depot) {
                root = i;
            }
        }
        DoubleC.clear();
        for (size_t i = 0; i < C.size(); i++)
        {
            DoubleC.push_back(C[i]);
        }
        for (size_t i = 0; i < C.size(); i++)
        {
            DoubleC.emplace_back(C[i].b, C[i].a, C[i].w);
        }


        //利用Cplex求解问题的整数规划
        IloEnv env;//创建环境变量。
        IloModel model(env);//创建模型对象
        IloArray<IloNumVar> x(env);//存放顶点的数组
        x.setSize(vertex1.size());
        for (IloInt i = 0; i < x.getSize(); i++) {
            x[i] = IloNumVar(env, 0, 1, ILOBOOL);
        }
        IloArray<IloNumVar> y(env);
        y.setSize(C.size());
        for (IloInt i = 0; i < y.getSize(); i++) {
            y[i] = IloNumVar(env, 0, 1, ILOBOOL);
        }
        IloArray<IloNumVar> flow(env);//网络流
        flow.setSize(DoubleC.size());
        for (IloInt i = 0; i < flow.getSize(); i++) {
            flow[i] = IloNumVar(env, 0, IloInfinity, ILOFLOAT);
        }

        IloExpr e(env);
        for (size_t j = 0; j < x.getSize(); ++j) {
            e += x[j];
        }
        model.add(IloMaximize(env, e));//设置目标函数
        e.end();//释放变量


        //约束1
        IloExpr m(env);
        for (size_t k = 0; k < C.size(); k++) {
            m += C[k].w * y[k];
        }
        model.add(m <= B);
        m.end();
        //约束2
        IloExpr t(env);
        for (size_t i = 0; i < vertex1.size(); i++) {
            t += x[i];
        }
        for (size_t i = 0; i < C.size(); i++) {
            t -= y[i];
        }
        model.add(t == 1);
        t.end();

        //约束3
        model.add(x[root] == 1);
        //约束4
        IloExpr z(env);
        for (size_t i = 0; i < vertex1.size(); i++)
        {
            z.clear();
            mark.clear();
            for (size_t j = 0; j < C.size(); j++)
            {

                if (vertex1[i] == C[j].a || vertex1[i] == C[j].b) {
                    mark.push_back(j);
                }
            }
            for (size_t k = 0; k < mark.size(); k++)
            {
                z += y[mark[k]];
            }
            model.add(z >= x[i]);
        }
        //约束5
        IloExpr r(env);
        in.clear();
        out.clear();
        for (size_t i = 0; i < DoubleC.size(); i++)
        {
            if (vertex1[root] == DoubleC[i].b) {
                in.push_back(i);
            }
            if (vertex1[root] == DoubleC[i].a) {
                out.push_back(i);
            }
        }

        for (size_t k = 0; k < in.size(); k++)
        {
            r += flow[in[k]];
        }
        for (size_t k = 0; k < out.size(); k++)
        {
            r -= flow[out[k]];
        }
        for (size_t i = 0; i < vertex1.size(); i++)
        {
            r += x[i];
        }
        model.add(r == 1);
        r.end();

        //约束6
        IloExpr f(env);
        for (size_t i = 0; i < vertex1.size(); i++)
        {
            in.clear();
            out.clear();
            f.clear();

            if (i != root) {
                for (size_t j = 0; j < DoubleC.size(); j++)
                {
                    if (vertex1[i] == DoubleC[j].b) {
                        in.push_back(j);
                    }
                    if (vertex1[i] == DoubleC[j].a) {
                        out.push_back(j);
                    }
                }
                for (size_t k = 0; k < in.size(); k++)
                {
                    f += flow[in[k]];
                }
                for (size_t t = 0; t < out.size(); t++)
                {
                    f -= flow[out[t]];
                }
                f -= x[i];
                model.add(f == 0);
            }
        }
        f.end();

        //约束7
        IloExpr cap(env);
        IloExpr ocap(env);
        int nums = vertex1.size() - 1;
        int as = C.size();
        for (size_t i = 0; i < C.size(); i++)
        {
            cap.clear();
            ocap.clear();
            cap += nums * y[i];
            cap -= flow[i];
            model.add(cap >= 0);
            ocap += nums * y[i];
            ocap -= flow[i + as];
            model.add(ocap >= 0);
        }
        cap.end();
        ocap.end();


        IloCplex cplex(model);//创建求解对象
        cplex.extract(model);//抽取模型
        cplex.setOut(env.getNullStream());
        cplex.setParam(IloCplex::TiLim, tmax);//设置最大运行时间tmax秒 
        cplex.solve();
        if (!cplex.solve()) {
            env.error() << "Failed to optimize LP." << endl;
            throw(-1);
        }

        cplex.getStatus();//获取当前解的状态，Infeasible\ Feasible\ Optimal
        
        
        bsf.clear();
        IloInt c = 0;
        for (size_t i = 0; i < C.size(); i++)
        {
            c = cplex.getValue(y[i]);
            if (c == 1) {
                bsf.emplace_back(C[i].a, C[i].b, C[i].w);
            }
        }
        if (bsf.size() > OPT.size()) {
            OPT.clear();
            for (size_t i = 0; i < bsf.size(); i++)
            {
                OPT.emplace_back(bsf[i].a, bsf[i].b, bsf[i].w);
            }
        }
        for (size_t i = 0; i < C.size(); i++)
        {
            if (exist(OPT, C[i])) {
                age[i].w += 1;
                if (age[i].w >= agemax) {
                    C.erase(C.begin() + i);
                    age.erase(age.begin() + i);
                }
            }
        }
        cout << "最优解为";
        env.out() << cplex.getObjValue() << endl;
        env.end();
    }
}
//打印地址
string graph_structure_dir_path;//存储每次过程生成的图结构(一次实验一个文件) //NOLINT
string pilot_process_dir_path;//存储每次过程生成的B、k、k_(一次实验一个文件) //NOLINT
string logs_dir_path;//存储每次过程的打印日志(一次实验一个文件) //NOLINT
string params_filepath;//存储本次实验的参数 //NOLINT
string result_filepath;//存储最终的结果,方便后续画图 //NOLINT
inline bool isDirExist(const string& filename) {
    struct stat buffer {};
    return (stat(filename.c_str(), &buffer) == 0);
}
//1.创建实验用的dir
void initDir(const string& op) {
    string base_dir = ".\\";//项目根目录(.与main.cpp同级,..为main.cpp上一级)
    string command;
    //创建experiments目录
    string base_experiments_dir = base_dir + "\\experiments";
    if (!isDirExist(base_experiments_dir)) {
        command = "mkdir " + base_experiments_dir;
        system(command.c_str());
    }
    //创建bcc文件夹
    string alg_experiments_dir = base_experiments_dir + "\\bcc";
    if (!isDirExist(alg_experiments_dir)) {
        command = "mkdir " + alg_experiments_dir;
        system(command.c_str());
    }
    //获取当前的日期
    time_t now_time = time(0);// NOLINT
    tm* now = localtime(&now_time);
    string today = to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday);
    //创建today_experiment_dir
    string today_experiment_dir = alg_experiments_dir + "\\" + today;
    if (!isDirExist(today_experiment_dir)) {
        command = "mkdir " + today_experiment_dir;
        system(command.c_str());
    }
    //创建实验根目录
    string experiment_name = to_string(now_time) + "_" + op;
    string experiment_dir = today_experiment_dir + "\\" + experiment_name;
    command = "mkdir " + experiment_dir;
    system(command.c_str());
    //创建graphs、pilot_process、logs文件夹
    string graph_structure_dir = experiment_dir + "\\graphs";
    command = "mkdir " + graph_structure_dir;
    system(command.c_str());
    //    string pilot_process_dir = experiment_dir + "\\pilot_process";
    //    command = "mkdir " + pilot_process_dir;
    //    system(command.c_str());
    string logs_dir = experiment_dir + "\\logs";
    command = "mkdir " + logs_dir;
    system(command.c_str());
    //init各个path的值
    graph_structure_dir_path = graph_structure_dir + "\\";//存储每次过程生成的图结构(一次实验一个文件)
//    pilot_process_dir_path = pilot_process_dir + "\\";//存储每次过程生成的B、k、k_等(一次实验一个文件)
    logs_dir_path = logs_dir + "\\";//存储每次过程的打印日志(一次实验一个文件)
    result_filepath = experiment_dir + "\\result.csv";//存储最终的结果,方便后续画图
    params_filepath = experiment_dir + "\\params.txt";//存储本次实验用的参数
}

void printGraphStructure(const string& s_index) {
    string filename = graph_structure_dir_path + "graph-" + s_index + ".txt";
    freopen(filename.c_str(), "wb", stdout);//加b是为了适配linux
    int e = n * (n - 1) / 2;//1+……+n-1 = (n-1+1)(n-1)/2
    printf("%d %d %d\n", n, e, depot);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            printf("%d %d %d\n", i, j, g[i][j]);
    fclose(stdout);
}
void printResult(const string& s_index) {
    string filename = logs_dir_path + "log-" + s_index + ".txt";
    freopen(filename.c_str(), "wb", stdout);//加b是为了适配linux
    printf("顶点1, 顶点2, 权重,\n");
    for (int i = 0; i < ValidSolution2.size(); ++i) {
        int a = ValidSolution2[i].w;
        printf("%d %d %d\n", ValidSolution2[i].a, ValidSolution2[i].b, a);
    }
    fclose(stdout);
}
void printResult2(const string& s_index) {
    string filename = logs_dir_path + "log-" + s_index + ".txt";
    freopen(filename.c_str(), "wb", stdout);//加b是为了适配linux
    printf("顶点1, 顶点2, 权重,\n");
    for (int i = 0; i < C.size(); ++i) {
        int a = C[i].w;
        printf("%d %d %d\n", C[i].a, C[i].b, a);
    }
    fclose(stdout);
}


    int main() {
        DWORD start, end;
        start = GetTickCount();
        srand((unsigned)time(NULL));//随机数种子，用当前时间保证随机,只定义一次就OK //NOLINT
        initDir("budget");
        n = 25;//需要测试的图的顶点数 
        readdat();// 将要测试的数据读取进来 
        depot = randInt(0, n - 1);
        vector<int> nums;
        for (size_t i = 0; i < n; i++)
        {
            nums.push_back(i);
        }
        depot = 11;
       srand((unsigned)time(NULL));
       CMSA(50, 10, 0.0, 200, 20);//根据调参得到的参数运行CMSA算法 
       
       //打印数据
       printGraphStructure(to_string(1));
       printResult2(to_string(1));
        end = GetTickCount() - start;
        return 0;
    }
