#!/usr/bin/env python3
"""Run notebook tests; BigNum is checked against Python's arbitrary precision int."""
import argparse
import pathlib
import random
import re
import subprocess
import tempfile

parser = argparse.ArgumentParser()
parser.add_argument("--compiler", default="c++")
parser.add_argument("--sanitize", action="store_true")
args = parser.parse_args()
root = pathlib.Path(__file__).resolve().parents[1]
flags = ["-std=c++17", "-Wall", "-Wextra"]
flags += (["-O1", "-g", "-fsanitize=address,undefined",
           "-fno-sanitize-recover=all"] if args.sanitize else ["-O2"])

def compile_file(src, dest):
    subprocess.run([args.compiler, *flags, "-I", str(root), str(src),
                    "-o", str(dest)], check=True)

with tempfile.TemporaryDirectory(prefix="icpc-audit-") as folder:
    temp = pathlib.Path(folder)
    for src in sorted((root / "tests").glob("*.cpp")):
        exe = temp / src.stem
        compile_file(src, exe)
        subprocess.run([str(exe)], check=True, timeout=60)

    # All snippets must coexist, and each carries the prologue hidden in the PDF.
    headers = sorted((root / "code").rglob("*.hpp"))
    for header in headers:
        lines = header.read_text().splitlines()
        assert lines[:4] == ["#include <bits/stdc++.h>", "#include <cassert>", "using namespace std;", ""], header
    combined = temp / "all_snippets.cpp"
    combined.write_text("\n".join('#include "' + str(h.relative_to(root)) + '"'
                                   for h in headers) + "\nint main() {}\n")
    compile_file(combined, temp / "all_snippets")
    print(f"{len(headers)} snippets compile together", flush=True)

    rng = random.Random(20261011)
    pairs = [("-000", "+0"), ("-5", "3"), ("-3", "-5"),
             ("0", "-" + "9" * 400), ("9" * 500, "9" * 500)]
    for _ in range(600):
        a = "".join(str(rng.randrange(10)) for _ in range(rng.randrange(1, 301)))
        b = "".join(str(rng.randrange(10)) for _ in range(rng.randrange(1, 301)))
        pairs.append((("-" if rng.randrange(2) else "+") + "00" + a,
                      ("-" if rng.randrange(2) else "") + "00" + b))
    result = subprocess.run([str(temp / "audit_regressions"), "--bignum"],
                            input="".join(a+" "+b+"\n" for a,b in pairs),
                            text=True, capture_output=True, check=True, timeout=30)
    outputs = result.stdout.splitlines()
    assert len(outputs) == len(pairs)
    for (sa,sb), line in zip(pairs, outputs):
        a,b = int(sa),int(sb)
        expected = [a+b,a-b,a*b,int(a<b),int(a>b),int(a<=b),
                    int(a>=b),int(a==b),int(a!=b)]
        assert list(map(int,line.split())) == expected, (sa,sb,line)
    print(f"BigNum: {len(pairs)} signed cases, up to 500 digits, match Python", flush=True)

    # The notebook explicitly tells users to change MOD for each problem.
    matrix = (root / "code/linear_algebra/matrix.hpp").read_text()
    matrix = "\n".join(matrix.splitlines()[4:])
    source = "#include <bits/stdc++.h>\n#include <cassert>\nusing namespace std;\n"
    for namespace, modulus in [("One", "1"), ("Large", "LLONG_MAX")]:
        adapted = re.sub(r"constexpr long long MOD = [^;]+;",
                         "constexpr long long MOD = " + modulus + ";", matrix)
        source += "namespace " + namespace + " {\n" + adapted + "\n}\n"
    source += r"""
int main() {
    One::Matrix a(1,1); assert((a^0).d[0][0]==0);
    Large::Matrix b(2,2);
    for(int i=0;i<2;++i)for(int j=0;j<2;++j)b.d[i][j]=LLONG_MAX-1;
    auto c=b*b;
    for(int i=0;i<2;++i)for(int j=0;j<2;++j)assert(c.d[i][j]==2);
    long long x=LLONG_MAX-1;
    Large::add(x,LLONG_MAX-1);assert(x==LLONG_MAX-2);
    cout << "Matrix: MOD=1 and MOD=LLONG_MAX OK\n";
}
"""
    variants = temp / "matrix_variants.cpp"
    variants.write_text(source)
    compile_file(variants, temp / "matrix_variants")
    subprocess.run([str(temp / "matrix_variants")], check=True, timeout=30)
print("All notebook tests passed.")
