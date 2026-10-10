#include <bits/stdc++.h>
#include <cassert>
using namespace std;
#include "../code/game_theory/game.hpp"
#include "../code/combinatorics/binomial.hpp"
#include "../code/number_theory/crt.hpp"
#include "../code/number_theory/bignum.hpp"
#include "../code/linear_algebra/gauss.hpp"
#include "../code/linear_algebra/matrix.hpp"
#include "../code/flow/dinic.hpp"
#include "../code/flow/min_cost_flow.hpp"
#include "../code/matching/hopcroft_karp.hpp"
#include "../code/matching/hungarian.hpp"
#include "../code/graph_logic/two_sat.hpp"
#include "../code/strings/basic.hpp"
#include "../code/strings/advanced.hpp"
#include "../code/dp_optimization/cht.hpp"
#include "../code/dp_optimization/li_chao.hpp"
#include "../code/dp_optimization/dnc.hpp"

int main() {
    mt19937 rng(42);
    {
        vector<vector<int>> g{{1,2},{3},{3},{}};
        assert((grundyDAG(g,{0,1,2,3}) == vector<int>{0,1,1,0}));
        assert((gameOutcome(g) == vector<int>{0,1,1,0}));
        assert((gameOutcome({{1},{0},{}}) == vector<int>{2,2,0}));
    }
    {
        Binomial b(40,1000000007);
        long long c[41][41]{};
        c[0][0]=1;
        for(int i=1;i<=40;++i) {
            c[i][0]=c[i][i]=1;
            for(int j=1;j<i;++j)c[i][j]=(c[i-1][j-1]+c[i-1][j])%b.p;
        }
        for(int i=0;i<=40;++i)for(int j=0;j<=i;++j)
            assert(b.choose(i,j)==c[i][j]);
        assert((vector<long long>{b.catalan(0),b.catalan(1),b.catalan(2),b.catalan(3),b.catalan(4)}
                == vector<long long>{1,1,2,5,14}));
    }
    {
        for(int m=2;m<=40;++m)for(int a=-40;a<=40;++a) {
            auto x=inverseMod(a,m);
            assert(bool(x)==(gcd(a,m)==1));
            if(x)assert(modNorm((long long)a*(*x),m)==1);
        }
        for(int m1=1;m1<=12;++m1)for(int m2=1;m2<=12;++m2)
            for(int r1=-2;r1<=2;++r1)for(int r2=-2;r2<=2;++r2) {
                auto got=mergeCRT(r1,m1,r2,m2);
                int l=lcm(m1,m2), brute=-1;
                for(int x=0;x<l;++x)
                    if(modNorm(x-r1,m1)==0 && modNorm(x-r2,m2)==0){brute=x;break;}
                assert(bool(got)==(brute!=-1));
                if(got)assert(got->first==brute && got->second==l);
            }
    }
    {
        auto a=gaussReal({{1,1,3},{1,-1,1}},2);
        assert(a.status==1 && abs(a.x[0]-2)<1e-10 && abs(a.x[1]-1)<1e-10);
        assert(gaussReal({{1,1,2},{2,2,4}},2).status==2);
        assert(gaussReal({{1,1,2},{1,1,3}},2).status==0);
        for(int tc=0;tc<1000;++tc) {
            int vars=1+rng()%6, rows=rng()%8;
            vector<uint64_t> mask(rows);vector<int> rhs(rows);
            for(int i=0;i<rows;++i){mask[i]=rng()% (1u<<vars);rhs[i]=rng()%2;}
            auto sol=gaussGF2(mask,rhs,vars);
            int count=0;
            for(int x=0;x<(1<<vars);++x) {
                bool ok=true;
                for(int i=0;i<rows;++i)
                    if((__builtin_parityll(mask[i]&x))!=rhs[i])ok=false;
                count+=ok;
            }
            assert((sol.status==0)==(count==0));
            if(count) {
                assert(count==(1<<(vars-sol.rank)));
                for(int i=0;i<rows;++i)
                    assert(__builtin_parityll(mask[i]&sol.x)==rhs[i]);
            }
        }
        Matrix fib(2, 2);
        fib.d[0][0]=fib.d[0][1]=fib.d[1][0]=1;
        Matrix identity=fib^0;
        assert(identity.d[0][0]==1 && identity.d[1][1]==1);
        assert(identity.d[0][1]==0 && identity.d[1][0]==0);
        assert((fib^10).d[0][1]==55);
        Matrix left(2,3), right(3,1);
        left.d[0][0]=1; left.d[0][1]=2; left.d[0][2]=3;
        left.d[1][0]=4; left.d[1][1]=5; left.d[1][2]=6;
        right.d[0][0]=7; right.d[1][0]=8; right.d[2][0]=9;
        Matrix product=left*right;
        assert(product.m==2 && product.n==1);
        assert(product.d[0][0]==50 && product.d[1][0]==122);
    }
    {
        assert(BigNum("-000") == BigNum("0"));
        assert((BigNum("-5")+BigNum("3")).val=="2");
        assert((BigNum("-5")+BigNum("3")).negative);
        assert((BigNum("-5")-BigNum("-3"))==BigNum("-2"));
        assert((BigNum("-3")-BigNum("-5"))==BigNum("2"));
        assert((BigNum("0")*BigNum("-7"))==BigNum("0"));
        assert((BigNum("99999999999999999999")*BigNum("99999999999999999999"))
            ==BigNum("9999999999999999999800000000000000000001"));
        for(int tc=0;tc<2000;++tc) {
            long long x=(long long)(rng()%2000000001)-1000000000;
            long long y=(long long)(rng()%2000000001)-1000000000;
            BigNum a(to_string(x)),b(to_string(y));
            assert((a<b) == (x<y));
            assert((a>b) == (x>y));
            assert((a<=b) == (x<=y));
            assert((a>=b) == (x>=y));
            auto result=[](const BigNum &v){return (v.negative?"-":"")+v.val;};
            assert(result(a+b)==to_string(x+y));
            assert(result(a-b)==to_string(x-y));
            assert(result(a*b)==to_string(x*y));
        }
    }
    {
        for(int tc=0;tc<400;++tc) {
            int n=2+rng()%5; Dinic d(n);
            vector<tuple<int,int,int>> edges;
            for(int u=0;u<n;++u)for(int v=0;v<n;++v)if(u!=v && rng()%3==0) {
                int cap=rng()%4; d.addEdge(u,v,cap);edges.push_back({u,v,cap});
            }
            long long got=d.maxflow(0,n-1), best=LLONG_MAX;
            for(int mask=0;mask<(1<<n);++mask)if((mask&1) && !(mask>>(n-1)&1)){
                long long cut=0;
                for(auto [u,v,c]:edges)if((mask>>u&1)&&!(mask>>v&1))cut+=c;
                best=min(best,cut);
            }
            assert(got==best);
            auto side=d.minCutSide(0);
            long long cut=0;
            for(auto [u,v,c]:edges)if(side[u]&&!side[v])cut+=c;
            assert(cut==got);
        }
        for(int tc=0;tc<250;++tc) {
            int n=4, need=1+rng()%3;
            vector<tuple<int,int,int,int>> es;
            MinCostFlow mf(n);
            for(int u=0;u<n;++u)for(int v=u+1;v<n;++v)if(rng()%2) {
                int cap=1+rng()%2, cost=(int)(rng()%9)-4;
                es.push_back({u,v,cap,cost});mf.addEdge(u,v,cap,cost);
            }
            auto got=mf.run(0,n-1,need);
            int bestFlow=-1, bestCost=INT_MAX;
            int states=1;
            for(auto [u,v,c,w]:es)states*=c+1;
            for(int state=0;state<states;++state) {
                int cur=state, bal[4]{}, cost=0;
                for(auto [u,v,c,w]:es){int f=cur%(c+1);cur/=c+1;bal[u]-=f;bal[v]+=f;cost+=f*w;}
                if(bal[1]||bal[2]||bal[0]!=-bal[3]||bal[3]>need)continue;
                if(bal[3]>bestFlow){bestFlow=bal[3];bestCost=cost;}
                else if(bal[3]==bestFlow)bestCost=min(bestCost,cost);
            }
            assert(got.first==bestFlow && got.second==bestCost);
        }
    }
    {
        for(int tc=0;tc<350;++tc) {
            int nl=1+rng()%6,nr=1+rng()%6;
            HopcroftKarp hk(nl,nr);vector<pair<int,int>> es;
            for(int u=0;u<nl;++u)for(int v=0;v<nr;++v)if(rng()%2){
                hk.addEdge(u,v);es.push_back({u,v});
            }
            int got=hk.maxMatching(), best=0;
            vector<int> dp(1<<nr,-1);dp[0]=0;
            for(int u=0;u<nl;++u){
                vector<int> ndp=dp;
                for(int mask=0;mask<(1<<nr);++mask)if(dp[mask]>=0)
                    for(int v:hk.adj[u])if(!(mask>>v&1))
                        ndp[mask|1<<v]=max(ndp[mask|1<<v],dp[mask]+1);
                dp.swap(ndp);
            }
            for(int x:dp)best=max(best,x);
            assert(got==best);
            auto [L,R]=hk.minVertexCover();
            assert((int)L.size()+(int)R.size()==got);
            vector<char> cl(nl),cr(nr);
            for(int u:L)cl[u]=1;
            for(int v:R)cr[v]=1;
            for(auto [u,v]:es)assert(cl[u]||cr[v]);
        }
        for(int tc=0;tc<300;++tc){
            int n=1+rng()%4,m=n+rng()%2;
            vector<vector<long long>> a(n,vector<long long>(m));
            for(auto& row:a)for(auto& x:row)x=(int)(rng()%15)-7;
            auto [got,as]=hungarian(a);
            long long brute=LLONG_MAX;vector<int> chosen(m);
            iota(chosen.begin(),chosen.end(),0);
            do {
                long long x=0;for(int i=0;i<n;++i)x+=a[i][chosen[i]];
                brute=min(brute,x);
            } while(next_permutation(chosen.begin(),chosen.end()));
            assert(got==brute);
            vector<int> uniq=as;sort(uniq.begin(),uniq.end());
            assert(unique(uniq.begin(),uniq.end())==uniq.end());
        }
    }
    {
        for(int tc=0;tc<1000;++tc) {
            int n=1+rng()%5,m=rng()%12;
            TwoSAT sat(n);vector<pair<int,int>> clauses;
            for(int j=0;j<m;++j) {
                int a=rng()%(2*n),b=rng()%(2*n);
                sat.either(a,b);clauses.push_back({a,b});
            }
            auto ans=sat.solve();
            bool possible=false;
            for(int mask=0;mask<(1<<n);++mask){
                bool ok=true;
                for(auto [a,b]:clauses) {
                    bool va=((mask>>(a/2)&1)==(a&1));
                    bool vb=((mask>>(b/2)&1)==(b&1));
                    if(!va && !vb)ok=false;
                }
                possible|=ok;
            }
            assert(bool(ans)==possible);
            if(ans)for(auto [a,b]:clauses)
                assert(((*ans)[a/2]==(a&1))||((*ans)[b/2]==(b&1)));
        }
    }
    {
        Trie trie;trie.insert("ab");trie.insert("abc");trie.insert("ab");
        assert(trie.countPrefix("ab")==3 && trie.countWord("ab")==2);
        assert(trie.countWord("a")==0 && trie.countPrefix("x")==0);
        for(int tc=0;tc<600;++tc) {
            string s;int n=rng()%22;
            for(int i=0;i<n;++i)s+=char('a'+rng()%3);
            auto z=zFunction(s);
            for(int i=0;i<n;++i){
                int k=0;while(i+k<n && s[k]==s[i+k])++k;
                assert(z[i]==k);
            }
            auto [d1,d2]=manacher(s);
            for(int i=0;i<n;++i){
                int k=0;while(i-k>=0&&i+k<n&&s[i-k]==s[i+k])++k;
                assert(d1[i]==k);
                k=0;while(i-k-1>=0&&i+k<n&&s[i-k-1]==s[i+k])++k;
                assert(d2[i]==k);
            }
            vector<int> sa(n);iota(sa.begin(),sa.end(),0);
            sort(sa.begin(),sa.end(),[&](int a,int b){return s.substr(a)<s.substr(b);});
            assert(suffixArray(s)==sa);
            AhoCorasick ac;vector<string> pats;
            int q=1+rng()%8;
            for(int i=0;i<q;++i){
                string p;int len=1+rng()%4;
                for(int j=0;j<len;++j)p+=char('a'+rng()%3);
                pats.push_back(p);ac.addPattern(p);
            }
            ac.build();
            long long brute=0;
            for(auto p:pats)for(int i=0;i+(int)p.size()<=n;++i)
                brute+=(s.compare(i,p.size(),p)==0);
            assert(ac.countMatches(s)==brute);
        }
    }
    {
        for(int tc=0;tc<500;++tc) {
            MonotoneCHT cht;vector<MonotoneCHT::Line> lines;
            long long x=-20,m=20;
            int steps=1+rng()%20;
            for(int i=0;i<steps;++i) {
                m-=1+rng()%3;long long b=(int)(rng()%101)-50;
                cht.add(m,b);lines.push_back({m,b});
                x+=rng()%4;
                __int128 brute=LiChao::INF;
                for(auto l:lines)brute=min(brute,MonotoneCHT::value(l,x));
                assert(cht.query(x)==brute);
            }
        }
        for(int tc=0;tc<500;++tc) {
            LiChao lc(-20,20);vector<LiChao::Line> lines;
            int count=1+rng()%20;
            for(int i=0;i<count;++i){
                LiChao::Line l{(int)(rng()%31)-15,(int)(rng()%101)-50};
                lc.add(l);lines.push_back(l);
                for(long long x=-20;x<=20;++x){
                    __int128 brute=LiChao::INF;
                    for(auto q:lines)brute=min(brute,LiChao::value(q,x));
                    assert(lc.query(x)==brute);
                }
            }
        }
        for(int tc=0;tc<200;++tc) {
            int n=1+rng()%30;vector<long long> prev(n+1);
            for(auto& x:prev)x=rng()%50;
            auto cost=[](int i,int j){return 1LL*(j-i)*(j-i);};
            auto got=divideConquerLayer(prev,cost);
            for(int j=1;j<=n;++j){
                long long best=LLONG_MAX;
                for(int i=0;i<j;++i)best=min(best,prev[i]+cost(i,j));
                assert(got[j]==best);
            }
        }
    }
    cout << "notebook sections OK\n";
}
