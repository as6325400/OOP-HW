#include <bits/stdc++.h>

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

vector<int> bfs(int dst, vector<vector<int>> &graph, int node_num){
  queue<int> save;
  vector<int> ans(node_num, -1);
  save.push(dst);
  ans[dst] = dst;
  while(!save.empty()){
    int n = save.front();
    save.pop();
    for(auto &i:graph[n]){
      if(ans[i] == -1){
        ans[i] = n;
        save.push(i);
      }
    }
  }
  return ans;
}

void output(vector<node> &n, vector<int> dst){
  for(int i = 0; i < n.size(); i++){
    cout << i << '\n';
    if(n[i].is_SDN()){
      for (int j = 0; j < dst.size(); j++)
        cout << dst[j] << ' ' << n[i].save_flow[dst[j]][0].first << ' ' << n[i].save_flow[dst[j]][0].second * 100 << "%" << '\n';
    }
    else{
      for (int j = 0; j < dst.size(); j++)
        cout << dst[j] << ' ' << n[i].save_flow[dst[j]][0].first << '\n';
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
    auto temp = bfs(dst[i], graph, node_num);
    for(int j = 0; j < node_num; j++){
      n[j].save_flow[dst[i]] = {{temp[j], 1.0}};
    }
  }
  output(n, dst);
  return 0;
}