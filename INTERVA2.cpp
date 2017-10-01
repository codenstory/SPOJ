/*
 * INTERVA2
 * TOPIC: BIT, sorting
 * status: Accepted
 */
#include <algorithm>
#include <set>
#include <unordered_map>
#include <map>
#include <cstdio>
#include <cassert>
#define N (1<<21)
#define L(k) ((k)&((~(k))+1ULL))
#define oo (1<<30)
typedef long long i64;
using namespace std;

i64 enc( i64 x, i64 y ) { return x | (y<<21); }

int getnum() {
	register int n = 0, sign = 1;
	static int dig[256] = {0}, ch = '0';
	if ( !dig['0'] )
		for ( int i = '0'; i <= '9'; dig[i++] = 1 ) ;
	if ( ch == EOF ) return +oo;
	for ( ;(ch = getchar_unlocked()) != EOF && ch != '-' && !dig[ch]; ) ;
	if ( ch == EOF )
		return +oo;
	if ( ch == '-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch = getchar_unlocked()) != EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return n*sign;
}

unordered_map<int,int> mp;
unordered_map<i64,int> cnt;
set<int> s;
int n,x[N],y[N],m,id[N],tr[N],K,ans[N];
i64 u[N];

struct comp {
	bool operator () ( const int &a, const int &b ) {
		if ( x[a] == x[b] ) 
			return y[a]>y[b];
		return x[a]<x[b];
	}
};

void update( unsigned int i, int dt ) {
	for ( ;i < (1<<K); tr[i] += dt, i += L(i) ) ;
}

int prefix( unsigned int i ) {
	int ans = 0;
	for ( ;i; ans += tr[i], i &= ~L(i) ) ;
	return ans;
}

int sum( unsigned int i, unsigned int j ) {
	return prefix(j)-prefix(i-1);
}

int main() {
	int i,j,k;
	for ( ;(n = getnum()) < +oo; ) {
		for ( m=0, s.clear(), mp.clear(), i = 0; i < n; ++i ) {
			j = getnum(), k = getnum();
			s.insert(j), s.insert(k);
			x[i] = j, y[i] = k, id[i] = i;
		}
		for ( set<int> :: iterator it = s.begin(); it != s.end(); mp[*it++] = ++m ) ;
		for ( K = 0; (1<<K) <= m; ++K ) ;
		for ( i = 0; i < (1<<K); tr[i++] = 0 ) ;
		for ( i = 0; i < n; ++i )
			x[i] = mp[x[i]], y[i] = mp[y[i]], u[i] = enc((i64)x[i],(i64)y[i]);
		for ( sort(id,id+n,comp()), cnt.clear(), i = 0; i < n; ++i ) {
			k = id[i];
			if ( i ) {
				assert( x[id[i]] > x[id[i-1]] || x[id[i]] == x[id[i-1]] && y[id[i]] <= y[id[i-1]] );
			}
			assert( 0 <= k && k < n );
			if ( cnt.find(u[k]) == cnt.end() )
				cnt[u[k]] = 1;
			else j = cnt[u[k]], cnt[u[k]] = j+1;
			ans[k] = sum(y[k],(1<<K)-1)-cnt[u[k]]+1;
			update(y[k],1);
		}
		for ( k = 0; k < n-1; ++k )
			printf("%d ",ans[k]);
		if ( n ) printf("%d\n",ans[n-1]);
	}
	return 0;
}

