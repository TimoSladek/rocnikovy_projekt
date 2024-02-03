#include "bits/stdc++.h"

using namespace std;

vector<pair<vector<int>, set<int>>> G;//kazdy vrchol obsahuje dvojicu vektor a set, vektor obsahuje vrcholy kde ide hrana z daneho vrcholu a set je mnozina parovani v ktorych sa nachadza dany vrchol
set<int> visited;// navstivene vrcholy
vector<int> path;
bool result = false;

bool isPossible() {
    for (int i = 1; i < G.size(); ++i) {
        if (G[i].second.size() != 4){
            return false;
        }
    }
    return true;
}

void checkIfPossible(int vertex) {
    if (visited.size()+1 == G.size()) {
        if (isPossible()){
            result = true;
        }
    } else if (visited.count(vertex)) {
        if (G[vertex].second.size() != 4) {
            return;
        }else {
            visited.erase(path[path.size() - 1]);
            checkIfPossible(path[path.size() - 1]);
        }
    } else {
        vector<int> availableMatchings;
        for (int j = 0; j < 4; ++j) {
            if (G[vertex].second.count(j))continue;
            availableMatchings.push_back(j);
        }
        for (int i = 0; i < G[vertex].first.size(); ++i) {
            int where = G[vertex].first[i];
            if (where == 0 || where == path[path.size()-1])continue;
            //1 parenie na hranu
            for (int j = 0; j < availableMatchings.size(); ++j) {
                if (G[where].second.count(availableMatchings[j]))continue;
                G[vertex].second.insert(availableMatchings[j]);
                G[where].second.insert(availableMatchings[j]);
                visited.insert(vertex);
                path.push_back(vertex);
                checkIfPossible(where);
                path.pop_back();
                visited.erase(vertex);
                G[vertex].second.erase(availableMatchings[j]);
                G[where].second.erase(availableMatchings[j]);
            }
            //2 parenia na hranu
            for (int j = 0; j < availableMatchings.size(); ++j) {
                for (int k = j + 1; k < availableMatchings.size(); ++k) {
                    if (G[where].second.count(availableMatchings[j]) || G[where].second.count(availableMatchings[k]))continue;
                    G[vertex].second.insert(availableMatchings[j]);
                    G[where].second.insert(availableMatchings[j]);
                    G[vertex].second.insert(availableMatchings[k]);
                    G[where].second.insert(availableMatchings[k]);
                    visited.insert(vertex);
                    path.push_back(vertex);
                    checkIfPossible(where);
                    path.pop_back();
                    visited.erase(vertex);
                    G[vertex].second.erase(availableMatchings[j]);
                    G[where].second.erase(availableMatchings[j]);
                    G[vertex].second.erase(availableMatchings[k]);
                    G[where].second.erase(availableMatchings[k]);
                }
            }
        }
    }
}

int main() {
    int x, y, n;
    //pocet vrcholov
    cout << "zadajte pocet vrcholov:" << endl;
    cin >> n;
    G.resize(n + 1);
    //vstupny graf
    //pre jednoduchost, trciace hrany idu do vrcholu 0 a vrcholy v grafe su od 1 po n
    cout << "zadajte hrany:" << endl;
    for (int i = 0; i < n + (n / 2) + 2; ++i) {
        cin >> x >> y;
        G[x].first.push_back(y);
        G[y].first.push_back(x);
    }
    //parovania pre trciace hrany
    bool possible = true;
    for (int i = 0; i < 4; ++i) {
        int count = 0;
        cout << "zadajte hrany ktore patria do parovania " << i+1 <<
        " alebo -1 pre pokracovanie:" << endl;
        while (true) {
            cin >> x;
            if (x == -1){
                break;
            }else {
                cin >> y;
            }
            count++;
            G[x].second.insert(i);
            G[y].second.insert(i);
        }
        if (count % 2 != 0) {
            possible = false;
        }
    }
    if (!possible) {
        cout << "false" << endl;
        return 0;
    }
    for (int i = 1; i < 5; ++i) {
        if (G[i].second.empty() || G[i].second.size() > 2) {
            cout << "false" << endl;
            return 0;
        }
    }
    path.push_back(0);
    checkIfPossible(G[0].first[0]);
    if (result) {
        cout << "true" << endl;
    } else {
        cout << "false" << endl;
    }
    return 0;
}
