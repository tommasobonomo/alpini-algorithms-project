using namespace std;
#include "alpini.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

#define graph vector<vector<int>>

bool hasCycleWrp(graph &grafo, int now, vector<bool> &visited, int prec,
                 vector<bool> &isValid) {
  visited[now] = true;
  for (int i = 0; i < grafo[now].size(); i++) {
    int vicino = grafo[now][i];
    if (vicino != prec && isValid[vicino]) {
      if (visited[vicino])
        return true;
      else {
        prec = now;
        hasCycleWrp(grafo, vicino, visited, prec, isValid);
      }
    }
  }

  return false;
}

bool hasCycle(graph &grafo, int nodo, vector<bool> &isValid) {
  vector<bool> visited(grafo.size());
  return hasCycleWrp(grafo, nodo, visited, -1, isValid);
}

bool isGraphEmpty(vector<bool> &isValid) {
  for (bool node : isValid) {
    if (node) {
      return false;
    }
  }
  return true;
}

// void unvalidateFoglie(graph g) {
//   queue<int> q;

//   for (int i = 0; i < g.size(); i++) {
//     if (noNeighbour(g[i]) < 2)
//       q.push(i);
//   }

//   while (!q.empty()) {

//     int tmp = q.front();
//     isValid[tmp] = false;

//     for (int k = 0; k < g[tmp].size(); k++) {
//       if (isValid[g[tmp][k]] == true && noNeighbour(g[g[tmp][k]]) < 2)
//         q.push(g[tmp][k]);
//     }
//     q.pop();
//   }
// }

int numNeighbours(vector<int> &neighbours, vector<bool> &isValid) {
  int res = 0;
  for (int neighbour : neighbours) {
    if (isValid[neighbour])
      res++;
  }
  return res;
}

void pruneLeaves(graph &grafo, vector<bool> &isValid) {
  queue<int> q;

  // Aggiungo tutte le foglie alla coda
  for (int i = 0; i < grafo.size(); i++) {
    if (numNeighbours(grafo[i], isValid) < 2) {
      q.push(i);
    }
  }

  while (!q.empty()) {
    int leaf = q.front();
    isValid[leaf] = false;

    for (int child : grafo[leaf]) {
      if (isValid[child] && numNeighbours(grafo[child], isValid) < 2) {
        q.push(child);
      }
    }
    q.pop();
  }
}

int main() {

  helpers::setup();
  ifstream in("input.txt");
  ofstream out("output.txt");

  int N, M;
  in >> N >> M;

  graph g(N);
  vector<bool> isValid;
  isValid.resize(N, true);
  vector<int> nodiRimossi;

  // for (int k = 0; k < N; k++) {
  //   isValid[k] = true;
  // }

  for (int i = 0; i < M; i++) {
    int src, dest;
    in >> src >> dest;

    g[src].push_back(dest);
    g[dest].push_back(src);
  }

  pruneLeaves(g, isValid);

  while (!isGraphEmpty(isValid)) {

    for (int i = 0; i < N; i++)
      if (isValid[i] == true) {
      }

    // isValid[maxNode] = false;
    // nodiRimossi.push_back(maxNode);
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