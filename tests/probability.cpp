#include <bits/stdc++.h>
#include <cassert>
using namespace std;
#include "../code/probability/dp.hpp"

bool close(ld a, ld b) { return abs(a - b) < 1e-12L; }

int main() {
    auto d = bernoulliCount({0.5L, 0.25L, 1.0L});
    assert(d.size() == 4);
    assert(close(d[0], 0));
    assert(close(d[1], 0.375L));
    assert(close(d[2], 0.5L));
    assert(close(d[3], 0.125L));
    assert(close(accumulate(d.begin(), d.end(), 0.0L), 1));

    vector<vector<pair<int, ld>>> edges(4);
    edges[0] = {{1, 0.25L}, {2, 0.75L}};
    edges[1] = {{3, 1.0L}};
    edges[2] = {{3, 1.0L}};
    vector<ld> cost{1, 2, 4, 0};
    auto e = expectedDAG(edges, cost, {0, 1, 2, 3});
    assert(close(e[3], 0));
    assert(close(e[1], 2));
    assert(close(e[2], 4));
    assert(close(e[0], 4.5L));

    mt19937 rng(42);
    uniform_int_distribution<int> pct(0, 100);
    for (int tc = 0; tc < 1000; ++tc) {
        vector<ld> p(1 + rng() % 12);
        for (ld& x : p) x = pct(rng) / 100.0L;
        auto a = bernoulliCount(p);
        vector<ld> b(p.size() + 1);
        for (int mask = 0; mask < (1 << p.size()); ++mask) {
            ld prob = 1;
            for (int i = 0; i < (int)p.size(); ++i)
                prob *= (mask >> i & 1) ? p[i] : 1 - p[i];
            b[__builtin_popcount((unsigned)mask)] += prob;
        }
        for (int k = 0; k <= (int)p.size(); ++k) assert(close(a[k], b[k]));
    }
    cout << "probability OK\n";
}
