/*
 * ELEVTRBL
 * TOPIC: bfs
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<20)
typedef long long i64;

i64 f,s,g,u,d;
int seen[N],yes,q[N],*head,*tail,hh,tt,dist;

i64 gcd( i64 x, i64 y ) { return !y?x:gcd(y,x%y); }

int main() {
	int i,j,k,x,y;
	for ( ;5 == scanf("%lld %lld %lld %lld %lld",&f,&s,&g,&u,&d); ) {
		if ( s == g ) {
			puts("0");
			continue ;
		}
		if ( d == 0 && u == 0 ) {
			puts("use the stairs");
			continue ;
		}
		if ( (g-s>0?g-s:s-g)%gcd(d,u) ) {
			puts("use the stairs");
			continue ;
		}
		for ( head=tail=q,seen[*tail++=s]=++yes,hh=0,tt=tail-head,dist=0; head < tail; ) {
			x = *head++;
			if ( hh++ == tt )
				hh = head-q, tt = tail-q, ++dist;
			if ( x == g ) break ;
			if ( (y=x-d) >= 1 && seen[y] != yes )
				seen[*tail++=y] = yes;
			if ( (y=x+u) <= f && seen[y] != yes )
				seen[*tail++=y] = yes;
		}
		if ( x != g ) 
			puts("use the stairs");
		else printf("%d\n",dist);
	}
	return 0;
}

