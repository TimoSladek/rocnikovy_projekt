#include "bits/stdc++.h"

using namespace std;

struct Graph {
    vector<pair<vector<int>, set<int>>> vertices;//kazdy vrchol obsahuje dvojicu vektor a set, vektor obsahuje vrcholy kde ide hrana z daneho vrcholu a set je mnozina parovani v ktorych sa nachadza dany vrchol
    vector<int> danglingEdges;
};

vector<Graph> graphs;
set<int> visited;// navstivene vrcholy
vector<int> path;
bool result = false;

bool isPossible(Graph &G) {
    for (int i = 1; i < G.vertices.size(); ++i) {
        if (G.vertices[i].second.size() != 4){
            return false;
        }
    }
    return true;
}

void checkIfPossible(Graph &G,int vertex) {
    if (visited.size()+1 == G.vertices.size()) {
        if (isPossible(G)){
            result = true;
        }
    } else if (visited.count(vertex)) {
        if (G.vertices[vertex].second.size() != 4) {
            return;
        }else {
            visited.erase(path.back());
            checkIfPossible(G, path.back());
        }
    } else {
        vector<int> availableMatchings;
        for (int j = 0; j < 4; ++j) {
            if (G.vertices[vertex].second.count(j))continue;
            availableMatchings.push_back(j);
        }
        for (int where : G.vertices[vertex].first) {
            if (where == 0 || where == path[path.size()-1])continue;
            //1 parenie na hranu
            for (int availableMatching : availableMatchings) {
                if (G.vertices[where].second.count(availableMatching))continue;
                G.vertices[vertex].second.insert(availableMatching);
                G.vertices[where].second.insert(availableMatching);
                visited.insert(vertex);
                path.push_back(vertex);
                checkIfPossible(G, where);
                path.pop_back();
                visited.erase(vertex);
                G.vertices[vertex].second.erase(availableMatching);
                G.vertices[where].second.erase(availableMatching);
            }
            //2 parenia na hranu
            for (int j = 0; j < availableMatchings.size(); ++j) {
                for (int k = j + 1; k < availableMatchings.size(); ++k) {
                    if (G.vertices[where].second.count(availableMatchings[j]) || G.vertices[where].second.count(availableMatchings[k]))continue;
                    G.vertices[vertex].second.insert(availableMatchings[j]);
                    G.vertices[where].second.insert(availableMatchings[j]);
                    G.vertices[vertex].second.insert(availableMatchings[k]);
                    G.vertices[where].second.insert(availableMatchings[k]);
                    visited.insert(vertex);
                    path.push_back(vertex);
                    checkIfPossible(G, where);
                    path.pop_back();
                    visited.erase(vertex);
                    G.vertices[vertex].second.erase(availableMatchings[j]);
                    G.vertices[where].second.erase(availableMatchings[j]);
                    G.vertices[vertex].second.erase(availableMatchings[k]);
                    G.vertices[where].second.erase(availableMatchings[k]);
                }
            }
        }
    }
}

bool areCompatibleEdges(const pair<vector<int>, set<int>> &edge1, const pair<vector<int>, set<int>> &edge2) {
    if (edge1.second.size() >= 2 && edge2.second.size() >= 2) {
        return edge1.second == edge2.second;
    } else if (edge1.second.size() < 2 && edge2.second.size() >= 2) {
        return includes(edge2.second.begin(), edge2.second.end(), edge1.second.begin(), edge1.second.end());
    } else if (edge1.second.size() >= 2 && edge2.second.size() < 2) {
        return includes(edge1.second.begin(), edge1.second.end(), edge2.second.begin(), edge2.second.end());
    }
    return true;
}

Graph combineGraphs(Graph &G1, Graph &G2, int e1, int e2) {
    int offset = G1.vertices.size();
    G1.vertices.resize(G1.vertices.size() + G2.vertices.size() - 1);

    for (int i = 1; i < G2.vertices.size(); ++i) {
        G1.vertices[offset + i - 1] = G2.vertices[i];
        for (int &j : G1.vertices[offset + i - 1].first) {
            j = (j == 0) ? 0 : j + offset - 1;
        }
    }

    int v1 = G1.danglingEdges[e1];
    int v2 = offset + G2.danglingEdges[e2] - 1;
    G1.vertices[v1].first.push_back(v2);
    G1.vertices[v2].first.push_back(v1);

    vector<int> newDanglingEdges;
    for (int i = 0; i < G1.danglingEdges.size(); ++i) {
        if (i != e1) newDanglingEdges.push_back(G1.danglingEdges[i]);
    }
    for (int i = 0; i < G2.danglingEdges.size(); ++i) {
        if (i != e2) newDanglingEdges.push_back(offset + G2.danglingEdges[i] - 1);
    }
    G1.danglingEdges = newDanglingEdges;

    return G1;
}

bool combineAllGraphs(vector<Graph> &graphss) {
    while (graphss.size() > 1) {
        bool merged = false;
        for (int i = 0; i < graphss.size() && !merged; ++i) {
            for (int j = i + 1; j < graphss.size() && !merged; ++j) {
                for (int e1 = 0; e1 < graphss[i].danglingEdges.size() && !merged; ++e1) {
                    for (int e2 = 0; e2 < graphss[j].danglingEdges.size() && !merged; ++e2) {
                        if (areCompatibleEdges(graphss[i].vertices[graphss[i].danglingEdges[e1]], graphs[j].vertices[graphs[j].danglingEdges[e2]])) {
                            graphss[i] = combineGraphs(graphss[i], graphss[j], e1, e2);
                            graphss.erase(graphss.begin() + j);
                            merged = true;
                        }
                    }
                }
            }
        }
        if (!merged) {
            return false;
        }
    }

    return true;
}

int main() {
    int numOfGraphs;
    cout << "Zadajte pocet grafov:" << endl;
    cin >> numOfGraphs;
    //pocet vrcholov
    for(int i = 0; i < numOfGraphs; ++i) {
        int x, y, n;
        cout << "zadajte pocet vrcholov: " << endl;
        cin >> n;
        //vstupny graf
        Graph G;
        G.vertices.resize(n + 1);
        //pre jednoduchost, trciace hrany idu do vrcholu 0 a vrcholy v grafe su od 1 po n
        cout << "zadajte hrany: " << endl;
        for (int j = 0; j < n + (n / 2) + 2; ++j) {
            cin >> x >> y;
            G.vertices[x].first.push_back(y);
            G.vertices[y].first.push_back(x);
        }
        //parovania pre trciace hrany
        bool possible = true;
        for (int j = 0; j < 4; ++j) {
            int count = 0;
            cout << "zadajte hrany ktore patria do parovania " << j + 1 <<
                 " alebo -1 pre pokracovanie: " << endl;
            while (true) {
                cin >> x;
                if (x == -1) {
                    break;
                } else {
                    cin >> y;
                }
                count++;
                G.vertices[x].second.insert(j);
                G.vertices[y].second.insert(j);
            }
            if (count % 2 != 0) {
                possible = false;
            }
        }
        if (!possible) {
            cout << "false" << endl;
            return 0;
        }
        for (int j = 1; j < 5; ++j) {
            if (G.vertices[j].second.empty() || G.vertices[j].second.size() > 2) {
                cout << "false" << endl;
                return 0;
            }
        }

        graphs.push_back(G);
    }

    if (!combineAllGraphs(graphs)) {
        cout << "false" << endl;
        return 0;
    }

    Graph &finalGraph = graphs[0];
    path.push_back(0);
    checkIfPossible(finalGraph, finalGraph.vertices[0].first[0]);
    if (result) {
        cout << "true" << endl;
    } else {
        cout << "false" << endl;
    }
    return 0;
}
