# Geometry snippets

- `integer.hpp`: exact predicates for integer coordinates, polygon area, and Graham scan.
- `real.hpp`: point and line formulas using `long double`, including line-circle intersection.

The notebook includes both files from `topics/geometry.tex`. Each snippet uses `bits/stdc++.h` and `using namespace std`, with only the geometry definitions after that. Run `tests/geometry.cpp` after changing either snippet. Check that coordinate bounds keep every intermediate `__int128` result in range, and tune `EPS` to the problem scale.
