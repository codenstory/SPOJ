/**
 * TOPIC: easy DP, coin change
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;
#define tol (1e-9)
#define N (100000)
#define MAXK (5)
#define oo numeric_limits<i64>::max()

i64 n,K;
bitset<N> b,is_square;
vector<i64> p,q;
i64 z[N][MAXK];

i64 calc_z( i64 x, i64 k ) {
    if ( z[x][k] < +oo )
        return z[x][k];
    if ( x == 0 )
        return z[x][k]= 1;
    assert( x >= 1 );
    if ( k == 1 )
        return z[x][k]= 1;
    assert( k >= 2 );
    z[x][k]= 0;
    for ( auto items= 0; items*k <= x; z[x][k]+= calc_z(x-items++*k,k-1) ) ;
    return z[x][k];
}

int main() {
    cin.tie(nullptr), ios_base::sync_with_stdio(false);
    i64 i,j,k,ts;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for ( i= 1; i*i < N; ++i )
        is_square[i*i]= true ;
    for ( b[2]= true, i= 3; i < N; b[i]= true, i+= 2 ) ;
    for ( i= 3; i < N; i+= 2 )
        for ( j= i*i; b[i] and j < N; b[j]= false, j+= i ) ;
    p.resize(b.count());
    for ( k= 0, i= 0; i < N; ++i )
        if ( b[i] )
            p[k++]= i;
    copy_if(p.begin(),p.end(),back_inserter(q),[&]( i64 x ) {
        /*for ( auto y= 1; y <= x/y; ++y )
            if ( is_square[x-y*y] )
                return true ;
        return false ;*/
        return x==2 or (x&3) == 1;
    });
    for ( i= 0; i < N; ++i )
        for ( k= 0; k < MAXK; z[i][k++]= +oo ) ;
    for ( cin >> ts; ts--; ) {
        cin >> n >> K;
        assert( n-1 < q.size() );
        n= q[n-1];
        cout << calc_z(n,K) << endl;
    }
}

