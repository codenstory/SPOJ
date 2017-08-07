/*
 * MSKYCODE: Sky code
 * TOPIC: inclusion-exclusion, bitmasks, number-theory, square-free numbers
 * status: Accepted
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <set>
#include <map>
#define tol (1e-13)
#define N 10100
#define MAXP 1240
#define SZ 7
#define L(k) ((k)&((~(k))+1UL))
typedef long long i64;
using namespace std;

int n,m,maxw,cnt[MAXP];
bool isprime[N],table[N][MAXP],mu[N];
vector<int> p,primes;
set<int> s,v[N];
map<int,int> mp;

void sieve() {
	int i,j,k;
	for ( i = 1; i < N; mu[i++] = true ) ;
	for ( isprime[2] = true, i = 3; i < N; isprime[i] = true, i += 2 ) ;
	for ( i = 3; i < N; i += 2 )
		for ( j=i+i,k=2; isprime[i] && j<N; isprime[j]=false, mu[j]=!(k%i)?false:mu[j],j+=i,++k );
	for ( k = 0, i = 2; i < N; ++i )
		if ( mu[i] ) ++k;
	for ( p.clear(), i = 0; i < N; ++i )
		if ( isprime[i] )
			p.push_back(i);
}

i64 c[N],_count[N];
vector<int> vec[N];
char which[1<<6];
bool parity[N];

i64 C( i64 m ) {
	if ( m <= 3 )
		return 0LL;
	return ((m*(m-1))/2)*((m-2)*(m-3)/2)/6;
}

int main() {
	int i,j,k,t,cs = 0;
	size_t maxsize;
	i64 num,ans,nn;
	for ( k = 0; k < 6; ++k )
		which[1<<k] = k;
	for ( sieve(); 1 == scanf("%d",&m) && m; ) {
		memset(cnt,0,sizeof cnt);
		for ( maxw=0,s.clear(), i = 0; i < m; ++i ) {
			scanf("%lld",&num), nn = num;
			for ( vec[i].clear(), t = 1, j = 0; j < (int)p.size() && p[j]*p[j] <= num; ++j ) {
				for ( k = 0; 0 == (num%p[j]); ++k, num /= p[j] );
				if ( k )
					s.insert(p[j]),t*=p[j],vec[i].push_back(p[j]);
			}
			if ( num > 1 ) 
				s.insert(num),t*=num,vec[i].push_back(num);
			if ( maxw < t ) maxw = t;
			c[i] = t;
			assert( mu[t] );
		}
		primes.clear();
		for ( set<int >::iterator it = s.begin(); it != s.end(); primes.push_back(*it++) ) ;
		assert( s.size() == primes.size() ) ;
		memset(_count,0,sizeof _count);
		for ( i = 0; i < m; ++i ) {
			for ( unsigned int v,u = 0; u < (1<<vec[i].size()); ++u ) {
				bool flag = false;
				for ( k = 1, v = u; v && (j=which[L(v)]) >= 0; v &= ~L(v) )
					k *= vec[i][j], flag = !flag;
				++_count[k], parity[k] = flag;
			}
			//assert( int2cell._count((int)c[i]) ) ;
			//++int2cell[(int)c[i]]->w;
		}
		/*
		for ( i = 0; i < (int)leaves.size(); q.push(leaves[i++]) ) ;
		for (;!q.empty();) { 
			cell *x = q.front(), *y; q.pop();
			if ( y = x->parent ) {
				y->w += x->w;
				if ( ++y->cnt == y->sons.size() ) 
					q.push(y);
			}
		}*/
		for ( ans = 0, i = 0; i <= maxw; ++i ) {
			//printf("Quadruples divisible by %d: %lld, sign = %d\n",rm[x],C(x->w),x->parity?-1:1);
			ans += (parity[i]?-1:1)*C(_count[i]);
		}
		printf("%lld\n",ans);
		/*for ( set<int>::iterator it = s.begin(); it != s.end(); ++it )  
			if ( cnt[*it] >= 4 )
				k = mp.size(), mp[*it] = k;
		int hist[6]; memset(hist,0,sizeof hist);*/
		/*for ( maxsize = 0, t = 0, i = 0; i < m; ++i ) {
			for ( v[i].clear(), k = 0; k < (int)vec[i].size(); ++k )
				if ( mp._count(vec[i][k]) )
					v[i].insert(mp[vec[i][k]]);
			if ( v[i].size() ) {
				maxsize = max(maxsize,v[i].size()), ++t;
				++hist[v[i].size()];
			}
		}
		printf("%d %d %d\n",t,maxsize,mp.size());
		*/
	}
	return 0;
}

