/**
 * TOPIC: sieve, STL bitset
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
using i64= int64_t;
#define N (100000001)

bitset<N> b;
int64_t n;
vector<int64_t> primes;

int main() {
    cin.tie(nullptr), ios_base::sync_with_stdio(false);
    int64_t i,j,k,ts;
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
#endif
    for ( b[2]= true, i= 3; i < N; i+= 2 )
        b[i]= true ;
    for ( i= 3; i < N; i+= 2 )
        if ( b[i] )
            for ( j= i*i; j < N; b[j]= false, j+= i ) ;
    for ( k= 0, i= 0; i < N; ++i )
        if ( b[i] ) ++k;
    for ( j= 0, primes.resize(k), i= 2; i < N; ++i )
        if ( b[i] )
            primes[j++]= i;
    assert( j == k );
    for (;(cin >> n) and n;) {
        auto it= upper_bound(primes.begin(),primes.end(),n);
        assert( it != primes.begin() );
        cout << fixed << setprecision(1) << fabs(distance(primes.begin(),it)-n/log(n))/distance(primes.begin(),it)*100.00 << endl;
    }
}

