/*
 */
#include <bits/stdc++.h>
#define BIT(k) (1ULL<<(k))
#define N 0x40
using namespace std;
typedef unsigned long long u64;

u64 m;
char g[N][N];

int main() {
	int i,j,k,cs= 0,ts,n;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %llu",&n,&m); ) {
		printf("Case #%d: ",++cs);
		if ( m > BIT(n-2) ) {
			puts("IMPOSSIBLE");
			continue ;
		}
		puts("POSSIBLE"), memset(g,'0',sizeof g), g[0][n-1]= '1', --m;
		for ( i = 0; i < n; g[i++][n]= '\0' ) ;
		assert( n >= 2 );
		for ( i= 1; i <= n-2; ++i ) 
			if ( m & BIT(n-i-2) ) {
				for ( g[0][i]= '1', j= i; j <= n-1; ++j )
					for ( k= j+1; k <= n-1; ++k )
						g[j][k]= '1';
			}
		for ( i = 0; i < n; puts(g[i++]) ) ;
	}
	return 0;
}

