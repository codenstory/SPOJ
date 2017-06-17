/*
 * NUMQDW: Number of Quite Different Words
 * TOPIC: matrix exponentiation, bitmasks, dp
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define MOD 4242LL
#define Q BIT(10)
#define INITIAL_STATE (0)
#define L(k) ((k)&((~(k))+1UL))
#define MAXA 7
typedef long long i64;
typedef unsigned int state;

void my_assert( int x ) {
	assert( x );
}

int m,g[MAXA][Q][Q],yes,SZ,A,SH;
i64 n,x[Q][Q],ax[Q][Q],tmp[Q][Q],ans;
char word[1<<14],which[1<<21];

int main() {
	int i,j,k,ts,a,b,c;
	state u,v,w,nu,uu,vv,base;
	for ( i = 0; i < 21; which[BIT(i)] = i, ++i ) ;
	for ( scanf("%d",&ts); ts-- && 3 == scanf("%lld %d %s",&n,&A,word); printf("%lld\n",ans) ) {
		assert( 1 <= A && A <= 6 );
		assert( n >= 0 );
		memset(x,0,sizeof x);
		for ( a = 1; a <= A; ++a )
			x[INITIAL_STATE][BIT(a)] = 1;
		for ( SZ = BIT(A+1), v = 1; v < BIT(A); ++v ) 
			for ( u=(v<<1), c = 1; c <= A; ++c )
				++x[u][u|BIT(c)];
		for ( ++yes, u = BIT(word[i=0]-'A'+1); word[++i]; u |= BIT(c) ) 
			for ( c = word[i]-'A'+1, base = ((MASK(A)<<1)&~u), vv = base; ; --vv, vv &= base ) {
				for ( uu = u; uu; --uu, uu &= u )
					if ( g[c][uu|vv][uu|vv|BIT(c)] != yes )
						--x[uu|vv][uu|vv|BIT(c)], g[c][uu|vv][uu|vv|BIT(c)] = yes;
				if ( vv == 0 ) break ;
			}
		/*
		puts("Transitions:");
		*/
		assert( SZ <= Q );
		for ( i = 0; i < SZ; ++i )
			for ( j = 0; j < SZ; ++j )
				ax[i][j] = (i==j?1:0);
		for ( ;n; n >>= 1 ) {
			if ( n&1 ) {
				for ( i = 0; i < SZ; ++i )
					for ( j = 0; j < SZ; ++j )
						for ( tmp[i][j] = 0, k = 0; k < SZ; ++k )
							tmp[i][j] += ax[i][k]*x[k][j], tmp[i][j] %= MOD;
				for ( i = 0; i < SZ; ++i )
					for ( j = 0; j < SZ; ++j )
						ax[i][j] = tmp[i][j];
			}
			for ( i = 0; i < SZ; ++i )
				for ( j = 0; j < SZ; ++j )
					for ( tmp[i][j] = 0, k = 0; k < SZ; ++k )
						tmp[i][j] += x[i][k]*x[k][j], tmp[i][j] %= MOD;
			for ( i = 0; i < SZ; ++i )
				for ( j = 0; j < SZ; ++j )
					x[i][j] = tmp[i][j];
		}
		for ( ans = 0, i = 0; i < SZ; ans += ax[INITIAL_STATE][i++], ans %= MOD ) ;
	}
	return 0;
}

