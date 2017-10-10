/*
 * CNTTREE
 * TOPIC: DP, combinatorics
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define L(k) ((k)&((~(k))+1ULL))
#define N 61
#define MAXDIAM N
#define MAXLEN N
#define BIT(k) (1ULL<<(k))
#define oo 0xffffffffffffffffull
typedef unsigned long long u64;

u64 adj[N],lst[N],
	z[N][MAXDIAM][MAXLEN];
int m,n,parent[N],q[N],*head,*tail,d[N],seen[N],yes;
char which[1<<21];

int who( u64 u ) {
	if ( u >= BIT(60) )
		return 60+which[u>>60];
	if ( u >= BIT(40) )
		return 40+which[u>>40];
	if ( u >= BIT(20) )
		return 20+which[u>>20];
	return which[u];
}

u64 calc_z( int x, int k, int l ) {
	u64 ax,bx,u,v,as,bs;
	int y,yy,t;
	if ( z[x][k][l] < +oo )
		return z[x][k][l];
	if ( !lst[x] ) 
		return z[x][k][l] = (l==0?1ULL:0ULL);
	if ( l == 0 )
		return z[x][k][l] = 1ULL;
	assert( l >= 1 );
	if ( l > k )
		return z[x][k][l] = 0ULL;

	if ( 2*l <= k ) {
		for ( ax = bx = 1ULL, u = lst[x]; u && (y = who(L(u))) >= 0; u &= ~L(u) ) {
			for ( as = bs = 1, t = 0; t <= l-2; ++t )
				as += calc_z(y,k,t), bs += calc_z(y,k,t);
			as += calc_z(y,k,l-1);
			ax *= as, bx *= bs;
		}
		z[x][k][l] = ax-bx;
	}
	else {
		for ( z[x][k][l] = 0, u = lst[x]; u && (y = who(L(u))) >= 0; z[x][k][l] += calc_z(y,k,l-1)*ax,  u &= ~L(u) ) 
			for ( ax = 1ULL, v = lst[x]&~L(u); v && (yy = who(L(v))) >= 0; ax *= as, v &= ~L(v) ) 
				for ( as = 1, t = 0; t <= k-l-1; ++t )
					as += calc_z(yy,k,t);
	}

	return z[x][k][l];

}

int main() {
	int i,j,k,l,ts,x,y;
	u64 ans,u,v;
	for ( i = 0; i < 21; which[1<<i] = i, ++i ) ;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&m); ) {
		for ( i = 0; i < n; ++i )
			for ( k = 0; k <= n; ++k )
				for ( l = 0; l <= n; ++l )
					z[i][k][l] = +oo;
		for ( i = 0; i < n; lst[i] = 0, adj[i++] = 0 ) ;
		for(k=0;k<n-1&&2==scanf("%d %d",&i,&j);adj[i]|=BIT(j),adj[j]|=BIT(i),++k);
		for ( head=tail=q, d[*tail++=0] = 0, seen[0] = ++yes; head < tail; )
			for ( u=adj[x=*head++]; u && (y=who(L(u)))>=0; u&=~L(u) )
				if ( seen[y] != yes )
					seen[*tail++=y]=yes, d[y]=d[parent[y]=x]+1, lst[x]|=BIT(y);
				else {
					assert( parent[x] == y );
				}
		for ( ans = 0, x = 0; x < n; ++x )
			for ( l = 0; l <= m; ++l )
				ans += calc_z(x,m,l);
		printf("%llu\n",ans);
	}
	return 0;
}

