/*
 * NPC2014F
 * TOPIC: sliding window
 * status: Accepted
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define tol (1e-13)
#define N (1<<19)
#define A 26
using namespace std;

char s[N];
int n,m,x[N],y[N],c[A+1],id[A+1],req[A+1];

int main() {
	int i,j,k,t,best;
	char tmp[0x20];
	for ( ;2 == scanf("%d %s",&n,s+1); ) {
		if ( n == 0 ) break ;
		for ( i = 1; i <= n; s[i++]-='a' ) ;
		scanf("%d",&m), memset(id,0,sizeof id);
		memset(req,0,sizeof req);
		for ( i = 1; i <= m; ++i ) {
			scanf("%d %s",x+i,tmp);
			assert( 'a' <= 0[tmp] && 0[tmp] <= 'z' );
			y[i] = 0[tmp]-'a';
			if ( !id[y[i]] ) 
		   		id[y[i]] = i;
			req[y[i]] = x[i];
		}
		if ( !m ) {
			puts("0");
			continue ;
		}
		for ( m = 0, i = 0; i < A; ++i )
			if ( id[i] )
				x[++m] = req[i], id[i] = m;
		memset(c,0,sizeof c);
		k = (id[s[i=1]]&&++c[s[i]]==x[id[s[i]]])?1:0;
		for ( best = n+n+0x400, j = i+1; j <= n && k < m; ++j )
			if ( id[s[j]] && ++c[s[j]] == x[id[s[j]]] && ++k ) ;
		if ( k < m ) {
bad:
			puts("Andy rapopo");
			continue ;
		}
		do {
			if ( (j-i) < best ) best = (j-i);
			for ( ;!(id[s[i]] && --c[s[i]] < x[id[s[i]]]); ++i ) ;
			if ( (j-i) < best ) best = (j-i);
			assert( c[s[i]] == x[id[s[i]]]-1 );
			assert( i < j );
			for ( --k, ++i; j <= n && k < m; ++j )
				if ( id[s[j]] && ++c[s[j]] == x[id[s[j]]] && ++k ) ;
			if ( k < m ) break ;
		} while ( 1 );
		if ( best == n+n+0x400 ) goto bad;
		else printf("%d\n",best);
		break ;
	}
	return 0;
}

