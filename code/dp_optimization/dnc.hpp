#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// prev[0..n], cur[j] = min_{0<=i<j}(prev[i]+cost(i,j)).
// opt[j] la chi so nho nhat dat min, khong giam theo j.
// INF danh dau vo nghiem; moi gia tri huu han phai nam trong (-INF,INF).
vector<long long> divideConquerLayer(
    const vector<long long>& prev,
    const function<long long(int,int)>& cost
) {
    int n = (int)prev.size() - 1;
    const long long INF = LLONG_MAX / 4;
    vector<long long> cur(n + 1, INF);
    function<void(int,int,int,int)> solve = [&](int l, int r, int ql, int qr) {
        if (l > r) return;
        int mid = l + (r - l) / 2, bestI = ql;
        for (int i = ql; i <= min(qr, mid - 1); ++i) {
            if (prev[i] == INF) continue;
            long long val = prev[i] + cost(i, mid);
            if (val < cur[mid]) cur[mid] = val, bestI = i;
        }
        // Neu tat ca deu INF, ql van la vi tri min trai nhat.
        solve(l, mid - 1, ql, bestI);
        solve(mid + 1, r, bestI, qr);
    };
    solve(1, n, 0, n - 1);
    return cur;
}
