/*
 * LUCKYNUM
 * TOPIC: ad-hoc, greedy
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define W (10000)
#define Q (W<<1)
#define enc(x,t) ((t)|((x)<<1))
#define LIMIT 201
#define oo (1<<30)
enum { SIX, EIGHT };

int n,yes,*head,*tail,q[Q],
	next[2][W],six[W],parent[Q],eight[W],
	rem[2][LIMIT],deg[LIMIT];

int main() {
	int i,j,k,ts,x,y,ok;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n) && ++yes; ) {
		if ( !(6%n) ) {
			puts("6");
			continue ;
		}
		if ( !(8%n) ) {
			puts("8");
			continue ;
		}
		for ( x = 0; x < W; ++x ) {
			next[SIX][x] = ((10*x+6)%n);
			next[EIGHT][x] = ((10*x+8)%n);
		}
		for ( deg[0] = 1, rem[SIX][0] = 0, rem[EIGHT][0] = 0, k = 1; k < LIMIT; ++k ) {
			rem[SIX][k] = next[SIX][rem[SIX][k-1]];
			rem[EIGHT][k] = next[EIGHT][rem[EIGHT][k-1]];
			deg[k] = (deg[k-1]*10)%n;
		}
		/*
		for ( i = 0; i < W; eight[i] = +oo, six[i++] = +oo ) ;
		for ( head=tail=q, six[*tail++=0]=0; head < tail; ) {
			y = next[SIX][x = *head++];
			if ( six[x] >= LIMIT ) continue ;
			if ( six[y] > six[x]+1 )
				six[*tail++=y] = six[x]+1;
		}
		for ( head=tail=q, eight[*tail++=0]=0; head < tail; ) {
			y = next[EIGHT][x = *head++];
			if ( eight[x] >= LIMIT ) continue ;
			if ( eight[y] > eight[x]+1 )
				eight[*tail++=y] = eight[x]+1;
		}*/
		for ( ok = 0, k = 2; k < LIMIT && !ok; ++k ) 
			for ( i = 0; i <= k && !ok; ++i ) 
				if ( (deg[k-i]*rem[EIGHT][i]+rem[SIX][k-i]) % n == 0 )
					x = i, y = k-i, ok = 1;
		if ( !ok ) puts("-1");
		else {
			for ( i = 0; i < x; ++i, putchar('8') ) ;
			for ( i = 0; i < y; ++i, putchar('6') ) ;
			putchar('\n');
		}
	}
	return 0;
}

