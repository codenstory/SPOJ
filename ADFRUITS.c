/*
 * ADFRUITS
 * TOPIC: dp, interview-level, lcs, greedy
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x80
#define SH 7

int m,n,c[N][N],best[N][N],mx[N],my[N],yes;
char x[N],y[N],z[N],lcs[N],*ptr;

void dump( int i, int j ) {
	int pi = (best[i][j]&127), pj = (best[i][j]>>SH);
	if ( i == 0 || j == 0 ) return ;
	dump(pi,pj);
	if ( i >= 1 && j >= 1 && pi == i-1 && pj == j-1 )
		*ptr++ = x[i];
}

int main() {
	int i,j,k;
	for ( ;2 == scanf("%s %s",x+1,y+1) && ++yes; ) {
		m = strlen(x+1), n = strlen(y+1);
		for ( i = 0; i <= m; ++i )
			for ( j = 0; j <= n; best[i][j] = 0, c[i][j++] = 0 ) ;
		for ( i = 1; i <= m; ++i )
			for ( j = 1; j <= n; ++j ) {
				if ( x[i] == y[j] ) 
					c[i][j] = 1+c[i-1][j-1], best[i][j] = (i-1)|((j-1)<<SH);
				if ( c[i][j-1] >= c[i][j] )
					c[i][j] = c[i][j-1], best[i][j] = i|((j-1)<<SH);
				if ( c[i-1][j] > c[i][j] )
					c[i][j] = c[i-1][j], best[i][j] = (i-1)|(j<<SH);
			}
		ptr = lcs, dump(m,n), *ptr = '\0';
		assert( ptr-lcs == c[m][n] );
		for ( i = 1, k = 0; k < c[m][n]; ++k ) {
			for ( j = i; j <= n && y[j] != lcs[k]; ++j ) ;
			assert( j <= n ) ;
			my[j] = yes, i = j+1;
		}
		for ( i = 1, k = 0; k < c[m][n]; ++k ) {
			for ( j = i; j <= m && x[j] != lcs[k]; ++j ) ;
			assert( j <= m ) ;
			mx[j] = yes, i = j+1;
		}
		for ( ptr = z, i = 1, j = 1; i <= m && j <= n; ) {
			for ( ;i <= m && mx[i] != yes; *ptr++ = x[i++] ) ;
			for ( ;j <= n && my[j] != yes; *ptr++ = y[j++] ) ;
			if ( i <= m && j <= n ) 
				*ptr++ = x[i], ++i, ++j;
		}
		for ( ;i <= m; *ptr++ = x[i++] ) ;
		for ( ;j <= n; *ptr++ = y[j++] ) ;
		*ptr = '\0', puts(z);
	}
	return 0;
}

