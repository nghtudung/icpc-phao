#include "../code/combinatorics/lagrange.hpp"

long long evaluate(const vector<long long>& c, long long x, long long p) {
    auto norm = [&](long long a) { a %= p; return a < 0 ? a + p : a; };
    x = norm(x);
    long long ans = 0;
    for (int i = (int)c.size() - 1; i >= 0; --i)
        ans = (long long)((((__int128)ans * x) + norm(c[i])) % p);
    return ans;
}

int main() {
    mt19937_64 rng(20261012);
    for (long long p : {2LL, 3LL, 5LL, 101LL, 1000000007LL,
                        9223372036854775783LL}) {
        int maxk = (int)min(12LL, p - 1);
        for (int trial = 0; trial < 150; ++trial) {
            int k = (int)(rng() % (maxk + 1));
            vector<long long> c(k + 1), y(k + 1);
            for (auto& a : c) a = (long long)(rng() % (unsigned long long)p);
            for (int i = 0; i <= k; ++i) y[i] = evaluate(c, i, p);
            for (long long t : {-1000000000000LL, -1LL, 0LL, 1LL,
                                (long long)k, (long long)k + 1,
                                1000000000000LL, LLONG_MIN, LLONG_MAX})
                assert(lagrange(y, t, p) == evaluate(c, t, p));
            long long t = (long long)rng();
            assert(lagrange(y, t, p) == evaluate(c, t, p));
        }
    }
}
