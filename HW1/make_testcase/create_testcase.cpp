#include <bits/stdc++.h>
#include <algorithm>
#include <random>

using namespace std;

struct disjoint_set
{
	vector<int> parent;
	vector<int> siz;
	void Init(int n)
	{
		parent.resize(n+1);
		siz.resize(n+1,1);
		for(int i=1;i<=n;i++) parent[i]=i;
	}
	int find(int x)
	{
		if(x==parent[x]) return x;
		return parent[x]=find(parent[x]);
	}
	bool same(int a,int b)
	{
		return find(a)==find(b);
	}
	void unite(int a,int b)
	{
		int pa=find(a),pb=find(b);
		if(siz[pa]>siz[pb]) swap(pa,pb);
		parent[pa]=pb;
		siz[pb]+=siz[pa];
	}
} dsu;

// 生成稠密圖
vector<tuple<int, int, int>> generate_dense_graph(int n, set<pair<int, int>> edges) {
    vector<tuple<int, int, int>> graph;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if(edges.count(make_pair(i, j)) == 0 && edges.count(make_pair(j, i)) == 0){
                edges.insert(make_pair(i, j));
                tuple<int, int, int> temp;
                int a = rand() % 20 + 1;
                temp = make_tuple(a, i, j);
                graph.push_back(temp);
                temp = make_tuple(a, j, i);
                graph.push_back(temp);
            }
        }
    }
    return graph;
}

// 生成一般圖
void generate_general_graph(int n, int m, set<pair<int, int>>& edges) {
    while (edges.size() < m) {
        int a = rand() % n;
        int b = rand() % n;
        if (a != b && edges.count(make_pair(a, b)) == 0 && edges.count(make_pair(b, a)) == 0) {
            edges.insert(make_pair(a, b));
        }
    }
}

// 生成聯通圖
void generate_connected_graph(int n, int m, set<pair<int, int>>& edges) {
    // 先建立一張完全圖
    auto graph = generate_dense_graph(n, edges);
    // 找出最小生成樹，以保證圖是聯通的
    disjoint_set ddd;
    ddd.Init(n);
    sort(graph.begin(), graph.end());
    for(auto [w,v,u]:graph)
    {
        if(!ddd.same(u,v))
        {
            ddd.unite(v,u);
            edges.insert(make_pair(v, u));
        }
    }
    // 產生其餘的邊
    generate_general_graph(n, m, edges);
}


int main() {
    srand(time(NULL));
    // 要生成幾筆測資
    ifstream infile("testcase.txt");
    // 檔案編號
    int id;
    // 節點數量
    int n;
    // 邊數量
    int m;
    // SDN數量
    int SDN_num;
    // DST數量
    int DST_num;
    // 源點數量
    int pair_num;
    // 流量大小 ， 實測後都約小於75
    int flow_size = 65;
    while(infile >> id >> n >> m >> SDN_num >> DST_num >> pair_num){
        
        cout << "testcase " << id << endl;
        // 寫入檔案
        ofstream outfile;
        outfile.open(to_string(id) + ".in");
        if (!outfile) {
            cerr << "Cant write file！" << endl;
            exit(1);
        }
        
        // 產生邊
        set<pair<int, int>> edges;
        generate_connected_graph(n, m, edges);
        
        outfile << n << " " << SDN_num << " " << DST_num << " " << m << " " << pair_num << endl;
        
        // 我每次都把前幾個節點當作SDN節點，所以這邊就不用亂數產生了，因為圖是隨機產生的，所以SDN節點也是隨機的分布在圖中
        for (int i = 0; i < SDN_num; i++)
        {   
            if (i == SDN_num - 1) {
                outfile << i << endl;
            } else {
                outfile << i << " ";
            }
        }
        // 接著是DST節點，隨機產生
        vector<int> save_dst;
        vector<bool> exist_dst(n, false);
        for(int i = 0; i < DST_num; i++) {
            int nnn = rand() % (n);
            if(exist_dst[nnn]) {
                i--;
            } 
            else{
                exist_dst[nnn] = true;
                save_dst.push_back(nnn);
                // if (i == DST_num - 1) {
                //     outfile << nnn << endl;
                // } else {
                //     outfile << nnn << " ";
                // }
            }
        }
        sort(save_dst.begin(), save_dst.end());
        for (int i = 0; i < DST_num; i++)
        {   
            if (i == DST_num - 1) {
                outfile << save_dst[i] << endl;
            } else {
                outfile << save_dst[i] << " ";
            }
        }
        int count = 0;
        for (auto edge : edges) {
            outfile << count << ' ' << edge.first << " " << edge.second << endl;
            count++;
        }

        // 隨機產生源點，只要不要和DST一樣就好
        vector<int> save;
        vector<bool> exist(n, false);
        for (int i = 0; i < pair_num; i++)
        {
            int t = rand() % (n);
            if(exist[t]) i--;
            else{
                exist[t] = true;
                save.push_back(t);
            }
        }
        // 輸出pair
        for (int i = 0; i < pair_num; i++)
        {
            outfile << i << " " << save[i] << ' ' <<save_dst[rand() % (DST_num)]  << ' ' << rand() % flow_size + 10 <<endl;
        }
        
        outfile.close();
    }
    return 0;
}