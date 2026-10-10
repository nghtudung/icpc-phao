#include <bits/stdc++.h>
#include <cassert>
using namespace std;

vector<int> zFunction(const string& s) {
    int n = (int)s.size();
    vector<int> z(n);
    if (n) z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}

// d1[i]: so ban kinh le; d2[i]: so ban kinh chan.
pair<vector<int>, vector<int>> manacher(const string& s) {
    int n = (int)s.size();
    vector<int> d1(n), d2(n);
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) ++k;
        d1[i] = k--;
        if (i + k > r) l = i - k, r = i + k;
    }
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n &&
               s[i - k - 1] == s[i + k]) ++k;
        d2[i] = k--;
        if (i + k > r) l = i - k - 1, r = i + k;
    }
    return {d1, d2};
}

struct Trie {
    struct Node {
        array<int, 26> next;
        int pass = 0, terminal = 0;
        Node() { next.fill(-1); }
    };
    vector<Node> t{Node()};
    void insert(const string& s) {
        int u = 0;
        ++t[u].pass;
        for (char ch : s) {
            int c = ch - 'a';
            if (t[u].next[c] == -1) {
                t[u].next[c] = (int)t.size();
                t.emplace_back();
            }
            u = t[u].next[c];
            ++t[u].pass;
        }
        ++t[u].terminal;
    }
    int countPrefix(const string& s) const {
        int u = 0;
        for (char ch : s) {
            u = t[u].next[ch - 'a'];
            if (u == -1) return 0;
        }
        return t[u].pass;
    }
    int countWord(const string& s) const {
        int u = 0;
        for (char ch : s) {
            u = t[u].next[ch - 'a'];
            if (u == -1) return 0;
        }
        return t[u].terminal;
    }
};
