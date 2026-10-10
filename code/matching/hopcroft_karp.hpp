#include <bits/stdc++.h>
#include <cassert>
using namespace std;

struct HopcroftKarp {
    int nl, nr;
    vector<vector<int>> adj;
    vector<int> ml, mr, dist;
    int distNil;
    HopcroftKarp(int nl, int nr)
        : nl(nl), nr(nr), adj(nl), ml(nl, -1), mr(nr, -1), dist(nl) {}
    void addEdge(int u, int v) { adj[u].push_back(v); }
    bool bfs() {
        queue<int> q;
        fill(dist.begin(), dist.end(), -1);
        for (int u = 0; u < nl; ++u)
            if (ml[u] == -1) dist[u] = 0, q.push(u);
        distNil = INT_MAX;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dist[u] >= distNil) continue;
            for (int v : adj[u]) {
                int w = mr[v];
                if (w == -1) distNil = dist[u] + 1;
                else if (dist[w] == -1)
                    dist[w] = dist[u] + 1, q.push(w);
            }
        }
        return distNil != INT_MAX;
    }
    bool dfs(int u) {
        for (int v : adj[u]) {
            int w = mr[v];
            if ((w == -1 && dist[u] + 1 == distNil) ||
                (w != -1 && dist[w] == dist[u] + 1 && dfs(w))) {
                ml[u] = v; mr[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }
    int maxMatching() {
        int ans = 0;
        while (bfs())
            for (int u = 0; u < nl; ++u)
                if (ml[u] == -1 && dfs(u)) ++ans;
        return ans;
    }
    pair<vector<int>, vector<int>> minVertexCover() const {
        vector<char> vl(nl), vr(nr);
        queue<int> q;
        for (int u = 0; u < nl; ++u)
            if (ml[u] == -1) vl[u] = 1, q.push(u);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) if (ml[u] != v && !vr[v]) {
                vr[v] = 1;
                int w = mr[v];
                if (w != -1 && !vl[w]) vl[w] = 1, q.push(w);
            }
        }
        vector<int> left, right;
        for (int u = 0; u < nl; ++u) if (!vl[u]) left.push_back(u);
        for (int v = 0; v < nr; ++v) if (vr[v]) right.push_back(v);
        return {left, right};
    }
};
