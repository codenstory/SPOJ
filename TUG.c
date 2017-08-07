/*
 * TUG
 * TOPIC: dp, maths, pigeonhole principle, neat
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (12000100)
#define BIT(k) (1ULL<<(k))
#define L(k) ((k)&((~(k))+1ULL))
#define END(t) (a[t][sum>>6]&BIT(sum&63))
#define MAXWEIGHT 100
#define MAXW MAXWEIGHT
typedef unsigned long long u64;

int n,c[N/100],seen[MAXW+1],yes;
u64 a[2][N>>6],b[2][((N>>6)+64)>>6],z[2][((((N>>6)+64)>>6)+64)>>6];
char which[1<<21];

int who( u64 u ) {
	if ( u >= BIT(60) ) return 60+which[u>>60];
	if ( u >= BIT(40) ) return 40+which[u>>40];
	if ( u >= BIT(20) ) return 20+which[u>>20];
	return which[u];
}

int A( int x ) {
	if ( x < N )
		return (a[0][x>>6]&BIT(x&63)) || (a[1][x>>6]&BIT(x&63));
	return 0;
}

int main() {
	int i,j,k,t,total,ts,l,x,shift,sum,ok,block,s,e,zblock,tz,zs,maxx;
	u64 u,v,w;
	for ( i = 0; i < 21; ++i ) which[1<<i] = i;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( sum = 0, i = 1; i <= n; scanf("%d",&c[i]), sum += c[i], maxx=(c[i]<maxx?c[i]:maxx), ++i ) ;
		if ( n <= MAXWEIGHT ) {
			for ( ok = 0, ++yes, i = 1; i <= n && !ok; ++i )
				if ( seen[c[i]] == yes ) ok = 1;
				else seen[c[i]] = yes;
			if ( ok ) {
				puts("YES");
				continue ;
			}
			memset(a,0,sizeof a), memset(b,0,sizeof b), memset(z,0,sizeof z), maxx = 0;
			for ( ok = 0, total = 0, t = 0, a[t][0]|=BIT(0), b[t][0]|=BIT(0), z[t][0]|=BIT(0), i = 1; i <= n && !ok; ++i, total += k )
				for ( k = c[i], t ^= 1, zs = 0, zblock = 0; zblock <= (((total>>6)>>6)>>6)+2 && !ok; ++zblock, zs += 64 )
					for ( w = z[t^1][zblock]; w && (tz = who(L(w))) >= 0 && !ok; w &= ~L(w) )
						for ( block = (zblock<<6)+tz, v = b[t^1][block]; v && (e=who(L(v))) >= 0 && !ok; v &= ~L(v) )
							for ( shift = (block<<6)+e, u = a[t^1][shift]; u && (j = who(L(u))) >= 0 && !ok; u &= ~L(u) ) {
								x = (shift<<6)+j+k, 0&&printf("marking %d\n",x), a[t][x>>6]|=BIT(x&63), b[t][(x>>6)>>6]|=BIT((x>>6)&63), z[t][((x>>6)>>6)>>6]|=BIT(((x>>6)>>6)&63);
								if ( x > 0 && (a[t^1][x>>6]&BIT(x&63)) ) ok = 1;
								x = (shift<<6)+j, 0&&printf("marking %d\n",x), a[t][x>>6]|=BIT(x&63), b[t][(x>>6)>>6]|=BIT((x>>6)&63), z[t][((x>>6)>>6)>>6]|=BIT(((x>>6)>>6)&63);
							}
			puts(ok?"YES":"NO");
		}
		else {
			puts("YES");
		}
	}
	return 0;
}

