#include <bits/stdc++.h>
#include <cassert>
using namespace std;

struct LiChao {
    struct Line { long long m, b; };
    struct Node { Line line{0, 0}; int left = -1, right = -1; bool has = false; };
    long long lo, hi;
    vector<Node> t{Node()};
    static constexpr __int128 INF = numeric_limits<__int128>::max();
    LiChao(long long lo, long long hi) : lo(lo), hi(hi) { assert(lo <= hi); }
    static __int128 value(Line a, long long x) {
        return (__int128)a.m * x + a.b;
    }
    void add(Line line) { addRec(0, lo, hi, line); }
    void addRec(int u, long long l, long long r, Line line) {
        if (!t[u].has) {
            t[u].line = line; t[u].has = true; return;
        }
        long long mid = l + (long long)(((__int128)r - l) / 2);
        bool left = value(line, l) < value(t[u].line, l);
        bool middle = value(line, mid) < value(t[u].line, mid);
        if (middle) swap(line, t[u].line);
        if (l == r) return;
        if (left != middle) {
            if (t[u].left == -1) {
                t[u].left = (int)t.size(); t.emplace_back();
            }
            addRec(t[u].left, l, mid, line);
        } else {
            if (t[u].right == -1) {
                t[u].right = (int)t.size(); t.emplace_back();
            }
            addRec(t[u].right, mid + 1, r, line);
        }
    }
    __int128 query(long long x) const {
        assert(lo <= x && x <= hi);
        return queryRec(0, lo, hi, x);
    }
    __int128 queryRec(int u, long long l, long long r, long long x) const {
        if (u == -1) return INF;
        __int128 ans = t[u].has ? value(t[u].line, x) : INF;
        if (l == r) return ans;
        long long mid = l + (long long)(((__int128)r - l) / 2);
        int v = x <= mid ? t[u].left : t[u].right;
        return min(ans, x <= mid ? queryRec(v, l, mid, x)
                                 : queryRec(v, mid + 1, r, x));
    }
};
