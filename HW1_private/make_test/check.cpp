#include "testlib.h"
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

class node
{
  private:
    // type用來記錄這個節點是否為SDN節點，true代表為SDN節點。
    bool type;
  public:
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

bool no_cycle(int numCourses, vector<vector<int>>& prerequisites) {
    vector<int> res(numCourses, 0);
    vector<vector<int>> graph(numCourses);
    for(auto &i:prerequisites){
        graph[i[1]].push_back(i[0]);
        res[i[0]]++;
    }
    queue<int> s;
    for(int i = 0; i < numCourses; i++)
        if(res[i] == 0) s.push(i);
    for(int i = 0; i <= numCourses; i++){
        if(i == numCourses) return true;
        if(s.empty()) break;
        int node = s.front();
        for(auto &j:graph[node]){
            res[j]--;
            if(res[j] == 0) s.push(j);
        }
        s.pop();
    }
    return false;
}


bool dfs(int source, vector<vector<int>> &to_nodeid, vector<bool> &check){
  // 需傳入起點和圖
  // 圖為一個二維陣列，每層第一個代表終點，後面代表next點
  if(check[source]) return true;
  bool temp = true;
  for(int i = 1; i < to_nodeid[source].size(); i++){
    auto ttt = dfs(to_nodeid[source][i], to_nodeid, check);
    if(ttt == false) return false;
    if(ttt == true) check[to_nodeid[source][i]] = true;
    temp = temp && ttt;
  }
  return temp;
}


void dfs2(vector<vector<int>> &to_nodeid, vector<vector<double>> &percantage, vector<double> &flow_size){
  vector<int> res(flow_size.size(), 0);
  for(int i = 0; i < flow_size.size(); i++){
    for(int k = 1; k < to_nodeid[i].size(); k++){
      res[to_nodeid[i][k]]++;
    }
  }
  queue<int> s;
  for(int i = 0; i < flow_size.size(); i++)
      if(res[i] == 0) s.push(i);
  for(int i = 0; i < flow_size.size(); i++){
      if(i == flow_size.size()) return;
      if(s.empty()){
        // cout << "dadawd" << '\n';
        break;
      }
      int node = s.front();
      // cout << node << '\n';
      for(int j = 1;j < to_nodeid[node].size(); j++){
          // cout << to_nodeid[node][j] << '\n';
          res[to_nodeid[node][j]]--;
          if(percantage[node].size() == 0) flow_size[to_nodeid[node][j]] += flow_size[node];
          else flow_size[to_nodeid[node][j]] += flow_size[node] * percantage[node][j - 1];
          if(res[to_nodeid[node][j]] == 0) s.push(to_nodeid[node][j]);
      }
      s.pop();
  }
  // cout << " flowsize\n";
  // for(int j = 0; j < flow_size.size(); j++){
  //   cout << flow_size[j] << '\n';
  // }
}



int main(int argc, char* argv[]) {
  registerTestlibCmd(argc, argv); // 注冊testlib命令行參數
  int node_num, SDN_node_num, Dst_num, Links_num, source_node_num;
  // 讀入input文件中的數據
  node_num = inf.readInt(); // 讀入數據長度
  SDN_node_num = inf.readInt();
  Dst_num = inf.readInt();
  Links_num = inf.readInt();
  source_node_num = inf.readInt();
  vector<node> n(node_num); //建立一個存點的陣列
  vector<int> dst(Dst_num);
  set<pair<int, int>> save_graph;
  vector<vector<int>> source(source_node_num);
  map<int, vector<pair<int, int>>> save_source; //建立每一個dst對source點
  map<int, int> dst_index;
  for (int i = 0; i < SDN_node_num; i++)
  {
    int SDN_node;
    SDN_node = inf.readInt();
    n[SDN_node].change_type();
  }
  for (int i = 0; i < Dst_num; i++)
  { 
    dst[i] = inf.readInt();
    dst_index[dst[i]] = i;
  }
  for (int i = 0; i < Links_num; i++)
  {
    int id, a, b;
    id = inf.readInt();
    a = inf.readInt();
    b = inf.readInt();
    save_graph.insert(make_pair(a, b));
    save_graph.insert(make_pair(b, a));
  }
  for(int i = 0; i < node_num; i++){
    save_graph.insert(make_pair(i, i));
  }
  for (int i = 0; i < source_node_num; i++)
  {
    int source_id, source_node, dst_node, flow;
    source_id = inf.readInt();
    source_node = inf.readInt();
    dst_node = inf.readInt();
    flow = inf.readInt();
    save_source[dst_node].push_back({source_node, flow});
    source[i].push_back(source_id);
    source[i].push_back(source_node);
    source[i].push_back(dst_node);
    source[i].push_back(flow);
  }
  // inf.readEof(); // 處理input文件結束
  // 處理input文件結束
  // 處理output文件開始
  vector<int> save_out_nodeid;
  vector<vector<vector<int>>> save_out_trace(node_num);
  // 用來存放每個點的路徑，第一維是點的編號，第二維是第幾個目的地，第三維是終點及next點
  vector<vector<vector<double>>> save_out_percantage(node_num);
  // 用來存放SDN點分叉的百分比，為了十座方便，第一維是點的編號，第二維是第幾個目的地，第三維是分叉的百分比
  for (int i = 0; i < node_num; i++)
  { 
    save_out_trace[i].resize(Dst_num);
    int out_nodeid;
    out_nodeid = ouf.readInt();
    ouf.readEoln();
    save_out_nodeid.push_back(out_nodeid);
    for(int j = 0; j < Dst_num; j++){
      auto s = ouf.readString();
      stringstream ss(s);
      int to_dst_id;
      ss >> to_dst_id;
      save_out_trace[i][j].push_back(to_dst_id);
      save_out_percantage[i].resize(Dst_num);
      if(!n[i].is_SDN()){
        int next_nodeid;
        ss >> next_nodeid;
        save_out_trace[i][j].push_back(next_nodeid);
      }
      else{
        string save_string;
        int count = 0;
        while(ss >> save_string){
          if(count % 2 == 0){
            save_out_trace[i][j].push_back(stoi(save_string));
          }
          else{
            save_string.pop_back();
            save_out_percantage[i][j].push_back(stod(save_string) * 0.01);
            if (stod(save_string) - 0.0 < 1e-6)
            {
              save_out_percantage[i][j].pop_back();
              save_out_trace[i][j].pop_back();
            }
            
          }
          count++;
        }
      }
    }
  }
  // 處理output文件結束
  // 檢查output文件開始
  // 檢查點的編號是否正確
  for (int i = 0; i < node_num; i++){
    if(save_out_nodeid[i] != i) quitf(_wa, "Node id error"); // 點的編號不正確
    if(save_out_trace[i].size() != Dst_num) quitf(_wa, "dst num error"); // 目的地數量不正確

    for(int j = 0; j < Dst_num; j++){
      if(n[i].is_SDN()){
        double sum = 0.0;
        for(int k = 0; k < save_out_percantage[i][j].size(); k++){
          sum += save_out_percantage[i][j][k];
        }
        if(sum > 1.01 || sum < 0.99){
          quitf(_wa, "SDN node percantage sum is not 100"); // SDN點的分叉百分比和不正確
        }
        int num = save_out_trace[i][j].size() + save_out_percantage[i][j].size();
        if(num % 2 == 0) quitf(_wa, "SDN node output error"); // SDN點的輸出不正確
      }
      if(!n[i].is_SDN() && save_out_trace[i][j].size() != 2) quitf(_wa, "normal node output error"); // 非SDN點的輸出不正確
      if(save_out_trace[i][j][0] != dst[j]) quitf(_wa, "dst node dst error"); // dst點的目的地不是自己
      for(int k = 1; k < save_out_trace[i][j].size(); k++){
        if(save_graph.count({i, save_out_trace[i][j][k]}) == 0){
          quitf(_wa, "link is not exist"); // 路徑不存在
        }
      }
    }
  }
  // 基本檢查通過，開始檢查流量，及環
  for(int j = 0; j < Dst_num; j++){
    vector<vector<int>> save_link;
    // 用來存放路徑上的link，第一維是link的起點，第二維是link的終點
    for(int i = 0; i < node_num; i++){
      if(i == dst[j]) continue;
      for(int k = 1; k < save_out_trace[i][j].size(); k++){
        vector<int> temp;
        temp.push_back(i);
        temp.push_back(save_out_trace[i][j][k]);
        save_link.push_back(temp);
      }
    }
    if(!no_cycle(node_num, save_link)) quitf(_wa, "have cycle"); // 有環
  }
  // 檢查是否有流量沒到終點的情況
  vector<vector<vector<int>>> ss_link(Dst_num);
  vector<vector<vector<double>>> percantage(Dst_num);
  // 用來存放路徑上的link，第一維是第幾個dst，第二維是link的起點，第三維是link的next點
  for(int i = 0; i < Dst_num; i++){
    ss_link[i].resize(node_num);
    percantage[i].resize(node_num);
    for(int j = 0; j < node_num; j++){
      if(j == dst[i]){
        ss_link[i][j].push_back(j);
        continue;
      }
      for(int k = 0; k < save_out_trace[j][i].size(); k++){
        ss_link[i][j].push_back(save_out_trace[j][i][k]);
      }
      for(int k = 0; k < save_out_percantage[j][i].size(); k++){
        // cout<<j<<' '<<save_out_trace[j][i][k + 1]<<' '<<save_out_percantage[j][i][k]<<endl;
        percantage[i][j].push_back(save_out_percantage[j][i][k]);
      }
    }
  }
  for(int i = 0; i < source_node_num; i++){
    int source_id = source[i][0];
    int source_node = source[i][1];
    int dst_node = source[i][2];
    int flow = source[i][3];
    vector<bool> check(node_num, false);
    check[dst_node] = true;
    if(dfs(source_node, ss_link[dst_index[dst_node]], check) == false) quitf(_wa, "flow not to dst"); // 有流量沒有到達終點
  }
  // cout<<Dst_num<<"\n";
  double ans = -1e9;
  for(int i = 0; i < Dst_num; i++){
    // cout<<i<<endl;
    vector<double> flow_size(node_num, 0);
    // 用來存放每個點的流量
    // cout<<"wdawdwadwadwadawdwad"<<endl;
    // cout<<save_source[dst[i]].size()<<endl;
    for (int j = 0; j < save_source[dst[i]].size(); j++)
    { 
      // cout << j << endl;
      flow_size[save_source[dst[i]][j].first] = (double)save_source[dst[i]][j].second;
    }
    // cout<<"wdawdwadwadwadawdwad"<<endl;
    dfs2(ss_link[i], percantage[i], flow_size);
    // cout<<"flow_size"<<endl;
    for(int j = 0; j < node_num; j++){
      if(j == dst[i]) continue;
      if(!n[j].is_SDN()){
        ans = max(ans, flow_size[j]);
      }else{
        for(int k = 0; k < percantage[i][j].size(); k++){
          ans = max(ans, flow_size[j] * percantage[i][j][k]);
        }
      }
    }
    // cout<<"ans"<<endl;
  }
  int ans_int = (int)round(ans);
  quitf(_ok, "all data is correct, max link load %d", ans_int); // 如果沒有報錯，則認為檢查通過
}
