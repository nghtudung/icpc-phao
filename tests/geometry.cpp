#include <bits/stdc++.h>
#include <cassert>
using namespace std;
#include "../code/geometry/integer.hpp"
#include "../code/geometry/real.hpp"

std::vector<IP> referenceHull(std::vector<IP> p) {
    std::sort(p.begin(), p.end());
    p.erase(std::unique(p.begin(), p.end()), p.end());
    if (p.size() <= 1) return p;
    std::vector<IP> lo, hi;
    for (IP q : p) {
        while (lo.size() >= 2 && cross(lo[lo.size()-2], lo.back(), q) <= 0)
            lo.pop_back();
        lo.push_back(q);
    }
    for (auto it = p.rbegin(); it != p.rend(); ++it) {
        while (hi.size() >= 2 && cross(hi[hi.size()-2], hi.back(), *it) <= 0)
            hi.pop_back();
        hi.push_back(*it);
    }
    lo.pop_back(); hi.pop_back();
    lo.insert(lo.end(), hi.begin(), hi.end());
    return lo;
}

int main() {
    assert(sign(cross({0,0}, {4,0}, {1,2})) == 1);
    assert(sign(cross({0,0}, {4,0}, {1,-2})) == -1);
    assert(onSegment({0,0}, {4,0}, {4,0}));
    assert(segmentsIntersect({0,0}, {4,4}, {0,4}, {4,0}));
    assert(segmentsIntersect({0,0}, {4,0}, {2,0}, {6,0}));
    assert(segmentsIntersect({1,1}, {1,1}, {0,1}, {2,1}));
    assert(!segmentsIntersect({0,0}, {1,0}, {2,0}, {3,0}));
    assert(!segmentsIntersect({0,0}, {1,1}, {2,0}, {3,1}));
    assert(lineSegmentRelation({0,0}, {4,0}, {1,-1}, {1,1}) == 1);
    assert(lineSegmentRelation({0,0}, {4,0}, {1,0}, {3,0}) == 2);
    assert(lineSegmentRelation({0,0}, {4,0}, {1,0}, {1,0}) == 1);
    assert(lineSegmentRelation({0,0}, {4,0}, {1,1}, {3,1}) == 0);

    std::vector<IP> square{{0,0}, {2,0}, {2,2}, {0,2}};
    assert(polygonArea2(square) == 8);
    std::reverse(square.begin(), square.end());
    assert(polygonArea2(square) == -8);
    std::vector<IP> tri{{0,0}, {1,0}, {0,1}};
    assert(polygonArea2(tri) == 1);

    Line axis{{0,0},{1,0}};
    auto p = lineIntersection(axis, {{1,-1},{0,1}});
    assert(p && std::abs(p->x - 1) < EPS && std::abs(p->y) < EPS);
    assert(!lineIntersection(axis, {{0,1},{1,0}}));
    assert(std::abs(distancePointSegment({2,1}, {0,0}, {1,0}) - std::sqrt(2.0L)) < EPS);
    assert(std::abs(distancePointSegment({2,1}, {1,0}, {1,0}) - std::sqrt(2.0L)) < EPS);
    assert(lineCircleIntersection(axis, {{0,0},1}).size() == 2);
    assert(lineCircleIntersection({{0,1},{1,0}}, {{0,0},1}).size() == 1);
    assert(lineCircleIntersection({{0,2},{1,0}}, {{0,0},1}).empty());

    assert(grahamHull({{0,0},{1,0},{2,0},{2,0}}).size() == 2);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> xy(-10, 10);
    for (int tc = 0; tc < 5000; ++tc) {
        int n = 1 + rng() % 20;
        std::vector<IP> points;
        for (int i = 0; i < n; ++i) points.push_back({xy(rng), xy(rng)});
        auto a = grahamHull(points), b = referenceHull(points);
        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        assert(a == b);
    }
    std::cout << "geometry OK\n";
}
