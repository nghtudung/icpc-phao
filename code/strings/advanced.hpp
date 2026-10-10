#include <bits/stdc++.h>
#include <cassert>
using namespace std;

struct AhoCorasick {
    struct Node {
        array<int, 26> next;
        int link = 0, out = 0;
        Node() { next.fill(-1); }
    };
    vector<Node> t{Node()};
    bool built = false;
    void addPattern(const string& s) {
        assert(!built);
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (t[u].next[c] == -1) {
                t[u].next[c] = (int)t.size();
                t.emplace_back();
            }
            u = t[u].next[c];
        }
        ++t[u].out;
    }
    void build() {
        assert(!built);
        built = true;
        queue<int> q;
        for (int c = 0; c < 26; ++c) {
            int v = t[0].next[c];
            if (v == -1) t[0].next[c] = 0;
            else q.push(v);
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            t[u].out += t[t[u].link].out;
            for (int c = 0; c < 26; ++c) {
                int v = t[u].next[c];
                if (v == -1) t[u].next[c] = t[t[u].link].next[c];
                else {
                    t[v].link = t[t[u].link].next[c];
                    q.push(v);
                }
            }
        }
    }
    long long countMatches(const string& s) const {
        assert(built);
        int u = 0;
        long long ans = t[0].out; // Mau rong khop ca truoc ky tu dau.
        for (char ch : s) {
            u = t[u].next[ch - 'a'];
            ans += t[u].out;
        }
        return ans;
    }
};

// s khong chua byte 0; tra ve vi tri cac hau to theo thu tu tu dien.
vector<int> suffixArray(const string& s) {
    string a = s + char(0);
    int n = (int)a.size();
    vector<int> p(n), c(n), cnt(max(256, n));
    for (unsigned char ch : a) ++cnt[ch];
    for (int i = 1; i < 256; ++i) cnt[i] += cnt[i-1];
    for (int i = 0; i < n; ++i)
        p[--cnt[(unsigned char)a[i]]] = i;
    int classes = 1;
    for (int i = 1; i < n; ++i) {
        if (a[p[i]] != a[p[i-1]]) ++classes;
        c[p[i]] = classes - 1;
    }
    vector<int> pn(n), cn(n);
    for (int len = 1; len < n && classes < n; len <<= 1) {
        for (int i = 0; i < n; ++i)
            pn[i] = (p[i] - len + n) % n;
        fill(cnt.begin(), cnt.begin() + classes, 0);
        for (int x : pn) ++cnt[c[x]];
        for (int i = 1; i < classes; ++i) cnt[i] += cnt[i-1];
        for (int i = n - 1; i >= 0; --i)
            p[--cnt[c[pn[i]]]] = pn[i];
        cn[p[0]] = 0;
        int newClasses = 1;
        for (int i = 1; i < n; ++i) {
            pair<int,int> x{c[p[i]], c[(p[i] + len) % n]};
            pair<int,int> y{c[p[i-1]], c[(p[i-1] + len) % n]};
            if (x != y) ++newClasses;
            cn[p[i]] = newClasses - 1;
        }
        c.swap(cn);
        classes = newClasses;
    }
    return vector<int>(p.begin() + 1, p.end());
}
