#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>

using ll = long long;
using namespace std;

class node
{
  private:
    // type用來記錄這個節點是否為SDN節點，true代表為SDN節點。
    bool type;
  public:
    map<unsigned int, vector<pair<unsigned int, double>>> save_flow;
    node(){
      type = 0;
    }
    bool is_SDN(){
      return type == true;
    }
    void change_type(){
      type = !type;
    }
};

vector<vector<int>> create_DAG(vector<vector<int>> &graph, int node_num, int dst, vector<node> &n){
  vector<vector<int>> DAG(node_num);
  queue<int> save;
  save.push(dst);
  vector<bool> visit(node_num, false);

  // 確認有沒有被其他點插入過
  vector<vector<bool>> check(node_num, vector<bool>(node_num, false));
  while (!save.empty())
  {
    int node = save.front();
    save.pop();
    // node 是被插的
    for(auto &i:graph[node]){
      // 代表i點沒被node點插過
      // 所以i點可以插入node點
      if(check[i][node]) continue;
      if(check[node][i]) continue;
      if(n[i].is_SDN()){
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
  return DAG;
}


vector<vector<double>> topological_dp(vector<vector<int>> &DAG, int node_num){
  vector<vector<double>> ans(node_num,vector<double>(node_num + 1, 0));
  vector<int> out_degree(node_num, 0);
  for(int i = 0; i < node_num; i++){
    out_degree[i] = DAG[i].size();
  }
  vector<vector<int>> out_graph(node_num);
  for(int i = 0; i < node_num; i++){
    for(auto &j:DAG[i]){
      out_graph[j].push_back(i);
    }
  }
  queue<int> save;
  for(int i = 0; i < node_num; i++){
    if(out_degree[i] == 0){
      save.push(i);
    }
  }
  while(!save.empty()){
    int node = save.front();
    // cout << node << '\n';
    save.pop();
    ans[node][node_num] = accumulate(ans[node].begin(), ans[node].end(), 0.0);
    // for(int i = 0; i < node_num; i++){
    //   cout << ans[node][i] << ' ';
    // }
    // cout << '\n';
    // cout << "edge: ";
    for(auto &i:out_graph[node]){
      // cout << i << ' ';
      ans[i][node] = ans[node][node_num] + 1;
    }
    // cout << '\n';
    for(auto &i:out_graph[node]){
      out_degree[i]--;
      if(out_degree[i] == 0){
        save.push(i);
      }
    }
  }
  // for(int i = 0; i <= node_num; i++){
  //   for(int j = 0; j < node_num; j++){
  //     cout << ans[j][i] << ' ';
  //   }
  //   cout << '\n';
  // }
  return ans;
}

void output(vector<node> &n, vector<int> &dst){
  for(int i = 0; i < n.size(); i++){
    cout << i << '\n';
    if(n[i].is_SDN()){
      for(int j = 0; j < dst.size(); j++){
        if(dst[j] == i){
          cout << i << ' ' << dst[j]  << " 100%\n";
          continue;
        }
        cout << dst[j] << ' ';
        for(auto &k:n[i].save_flow[dst[j]]){
          cout << k.first << ' ' << k.second * 100.0<< "% ";
        }
        cout << '\n';
      }
    }else{
      for(int j = 0; j < dst.size(); j++){
        if(dst[j] == i){
          cout << i << ' ' << dst[j] << '\n';
          continue;
        }
        cout << dst[j] << ' ' << n[i].save_flow[dst[j]][0].first << "\n";
      }
    }
  }
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int node_num, SDN_node_num, Dst_num, Links_num, source_node_num;
  cin >> node_num >> SDN_node_num >> Dst_num >> Links_num >> source_node_num;
  vector<node> n(node_num); //建立一個存點的陣列
  vector<vector<int>> graph(node_num);
  vector<int> dst(Dst_num);
  map<int, vector<int>> save_source; //建立每一個dst對source點
  for(int i = 0; i < SDN_node_num; i++){
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
  for(int i = 0; i < Links_num; i++){
    // 建圖
    int id, start, end;
    cin >> id >> start >> end;
    graph[start].push_back(end);
    graph[end].push_back(start);
  }
  for(int i = 0; i < source_node_num; i++){
    // 建立source跟dst的點對
    // baseline不用考慮flow大小
    int id, source, dst, flow_size;
    cin >> id >> source >> dst >> flow_size;
    save_source[dst].push_back(source);
  }
  for(int i = 0; i < Dst_num; i++){
    // 建立一個DAG
    vector<vector<int>> DAG = create_DAG(graph, node_num, dst[i], n);
    // n[i].save_flow[dst[i]].push_back(make_pair(dst[i], 1.0));
    auto ans = topological_dp(DAG, node_num);
    for(int j = 0; j < node_num; j++){
      auto nums = (double)ans[j][node_num];
      if(nums == 0){
        n[j].save_flow[dst[i]].push_back(make_pair(dst[i], 1.0));
        continue;
      }
      if(!n[j].is_SDN()) n[j].save_flow[dst[i]].push_back(make_pair(DAG[j][0], 1.0));
      else{
        for(int k = 0; k < node_num; k++){
          if(ans[j][k] != 0){
            n[j].save_flow[dst[i]].push_back(make_pair(k, ans[j][k]/nums));
          }
        }
      }
    }
  }
  output(n, dst);
  return 0;
}