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
	map<unsigned int, vector<pair<unsigned int, double>>> save_flow;
	node() {
		type = 0;
	}
	bool is_SDN() {
		return type == true;
	}
	void change_type() {
		type = !type;
	}
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

void output(vector<node> &n, vector<int> &dst) {
	for (int i = 0; i < n.size(); i++) {
		cout << i << '\n';
		if (n[i].is_SDN()) {
			for (int j = 0; j < dst.size(); j++) {
				if (dst[j] == i) {
					cout << i << ' ' << dst[j] << " 100%\n";
					continue;
				}
				cout << dst[j] << ' ';
				for (auto &k : n[i].save_flow[dst[j]]) {
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

vector<vector<int>> Create_DAG(int node_num, vector<vector<int>> &level, vector<node> &n, vector<vector<bool>> &graph) {
	vector<vector<int>> DAG(node_num);
	for (int i = node_num - 1; i >= 1; i--) {
		if (level[i].size() == 0) continue;
		// 便歷那一層的節點數
		for (int j = 0; j < level[i].size(); j++) {
			for (int k = 0; k < level[i - 1].size(); k++) {
				if (graph[level[i][j]][level[i - 1][k]] == true) {
					DAG[level[i][j]].push_back(level[i - 1][k]);
					// cout << level[i][j] << ' ' << level[i - 1][k] << ' ' << endl;
					if (!n[level[i][j]].is_SDN()) break;
				}
			}
		}
	}
	return DAG;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	int node_num, SDN_node_num, Dst_num, Links_num, source_node_num;
	cin >> node_num >> SDN_node_num >> Dst_num >> Links_num >> source_node_num;
	vector<node> n(node_num); //建立一個存點的陣列
	vector<vector<int>> graph(node_num);
	vector<vector<bool>> graph_matrix(node_num, vector<bool>(node_num, false));
	vector<int> dst(Dst_num);
	map<int, vector<int>> save_source; //建立每一個dst對source點
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
		save_source[dst].push_back(source);
	}
	for (int i = 0; i < Dst_num; i++) {
		auto temp = bfs(dst[i], graph, node_num);
		auto DAG = Create_DAG(node_num, temp, n, graph_matrix);
		n[dst[i]].save_flow[dst[i]].push_back({ dst[i], 1.0 });

		// for (int j = 0; j < node_num; j++)
		// {
		// 	cout << j << endl;
		// 	for (auto a : DAG[j]) {
		// 		cout << a << ' ';
		// 	}
		// 	cout << endl;
		// }

		for (int j = 0; j < node_num; j++) {
			double number = (double)DAG[j].size();
			for (int k = 0; k < DAG[j].size(); k++) {
				n[j].save_flow[dst[i]].push_back({DAG[j][k], 1.0 / number});
			}
		}
	}
	output(n, dst);
	return 0;
}