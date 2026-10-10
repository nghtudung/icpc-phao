#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Ma tran chi phi n x m, n <= m, tat ca cac o huu han.
pair<__int128, vector<int>> hungarian(const vector<vector<long long>>& a) {
    int n = (int)a.size();
    if (n == 0) return {0, {}};
    int m = (int)a[0].size();
    assert(n <= m);
    for (const auto& row : a) assert((int)row.size() == m);
    const __int128 INF = (__int128)1 << 120;
    vector<__int128> u(n + 1), v(m + 1);
    vector<int> p(m + 1), way(m + 1);
    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<__int128> minv(m + 1, INF);
        vector<char> used(m + 1);
        do {
            used[j0] = 1;
            int i0 = p[j0], j1 = 0;
            __int128 delta = INF;
            for (int j = 1; j <= m; ++j) if (!used[j]) {
                __int128 cur = (__int128)a[i0 - 1][j - 1] - u[i0] - v[j];
                if (cur < minv[j]) minv[j] = cur, way[j] = j0;
                if (minv[j] < delta) delta = minv[j], j1 = j;
            }
            for (int j = 0; j <= m; ++j) {
                if (used[j]) u[p[j]] += delta, v[j] -= delta;
                else minv[j] -= delta;
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    vector<int> assignment(n);
    for (int j = 1; j <= m; ++j)
        if (p[j]) assignment[p[j] - 1] = j - 1;
    __int128 cost = 0;
    for (int i = 0; i < n; ++i) cost += a[i][assignment[i]];
    return {cost, assignment};
}
