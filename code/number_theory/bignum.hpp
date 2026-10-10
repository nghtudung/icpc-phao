#include <bits/stdc++.h>
#include <cassert>
using namespace std;

struct BigNum {
    string val;
    bool negative = false;

    BigNum(string s = "0") {
        assert(!s.empty());
        if (s[0] == '-' || s[0] == '+') {
            negative = (s[0] == '-');
            s.erase(0, 1);
        }
        assert(!s.empty());
        assert(all_of(s.begin(), s.end(), [](char c) { return '0' <= c && c <= '9'; }));
        auto p = s.find_first_not_of('0');
        val = (p == string::npos ? "0" : s.substr(p));
        if (val == "0") negative = false;
    }

    void print(bool endline = false) const {
        if (negative) cout << '-';
        cout << val;
        if (endline) cout << '\n';
    }

    void input() {
        string s;
        cin >> s;
        *this = BigNum(s);
    }

    static int cmpAbs(const string &a, const string &b) {
        if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
        return a == b ? 0 : (a < b ? -1 : 1);
    }

    static string addString(const string &a, const string &b) {
        string res;
        int carry = 0;
        for (int i = (int)a.size() - 1, j = (int)b.size() - 1;
             i >= 0 || j >= 0 || carry; --i, --j) {
            int cur = carry + (i >= 0 ? a[i] - '0' : 0)
                            + (j >= 0 ? b[j] - '0' : 0);
            res += char('0' + cur % 10);
            carry = cur / 10;
        }
        reverse(res.begin(), res.end());
        return res;
    }

    // Precondition: a >= b as nonnegative decimal integers.
    static string subString(const string &a, const string &b) {
        string res;
        int borrow = 0;
        for (int i = (int)a.size() - 1, j = (int)b.size() - 1;
             i >= 0; --i, --j) {
            int cur = a[i] - '0' - borrow - (j >= 0 ? b[j] - '0' : 0);
            borrow = (cur < 0);
            if (borrow) cur += 10;
            res += char('0' + cur);
        }
        while (res.size() > 1 && res.back() == '0') res.pop_back();
        reverse(res.begin(), res.end());
        return res;
    }

    bool operator < (const BigNum &b) const {
        if (negative != b.negative) return negative;
        int c = cmpAbs(val, b.val);
        return negative ? c > 0 : c < 0;
    }
    bool operator > (const BigNum &b) const { return b < *this; }
    bool operator <= (const BigNum &b) const { return !(b < *this); }
    bool operator >= (const BigNum &b) const { return !(*this < b); }
    bool operator == (const BigNum &b) const {
        return negative == b.negative && val == b.val;
    }
    bool operator != (const BigNum &b) const { return !(*this == b); }

    BigNum operator + (const BigNum &b) const {
        BigNum res;
        if (negative == b.negative) {
            res.val = addString(val, b.val);
            res.negative = negative;
        } else {
            int c = cmpAbs(val, b.val);
            if (c == 0) return BigNum("0");
            res.val = c > 0 ? subString(val, b.val) : subString(b.val, val);
            res.negative = c > 0 ? negative : b.negative;
        }
        if (res.val == "0") res.negative = false;
        return res;
    }

    BigNum operator - (const BigNum &b) const {
        BigNum opposite = b;
        if (opposite.val != "0") opposite.negative = !opposite.negative;
        return *this + opposite;
    }

    BigNum operator * (const BigNum &b) const {
        vector<int> digits(val.size() + b.val.size());
        for (int i = (int)val.size() - 1; i >= 0; --i)
            for (int j = (int)b.val.size() - 1; j >= 0; --j) {
                int p = i + j + 1;
                int cur = digits[p] + (val[i] - '0') * (b.val[j] - '0');
                digits[p] = cur % 10;
                digits[p - 1] += cur / 10;
            }
        string s;
        for (int x : digits) s += char('0' + x);
        BigNum res(s);
        if (res.val != "0") res.negative = negative != b.negative;
        return res;
    }
};
