/*
 * CUBEFR: Cube Free Numbers
 * TOPIC: number theory, sieve
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<21)
#define BIT(k) (1ULL<<(k))
#define TST(a,k) (a[(k)>>3] & BIT((k)&7))
#define CLR(a,k) (a[(k)>>3] &= ~BIT((k)&7))
#define SET(a,k) (a[(k)>>3] |= BIT((k)&7))

int n,*p,m,id[N];
unsigned char a[N>>3],b[N>>3];

void sieve() {
	int i,j,k;
	for ( i = 1; i < N; SET(b,i), ++i ) ;
	for ( SET(a,2), i = 3; i < N; SET(a,i), i += 2 ) ; 
	for ( i = 2; i < N; ++i )
		if ( TST(a,i) ) 
			for ( j = i+i, k = 2; j < N; CLR(a,j), j += i, ++k )
				if ( !(k%i) && !((k/i)%i) )
					CLR(b,j);
	for ( i = 1; i < N; ++i )
		if ( TST(b,i) ) ++m;
	for ( k = 0, p = (int *)malloc(m*sizeof *p), i = 1; i < N; ++i )
		if ( TST(b,i) )
			p[k++] = i, id[i] = k-1;
}

int main() {
	int i,j,k,ts,cs = 0;
	for ( sieve(), scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		printf("Case %d: ",++cs);
		if ( !TST(b,n) ) 
			puts("Not Cube Free");
		else {
			printf("%d\n",id[n]+1);
		}
	}
	return 0;
}

