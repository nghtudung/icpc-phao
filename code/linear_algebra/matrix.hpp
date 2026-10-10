#include <bits/stdc++.h>
#include <cassert>
using namespace std;

constexpr int MATRIX_SIZE = 64;
constexpr long long MOD = 1'000'000'007LL;
static_assert(MOD > 0);

// 0 <= a,b < MOD; tranh tran ngay ca khi MOD gan LLONG_MAX.
void add(long long &a, long long b) {
    if (a >= MOD - b) a -= MOD - b;
    else a += b;
}

struct Matrix {
    int m, n;
    long long d[MATRIX_SIZE][MATRIX_SIZE];

    Matrix(int _m = 0, int _n = 0) : m(_m), n(_n) {
        assert(0 <= m && m <= MATRIX_SIZE);
        assert(0 <= n && n <= MATRIX_SIZE);
        memset(d, 0, sizeof d);
    }

    Matrix operator * (const Matrix &other) const {
        assert(n == other.m);
        int x = m, y = n, z = other.n;
        Matrix ans(x, z);
        for (int i = 0; i < x; ++i)
            for (int j = 0; j < y; ++j)
                for (int k = 0; k < z; ++k)
                    add(ans.d[i][k], (__int128)d[i][j] * other.d[j][k] % MOD);
        return ans;
    }

    Matrix operator ^ (long long k) const {
        assert(m == n && k >= 0);
        Matrix ans(n, n);
        for (int i = 0; i < n; ++i) ans.d[i][i] = 1 % MOD;
        Matrix mul = *this;
        for (; k; k >>= 1, mul = mul * mul)
            if (k & 1) ans = ans * mul;
        return ans;
    }
};
