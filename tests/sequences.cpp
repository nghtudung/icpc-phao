#include "../code/quick_reference/sequences.hpp"

int main() {
    assert(golomb(0) == vector<int>({0}));
    assert(golomb(15) == vector<int>({0, 1, 2, 2, 3, 3, 4, 4, 4,
                                     5, 5, 5, 6, 6, 6, 6}));
    assert(kolakoski(0).empty());
    assert(kolakoski(15) == vector<int>({1, 2, 2, 1, 1, 2, 1, 2,
                                         2, 1, 2, 2, 1, 1, 2}));
    vector<int> g = golomb(10000);
    vector<int> count(g.back() + 1);
    for (int i = 1; i < (int)g.size(); ++i) {
        assert(g[i] >= g[i - 1]);
        ++count[g[i]];
    }
    for (int v = 1; v < g.back(); ++v) assert(count[v] == g[v]);
    vector<int> k = kolakoski(10000);
    int pos = 0, run = 0;
    while (pos < (int)k.size()) {
        int end = pos;
        while (end < (int)k.size() && k[end] == k[pos]) ++end;
        if (end < (int)k.size()) assert(end - pos == k[run]);
        pos = end;
        ++run;
    }
    for (unsigned long long n = 0; n < 10000; ++n) {
        assert(thueMorse(n) == (__builtin_popcountll(n) & 1));
        assert(thueMorse(2 * n) == thueMorse(n));
        assert(thueMorse(2 * n + 1) == 1 - thueMorse(n));
    }
}
