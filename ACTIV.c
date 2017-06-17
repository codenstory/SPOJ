/*
 * ACTIV: Activities
 * TOPIC: binary search, dp
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD (100000000LL)
#define N (1<<17)
typedef long long i64;

int n;
i64 z[N];
typedef struct cell {
	int x,y;
} cell;
cell c[N];

int cmp( const void *a, const void *b ) {
	cell *p = (cell *)a,
		 *q = (cell *)b;
	if ( p->y == q->y ) {
		if ( p->x == q->x ) return 0;
		return p->x < q->x ? -1:1;
	}
	return p->y < q->y?-1:1;
}

int main() {
	int i,j,k,low,high,mid;
	for ( ;1 == scanf("%d",&n) && n != -1; ) {
		for ( i = 1; i <= n; scanf("%d %d",&c[i].x,&c[i].y), ++i );
		qsort(c+1,n,sizeof *c,cmp);
		for ( z[1] = 1, i = 2; i <= n; ++i ) {
			if ( c[i-1].y <= c[i].x )
				z[i] = (((z[i-1]<<1)%MOD)+1)%MOD;
			else {
				assert( c[i-1].y > c[i].x );
				if ( c[1].y > c[i].x )
					z[i] = ((z[i-1]+1)%MOD);
				else {
					assert( c[1].y <= c[i].x );
					assert( c[i-1].y > c[i].x );
					for ( low = 1, high = i-1; low+1 < high; c[mid=(low+high)/2].y<=c[i].x?(low=mid):(high=mid) );
					z[i] = (z[low]+z[i-1]+1)%MOD;
				}
			}
		}
		printf("%08lld\n",z[n]);
	}
	return 0;
}

