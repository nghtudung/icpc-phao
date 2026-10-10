#include <bits/stdc++.h>
#include <cassert>
using namespace std;

struct MonotoneCHT {
    struct Line { long long m, b; };
    vector<Line> h;
    int ptr = 0;
    static __int128 value(Line a, long long x) {
        return (__int128)a.m * x + a.b;
    }
    static bool bad(Line a, Line b, Line c) {
        return ((__int128)b.b - a.b) * ((__int128)b.m - c.m) >=
               ((__int128)c.b - b.b) * ((__int128)a.m - b.m);
    }
    // Do doc m khong tang; truy van x khong giam.
    // Cac tich trong bad() phai vua __int128.
    void add(long long m, long long b) {
        assert(h.empty() || m <= h.back().m);
        Line line{m, b};
        if (!h.empty() && h.back().m == m) {
            if (h.back().b <= b) return;
            h.pop_back();
        }
        while (h.size() >= 2 && bad(h[h.size()-2], h.back(), line))
            h.pop_back();
        h.push_back(line);
        ptr = min(ptr, (int)h.size() - 1);
    }
    __int128 query(long long x) {
        assert(!h.empty());
        while (ptr + 1 < (int)h.size() &&
               value(h[ptr+1], x) <= value(h[ptr], x)) ++ptr;
        return value(h[ptr], x);
    }
};
