/*
 * INCDSEQ
 * TOPIC: dp, BIT
 * status: Accepted
 */
#include <iostream>
#include <set>
#include <map>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXL (15)
#define N (1<<MAXL)
#define K (0x40)
#define MOD (5000000LL)
#define oo (1LL<<60)
#define L(k) ((k)&((~(k))+1ULL))
using namespace std;
typedef long long i64;

int n,m,c[N],U,LOG;
i64 z[K][N];
set<int> s;
map<int,int> mp;

void update( const int k, unsigned int i, i64 dv ) {
	for (;i<(1<<LOG); z[k][i]+=dv,z[k][i]%=MOD,i+=L(i) ) ;
}

i64 f( const int k, unsigned int i ) {
	i64 s = 0;
	for (;i; s += z[k][i], s %= MOD, i &= ~L(i) ) ;
	return s;
}

i64 sum( const int k, unsigned int i, unsigned int j ) {
	if ( i > j ) return 0;
	return (f(k,j)-f(k,i-1)+MOD)%MOD;
}

int main() {
	int i,j,k,val;
	i64 ans,dv;
	for ( ;2 == scanf("%d %d",&n,&m) && (m||n); ) {
		for ( mp.clear(), U = 0, s.clear(), i = 1; i <= n; scanf("%d",&c[i++]), s.insert(c[i-1]) ) ;
		for ( set<int> :: iterator it=s.begin(); it!=s.end(); mp[*it++]=++U ) ;
		for ( LOG=0; (1<<LOG)<=U; ++LOG ) ;
		for ( i=1; i<=n; ) {
			for ( val=mp[c[i++]], k=1; k<=m && k<=i-1; ++k ) {
				dv = sum(k,val,val);
				update(k,val,MOD-dv);
				if ( k == 1 ) {
					update(k,val,1);
					continue ;
				}
				i64 dv=f(k-1,val-1);
				if ( !dv ) continue ;
				update(k,val,dv);
			}
		}
		for ( ans=0,i=1; i<=U; ans+=sum(m,i,i), ans%=MOD, ++i ) ;
		printf("%lld\n",ans);
		break ;
	}
	return 0;
}

