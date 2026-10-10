#include <bits/stdc++.h>
#include <cassert>
using namespace std;
#include "../code/number_theory/bignum.hpp"
#include "../code/number_theory/crt.hpp"
#include "../code/combinatorics/binomial.hpp"
#include "../code/linear_algebra/gauss.hpp"
#include "../code/flow/dinic.hpp"
#include "../code/flow/min_cost_flow.hpp"
#include "../code/matching/hungarian.hpp"
#include "../code/graph_logic/two_sat.hpp"
#include "../code/dp_optimization/li_chao.hpp"
#include "../code/dp_optimization/cht.hpp"
#include "../code/dp_optimization/dnc.hpp"
#include "../code/strings/advanced.hpp"

int main(int argc, char**) {
    if (argc > 1) {
        string a, b;
        while (cin >> a >> b) {
            BigNum x(a), y(b);
            (x+y).print(); cout << ' ';
            (x-y).print(); cout << ' ';
            (x*y).print(); cout << ' ';
            cout << (x<y) << ' ' << (x>y) << ' ' << (x<=y) << ' '
                 << (x>=y) << ' ' << (x==y) << ' ' << (x!=y) << '\n';
        }
        return 0;
    }
    mt19937_64 rng(20261011);
    {
        LiChao tree(LLONG_MIN, LLONG_MAX);
        assert(tree.query(0) == LiChao::INF);
        vector<LiChao::Line> lines{{LLONG_MIN,LLONG_MAX},
                                 {LLONG_MAX,LLONG_MIN},{0,0}};
        for (auto line : lines) {
            tree.add(line);
            // The first line at LLONG_MIN exceeds the old 2^120 sentinel.
            for (long long x : {LLONG_MIN, -1LL, 0LL, 1LL, LLONG_MAX}) {
                __int128 expected = LiChao::INF;
                for (auto prev : lines) {
                    expected = min(expected, LiChao::value(prev,x));
                    if (prev.m == line.m) break;
                }
                assert(tree.query(x) == expected);
            }
        }
    }
    {
        MinCostFlow f(3);
        f.addEdge(0,1,2,LLONG_MAX/2);
        f.addEdge(1,2,2,LLONG_MAX/2);
        auto result=f.run(0,2,2);
        assert(result.first==2 && result.second==(__int128)4*(LLONG_MAX/2));
        assert(f.run(0,2,1).first==0);
        MinCostFlow negative(2);
        negative.addEdge(0,1,1,LLONG_MIN);
        assert(negative.run(0,1,1).second==(__int128)LLONG_MIN);
        MinCostFlow cycle(4);
        cycle.addEdge(0,1,1,0);
        cycle.addEdge(2,3,1,-2); cycle.addEdge(3,2,1,1);
        bool rejected=false;
        try { cycle.run(0,1,1); } catch (const invalid_argument&) { rejected=true; }
        assert(rejected);
        // Exhaust all feasible flows; costs are nonnegative after a potential
        // transformation, so cycles can exist but no cycle has negative cost.
        for (int tc=0;tc<300;++tc) {
            int n=4, need=rng()%4, m=1+rng()%7;
            int phi[4]; for (int& x:phi) x=int(rng()%11)-5;
            vector<array<int,4>> es;
            MinCostFlow mf(n);
            int states=1;
            for (int i=0;i<m;++i) {
                int u=rng()%n,v=rng()%n,cap=1+rng()%2;
                int cost=int(rng()%4)+phi[v]-phi[u];
                es.push_back({u,v,cap,cost}); mf.addEdge(u,v,cap,cost);
                states*=cap+1;
            }
            int bestFlow=-1,bestCost=INT_MAX;
            for (int mask=0;mask<states;++mask) {
                int cur=mask,bal[4]{},cost=0;
                for (auto [u,v,cap,w]:es) {
                    int f=cur%(cap+1);cur/=cap+1;
                    bal[u]-=f;bal[v]+=f;cost+=f*w;
                }
                if (bal[1]||bal[2]||bal[0]!=-bal[3]||bal[3]<0||bal[3]>need) continue;
                if (bal[3]>bestFlow) bestFlow=bal[3],bestCost=cost;
                else if (bal[3]==bestFlow) bestCost=min(bestCost,cost);
            }
            auto got=mf.run(0,3,need);
            assert(got.first==bestFlow && got.second==bestCost);
        }
        Dinic d(3);
        d.addEdge(0,0,LLONG_MAX); d.addEdge(0,1,2); d.addEdge(0,1,3);
        d.addEdge(1,0,4); d.addEdge(1,2,5);
        assert(d.maxflow(0,2)==5 && d.maxflow(0,2)==0);
    }
    {
        assert(hungarian({{LLONG_MIN}}).first==(__int128)LLONG_MIN);
        assert(hungarian({{LLONG_MAX,LLONG_MAX},{LLONG_MAX,LLONG_MAX}}).first
               ==(__int128)2*LLONG_MAX);
        for (int tc=0;tc<200;++tc) {
            int n=1+rng()%4,m=n+rng()%2;
            vector<vector<long long>> a(n,vector<long long>(m));
            for (auto& row:a) for (auto& x:row) {
                long long delta=rng()%1000;
                x=rng()%2 ? LLONG_MAX-delta : LLONG_MIN+delta;
            }
            auto got=hungarian(a);
            vector<int> p(m);iota(p.begin(),p.end(),0);
            __int128 best=numeric_limits<__int128>::max();
            do {
                __int128 cost=0;
                for(int i=0;i<n;++i) cost+=a[i][p[i]];
                best=min(best,cost);
            } while(next_permutation(p.begin(),p.end()));
            assert(got.first==best);
        }
    }
    {
        for(int tc=0;tc<2000;++tc) {
            int n=1+rng()%5,m=rng()%15;
            TwoSAT sat(n); vector<pair<int,int>> implications;
            for(int i=0;i<m;++i) {
                int a=rng()%(2*n),b=rng()%(2*n);
                sat.implication(a,b); implications.push_back({a,b});
            }
            auto valid=[&](int mask) {
                for(auto [a,b]:implications)
                    if (((mask>>(a/2)&1)==(a&1)) && ((mask>>(b/2)&1)!=(b&1)))
                        return false;
                return true;
            };
            bool possible=false;
            for(int mask=0;mask<(1<<n);++mask) possible|=valid(mask);
            auto got=sat.solve(); assert(bool(got)==possible);
            if(got) {
                int mask=0;for(int i=0;i<n;++i)mask|=(*got)[i]<<i;
                assert(valid(mask));
            }
        }
    }
    {
        AhoCorasick ac;
        ac.addPattern(""); ac.addPattern(""); ac.addPattern("a");
        ac.addPattern("aa"); ac.addPattern("a"); ac.build();
        assert(ac.countMatches("")==2);
        assert(ac.countMatches("aa")==6+4+1);
        AhoCorasick empty; empty.build(); assert(empty.countMatches("abc")==0);
        // High bytes must be ordered as bytes, not as signed char values.
        string s; for(int i=255;i>=1;--i)s+=char(i);
        auto sa=suffixArray(s); assert(sa.size()==255);
        for(int i=0;i<255;++i) assert(sa[i]==254-i);
    }
    {
        const long long INF=LLONG_MAX/4;
        int n=200000,first=150000;
        vector<long long> prev(n+1,INF);prev[first]=0;
        auto got=divideConquerLayer(prev,[](int i,int j){return 1LL*(j-i)*(j-i);});
        for(int j=1;j<=n;++j)
            assert(got[j]==(j<=first ? INF : 1LL*(j-first)*(j-first)));
        for(int tc=0;tc<300;++tc) {
            MonotoneCHT hull;vector<MonotoneCHT::Line> lines;
            long long m=30,x=-30;
            for(int i=0;i<100;++i) {
                m-=rng()%3;
                long long b=(long long)(rng()%201)-100;
                hull.add(m,b);lines.push_back({m,b});
                x+=rng()%3;
                __int128 best=LiChao::INF;
                for(auto line:lines)best=min(best,MonotoneCHT::value(line,x));
                assert(hull.query(x)==best);
            }
        }
    }
    {
        vector<uint64_t> a;vector<int> b;
        for(int i=0;i<64;++i) a.push_back(1ULL<<i),b.push_back(i%2);
        auto got=gaussGF2(a,b,64);
        assert(got.status==1 && got.rank==64 && got.x==0xaaaaaaaaaaaaaaaaULL);
        assert(gaussReal({},0).status==1);
        assert(gaussReal({},3).status==2);
        assert(gaussReal({{1}},0).status==0);
        for(int tc=0;tc<200;++tc) {
            int n=1+rng()%8;
            vector<ld> expected(n);
            for(auto& x:expected)x=(int)(rng()%21)-10;
            vector<vector<ld>> a(n,vector<ld>(n+1));
            for(int i=0;i<n;++i) {
                for(int j=0;j<n;++j)a[i][j]=(int)(rng()%11)-5;
                a[i][i]+=100; // Strict diagonal dominance implies invertibility.
                for(int j=0;j<n;++j)a[i][n]+=a[i][j]*expected[j];
            }
            auto got=gaussReal(a,n);assert(got.status==1);
            for(int i=0;i<n;++i)assert(abs(got.x[i]-expected[i])<1e-9L);
        }
        bool overflow=false;
        try { mergeCRT(0,LLONG_MAX,0,2); } catch(const overflow_error&){overflow=true;}
        assert(overflow);
        auto crt=mergeCRT(LLONG_MIN,LLONG_MAX,0,1);
        assert(crt && crt->first==LLONG_MAX-1 && crt->second==LLONG_MAX);
        // Largest prime below 2^63: compare binomial formula with recurrence.
        Binomial bin(200,9223372036854775783LL);
        vector<long long> cat(101);cat[0]=1;
        for(int i=1;i<=100;++i) {
            for(int j=0;j<i;++j)
                cat[i]=(cat[i]+(__int128)cat[j]*cat[i-1-j])%bin.p;
            assert(bin.catalan(i)==cat[i]);
        }
        istringstream in("-00012");auto oldin=cin.rdbuf(in.rdbuf());
        BigNum v;v.input();cin.rdbuf(oldin);
        assert(v==BigNum("-12"));
        ostringstream out;auto oldout=cout.rdbuf(out.rdbuf());
        BigNum("-0").print(true);cout.rdbuf(oldout);
        assert(out.str()=="0\n");
    }
    cout<<"audit regressions OK\n";
}
