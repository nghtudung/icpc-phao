#include <bits/stdc++.h>
#include <cassert>
using namespace std;

struct TwoSAT {
    int n;
    vector<vector<int>> g, rg;
    TwoSAT(int n) : n(n), g(2*n), rg(2*n) {}
    int lit(int var, bool value) const { return 2*var + (int)value; }
    void addEdge(int a, int b) {
        g[a].push_back(b);
        rg[b].push_back(a);
    }
    // Menh de a => b can ca canh phan dao !b => !a.
    void implication(int a, int b) {
        addEdge(a, b);
        addEdge(b ^ 1, a ^ 1);
    }
    void either(int a, int b) {
        implication(a ^ 1, b);
    }
    optional<vector<int>> solve() const {
        int N = 2*n;
        vector<char> seen(N);
        vector<int> order;
        for (int root = 0; root < N; ++root) if (!seen[root]) {
            vector<pair<int,int>> st{{root, 0}};
            seen[root] = 1;
            while (!st.empty()) {
                auto& [u, idx] = st.back();
                if (idx == (int)g[u].size()) {
                    order.push_back(u); st.pop_back();
                } else {
                    int v = g[u][idx++];
                    if (!seen[v]) seen[v] = 1, st.push_back({v, 0});
                }
            }
        }
        vector<int> comp(N, -1);
        int cc = 0;
        for (int z = N - 1; z >= 0; --z) {
            int root = order[z];
            if (comp[root] != -1) continue;
            vector<int> st{root};
            comp[root] = cc;
            while (!st.empty()) {
                int u = st.back(); st.pop_back();
                for (int v : rg[u]) if (comp[v] == -1)
                    comp[v] = cc, st.push_back(v);
            }
            ++cc;
        }
        vector<int> ans(n);
        for (int i = 0; i < n; ++i) {
            if (comp[2*i] == comp[2*i+1]) return nullopt;
            ans[i] = comp[2*i+1] > comp[2*i];
        }
        return ans;
    }
};
