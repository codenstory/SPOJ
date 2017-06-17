/*
 * BUD13TLF: Boxes
 * TOPIC: dp 
 * status:
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD 10007LL
#define N 0x80
#define MAXW 0x400
typedef long long i64;

int n,W,w[N];
i64 c[N][N],F[N],
	K[MAXW];

int main() {
	int i,j,k,cs = 0,ts;
	i64 ans;
	for ( i = 0; i < N; ++i )
		c[i][0] = 1LL;
	for ( i = 1; i < n; ++i )
		for ( j = 1; j <= i; ++j )
			c[i][j] = (c[i-1][j]+c[i-1][j-1])%MOD;
	for ( F[0] = 1, i = 1; i < N; ++i )
		F[i] = (F[i-1]*i)%MOD;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&W); printf("Case %d: %lld\n",++cs,ans) ) {
		for ( memset(K,0,sizeof K), i = 0; i < n; scanf("%d",&w[i]), ++K[w[i++]] ) ;
		for ( n = 1, k = 1; k < MAXW; ++k )
			if ( 
	}
	return 0;
}

