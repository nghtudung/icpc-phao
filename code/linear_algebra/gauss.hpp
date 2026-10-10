#include <bits/stdc++.h>
#include <cassert>
using namespace std;

using ld = long double;
const ld GAUSS_EPS = 1e-12L;
struct GaussResult {
    int status; // 0: vo nghiem, 1: duy nhat, 2: vo so
    vector<ld> x;
};
// a la ma tran n x (m+1), cot cuoi la ve phai.
GaussResult gaussReal(vector<vector<ld>> a, int m) {
    int n = (int)a.size(), row = 0;
    vector<int> where(m, -1);
    for (int col = 0; col < m && row < n; ++col) {
        int sel = row;
        for (int i = row + 1; i < n; ++i)
            if (abs(a[i][col]) > abs(a[sel][col])) sel = i;
        if (abs(a[sel][col]) <= GAUSS_EPS) continue;
        swap(a[sel], a[row]);
        where[col] = row;
        ld div = a[row][col];
        for (int j = col; j <= m; ++j) a[row][j] /= div;
        for (int i = 0; i < n; ++i) if (i != row) {
            ld factor = a[i][col];
            for (int j = col; j <= m; ++j)
                a[i][j] -= factor * a[row][j];
        }
        ++row;
    }
    for (int i = 0; i < n; ++i) {
        bool zero = true;
        for (int j = 0; j < m; ++j)
            if (abs(a[i][j]) > GAUSS_EPS) zero = false;
        if (zero && abs(a[i][m]) > GAUSS_EPS) return {0, {}};
    }
    vector<ld> x(m);
    for (int j = 0; j < m; ++j)
        if (where[j] != -1) x[j] = a[where[j]][m];
    return {row == m ? 1 : 2, x};
}

struct Gauss2Result {
    int status, rank; // status: 0 vo nghiem, 1 duy nhat, 2 nhieu nghiem
    uint64_t x;       // nghiem voi cac bien tu do bang 0
};
// a[i] chua he so cua toi da 64 an; b[i] la ve phai 0/1.
Gauss2Result gaussGF2(vector<uint64_t> a, vector<int> b, int vars) {
    int n = (int)a.size(), row = 0;
    vector<int> where(vars, -1);
    for (int col = 0; col < vars && row < n; ++col) {
        int sel = row;
        while (sel < n && !(a[sel] >> col & 1ULL)) ++sel;
        if (sel == n) continue;
        swap(a[sel], a[row]); swap(b[sel], b[row]);
        where[col] = row;
        for (int i = 0; i < n; ++i) if (i != row && (a[i] >> col & 1ULL)) {
            a[i] ^= a[row];
            b[i] ^= b[row];
        }
        ++row;
    }
    for (int i = 0; i < n; ++i)
        if (a[i] == 0 && b[i]) return {0, row, 0};
    uint64_t x = 0;
    for (int col = 0; col < vars; ++col)
        if (where[col] != -1 && b[where[col]]) x |= 1ULL << col;
    return {row == vars ? 1 : 2, row, x};
}
