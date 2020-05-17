/**
 * SEQ5
 * TOPIC: sliding window, caterpillar algorithm
 */
#include <bits/stdc++.h>


#define N (1<<20)
using u64= std::uint64_t;

int n;
u64 L,R;
std::unordered_map<u64,int> cnt;
u64 c[N];

void inc( u64 x ) {
    if ( not cnt.count(x) )
        cnt[x]= 0;
    ++cnt[x];
}

void dec( u64 x ) {
    if ( not cnt.count(x) ) {
        std::cerr << "Attempting to decrease this key: " << x << std::endl;
        assert( false );
    }
    if ( not --cnt[x] )
        cnt.erase(x);
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    int i,j,k;
    u64 ans;
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    for ( ;(is >> n >> L >> R) and n; os << ans << std::endl ) {
        for ( ans= 0, cnt.clear(), i= 0; i < n; is >> c[i++] ) ;
#define good(i,j) ((not too_many(i,j)) and (not too_few(i,j)))
#define too_few(i,j) (cnt.size() < L)
#define too_many(i,j) (cnt.size() > R)
#define valid(i,j) ((i) < (j))
        for ( i= 0, j= 0; j < n and too_few(i,j); inc(c[j++]) ) ;
        for (;valid(i,j) and good(i,j); ) {
            for ( ;valid(i,j) and good(i,j); printf("(1) [%d,%d]\n",i,j-1), ++ans, inc(c[j++]) ) ;
            for ( dec(c[i++]); valid(i,j) and too_many(i,j); dec(c[i++]) ) ;
            for ( ;valid(i,j) and good(i,j); printf("(2) [%d,%d]\n",i,j-1), ++ans, dec(c[i++]) ) ;
            for ( ;valid(i,j) and j < n and too_few(i,j); inc(c[++j]) ) ;
        }
    }
    return 0;
}
    