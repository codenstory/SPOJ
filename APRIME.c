/*
 * APRIME
 * TOPIC: backtracking
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Q 		   (510000ULL)
#define N 		    0x400
#define BIT(k) 	   (1ULL<<(k))
#define MASK(k)    (BIT(k)-1ULL)
#define CLR(a,k,W) (a[(k)>>W]&=~BIT((k)&MASK(W)))
#define SET(a,k,W) (a[(k)>>W]|=BIT((k)&MASK(W)))
#define TST(a,k,W) (a[(k)>>W]&BIT((k)&MASK(W)))
#define L(k) 	   ((k)&((~(k))+1ULL))
typedef unsigned long long u64;

unsigned char a[Q>>3],which[1<<21];
u64 u[(N>>6)+1],adj[N][(N>>6)+1];
int m,n,d,c[N],left,right;

int who( u64 u ) {
	assert( !(u&(u-1)) );
	if ( u >= BIT(60) ) return 60+which[u>>60];
	if ( u >= BIT(40) ) return 40+which[u>>40];
	if ( u >= BIT(20) ) return 20+which[u>>20];
	return which[u];
}

void sieve() {
	u64 i,j;
	for(SET(a,2,3),i=3;i<Q;SET(a,i,3),i+=2);
	for(i=3;i<Q;i+=2)for(j=i*i;TST(a,i,3)&&j<Q;CLR(a,j,3),j+=i);
}

int backtracking( int k ) {
	u64 v;
	int i,j,t,l,sum,ok,shift;
	/* Base case of backtracking; if solution found, output it and return Success */
	if ( k == m-n+1 ) {
		for ( i = 0; i < k-1; printf("%d,",c[i++]) ) ;
		return printf("%d\n",c[i]);
	}
	/* Try to expand the current solution vector "c" by putting into the next
	 * position "k" all the eligible candidates */
	for ( shift = 0, i = 0; i < (N>>6); ++i, shift += 64 )
		for ( v = k?u[i]&adj[c[k-1]][i]:u[i]; v && (t=shift+who(L(v))) >= 0; v &= ~L(v) ) { 
			for ( ok = 1, sum = t, l = 2, j = k-1; j >= 0 && l <= d && ok; ok &= !TST(a,sum+c[j],3), ++l, sum+=c[j--] ) ;
			if ( !ok ) continue ;
			CLR(u,t,6), c[k] = t; /* remove "i" from the candidate pool, c[k] = candidate */
			if ( backtracking(k+1) ) 
				return 1;
			SET(u,t,6); /* add "i" to the candidate pool */
		}
	/* if no solution found during the expansion, return False */
	return 0;
}

int main() {
	int i,j;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for ( sieve(), i=1; i<N; ++i )
		for ( j=1; j<N; ++j )
			if ( i!=j && !TST(a,i+j,3) )
				SET(adj[i],j,6);
	for ( ;3==scanf("%d %d %d",&n,&m,&d) && (n||m||d); ) {
		for ( memset(u,0,sizeof u), i = n; i <= m; SET(u,i,6), ++i ) ;
		left = (n>>6)-1<0?0:(n>>6)-1, right = (m>>6)+1>=(N>>6)+1?(N>>6):(m>>6)+1;
		if ( !backtracking(0) ) puts("No anti-prime sequence exists.");
	}
	return 0;
}

