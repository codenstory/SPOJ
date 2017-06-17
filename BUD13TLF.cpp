/*
 * BUD13TLF: Boxes
 * TOPIC: dp, binomial coefficients, combinatorics
 * status: Accepted
 */
#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#define MOD 10007LL
#define N 0x80
#define MAXW 0x400
#define weight first
#define counts second
typedef long long i64;
using namespace std;

int n;
i64 c[N][N],
	K[MAXW],
	pref[N],P[N];
pair<int,int> r[N];
i64 a[N][N][MAXW];

i64 multinomial( int *k, int n, int sum ) {
	if ( n <= 1 ) 
		return 1LL;
	return (c[sum][*k]*multinomial(k+1,n-1,sum-*k))%MOD;
}

int main() {
	int i,j,k,l,cs = 0,ts,m,head[N];
	i64 ans,total,W,maxw,t;
	for ( i = 0; i < N; c[i++][0] = 1LL ) ;
	for ( i = 1; i < N; ++i )
		for ( j = 1; j <= i; ++j )
			c[i][j] = (c[i-1][j]+c[i-1][j-1])%MOD;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %lld",&n,&W); printf("Case %d: %lld\n",++cs,ans) ) {
		for ( memset(K,0,sizeof K), i = 0; i < n; scanf("%d",&k), ++K[k], ++i ) ;
		for ( n = 0, k = 1; k < MAXW; ++k )
			if ( K[k] )
				r[++n] = make_pair(k,K[k]);
		for ( pref[0] = P[0] = 0, i = 1; i <= n; ++i ) 
			pref[i] = pref[i-1]+r[i].counts, P[i] = P[i-1]+r[i].counts*r[i].weight;
		memset(a,0,sizeof a);
		for ( i = 1, j = 0; i < n; head[j++] = r[i++].counts ) ;
		for ( a[0][n+1][0] = 1LL, ans = 0, i = n; i >= 1; --i ) {
			if ( (maxw=W-P[i-1]) < 0 ) 
				goto next;
			for ( k = 0; k <= pref[n]-pref[i-1]-1; ++k )
				for ( t = k; t <= maxw; ++t ) 
					for ( l = 0; l <= k && l <= r[i].counts-1 && l*r[i].weight <= t; ++l )
						a[k][i][t] += c[k][l]*a[k-l][i+1][t-l*r[i].weight], a[k][i][t] %= MOD;
			for ( t = max(maxw-r[i].weight+1,0LL); t <= maxw; ++t )
				for ( k = 0; k <= pref[n]-pref[i-1]-1; ++k ) {
					int tmp = head[i];
					head[i] = k;
					ans += multinomial(head,i,pref[i-1]+k)*a[k][i][t], ans %= MOD;
				}
			for ( t = 0; t <= maxw; ++t ) 
				for ( k = 0; k <= pref[n]-pref[i-1]; ++k )
					a[k][i][t] = 0;
			next:
			for ( maxw = W, k = 0; k <= pref[n]-pref[i-1]; ++k )
				for ( t = k; t <= maxw; ++t ) 
					for ( l = 0; l <= k && l <= r[i].counts && l*r[i].weight <= t; ++l )
						a[k][i][t] += c[k][l]*a[k-l][i+1][t-l*r[i].weight], a[k][i][t] %= MOD;
		}
		if ( P[n] <= W ) {
			int coeffs[N];
			for ( j = 0, i = 1; i <= n; coeffs[j++] = r[i++].counts ) ;
			ans += multinomial(coeffs,n,pref[n]), ans %= MOD;
		}
	}
	return 0;
}

