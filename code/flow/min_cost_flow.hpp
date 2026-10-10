#include <bits/stdc++.h>
#include <cassert>
using namespace std;

struct MinCostFlow {
    struct Edge { int to, rev; long long cap; __int128 cost; };
    int n;
    vector<vector<Edge>> g;
    MinCostFlow(int n) : n(n), g(n) {}
    void addEdge(int u, int v, long long cap, long long cost) {
        assert(cap >= 0);
        int i = (int)g[u].size(), j = (int)g[v].size();
        g[u].push_back({v, j + (u == v), cap, cost});
        g[v].push_back({u, i, 0, -(__int128)cost});
    }
    // s != t, need >= 0. Tong chi phi phai vua __int128.
    // Bao loi neu mang du ban dau co chu trinh am (ke ca o thanh phan roi).
    pair<long long, __int128> run(int s, int t, long long need = LLONG_MAX) {
        assert(s != t && need >= 0);
        const __int128 INF = (__int128)1 << 120;
        // Sieu nguon ngam noi toi moi dinh bang canh chi phi 0.
        vector<__int128> pot(n, 0);
        for (int it = 0; it < n; ++it) {
            bool changed = false;
            for (int u = 0; u < n; ++u)
                for (auto ed : g[u]) if (ed.cap > 0 && pot[u] + ed.cost < pot[ed.to]) {
                    pot[ed.to] = pot[u] + ed.cost;
                    changed = true;
                }
            if (!changed) break;
            if (it == n - 1) throw invalid_argument("negative cost cycle");
        }
        long long flow = 0;
        __int128 cost = 0;
        while (flow < need) {
            vector<__int128> dist(n, INF);
            vector<int> pv(n, -1), pe(n, -1);
            priority_queue<pair<__int128,int>, vector<pair<__int128,int>>,
                           greater<pair<__int128,int>>> pq;
            dist[s] = 0; pq.push({0, s});
            while (!pq.empty()) {
                auto [d, u] = pq.top(); pq.pop();
                if (d != dist[u]) continue;
                for (int i = 0; i < (int)g[u].size(); ++i) {
                    auto& ed = g[u][i];
                    if (ed.cap == 0) continue;
                    __int128 nd = d + ed.cost + pot[u] - pot[ed.to];
                    if (nd < dist[ed.to]) {
                        dist[ed.to] = nd; pv[ed.to] = u; pe[ed.to] = i;
                        pq.push({nd, ed.to});
                    }
                }
            }
            if (dist[t] == INF) break;
            for (int v = 0; v < n; ++v)
                if (dist[v] < INF) pot[v] += dist[v];
            long long add = need - flow;
            for (int v = t; v != s; v = pv[v])
                add = min(add, g[pv[v]][pe[v]].cap);
            for (int v = t; v != s; v = pv[v]) {
                Edge& ed = g[pv[v]][pe[v]];
                ed.cap -= add;
                g[v][ed.rev].cap += add;
                cost += (__int128)add * ed.cost;
            }
            flow += add;
        }
        return {flow, cost};
    }
};
