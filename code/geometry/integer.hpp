#include <bits/stdc++.h>
#include <cassert>
using namespace std;

using i128 = __int128_t;

struct IP {
    long long x, y;
    bool operator==(const IP& b) const { return x == b.x && y == b.y; }
    bool operator<(const IP& b) const {
        return x != b.x ? x < b.x : y < b.y;
    }
};

i128 cross(IP a, IP b, IP c) {
    i128 ux = (i128)b.x - a.x, uy = (i128)b.y - a.y;
    i128 vx = (i128)c.x - a.x, vy = (i128)c.y - a.y;
    return ux * vy - uy * vx;
}
i128 dot(IP a, IP b, IP c) {
    i128 ux = (i128)b.x - a.x, uy = (i128)b.y - a.y;
    i128 vx = (i128)c.x - a.x, vy = (i128)c.y - a.y;
    return ux * vx + uy * vy;
}
i128 dist2(IP a, IP b) {
    i128 dx = (i128)a.x - b.x, dy = (i128)a.y - b.y;
    return dx * dx + dy * dy;
}
int sign(i128 x) { return (x > 0) - (x < 0); }
bool onSegment(IP a, IP b, IP p) {
    return cross(a, b, p) == 0 &&
        min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
        min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
bool segmentsIntersect(IP a, IP b, IP c, IP d) {
    int x = sign(cross(a, b, c)), y = sign(cross(a, b, d));
    int z = sign(cross(c, d, a)), w = sign(cross(c, d, b));
    if (!x && onSegment(a, b, c)) return true;
    if (!y && onSegment(a, b, d)) return true;
    if (!z && onSegment(c, d, a)) return true;
    if (!w && onSegment(c, d, b)) return true;
    return x * y < 0 && z * w < 0;
}
// Duong thang AB va doan CD: 0 khong giao, 1 diem, 2 ca doan CD.
int lineSegmentRelation(IP a, IP b, IP c, IP d) {
    int x = sign(cross(a, b, c)), y = sign(cross(a, b, d));
    if (!x && !y) return c == d ? 1 : 2;
    return (!x || !y || x != y) ? 1 : 0;
}

// Hai lan dien tich co dau; duong neu thu tu dinh nguoc kim dong ho.
i128 polygonArea2(const vector<IP>& p) {
    i128 s = 0;
    for (int i = 0, n = (int)p.size(); i < n; ++i) {
        int j = (i + 1) % n;
        s += ((i128)p[i].x - p[j].x) * ((i128)p[i].y + p[j].y);
    }
    return s;
}
// Graham scan: bao loi nguoc kim dong ho, khong lap dinh dau.
vector<IP> grahamHull(vector<IP> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    if (p.size() <= 1) return p;
    auto base = min_element(p.begin(), p.end(), [](IP a, IP b) {
        return a.y != b.y ? a.y < b.y : a.x < b.x;
    });
    swap(p[0], *base);
    IP o = p[0];
    sort(p.begin() + 1, p.end(), [o](IP a, IP b) {
        i128 t = cross(o, a, b);
        return t ? t > 0 : dist2(o, a) < dist2(o, b);
    });
    vector<IP> h;
    for (IP q : p) {
        while (h.size() >= 2 && cross(h[h.size()-2], h.back(), q) <= 0)
            h.pop_back();
        h.push_back(q);
    }
    return h;
}
