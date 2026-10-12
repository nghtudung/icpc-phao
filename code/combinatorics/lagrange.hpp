#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// p nguyen to; y[i] = P(i), 0 <= i <= k < p.
long long lagrange(const vector<long long>& y, long long t, long long p) {
    assert(p >= 2 && !y.empty() && (long long)y.size() <= p);
    auto norm = [&](long long a) { a %= p; return a < 0 ? a + p : a; };
    auto mul = [&](long long a, long long b) {
        return (long long)((__int128)a * b % p);
    };
    auto power = [&](long long a, long long e) {
        long long r = 1;
        for (; e; e >>= 1, a = mul(a, a))
            if (e & 1) r = mul(r, a);
        return r;
    };
    int k = (int)y.size() - 1;
    t = norm(t);
    if (t <= k) return norm(y[(int)t]);

    vector<long long> pref(k + 2, 1), suf(k + 2, 1), invfact(k + 1);
    long long fact = 1;
    for (int i = 1; i <= k; ++i) fact = mul(fact, i);
    invfact[k] = power(fact, p - 2);
    for (int i = k; i >= 1; --i) invfact[i - 1] = mul(invfact[i], i);
    for (int i = 0; i <= k; ++i) pref[i + 1] = mul(pref[i], norm(t - i));
    for (int i = k; i >= 0; --i) suf[i] = mul(suf[i + 1], norm(t - i));

    long long ans = 0;
    for (int i = 0; i <= k; ++i) {
        long long term = mul(norm(y[i]), mul(pref[i], suf[i + 1]));
        term = mul(term, mul(invfact[i], invfact[k - i]));
        if ((k - i) & 1) term = term ? p - term : 0;
        ans = (long long)(((__int128)ans + term) % p);
    }
    return ans;
}
