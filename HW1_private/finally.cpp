#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <bits/stdc++.h>
#define pii pair<int, int>

using namespace std;

int node_num, SDN_node_num, Dst_num, Links_num, source_node_num, counter = 0;

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
	// 紀錄反向邊的index
	int rev;
	Edge() {}
	Edge(int _from, int _to, int _cap, int _rev) :from(_from), to(_to), cap(_cap), rev(_rev) {}
};
 
vector<vector<int>> level(int dst, vector<vector<int>> &graph) {
	// 一般BFS，用來做分層
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
 
vector<vector<int>> bfs(int dst, vector<vector<int>> &graph) {
	// 就一般的bfs，做出DAG
	vector<vector<int>> DAG(node_num);
	queue<int> save;
	save.push(dst);
	vector<bool> visited(node_num, false);
	visited[dst] = true;
	while (!save.empty()) {
		int n = save.front();
		save.pop();
		for (int &nnn : graph[n]) {
			if (!visited[nnn]) {
				visited[nnn] = true;
				save.push(nnn);
				DAG[nnn].push_back(n);
			}
		}
	}
	return DAG;
}
 
bool no_cycle(int numCourses, vector<vector<int>>& DAG, pair<int, int> e) {
	// 被插入的點如果可以通到插入點，代表不可新增邊，會有環
	vector<bool> visited(node_num, false);
	queue<int> save;
	save.push(e.second);
	visited[e.second] = true;
	while (!save.empty()) {
		int n = save.front();
		save.pop();
		for (int &nnn : DAG[n]) {
			if (nnn == e.first) return false;
			if (!visited[nnn]) {
				visited[nnn] = true;
				save.push(nnn);
			}
		}
	}
	return true;
}
 
vector<vector<int>> bfs2(int dst, vector<vector<int>> &graph, vector<node> &node_v, vector<vector<bool>> &adj_m, vector<pair<int, int>> &source) {
	// 用一個set紀錄那些點在海綿裡
	set<int> s;
	// set<int> s2;
	// for(auto &i:source) s2.insert(i.first);
	vector<vector<int>> DAG(node_num);
	queue<int> save;
	save.push(dst);
	vector<bool> visited(node_num, false);
	visited[dst] = true;
	s.insert(dst);
	while (!save.empty()) {
		int n = save.front();
		save.pop();
		for (int &nnn : graph[n]) {
			if (!visited[nnn] and node_v[nnn].is_SDN()) {
				visited[nnn] = true;
				save.push(nnn);
				DAG[nnn].push_back(n);
				s.insert(nnn);
			}
		}
	}
	// 做分層，因為只要是後面的點插入前面的點，保證不會有環，這裡的距離指的是距離目的地的距離
	auto origin = bfs(dst, graph);
	vector<int> in(node_num, 0);
	for(int i = 0; i < source.size(); i++){
		int o_node = source[i].first;
		while(o_node != dst){
			in[o_node]++;
			o_node = origin[o_node][0];
		}
	}
	// 給下面非SDN插邊的預處理
	auto lev = level(dst, graph);
	vector<multiset<pii>> save_level_weight(node_num);
	for (int i = node_num - 1; i >= 0; i--) {
		for (int j = 0; j < lev[i].size(); j++) {
			if (!s.count(lev[i][j])) save_level_weight[i].insert({ in[lev[i][j]], lev[i][j] });
		}
	}
	for (int i = node_num - 1; i >= 1; i--) {
		for (auto &j : lev[i]) {
			if (visited[j]) continue;
			for (auto &k : lev[i - 1]) {
				if (s.count(k) and adj_m[j][k]) {
					DAG[j].push_back(k);
					visited[j] = true;
					break;
				}
			}
			if (!visited[j]) {
				if (node_v[j].is_SDN()) {
					for (auto &k : lev[i - 1]) {
						// 如果是SDN節點，盡量往前插邊
						if (adj_m[j][k]) {
							DAG[j].push_back(k);
							visited[j] = true;
							if (!node_v[j].is_SDN()) break;
						}
					}
				}
				// 在同樣為最短路徑，且前方無SDN點的情況下，盡量往沒有流會經過的一般點塞
				for(auto &k:save_level_weight[i - 1]){
					if(adj_m[j][k.second]){
						DAG[j].push_back(k.second);
						visited[j] = true;
						save_level_weight[i - 1].insert({ k.first + 1, k.second });
						save_level_weight[i - 1].erase(k);
						break;
					}
				}
			}
			// 官方judge會有二個case過不了，推估在靠近dst時很稠密
			// if(node_v[j].is_SDN()){
			// 	// SDN點在往後插入不會有環的情況下，也往後插入，SDN點連越多邊越好，在最大流算法下不會有壞處
			// 	for(int k = 0; k < node_num; k++)
			// 		if(no_cycle(node_num, DAG, {j, k}) and adj_m[j][k]){
			// 			DAG[j].push_back(k);
			// 		}
			// }
		}
	}
	return DAG;
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
					if (k.second == 0) continue;
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

void dfs_add_edge(vector<vector<int>> &graph, vector<int> &edge_num, int src, int dst, int flow) {
	if (src == dst) return;
	for (int i = 0; i < edge_num[src]; i++) {
		for (int j = 0; j < flow; j++) {
			graph[src].push_back(graph[src][i]);
		}
		dfs_add_edge(graph, edge_num, graph[src][i], dst, flow / edge_num[src]);
	}
}
 
 
vector<vector<int>> Create_DAG(int dst, vector<vector<int>> &graph_l, vector<node> &n, vector<vector<bool>> &graph_m, vector<pair<int, int>> &source) {
	vector<vector<int>> DAG(node_num);
	// 當全為SDN點時
	if (node_num == SDN_node_num) {
		// 做類似baseline的bfs就好
		DAG = bfs(dst, graph_l);
		for (int i = 0; i < source.size(); i++) {
			int nnn = source[i].first;
			while (nnn != dst) {
				if (n[nnn].is_SDN()) {
					// 在路徑上插入很多平行邊做分流
					for (int j = 0; j < 25 * source[i].second; j++) {
						DAG[nnn].push_back(DAG[nnn][0]);
					}
				}
				nnn = DAG[nnn][0];
			}
		}
	}
	else {
		// 反之，從dst出去找，只找sdn點，要做出一個類似海綿的效果，多大的流進來都會被稀釋
		// bfs2為只連接sdn點的bfs
		DAG = bfs2(dst, graph_l, n, graph_m, source);
		vector<int> edge_num(node_num, 0);
		// 計算暴力加邊前的邊數，以免邊指數量及增加
		for (int i = 0; i < node_num; i++)
			edge_num[i] = DAG[i].size();
		for (int i = 0; i < source.size(); i++) {
			int nnn = source[i].first;
			// dfs_add_edge(DAG, edge_num, nnn, dst, 15 * source[i].second);
			while (nnn != dst) {
				if (n[nnn].is_SDN()) {
					for (int j = 0; j < 24 * source[i].second; j++) {
						DAG[nnn].push_back(DAG[nnn][0]);
					}
				}
				nnn = DAG[nnn][0];
			}
		}
	}
	return DAG;
}
 
bool vis[600];
 
void add_edge(int cap, int from, int to, vector<vector<Edge>> &G) {
	// 增加邊
	G[from].push_back(Edge(from, to, cap, G[to].size()));
	G[to].push_back(Edge(to, from, 0, G[from].size() - 1));
}
 
int dfs(int now, int target, int flow, vector<vector<Edge>> &G, vector<node> &n) {
	// 往下dfs，並補反向邊
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
 
int max_flow(int s, int t, int flow, vector<vector<Edge>> &G, vector<node> &n) {
	// Ford–Fulkerson演算法，因為|f|很小所以用這個
	// 計算最大流
	int r = 0;
	// 往所有可能到達的路徑做dfs，直到沒有路徑可以走為止，或者packet送玩了
	while (1) {
		memset(vis, 0, sizeof(vis));
		int f = dfs(s, t, flow, G, n);
		if (f <= 0) break;;
		r += f;
	}
	return r;
}
 
void save_node_packet(vector<node> &n, vector<vector<int>> &DAG, vector<vector<int>> &adj_m, vector<pair<int, int>> &s) {
	// 處理每個點的packet數量，並且將packet數量儲存到node裡面，使用流出量等於這個節點的附載來實作
	// 克西荷夫定理，節點分析
	int node_num = n.size() - 1;
	for (int i = 0; i < node_num; i++) {
		for (auto &j : DAG[i]) {
			n[i].packet += adj_m[i][j];
		}
	}
 
}
 
bool judge_max_flow(int packet, vector<node> &n, vector<pair<int, int>> &source, vector<vector<int>> &DAG, int dst, vector<vector<Edge>> &G) {
	// 建立一個新的點，一開始將所有packet匯集在這裡流出去
	// 這樣可以方便處理多源點問題
	for (auto &i : n)
		i.packet = 0;
	// 將所有點負載設為0
	int node_num = n.size() - 1;
	// 計算要傳送到dst的總packet數
	int total = accumulate(source.begin(), source.end(), 0, [](int a, pair<int, int> b) {return a + b.second; });
	// 假想一個點，有所有的流量，從這個假想點出發以方便實作多源點最大流
	n.back().packet = total;
	// 從假想點到各個真實源點
	for (int i = 0; i < source.size(); i++) {
		add_edge(source[i].second, node_num, source[i].first, G);
	}
	// 將無向圖加入流的邊
	for (int i = 0; i < node_num; i++) {
		for (int j = 0; j < DAG[i].size(); j++) {
			add_edge(packet, i, DAG[i][j], G);
		}
	}
	// 如果全部的packet都可以送達，則回傳true
	if (max_flow(node_num, dst, total, G, n) == total) {
		return true;
	}
	return false;
}
 
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
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
		int id, source, dst, flow_size;
		cin >> id >> source >> dst >> flow_size;
		save_source[dst].push_back({ source, flow_size });
	}
	for (int i = 0; i < Dst_num; i++) {
		// 建立DAG
		auto DAG = Create_DAG(dst[i], graph, n, graph_matrix, save_source[dst[i]]);
		// 用DAG建立計算最大流的圖
		vector<vector<Edge>> G;
		int flow_size = 1;
		int left = 0, right = 300;
		// 計錄二分搜的結果
		vector<int> table(301, -1);
		G.resize(node_num + 1);
		while (left <= right) {
			int mid = (left + right) / 2;
			if (table[mid - 1] == -1) {
				G.resize(node_num + 1);
				// 對邊的容量進行二分搜
				table[mid - 1] = judge_max_flow(mid - 1, n, save_source[dst[i]], DAG, dst[i], G) ? 1 : 0;
				G.clear();
			}
			if (table[mid] == -1) {
				G.resize(node_num + 1);
				// 對邊的容量進行二分搜
				table[mid] = judge_max_flow(mid, n, save_source[dst[i]], DAG, dst[i], G) ? 1 : 0;
				G.clear();
			}
			if (table[mid] == true && table[mid - 1] == false) {
				flow_size = mid;
				break;
			}
			table[mid] == true ? right = mid - 1 : left = mid + 1;
		}
		G.clear();
		G.resize(node_num + 1);
		// 為了實作方便，把最後二分搜出來的邊的容量再拿去做最大流
		judge_max_flow(flow_size, n, save_source[dst[i]], DAG, dst[i], G);
		vector<vector<int>> adj_m(node_num, vector<int>(node_num, 0));
		// 用殘量圖轉換出每個邊的當前負載
		for (int j = 0; j < node_num; j++) {
			for (int k = 0; k < G[j].size(); k++) {
				if (G[j][k].to >= node_num or G[j][k].from >= node_num) continue;
				adj_m[G[j][k].from][G[j][k].to] = flow_size - G[j][k].cap;
			}
		}
		// 計算出每個節點的當前負載
		save_node_packet(n, DAG, adj_m, save_source[dst[i]]);
		// 計算所有SDN節點的分流比例，使用 流向的邊的附載 / 流出節點的負載
		for (int j = 0; j < node_num; j++) {
			double sum = (double)n[j].packet;
			// 若有點沒有任何的負載，則平均分配，或隨便分配都沒差，反正也沒東西流過去
			if (sum == 0) {
				sum = DAG[j].size();
				if (sum == 0) continue;
				for (auto &k : DAG[j]) {
					n[j].save_flow[dst[i]].push_back({ k, 1.0 / sum });
				}
			}
			else {
				// 反之，則計算出分流比例
				for (auto &k : DAG[j]) {
					n[j].save_flow[dst[i]].push_back({ k, adj_m[j][k] / sum });
				}
			}
		}
	}
	// 輸出結果
	output(n, dst, node_num);
	return 0;
}