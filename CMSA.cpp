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
#include<ilcplex/ilocplex.h>
#include <stdio.h>
#include<fstream>
#include<Windows.h>
#pragma warning( disable : 4996 )
using namespace std;
ILOSTLBEGIN

const int MAX_V = 10001;
const int INF = 0x3f3f3f3f;//INF
int n, depot, Max_A;//���������������Ȩ��


int w_lower = 10;//ͼ�б�Ȩ���½�
int w_upper = 100;//ͼ�б�Ȩ���Ͻ�


int g[MAX_V][MAX_V];//�洢������ľ���


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
            return false;//��ʾ����
        }
    }
    return true;
}

vector<edge>Graph;//���ɵ���ȫͼ
vector<edge> ValidSolution;
vector<edge> ValidSolution2;//������ŵõ��Ŀ��н�
vector<edge>DGraph;

int randInt(int a, int b) {
    return rand() % (b - a + 1) + a;//NOLINT
}


void generateMetricCompleteGraph() {
    printf("====generateMetricCompleteGraph()====\n");
    Graph.clear();
    Max_A = 0;
    //��g��ʼ��
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i == j) g[i][j] = 0;
            else g[i][j] = INF;
    //���������ȫͼ��Ȩ��, w_lower<=w<=w_upper;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            g[i][j] = randInt(w_lower, w_upper);
            g[j][i] = g[i][j];
        }
    }
    printf("n=%d, w_lower=%d, w_upper=%d\n", n, w_lower, w_upper);
    //�������depot
    depot = randInt(0, n - 1);
    printf("depot=%d\n", depot);
    //ʹ��floyd������������ɵ�ͼ���metric space�µ���ȫͼ
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
    in >> temp;
    cout << temp << endl;
    in >> ding;
    cout << ding << endl;
    in >> index;
    cout << index << endl;

    while (in)
    {
        in >> temp;             //��һ����Ų���
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

vector<edge> A;//�������н��ж������ڵı�
vector<edge> Pro;//���A��ÿ��ߵ�ѡ�����
vector<int> vertex;//��ŵ�ǰ���н�����Ķ���
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
                A.emplace_back(root, j, g[root][j]);//��ʼ��A
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
            Pro[i].w = (double)Pro[i].w / sum;//��A�бߵı�Ȩ��Ϊ��ѡ����ʴ�����µı߼���
        }
        double to = 0;
        for (int i = 0; i < Pro.size(); ++i) {
            to += Pro[i].w;
        }
        //        cout<<to<<endl;
        int edges = 0;//��ž������ѡ����ʵıߵ��±�
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
        }//��֤��ķ���С�ڵ���Ԥ��
        //���¸�A��ֵ
        A.clear();
        for (int i = 0; i < ValidSolution.size(); ++i) {
            if (find(vertex.begin(), vertex.end(), ValidSolution[i].a) == vertex.end()) {
                vertex.push_back(ValidSolution[i].a);
            }
            if (find(vertex.begin(), vertex.end(), ValidSolution[i].b) == vertex.end()) {
                vertex.push_back(ValidSolution[i].b);
            }
        }//����ǰ���н�����Ķ������������
       

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
vector<edge> C;//�����������ʵ��
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
        }//������ʵ��
        
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


        //����Cplex�������������滮
        IloEnv env;//��������������
        IloModel model(env);//����ģ�Ͷ���
        IloArray<IloNumVar> x(env);//��Ŷ��������
        x.setSize(vertex1.size());
        for (IloInt i = 0; i < x.getSize(); i++) {
            x[i] = IloNumVar(env, 0, 1, ILOBOOL);
        }
        IloArray<IloNumVar> y(env);
        y.setSize(C.size());
        for (IloInt i = 0; i < y.getSize(); i++) {
            y[i] = IloNumVar(env, 0, 1, ILOBOOL);
        }
        IloArray<IloNumVar> flow(env);//������
        flow.setSize(DoubleC.size());
        for (IloInt i = 0; i < flow.getSize(); i++) {
            flow[i] = IloNumVar(env, 0, IloInfinity, ILOFLOAT);
        }

        IloExpr e(env);
        for (size_t j = 0; j < x.getSize(); ++j) {
            e += x[j];
        }
        model.add(IloMaximize(env, e));//����Ŀ�꺯��
        e.end();//�ͷű���


        //Լ��1
        IloExpr m(env);
        for (size_t k = 0; k < C.size(); k++) {
            m += C[k].w * y[k];
        }
        model.add(m <= B);
        m.end();
        //Լ��2
        IloExpr t(env);
        for (size_t i = 0; i < vertex1.size(); i++) {
            t += x[i];
        }
        for (size_t i = 0; i < C.size(); i++) {
            t -= y[i];
        }
        model.add(t == 1);
        t.end();

        //Լ��3
        model.add(x[root] == 1);
        //Լ��4
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
        //Լ��5
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

        //Լ��6
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

        //Լ��7
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


        IloCplex cplex(model);//����������
        cplex.extract(model);//��ȡģ��
        cplex.setOut(env.getNullStream());
        cplex.setParam(IloCplex::TiLim, tmax);//�����������ʱ��tmax�� 
        cplex.solve();
        if (!cplex.solve()) {
            env.error() << "Failed to optimize LP." << endl;
            throw(-1);
        }

        cplex.getStatus();//��ȡ��ǰ���״̬��Infeasible\ Feasible\ Optimal
        
        
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
        cout << "���Ž�Ϊ";
        env.out() << cplex.getObjValue() << endl;
        env.end();
    }
}
//��ӡ��ַ
string graph_structure_dir_path;//�洢ÿ�ι������ɵ�ͼ�ṹ(һ��ʵ��һ���ļ�) //NOLINT
string pilot_process_dir_path;//�洢ÿ�ι������ɵ�B��k��k_(һ��ʵ��һ���ļ�) //NOLINT
string logs_dir_path;//�洢ÿ�ι��̵Ĵ�ӡ��־(һ��ʵ��һ���ļ�) //NOLINT
string params_filepath;//�洢����ʵ��Ĳ��� //NOLINT
string result_filepath;//�洢���յĽ��,���������ͼ //NOLINT
inline bool isDirExist(const string& filename) {
    struct stat buffer {};
    return (stat(filename.c_str(), &buffer) == 0);
}
//1.����ʵ���õ�dir
void initDir(const string& op) {
    string base_dir = ".\\";//��Ŀ��Ŀ¼(.��main.cppͬ��,..Ϊmain.cpp��һ��)
    string command;
    //����experimentsĿ¼
    string base_experiments_dir = base_dir + "\\experiments";
    if (!isDirExist(base_experiments_dir)) {
        command = "mkdir " + base_experiments_dir;
        system(command.c_str());
    }
    //����bcc�ļ���
    string alg_experiments_dir = base_experiments_dir + "\\bcc";
    if (!isDirExist(alg_experiments_dir)) {
        command = "mkdir " + alg_experiments_dir;
        system(command.c_str());
    }
    //��ȡ��ǰ������
    time_t now_time = time(0);// NOLINT
    tm* now = localtime(&now_time);
    string today = to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday);
    //����today_experiment_dir
    string today_experiment_dir = alg_experiments_dir + "\\" + today;
    if (!isDirExist(today_experiment_dir)) {
        command = "mkdir " + today_experiment_dir;
        system(command.c_str());
    }
    //����ʵ���Ŀ¼
    string experiment_name = to_string(now_time) + "_" + op;
    string experiment_dir = today_experiment_dir + "\\" + experiment_name;
    command = "mkdir " + experiment_dir;
    system(command.c_str());
    //����graphs��pilot_process��logs�ļ���
    string graph_structure_dir = experiment_dir + "\\graphs";
    command = "mkdir " + graph_structure_dir;
    system(command.c_str());
    //    string pilot_process_dir = experiment_dir + "\\pilot_process";
    //    command = "mkdir " + pilot_process_dir;
    //    system(command.c_str());
    string logs_dir = experiment_dir + "\\logs";
    command = "mkdir " + logs_dir;
    system(command.c_str());
    //init����path��ֵ
    graph_structure_dir_path = graph_structure_dir + "\\";//�洢ÿ�ι������ɵ�ͼ�ṹ(һ��ʵ��һ���ļ�)
//    pilot_process_dir_path = pilot_process_dir + "\\";//�洢ÿ�ι������ɵ�B��k��k_��(һ��ʵ��һ���ļ�)
    logs_dir_path = logs_dir + "\\";//�洢ÿ�ι��̵Ĵ�ӡ��־(һ��ʵ��һ���ļ�)
    result_filepath = experiment_dir + "\\result.csv";//�洢���յĽ��,���������ͼ
    params_filepath = experiment_dir + "\\params.txt";//�洢����ʵ���õĲ���
}

void printGraphStructure(const string& s_index) {
    string filename = graph_structure_dir_path + "graph-" + s_index + ".txt";
    freopen(filename.c_str(), "wb", stdout);//��b��Ϊ������linux
    int e = n * (n - 1) / 2;//1+����+n-1 = (n-1+1)(n-1)/2
    printf("%d %d %d\n", n, e, depot);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            printf("%d %d %d\n", i, j, g[i][j]);
    fclose(stdout);
}
void printResult(const string& s_index) {
    string filename = logs_dir_path + "log-" + s_index + ".txt";
    freopen(filename.c_str(), "wb", stdout);//��b��Ϊ������linux
    printf("����1, ����2, Ȩ��,\n");
    for (int i = 0; i < ValidSolution2.size(); ++i) {
        int a = ValidSolution2[i].w;
        printf("%d %d %d\n", ValidSolution2[i].a, ValidSolution2[i].b, a);
    }
    fclose(stdout);
}
void printResult2(const string& s_index) {
    string filename = logs_dir_path + "log-" + s_index + ".txt";
    freopen(filename.c_str(), "wb", stdout);//��b��Ϊ������linux
    printf("����1, ����2, Ȩ��,\n");
    for (int i = 0; i < C.size(); ++i) {
        int a = C[i].w;
        printf("%d %d %d\n", C[i].a, C[i].b, a);
    }
    fclose(stdout);
}


    int main() {
        DWORD start, end;
        start = GetTickCount();
        srand((unsigned)time(NULL));//��������ӣ��õ�ǰʱ�䱣֤���,ֻ����һ�ξ�OK //NOLINT
        initDir("budget");
        n = 25;//��Ҫ���Ե�ͼ�Ķ����� 
        readdat();// ��Ҫ���Ե����ݶ�ȡ���� 
        depot = randInt(0, n - 1);
        vector<int> nums;
        for (size_t i = 0; i < n; i++)
        {
            nums.push_back(i);
        }
        depot = 11;
       srand((unsigned)time(NULL));
       CMSA(50, 10, 0.0, 200, 20);//���ݵ��εõ��Ĳ�������CMSA�㷨 
       
       //��ӡ����
       printGraphStructure(to_string(1));
       printResult2(to_string(1));
        end = GetTickCount() - start;
        return 0;
    }
