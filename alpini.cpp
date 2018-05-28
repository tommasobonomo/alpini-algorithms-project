using namespace std;
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

#include "alpini.h"

#define graph vector<vector<int>>

bool compareWeights(pair<int, int> x, pair<int, int> y) {
  return x.second > y.second;
}

void printDebug(int rem_node_index, int rem_node, vector<bool> &isValid,
                vector<int> &availableNodes, vector<int> &solution) {
  cout << "DEBUG:\n";
  cout << "rem_node_index  = " << rem_node_index << endl;
  cout << "rem_node        = " << rem_node << endl;

  cout << "isValid:\n";
  for (int i = 0; i < isValid.size(); i++) {
    cout << i << " ";
  }
  cout << endl;
  for (int i = 0; i < isValid.size(); i++) {
    cout << isValid[i] << " ";
  }
  cout << endl;

  cout << "availableNodes:\n";
  for (int i = 0; i < availableNodes.size(); i++) {
    cout << availableNodes[i] << " ";
  }
  cout << endl;

  cout << "solution:\n";
  for (int i = 0; i < solution.size(); i++) {
    cout << solution[i] << " ";
  }
  cout << endl;
}

// Funzione helper un attimo incasinata quindi la metto da parte
void removeNodeFromAvailable(vector<int> &availableNodes, const int node) {
  // find() trova l'iteratore dell'elemento node in availableNodes
  // erase() elimina l'elemento dal vettore tramite l'iteratore
  availableNodes.erase(
      find(availableNodes.begin(), availableNodes.end(), node));
}

// Funzione helper che controlla se ho finito il grafo
bool isGraphEmpty(const vector<int> &availableNodes) {
  return availableNodes.size() == 0;
}

int numNeighbours(const vector<int> &neighbours, const vector<bool> &isValid) {
  int res = 0;
  for (int neighbour : neighbours) {
    if (isValid[neighbour])
      res++;
  }
  return res;
}

int fattoreG(const graph &grafo, const int nodo, const vector<bool> &isValid) {
  int res = 0;
  for (int vicino : grafo[nodo]) {
    if (isValid[vicino]) {
      res += numNeighbours(grafo[vicino], isValid);
    }
  }
  return res;
}

void pruneLeavesAndSort(const graph &grafo, vector<bool> &isValid,
                        vector<int> &availableNodes) {
  queue<int> q;

  // Aggiungo tutte le foglie alla coda
  for (int i = 0; i < grafo.size(); i++) {
    if (isValid[i] && numNeighbours(grafo[i], isValid) < 2) {
      q.push(i);
      isValid[i] = false;
    }
  }

  // Per ogni foglia, la rimuovo e vedo se questo implica la creazione
  // di nuove foglie. Se si, aggiungo alla coda
  while (!q.empty()) {
    int leaf = q.front();

    for (int child : grafo[leaf]) {
      // Se child fa parte del grafo e è una foglia, aggiungilo alla coda
      if (isValid[child] && numNeighbours(grafo[child], isValid) < 2) {
        q.push(child);
        isValid[child] = false;
      }
    }

    // Tolgo la foglia dai nodi disponibili
    removeNodeFromAvailable(availableNodes, leaf);
    q.pop();
  }

  vector<pair<int, int>> nodeAndG(availableNodes.size());
  for (int i = 0; i < availableNodes.size(); i++) {
    nodeAndG[i].first = availableNodes[i];
    int deg = numNeighbours(grafo[availableNodes[i]], isValid);
    int fattG = fattoreG(grafo, availableNodes[i], isValid);
    long int rapp = deg * fattG;
    nodeAndG[i].second = rapp;
  }
  sort(nodeAndG.begin(), nodeAndG.end(), compareWeights);

  for (int i = 0; i < availableNodes.size(); i++) {
    availableNodes[i] = nodeAndG[i].first;
  }
}

void branchNbound(const graph &grafo, vector<bool> &isValid,
                  vector<int> &availableNodes, vector<int> &solution, int &best,
                  ofstream &out) {
  if (isGraphEmpty(availableNodes)) {
    best = solution.size();
    out << best;
    for (int k = 0; k < solution.size(); k++) {
      out << " " << solution[k];
    }
    out << " #\n";
    out.flush();
  } else {

    for (int choice : availableNodes) {
      if (solution.size() + 1 < best) {

        vector<int> tmpSolution = solution;
        tmpSolution.push_back(choice);

        vector<int> tmpAvailableNodes = availableNodes;
        removeNodeFromAvailable(tmpAvailableNodes, choice);

        vector<bool> tmpIsValid = isValid;
        tmpIsValid[choice] = false;

        pruneLeavesAndSort(grafo, tmpIsValid, tmpAvailableNodes);

        branchNbound(grafo, tmpIsValid, tmpAvailableNodes, tmpSolution, best,
                     out);
      }
    }
  }
}

int main() {

  helpers::setup();
  ifstream in("input.txt");
  ofstream out("output.txt");

  int N, M;
  in >> N >> M;

  graph g(N);

  srand(time(NULL));

  for (int i = 0; i < M; i++) {
    int src, dest;
    in >> src >> dest;

    g[src].push_back(dest);
    g[dest].push_back(src);
  }

  int best = N;

  vector<bool> isValid;
  isValid.resize(N, true);
  vector<int> solution;
  solution.resize(0);

  // Inizializzo vettore dei nodi disponibili con
  // tutti i nodi del grafo
  vector<int> availableNodes;
  availableNodes.resize(0);
  for (int i = 0; i < N; i++) {
    availableNodes.push_back(i);
  }

  pruneLeavesAndSort(g, isValid, availableNodes);

  branchNbound(g, isValid, availableNodes, solution, best, out);

  // completelyRandom(g, out);

  return 0;
}
