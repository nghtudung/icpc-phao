#include <bits/stdc++.h>
#include <cassert>
using namespace std;

using ld = long double;

// p[i]: xac suat thanh cong o phep thu i, cac phep thu doc lap.
vector<ld> bernoulliCount(const vector<ld>& p) {
    int n = (int)p.size();
    vector<ld> dp(n + 1);
    dp[0] = 1;
    for (int i = 0; i < n; ++i) {
        for (int k = i + 1; k >= 1; --k)
            dp[k] = dp[k] * (1 - p[i]) + dp[k - 1] * p[i];
        dp[0] *= 1 - p[i];
    }
    return dp;
}

// order: thu tu topo; moi canh u -> v di ve phia sau trong order.
// terminal: cost = 0, khong co canh di ra.
vector<ld> expectedDAG(
    const vector<vector<pair<int, ld>>>& edges,
    const vector<ld>& cost, const vector<int>& order
) {
    int n = (int)edges.size();
    vector<ld> e(n);
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        e[u] = cost[u];
        for (auto [v, prob] : edges[u]) e[u] += prob * e[v];
    }
    return e;
}
