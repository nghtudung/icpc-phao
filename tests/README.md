# Snippet tests

Keep focused tests for the code snippets selected for the notebook. Test files are not included in the PDF.

Run from the repository root:

```sh
python3 tests/run.py --compiler g++-16
python3 tests/run.py --compiler clang++ --sanitize
```

Use the installed GCC version in place of `g++-16`. The runner builds all C++ test
files, checks that the snippets can be combined, compares BigNum with Python
integers, and tests the matrix snippet with modulus 1 and `LLONG_MAX`.
Sanitizers check invalid memory access and undefined arithmetic.
