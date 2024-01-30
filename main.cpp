#include "bits/stdc++.h"

using namespace std;

vector<pair<vector<int>, set<int>>> G;//kazdy vrchol obsahuje dvojicu vektor a set, vektor obsahuje vrcholy kde ide hrana z daneho vrcholu a set je mnozina parovani v ktorych sa nachadza dany vrchol
map<pair<int, int>, set<int>> edges;//key==dvojica vrcholov medzi ktorymi je dana hrana, value==mnozina parovani v ktorych je dana hrana
vector<int> visited;// navstivene vrcholy

//dorobit backtracking
bool checkIfPossible(int vertex){
    for (int i = 1; i < G.size(); ++i) {

    }
    return true;
}

int main() {
    int x, y, n;
    bool possible = true;
    //pocet vrcholov
    cin >> n;
    G.resize(n + 1);
    //vstupny graf
    for (int i = 0; i < n + (n / 2); ++i) {
        cin >> x >> y;
        G[x].first.push_back(y);
        G[y].first.push_back(x);
    }
    //parovania pre trciace hrany
    for (int i = 0; i < 4; ++i) {
        int count = 0;
        //i je hrana, davam pre trciace hrany v ktorych parovaniach sa nachadzaju
        while(cin >> x >> y){
            count++;
            edges[make_pair(x, y)].insert(i);
            G[x].second.insert(i);
            G[y].second.insert(i);
        }
        if (count%2 != 0){
            possible = false;
        }
    }
    if (!possible){
        cout << false << endl;
        return 0;
    }
    for (int i = 0; i < 4; ++i) {
        if (G[i].second.empty() || G[i].second.size() > 2){
            cout << false << endl;
            return 0;
        }

    }
    cout << checkIfPossible(0) << endl;
    return 0;
}
