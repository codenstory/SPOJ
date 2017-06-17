/*
 * ADAGAME: Ada and Game
 * TOPIC: dp
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (10000)
#define oo (1<<30)
#define MAXM 0x80
enum { A, V };

int m,n,z[2][N][MAXM],original,seen[2][N][MAXM],yes,to[N][4];
char D[N][5];

int calc_z( int t, int n, int moves_left ) {
	int i,j,k;
	if ( seen[t][n][moves_left] == yes )
		return z[t][n][moves_left];
	seen[t][n][moves_left] = yes;
	if ( moves_left == 0 )
		return z[t][n][moves_left] = original<n?A:V;
	assert( moves_left >= 1 );
	for ( i = 0; i < 4; ++i ) 
		if ( calc_z(t^1,to[n][i],moves_left-1) == t )
			return z[t][n][moves_left] = t;
	return z[t][n][moves_left] = t^1;
}

int main() {
	int i,j,k,ts,t;
	char tmp;
	for ( i = 0; i < N; sprintf(D[i],"%04d",i), ++i ) ;
	for ( i = 0; i < N; ++i ) 
		for ( j = 0; j < 4; ++j ) 
			k=(tmp=D[i][j])-'0',++k,k%=10,D[i][j]=k+'0',to[i][j]=atoi(D[i]),D[i][j]=tmp;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m) && ++yes; original = n, puts(calc_z(A,n,m)==A?"Ada":"Vinit") ) ;
	return 0;
}

