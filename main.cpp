#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple> // 
using namespace std;

// ==============================
// 資料結構定義
// ==============================
struct Edge {
    int u, v, w;
};

// 並查集 Disjoint Set Union
class DSU {
private:
    vector<int> parent, rankv;

public:
    DSU(int n) {
        parent.resize(n + 1);
        rankv.resize(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }

    bool unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);

        if (rootA == rootB) return false;

        if (rankv[rootA] < rankv[rootB]) {
            parent[rootA] = rootB;
        } else if (rankv[rootA] > rankv[rootB]) {
            parent[rootB] = rootA;
        } else {
            parent[rootB] = rootA;
            rankv[rootA]++;
        }
        return true;
    }
};

// ==============================
// 工具函式
// ==============================
void printEdge(const Edge& e) {
    cout << e.u << " - " << e.v << " : " << e.w;
}

void printMST(const vector<Edge>& mst) {
    int total = 0;
    cout << "\nMST edges:\n";
    for (const auto& e : mst) {
        printEdge(e);
        cout << "\n";
        total += e.w;
    }
    cout << "Total weight = " << total << "\n";
}

// ==============================
// 1. Kruskal Algorithm
// ==============================
void kruskalMST(int n, vector<Edge> edges) {
    cout << "==============================\n";
    cout << "Kruskal Algorithm\n";
    cout << "==============================\n";

    vector<Edge> mst;

    // Step 1: 排序
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });

    // Step 2: DSU
    DSU dsu(n);

    cout << "Selection steps:\n";

    // Step 3: 選邊
    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mst.push_back(e);
            printEdge(e);
            cout << "\n";

            if (mst.size() == n - 1) break;
        }
    }

    printMST(mst);
    cout << "\n";
}

// ==============================
// 2. Prim Algorithm
// ==============================
void primMST(int n, const vector<vector<pair<int, int>>>& adj, int start = 1) {
    cout << "==============================\n";
    cout << "Prim Algorithm\n";
    cout << "==============================\n";

    vector<bool> inMST(n + 1, false);
    vector<Edge> mst;

    priority_queue<
        tuple<int, int, int>,
        vector<tuple<int, int, int>>,
        greater<tuple<int, int, int>>
    > pq;

    // Step 1
    inMST[start] = true;

    // Step 2
    for (const auto& [v, w] : adj[start]) {
        pq.push({w, start, v});
    }

    cout << "Selection steps:\n";

    // Step 3
    while (!pq.empty() && mst.size() < n - 1) {
        auto [w, u, v] = pq.top();
        pq.pop();

        if (inMST[v]) continue;

        inMST[v] = true;
        mst.push_back({u, v, w});

        printEdge({u, v, w});
        cout << "\n";

        for (const auto& [nv, nw] : adj[v]) {
            if (!inMST[nv]) {
                pq.push({nw, v, nv});
            }
        }
    }

    printMST(mst);
    cout << "\n";
}

// ==============================
// 3. Boruvka Algorithm
// ==============================
void boruvkaMST(int n, const vector<Edge>& edges) {
    cout << "==============================\n";
    cout << "Sollin / Boruvka Algorithm\n";
    cout << "==============================\n";

    vector<Edge> mst;

    DSU dsu(n);
    int numComponents = n;
    int round = 1;

    while (numComponents > 1) {
        cout << "Round " << round << ":\n";

        vector<int> cheapest(n + 1, -1);

        // Step 3
        for (int i = 0; i < edges.size(); i++) {
            int set1 = dsu.find(edges[i].u);
            int set2 = dsu.find(edges[i].v);

            if (set1 == set2) continue;

            if (cheapest[set1] == -1 ||
                edges[i].w < edges[cheapest[set1]].w)
                cheapest[set1] = i;

            if (cheapest[set2] == -1 ||
                edges[i].w < edges[cheapest[set2]].w)
                cheapest[set2] = i;
        }

        bool merged = false;

        // Step 4
        for (int i = 1; i <= n; i++) {
            if (cheapest[i] != -1) {
                int idx = cheapest[i];
                int u = edges[idx].u;
                int v = edges[idx].v;

                if (dsu.unite(u, v)) {
                    mst.push_back(edges[idx]);
                    printEdge(edges[idx]);
                    cout << "\n";

                    numComponents--;
                    merged = true;
                }
            }
        }

        if (!merged) break;

        cout << "\n";
        round++;
    }

    printMST(mst);
    cout << "\n";
}

// ==============================
// 主程式
// ==============================
int main() {
    int n = 6;

    vector<Edge> edges = {
        {1, 2, 16},
        {1, 5, 19},
        {5, 4, 18},
        {2, 4, 6},
        {2, 3, 5},
        {4, 3, 10},
        {1, 6, 22},
        {5, 6, 33},
        {6, 2, 12},
        {6, 4, 14}
    };

    vector<vector<pair<int, int>>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.u].push_back({e.v, e.w});
        adj[e.v].push_back({e.u, e.w});
    }

    kruskalMST(n, edges);
    primMST(n, adj, 1);
    boruvkaMST(n, edges);

    return 0;
}
