/*
 * ASSIGN4: Another Assignment Problem
 * TOPIC: kuhn-munkres
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Q (1<<10)
#define NEXT(k) (((k)+1)&(Q-1))
#define PREV(k) (((k)+Q-1)&(Q-1))
#define is_tight(x,y) (p[S][x]+p[T][y] == C[x][y])
#define N (1<<10)
enum { S, T };

int a[2][N],C[N][N],p[2][N],n[2],delta,seen[2][N],indeed,
	excess[2][N];

void floodfill( int x ) {
	int y;
	assert( excess[S][x] > 0 );
	for ( y = 0; y < n[T]; ++y )
		if ( is_tight(x,y) ) {
			assert( excess[T][y] == 0 ) ;
			for ( z = 0; z < n[S]; ++z )

		}
}

int main() {
	int i,j,k;
	return 0;
}

