/*
 * AIRLINES: Jumbo Airlines
 * TOPIC: bitmasks, scc, dp
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXM (15)
#define MAXN (1000000001)
#define MOD 420047LL
#define BIT(k) (1UL<<(k))
#define MASK(k) (BIT(k)-1ULL)
typedef long long i64;

int n,m,K,SZ;
char bts[1<<21],good[1<<21];
i64 z[2][1<<15][0x40],ans,A[1<<10][1<<10],ax[1<<10][1<<10],
	res[1<<10],vec[1<<10],tmp[1<<10][1<<10];
unsigned int idx[5][1<<4][1<<4],len[5][1<<4],g[5][1<<4],L[5];

int first_type( int m, int n, int K ) {
	return 1 <= m && m <= 15 && 1 <= n && n <= 50 && 3 <= K && K <= 50;
}

int second_type( int m, int n, int K ) {
	int res = 1 <= m && m <= 4 && 1 <= n && n <= 1000000000 && 1 <= K && K <= 5;
	return res;
}

int main() {
	int i,j,k,t,l,deg;
	unsigned int u,v,base;
	for ( u = 0; u < BIT(21); ++u ) {
		bts[u] = bts[u>>1]+(u&1);
		for ( good[u] = 1, i = 0; i+1 < 21 && good[u]; ++i )
			if ( ((u>>i)&1) && ((u>>(i+1))&1) )
				good[u] = 0;
		if ( good[u] ) 
			for ( m = 1; m <= 4; ++m )
				if ( u < (1<<m) )
					g[m][L[m]++] = u;
	}
	for ( m = 1; m <= 4; ++m )
		for ( i = 0; i < L[m]; ++i )
			for ( j = 0; j < L[m]; ++j )
				if ( !(g[m][i]&g[m][j]) )
					idx[m][i][len[m][i]++] = j;
	for ( ;3 == scanf("%d %d %d",&m,&n,&K); printf("%lld\n",ans) ) {
		if ( m > n ) 
			k = m, m = n, n = k;
		assert( m <= n );
		if ( first_type(m,n,K) ) {
			for ( memset(z,0,sizeof z), t = 0, u = 0; u < (1<<m); ++u )
				if ( bts[u] <= K && good[u] ) 
					z[t][u][bts[u]] = 1LL;
			for ( i = 2; i <= n; ++i ) 
				for ( t^=1, memset(z[t],0,sizeof z[t]), k = 0; k <= K; ++k )
					for ( u = 0; u < (1<<m); ++u )
						if ( bts[u] <= k && good[u] ) {
							for ( base = (~u)&MASK(m), v = base; v; v = ((v-1)&base) )
								if ( bts[v] <= k-bts[u] && good[v] ) {
									assert( !(u&v) );
									z[t][u][k] += z[t^1][v][k-bts[u]], z[t][u][k] %= MOD;
								}
							z[t][u][k] += z[t^1][0][k-bts[u]], z[t][u][k] %= MOD;
						}
			for ( ans = 0, u = 0; u < (1<<m); ans += z[t][u++][K], ans %= MOD ) ;
		}
		else {
			assert( second_type(m,n,K) );
			memset(ax,0,sizeof ax), memset(A,0,sizeof A);
			for ( SZ = L[m]*(K+1), i = 0; i < L[m]; ++i )
				for ( u = g[m][i], k = 0; k <= K; ++k )
					if ( bts[u] <= k )
						for ( l = 0; l < len[m][i]; ++l )
							if ( bts[v=g[m][j = idx[m][i][l]]] <= k-bts[u] ) 
								A[k*L[m]+i][(k-bts[u])*L[m]+j] = 1LL;
			for ( i = 0; i < SZ; ++i )
				for ( j = 0; j < SZ; ++j )
					ax[i][j] = (i==j?1LL:0LL);
			for ( i = 0; i < L[m]; ++i )
				for ( k = 0; k <= K; ++k )
					vec[k*L[m]+i] = bts[g[m][i]]==k?1:0;
			for ( deg = n-1; deg; deg >>= 1 ) {
				if ( deg&1 ) {
					for ( i = 0; i < SZ; ++i )
						for ( j = 0; j < SZ; ++j )
							for ( tmp[i][j] = 0, k = 0; k < SZ; ++k )
								tmp[i][j] += ax[i][k]*A[k][j], tmp[i][j] %= MOD;
					for ( i = 0; i < SZ; ++i )
						for ( j = 0; j < SZ; ++j )
							ax[i][j] = tmp[i][j];
				}
				for ( i = 0; i < SZ; ++i )
					for ( j = 0; j < SZ; ++j )
						for ( tmp[i][j] = 0, k = 0; k < SZ; ++k )
							tmp[i][j] += A[i][k]*A[k][j], tmp[i][j] %= MOD;
				for ( i = 0; i < SZ; ++i )
					for ( j = 0; j < SZ; ++j )
						A[i][j] = tmp[i][j];
			}
			for ( k = 0; k <= K; ++k )
				for ( i = 0; i < L[m]; ++i )
					for ( res[t = k*L[m]+i] = 0, j = 0; j < SZ; ++j )
						res[t] += ax[t][j]*vec[j], res[t] %= MOD;
			for ( ans = 0, i = 0; i < L[m]; ++i )
				ans += res[K*L[m]+i], ans %= MOD;
		}
	}
	return 0;
}

