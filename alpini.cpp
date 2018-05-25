using namespace std;
#include "alpini.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

int N, M;
#define graph vector<vector<int>>

vector<bool> isValid;
vector<int> numeroVicini;

bool hasCycleWrp(graph &grafo, int now, vector<bool> &visited, int prec) {
  visited[now] = true;
  for (int i = 0; i < grafo[now].size(); i++) {
    int vicino = grafo[now][i];
    if (vicino != prec && isValid[vicino]) {
      if (visited[vicino])
        return true;
      else {
        prec = now;
        hasCycleWrp(grafo, vicino, visited, prec);
      }
    }
  }

  return false;
}

bool hasCycle(graph &grafo, int nodo) {
  vector<bool> visited(grafo.size());
  return hasCycleWrp(grafo, nodo, visited, -1);
}

int noNeighbour(vector<int> v) {
  int res = 0;
  for (int k = 0; k < v.size(); k++) {
    if (isValid[v[k]] == true)
      res++;
  }

  return res;
}

void unvalidateFoglie(graph g) {
  queue<int> q;

  for (int i = 0; i < g.size(); i++) {
    if (noNeighbour(g[i]) < 2)
      q.push(i);
  }

  while (!q.empty()) {

    int tmp = q.front();
    isValid[tmp] = false;

    for (int k = 0; k < g[tmp].size(); k++) {
      if (isValid[g[tmp][k]] == true && noNeighbour(g[g[tmp][k]]) < 2)
        q.push(g[tmp][k]);
    }
    q.pop();
  }
}

bool allIsInvalid(graph g) {
  for (int i = 0; i < N; i++) {
    if (isValid[i] == true)
      return false;
  }

  return true;
}

void aggiornaNumeriVicini(graph g) {
  for (int i = 0; i < g.size(); i++) {
    if (isValid[i] == true)
      numeroVicini[i] = noNeighbour(g[i]);
    else
      numeroVicini[i] = 0;
  }
}

int coefficenteGiorgio(graph g, int node) {
  int tot = 0;
  for (int i = 0; i < g[node].size(); i++) {
    tot = tot + numeroVicini[g[node][i]];
  }

  return tot;
}

int main() {
  helpers::setup();
  ifstream in("input.txt");
  ofstream out("output.txt");

  in >> N >> M;

  graph g(N);
  isValid.resize(N);
  numeroVicini.resize(N);
  vector<int> nodiRimossi;

  for (int k = 0; k < N; k++) {
    isValid[k] = true;
  }

  for (int i = 0; i < M; i++) {
    int src, dest;
    in >> src >> dest;

    g[src].push_back(dest);
    g[dest].push_back(src);
  }

  unvalidateFoglie(g);
  int maxNode;
  int maxValue;

  while (allIsInvalid(g) == false) {
    aggiornaNumeriVicini(g);

    maxNode = 0;
    maxValue = 0;
    for (int i = 0; i < N; i++)
      if (isValid[i] == true) {
        int tmp = coefficenteGiorgio(g, i);

        if (maxValue < tmp) {
          maxNode = i;
          maxValue = tmp;
        }
      }

    isValid[maxNode] = false;
    unvalidateFoglie(g);
    nodiRimossi.push_back(maxNode);
  }
  out << nodiRimossi.size();
  for (int k = 0; k < nodiRimossi.size(); k++) {
    out << " " << nodiRimossi[k];
  }
  out << "#";

  // cout << "Nodi vivi:\n";
  // unvalidateFoglie(g);
  // for(int j=0;j<N;j++){
  //   if(isValid[j]==true)
  //     cout << j << " ";
  // }

  return 0;
}