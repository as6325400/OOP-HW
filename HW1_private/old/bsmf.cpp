#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <bits/stdc++.h>

using namespace std;


class node
{
private:
	// type用來記錄這個節點是否為SDN節點，true代表為SDN節點。
	bool type;
public:
	int packet;
	map<unsigned int, vector<pair<unsigned int, double>>> save_flow;
	node() :packet(0), type(false) {}
	bool is_SDN() {
		return type == true;
	}
	void change_type() {
		type = !type;
	}
};

class Edge {
public:
	int from;
	int to;
	int cap;
	int rev;
	Edge() {}
	Edge(int _from, int _to, int _cap, int _rev) :from(_from), to(_to), cap(_cap), rev(_rev) {}
};

vector<vector<int>> bfs(int dst, vector<vector<int>> &graph, int node_num) {
	queue<int> save;
	vector<bool> visited(node_num, false);
	vector<vector<int>> ans(node_num);
	save.push(dst);
	visited[dst] = true;
	int level = 0;
	while (!save.empty()) {
		int len = save.size();
		for (int i = 0; i < len; i++) {
			int n = save.front();
			save.pop();
			ans[level].push_back(n);
			for (int &nnn : graph[n]) {
				if (!visited[nnn]) {
					visited[nnn] = true;
					save.push(nnn);
				}
			}
		}
		level++;
	}
	return ans;
}

void output(vector<node> &n, vector<int> &dst, int node_num) {
	for (int i = 0; i < node_num; i++) {
		cout << i << '\n';
		if (n[i].is_SDN()) {
			for (int j = 0; j < dst.size(); j++) {
				if (dst[j] == i) {
					cout << i << ' ' << dst[j] << " 100%\n";
					continue;
				}
				cout << dst[j] << ' ';
				for (auto &k : n[i].save_flow[dst[j]]) {
					if(k.second == 0) continue;
					cout << k.first << ' ' << k.second * 100.0 << "% ";
				}
				cout << '\n';
			}
		}
		else {
			for (int j = 0; j < dst.size(); j++) {
				if (dst[j] == i) {
					cout << i << ' ' << dst[j] << '\n';
					continue;
				}
				cout << dst[j] << ' ' << n[i].save_flow[dst[j]][0].first << "\n";
			}
		}
	}
}

bool have_cycle(vector<vector<int>> &DAG, int src, int dst){
		int num = DAG.size();
		vector<bool> visit(num, false);
		visit[src] = true;
		// 起點到終點
		queue<int> save;
		save.push(src);
		while(!save.empty()){
				int len = save.size();
				for(int i = 0; i < len; i++){
						int node = save.front();
						save.pop();
						if(node == dst) return true;
						for(auto &j: DAG[node]){
								if(visit[j]) continue;
								visit[j] = true;
								save.push(j);
						}
				}
		}
		return false;
}

// 有優化速度的空間
vector<vector<int>> Create_DAG(int node_num, vector<vector<int>> &level, vector<node> &n, vector<vector<int>> &graph) {
	vector<vector<int>> DAG(node_num);
	map<int, int> level_map;
	for(int i = 0; i < level.size(); i++){
			for(auto &j: level[i]){
					level_map[j] = i;
			}
	}
  vector<bool> visit(node_num, false);
 	queue<int> save;
  save.push(level[0][0]);
  // 確認有沒有被其他點插入過
  vector<vector<bool>> check(node_num, vector<bool>(node_num, false));
  while (!save.empty())
  { 
    int len = save.size();
    vector<int> temp;
    for(int k = 0; k < len; k++){
        int node = save.front();
        save.pop();
        temp.push_back(node);
    }
    sort(temp.begin(), temp.end());
    for(int k = 0; k < len; k++){
        int node = temp[k];
        // node 是被插的
        for(auto &i:graph[node]){
            // 代表i點沒被node點插過
            // 所以i點可以插入node點
            if(check[i][node]) continue;
            if(check[node][i]) continue;
            if(n[i].is_SDN()){
                // assert(level[level_map[i]].size() < 0.8 * node_num);
                // clock_t end = clock();
                // double cpu_time_used = ((double) (end - j.starttime())) / CLOCKS_PER_SEC;
                // cerr << cpu_time_used << '\n';
                if(level_map[i] >= level_map[node]){
                    // if(j.num() > 45) continue;
                    if(have_cycle(DAG, node, i)){
                        continue;
                    }
                }
                DAG[i].push_back(node);
                // node點被i點插過
                check[node][i] = true;
            }else{
                // 如果一般點已經插入過別人，continue
                if(visit[i]) continue;
                DAG[i].push_back(node);
                visit[i] = true;
                check[node][i] = true;
                // 關於一般點要不要給很多點插入，這個作保留來實驗。
            }
            save.push(i);
        }
    }
  }
  return DAG;
}

bool vis[600];

void add_edge(int cap, int from, int to, vector<vector<Edge>> &G) {
	G[from].push_back(Edge(from, to, cap, G[to].size()));
	G[to].push_back(Edge(to, from, 0, G[from].size() - 1));
}

int dfs(int now, int target, int flow, vector<vector<Edge>> &G, vector<node> &n) {
	if (now == target) return flow;
	vis[now] = true;
	for (int i = 0; i < G[now].size(); i++) {
		Edge &e = G[now][i];
		if (!vis[e.to] && e.cap > 0) {
			int d = dfs(e.to, target, min(flow, e.cap), G, n);
			if (d > 0) {
				e.cap -= d;
				G[e.to][e.rev].cap += d;
				return d;
			}
		}
	}
	return 0;
}


void change_capicity(int s, int t, vector<vector<Edge>> &G, vector<int> &pre, int flow){
    int cur = t;
    while(cur != s){
        int p = pre[cur];
        for(auto &i: G[p]){
            if(i.to == cur){
                i.cap -= flow;
                G[cur][i.rev].cap += flow;
                break;
            }
        }
        cur = p;
    }
}

int bfs_max_flow(int s, int t, vector<vector<Edge>> &G) {
    vector<bool> visited(G.size(), false);
    queue<int> save;
    vector<int> pre(G.size(), -1);
    int flow = 1e9;
    save.push(s);
    visited[s] = true;
    while(!save.empty()){
        int len = save.size();
        for(int i = 0; i < len; i++){
            int node = save.front();
            save.pop();
            if(node == t){
                change_capicity(s, t, G, pre, flow);
                return flow;
            }
            for(auto &j: G[node]){
                if(visited[j.to]) continue;
                if(j.cap == 0) continue;
                visited[j.to] = true;
                save.push(j.to);
                flow = min(flow, j.cap);
                pre[j.to] = node;
            }
        }
    }
    return -1;
}

int max_flow(int s, int t, int flow, vector<vector<Edge>> &G, vector<node> &n) {
	int max_flow = 0;
    int node_id = n.size() - 1;
	while (bfs_max_flow(s, t, G) != -1) {
		max_flow += flow;
	}
	return max_flow;
}

void save_node_packet(vector<node> &n, vector<vector<int>> &DAG, vector<vector<int>> &adj_m, vector<pair<int, int>> &s){
	int node_num = n.size() - 1;
	for(int i = 0; i < node_num; i++){
		for(auto &j: DAG[i]){
			n[i].packet += adj_m[i][j];
		}
	}

}

bool judge_max_flow(int packet, vector<node> &n, vector<pair<int, int>> &source, vector<vector<int>> &DAG, int dst, vector<vector<Edge>> &G) {
	// 建立一個新的點，一開始將所有packet匯集在這裡流出去
	// 這樣可以處理多源點問題
	for (auto &i : n)
		i.packet = 0;
	int node_num = n.size() - 1;
	int total = accumulate(source.begin(), source.end(), 0, [](int a, pair<int, int> b) {return a + b.second; });
	n.back().packet = total;
	for (int i = 0; i < source.size(); i++) {
		add_edge(source[i].second, node_num, source[i].first, G);
	}
	// 將無向圖加入流的邊
	for (int i = 0; i < node_num; i++) {
		for (int j = 0; j < DAG[i].size(); j++) {
			add_edge(packet, i, DAG[i][j], G);
		}
	}
	if (max_flow(node_num, dst, total, G, n) == total){
		return true;
	}
	return false;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	int node_num, SDN_node_num, Dst_num, Links_num, source_node_num;
	cin >> node_num >> SDN_node_num >> Dst_num >> Links_num >> source_node_num;
	vector<node> n(node_num + 1); //建立一個存點的陣列
	vector<vector<int>> graph(node_num);
	vector<vector<bool>> graph_matrix(node_num, vector<bool>(node_num, false));
	vector<int> dst(Dst_num);
	map<int, vector<pair<int, int>>> save_source; //建立每一個dst對source點
	for (int i = 0; i < SDN_node_num; i++) {
		// 選定SDN_node
		int k;
		cin >> k;
		n[k].change_type();
	}
	for (int i = 0; i < Dst_num; i++)
	{
		// 這裡沒什麼意義，因為下面就可以記錄source跟dst的pair，到時候再紀錄就好
		// 如果judge更新後發現有dst不會有任何的packet送達，這裡再做改變
		cin >> dst[i];
	}
	for (int i = 0; i < Links_num; i++) {
		// 建圖
		int id, start, end;
		cin >> id >> start >> end;
		graph_matrix[start][end] = true;
		graph_matrix[end][start] = true;
		graph[start].push_back(end);
		graph[end].push_back(start);
	}
	for (int i = 0; i < source_node_num; i++) {
		// 建立source跟dst的點對
		// baseline不用考慮flow大小
		int id, source, dst, flow_size;
		cin >> id >> source >> dst >> flow_size;
		save_source[dst].push_back({ source, flow_size });
	}
	for (int i = 0; i < Dst_num; i++) {
		auto temp = bfs(dst[i], graph, node_num);
		auto DAG = Create_DAG(node_num, temp, n, graph);
		vector<vector<Edge>> G;
		int flow_size;
		int left = 0, right = 300;
		vector<int> table(301, -1);
		G.resize(node_num + 1);
		while(left <= right){
			int mid = (left + right) / 2;
			if(table[mid - 1] == -1){
				G.resize(node_num + 1);
				table[mid - 1] = judge_max_flow(mid - 1, n, save_source[dst[i]], DAG, dst[i], G) ? 1 : 0;
				G.clear();
			}
			if(table[mid] == -1){
				G.resize(node_num + 1);
				table[mid] = judge_max_flow(mid, n, save_source[dst[i]], DAG, dst[i], G) ? 1 : 0;
				G.clear();
			}
			if(table[mid] == true && table[mid - 1] == false){
				flow_size = mid;
				break;
			}
			table[mid] == true ? right = mid - 1 : left = mid + 1;
		}
		G.clear();
		G.resize(node_num + 1);
		judge_max_flow(flow_size, n, save_source[dst[i]], DAG, dst[i], G);
		vector<vector<int>> adj_m(node_num, vector<int>(node_num, 0));
		for (int j = 0; j < node_num; j++) {
			for (int k = 0; k < G[j].size(); k++) {
				if (G[j][k].to >= node_num or G[j][k].from >= node_num) continue;
				adj_m[G[j][k].from][G[j][k].to] = flow_size - G[j][k].cap;
			}
		}
		save_node_packet(n, DAG, adj_m, save_source[dst[i]]);
		for (int j = 0; j < node_num; j++) {
			double sum = (double)n[j].packet;
			if (sum == 0) {
				sum = DAG[j].size();
				for (auto &k : DAG[j]) {
					n[j].save_flow[dst[i]].push_back({ k, 1.0 / sum });
				}
			}
			else {
				for (auto &k : DAG[j]) {
					n[j].save_flow[dst[i]].push_back({ k, adj_m[j][k] / sum });
				}
			}
		}
	}
	output(n, dst, node_num);
	return 0;
}