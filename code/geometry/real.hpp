#include <bits/stdc++.h>
#include <cassert>
using namespace std;

using ld = long double;
const ld EPS = 1e-12L;

struct FP {
    ld x, y;
    FP operator+(FP b) const { return {x + b.x, y + b.y}; }
    FP operator-(FP b) const { return {x - b.x, y - b.y}; }
    FP operator*(ld k) const { return {x * k, y * k}; }
    FP operator/(ld k) const { return {x / k, y / k}; }
};
ld dot(FP a, FP b) { return a.x * b.x + a.y * b.y; }
ld cross(FP a, FP b) { return a.x * b.y - a.y * b.x; }
ld norm2(FP a) { return dot(a, a); }
ld length(FP a) { return sqrt(norm2(a)); }

struct Line { FP p, v; }; // p + t*v, v != (0,0)
struct Circle { FP c; ld r; };

// nullopt: hai duong song song hoac trung nhau.
optional<FP> lineIntersection(Line a, Line b) {
    ld d = cross(a.v, b.v);
    if (abs(d) <= EPS) return nullopt;
    ld t = cross(b.p - a.p, b.v) / d;
    return a.p + a.v * t;
}
ld distancePointLine(FP q, Line l) {
    return abs(cross(l.v, q - l.p)) / length(l.v);
}
ld distancePointSegment(FP q, FP a, FP b) {
    FP v = b - a;
    if (norm2(v) <= EPS * EPS) return length(q - a);
    ld t = clamp(dot(q - a, v) / norm2(v), 0.0L, 1.0L);
    return length(q - (a + v * t));
}
// Tra ve 0, 1 hoac 2 giao diem voi duong thang vo han.
vector<FP> lineCircleIntersection(Line l, Circle c) {
    ld t = dot(c.c - l.p, l.v) / norm2(l.v);
    FP foot = l.p + l.v * t;
    ld h2 = c.r * c.r - norm2(foot - c.c);
    if (h2 < -EPS) return {};
    if (abs(h2) <= EPS) return {foot};
    ld shift = sqrt(max((ld)0, h2 / norm2(l.v)));
    return {foot - l.v * shift, foot + l.v * shift};
}
