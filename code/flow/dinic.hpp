#include <bits/stdc++.h>
#include <cassert>
using namespace std;

struct Dinic {
    struct Edge { int to; long long cap; };
    int n;
    vector<Edge> e;
    vector<vector<int>> adj;
    vector<int> level, ptr;
    Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}
    void addEdge(int u, int v, long long cap) {
        assert(cap >= 0);
        adj[u].push_back((int)e.size());
        e.push_back({v, cap});
        adj[v].push_back((int)e.size());
        e.push_back({u, 0});
    }
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(s); level[s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : adj[u]) if (e[id].cap > 0 && level[e[id].to] == -1) {
                level[e[id].to] = level[u] + 1;
                q.push(e[id].to);
            }
        }
        return level[t] != -1;
    }
    long long dfs(int u, int t, long long pushed) {
        if (u == t || pushed == 0) return pushed;
        for (int& i = ptr[u]; i < (int)adj[u].size(); ++i) {
            int id = adj[u][i], v = e[id].to;
            if (level[v] != level[u] + 1 || e[id].cap == 0) continue;
            long long got = dfs(v, t, min(pushed, e[id].cap));
            if (got) {
                e[id].cap -= got;
                e[id ^ 1].cap += got;
                return got;
            }
        }
        return 0;
    }
    // Tra luong day THEM; dap an phai vua long long.
    long long maxflow(int s, int t) {
        assert(s != t);
        long long flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long got = dfs(s, t, LLONG_MAX)) {
                if (flow > LLONG_MAX - got) throw overflow_error("flow overflow");
                flow += got;
            }
        }
        return flow;
    }
    vector<char> minCutSide(int s) const {
        vector<char> vis(n);
        queue<int> q;
        q.push(s); vis[s] = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : adj[u])
                if (e[id].cap > 0 && !vis[e[id].to])
                    vis[e[id].to] = 1, q.push(e[id].to);
        }
        return vis;
    }
};
