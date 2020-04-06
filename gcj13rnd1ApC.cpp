/**
 * TOPIC: probability, Bayes theorem
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
enum { FIVE, TWELVE };

std::istream &is = std::cin;
std::ostream &os = std::cout;

std::vector<std::vector<int>> partitions[2];
std::map<std::pair<i64,int>,double> mp[2];
std::vector<double> P[2];
i64 C[20][20];

double fnc( const int M, const std::vector<int> &p ) {
    static i64 factorial[16]= {0};
    i64 i;
    if ( not factorial[0] )
        for ( factorial[0]= 1, i= 1; i < 16; ++i )
            factorial[i]= factorial[i-1]*i;
    auto n= std::accumulate(p.begin(),p.end(),0);
    i64 prob= factorial[n];
    assert( p.size() == M-2+1 );
    for ( i= 0; i < p.size(); ++i ) {
        assert( prob%factorial[p[i]] == 0 );
        prob /= factorial[p[i]];
    }
    //assert( n == 12 );
    return prob/pow(M-1,n);
}

void calc_partitions( const int t, const int N, const int M, std::vector<int> &p, int d, int cursum ) {
    if ( cursum == N ) {
        //assert( cursum == 12 );
        assert( p.size() == M-2+1 );
        assert( std::accumulate(p.begin(),p.end(),0) == N );
        partitions[t].push_back(p), P[t].push_back(fnc(M,p));
        return ;
    }
    if ( cursum > N or d > M ) return ;
    assert( d <= M );
    for ( int k= 0; k+cursum <= N; ++k ) {
        p[d-2]= k, calc_partitions(t,N,M,p,d+1,cursum+k), p[d-2]= 0;
    }
}

void calc_conditional_probability(
        const int t,
        i64 &good, i64 &total,
        const int M, const int idx, const i64 r, const int K,
        int i, i64 prod, int quantity,
        std::vector<int> &counts,
        i64 acc ) {
    if ( i == M+1 ) {
        if (prod == r) {
            assert( acc >= 1 );
            good+= acc;
        }
        return ;
    }
    for ( i64 ax= 1, k= 0; k <= partitions[t][idx][i-2] and 0 == (r%(prod*ax)); ++k, ax*= i ) {
        counts[i-2] = k, acc*= C[partitions[t][idx][i-2]][k];
        calc_conditional_probability(t, good, total, M, idx, r, K, i + 1, prod * ax, quantity + k,counts,acc);
        acc/= C[partitions[t][idx][i-2]][k], counts[i-2]= 0;
    }
}

double calc_mp( const int t, i64 v, int idx, const int M, const int K ) {
    if ( mp[t].count({v,idx}) ) {
        return mp[t][{v, idx}];
    }
    i64 good = 0, total = 0;
    std::vector<int> counts(M-2+1);
    calc_conditional_probability(t, good, total, M, idx, v, K, 2, 1ll, 0,counts,1ll);
    assert( good <= (1<<12) );
    return mp[t][{v, idx}] = good;
}

std::string solve_query( const int t, int N, int M, int K ) {
    std::vector<i64> r(K);
    for ( auto &v: r )
        is >> v;
    int most_probable= -1;
    double best_sum= -1.00;

    std::vector<double> ans(partitions[t].size());
    for ( int i= 0; i < partitions[t].size(); ++i )
        ans[i]= P[t][i];
    assert( fabs(std::accumulate(ans.begin(),ans.end(),0.00)-1.00) < 1e-6 );

    assert( not r.empty() );
    for (long val : r) {
        double total= 0.00;
        for (int idx = 0; idx < partitions[t].size(); ++idx)
            total+= calc_mp(t,val,idx,M,K)*ans[idx];
        assert( total >= 0 );
        //if ( total < 1e-7 ) break ;
        //std::cerr << "total= " << std::setprecision(6) << std::fixed << total << std::endl;
        //assert( total >= 1e-7 );
        for ( int i= 0; i < partitions[t].size(); ++i ) {
            if ( fabs(total) < 1e-7 ) {
                /*assert( fabs(calc_mp(t,val,i,M,K)*ans[i]) < 1e-7 );
                std::cerr << "val= " << val << std::endl;
                std::cerr << ""
                std::cerr << "prob= " << calc_mp(t,val,i,M,K) << std::endl;
                assert( false );
                */
                goto nx;
            }
            ans[i] = calc_mp(t, val, i, M, K) * ans[i] / total;
        }
        assert( fabs(std::accumulate(ans.begin(),ans.end(),0.00)-1.00) < 1e-6 );
    }

    nx:
    for ( int i= 0; i < partitions[t].size(); ++i )
        if ( ans[i] > best_sum )
            best_sum= ans[i], most_probable= i;

    assert( most_probable >= 0 and most_probable < partitions[t].size() );

    std::string res{};
    res.reserve(N);
    for ( int i= 0; i < partitions[t][most_probable].size(); ++i )
        for ( int k= 0; k < partitions[t][most_probable][i]; ++k )
            res.push_back(i+'2');
    return res;
}

void solve( const int t, int cs ) {
    int R,N,M,K;
    is >> R >> N >> M >> K;
    os << "Case #" << cs << ":" << std::endl;
    for ( int qr= R; qr--; ) {
        os << solve_query(t,N,M,K) << std::endl;
    }
}

int main() {
    int cs = 0, ts;
    memset(C,0,sizeof C);
    for (auto i= 0; i < 20; ++i )
        C[i][0]= 1;
    for ( int i= 1; i < 20; ++i )
        for ( int j= 1; j <= i; ++j )
            C[i][j]= C[i-1][j]+C[i-1][j-1];
    std::vector<int> v(8-2+1);
    std::fill(v.begin(),v.end(),0);
    calc_partitions(TWELVE,12,8,v,2,0);
    //calc_conditional_probability(TWELVE,good,total,8,)
    //std::cerr << "Partitions: " << partitions[TWELVE].size() << std::endl;
    //std::vector<int> u(5-2+1);
    //std::fill(u.begin(),u.end(),0);
    //calc_partitions(FIVE,3,5,u,2,0);
    for ( is >> ts; ts--; solve(TWELVE,++cs) ) ;
    return 0;
}   
 