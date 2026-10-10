#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// a,b >= 0; khong dong thoi bang 0.
long long extendedGcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long g = extendedGcd(b, a % b, x1, y1);
    x = y1;
    y = (long long)((__int128)x1 - (__int128)(a / b) * y1);
    return g;
}
long long modNorm(long long a, long long m) {
    a %= m;
    if (a < 0) a += m;
    return a;
}
optional<long long> inverseMod(long long a, long long m) {
    if (m <= 1) return nullopt;
    long long x, y;
    if (extendedGcd(modNorm(a, m), m, x, y) != 1) return nullopt;
    return modNorm(x, m);
}
// r1 (mod m1), r2 (mod m2); m1,m2 > 0.
optional<pair<long long, long long>> mergeCRT(
    long long r1, long long m1, long long r2, long long m2
) {
    r1 = modNorm(r1, m1);
    r2 = modNorm(r2, m2);
    long long x, y, g = extendedGcd(m1, m2, x, y);
    long long diff = r2 - r1;
    if (diff % g) return nullopt;
    long long mod = m2 / g;
    __int128 t = (__int128)(diff / g) * x % mod;
    if (t < 0) t += mod;
    __int128 lcm = (__int128)(m1 / g) * m2;
    if (lcm > LLONG_MAX) throw overflow_error("CRT modulus overflow");
    __int128 r = (r1 + (__int128)m1 * t) % lcm;
    return pair<long long, long long>{(long long)r, (long long)lcm};
}
