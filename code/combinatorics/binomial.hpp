#include <bits/stdc++.h>
#include <cassert>
using namespace std;

struct Binomial {
    long long p;
    vector<long long> fact, invfact;
    long long mul(long long a, long long b) const {
        return (long long)((__int128)a * b % p);
    }
    long long power(long long a, long long e) const {
        long long r = 1;
        for (; e; e >>= 1, a = mul(a, a))
            if (e & 1) r = mul(r, a);
        return r;
    }
    // p nguyen to, 0 <= N < p.
    Binomial(int N, long long prime)
        : p(prime), fact(N + 1), invfact(N + 1) {
        fact[0] = 1;
        for (int i = 1; i <= N; ++i) fact[i] = mul(fact[i - 1], i);
        invfact[N] = power(fact[N], p - 2);
        for (int i = N; i >= 1; --i)
            invfact[i - 1] = mul(invfact[i], i);
    }
    long long choose(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return mul(fact[n], mul(invfact[k], invfact[n - k]));
    }
    long long catalan(int n) const {
        long long r = choose(2 * n, n) - choose(2 * n, n + 1);
        return r < 0 ? r + p : r;
    }
};
