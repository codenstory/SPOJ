/*
 * TRVCOST
 * TOPIC: Floyd-Warshall
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x200
typedef long long i64;

int n,is[N][N],yes,path[N][N];
i64 d[N][N];

int main() {
	int i,j,k,qr,l,src;
	for ( ;1 == scanf("%d",&n) && ++yes; ) {
		for ( l = 0; l < n && 3 == scanf("%d %d %d",&i,&j,&k); ++l )
			is[i][j] = is[j][i] = yes, d[i][j] = d[j][i] = k, path[i][j] = path[j][i] = yes;
		for ( i = 0; i < N; ++i )
			is[i][i] = yes, path[i][i] = yes;
		for ( k = 0; k < N; ++k )
			for ( i = 0; i < N; ++i )
				for ( j = 0; j < N && path[i][k] == yes; ++j )
					if ( path[k][j] == yes && (path[i][j] != yes || d[i][j] > d[i][k]+d[k][j]) )
						d[i][j] = d[i][k]+d[k][j], path[i][j] = yes;
		scanf("%d",&src);
		for ( scanf("%d",&qr); qr-- && 1 == scanf("%d",&i); ) 
			if ( path[src][i] == yes )
				printf("%lld\n",d[src][i]);
			else puts("NO PATH");
	}
	return 0;
}

