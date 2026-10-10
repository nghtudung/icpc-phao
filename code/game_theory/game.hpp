#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// order la thu tu topo cua do thi trang thai khong chu trinh.
vector<int> grundyDAG(const vector<vector<int>>& g, const vector<int>& order) {
    int n = (int)g.size();
    vector<int> ans(n);
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        vector<char> seen(g[u].size() + 1);
        for (int v : g[u]) if (ans[v] < (int)seen.size()) seen[ans[v]] = 1;
        while (seen[ans[u]]) ++ans[u];
    }
    return ans;
}

// 0: thua, 1: thang, 2: hoa neu cho phep choi vo han.
vector<int> gameOutcome(const vector<vector<int>>& g) {
    int n = (int)g.size();
    vector<vector<int>> rev(n);
    vector<int> remain(n), ans(n, 2);
    queue<int> q;
    for (int u = 0; u < n; ++u) {
        remain[u] = (int)g[u].size();
        for (int v : g[u]) rev[v].push_back(u);
        if (g[u].empty()) ans[u] = 0, q.push(u);
    }
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int u : rev[v]) {
            if (ans[u] != 2) continue;
            if (ans[v] == 0) ans[u] = 1, q.push(u);
            else if (--remain[u] == 0) ans[u] = 0, q.push(u);
        }
    }
    return ans;
}
