#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Tra ve a[1..n], a[0] khong su dung.
vector<int> golomb(int n) {
    assert(n >= 0);
    vector<int> a(n + 1);
    if (n >= 1) a[1] = 1;
    if (n >= 2) a[2] = 2;
    for (int i = 3; i <= n; ++i)
        a[i] = 1 + a[i - a[a[i - 1]]];
    return a;
}

// Tra ve n so hang dau; chi so vector bat dau tu 0.
vector<int> kolakoski(int n) {
    assert(n >= 0);
    vector<int> a = {1, 2, 2};
    for (int read = 2; (int)a.size() < n; ++read) {
        int digit = (read % 2 == 0 ? 1 : 2);
        for (int j = 0; j < a[read] && (int)a.size() < n; ++j)
            a.push_back(digit);
    }
    a.resize(n);
    return a;
}

// Thue-Morse: parity cua so bit 1 trong n (n bat dau tu 0).
int thueMorse(unsigned long long n) {
    return __builtin_parityll(n);
}
