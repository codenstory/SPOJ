/*
 * LUCKYNUM
 * TOPIC: bfs
 * status: TLE
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define LIM 201
#define N (1<<17)
#define enc(t,u) ((t)|((u)<<1))
#define oo 0xfffffffful

unsigned int d[N],q[N],*head,*tail,parent[N];
int n,seen[N],yes,precalc[LIM];
char move[N],out[22*LIM],*qtr = out;

void dump( unsigned int u ) {
	char o[LIM],*ptr = o;
	int i,j,k;
	while ( d[u] >= 2 ) {
		*ptr++ = move[u]+'0';
		u = parent[u];
	}
	*ptr++ = move[u]+'0';
	for ( ;--ptr >= o; *qtr++ = *ptr ) ;
	*qtr++ = '\n';
}

int main() {
	int i,j,k,t,ts;
	unsigned int u,v,w,nw;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n) && ++yes; ) {
		if ( 0==(6%n) ) {
			*qtr++ = '6', *qtr++ = '\n';
			continue ;
		}
		if ( 0==(8%n) ) {
			*qtr++ = '8', *qtr++ = '\n';
			continue ;
		}
		for ( precalc[0] = 0, i = 1; i < LIM; ++i )
			precalc[i] = (precalc[i-1]*10+8)%n;
		d[enc(0,0)] = d[enc(1,0)] = +oo, head = tail = q;
		/*
		for (d[*tail++=u=enc(1,(6%n))]=1,move[u]=6,seen[u]=yes;head<tail;) {
			if ( d[u] == LIM-1 ) continue ;
			t=((u=*head++)&1), w = (u>>1);
			v = enc(1,(10*w+6)%n);
			if ( seen[v] != yes || d[v] > d[u]+1 )
				d[*tail++ = v] = d[u]+1, move[v] = 6, parent[v] = u, seen[v] = yes;
		}
		for (d[*tail++=u=enc(0,(8%n))]=1,move[u]=8,seen[u]=yes;head<tail;) {
			if ( d[u] == LIM-1 ) continue ;
			t=((u=*head++)&1), w = (u>>1);
			v = enc(1,(10*w+8)%n);
			if ( seen[v] != yes || d[v] > d[u]+1 )
				d[*tail++ = v] = d[u]+1, move[v]=8, parent[v] = u, seen[v] = yes;
		}*/
		for(d[*tail++=u=enc(0,(8%n))]=1,move[u]=8,seen[u]=yes,d[*tail++=u=enc(1,(6%n))]=1,move[u]=6,seen[u]=yes;head<tail;){
			if ( d[u] == LIM-1 ) continue ;
			t=((u=*head++)&1), w = (u>>1);
			if ( !t ) {
				v = enc(1,(10*w+6)%n);
				if ( seen[v] != yes || d[v] > d[u]+1 )
					d[*tail++ = v] = d[u]+1, move[v] = 6, parent[v] = u, seen[v] = yes;
				v = enc(0,nw=precalc[d[u]+1]);
				if ( seen[v] != yes || d[v] > d[u]+1 )
					if ( !(seen[enc(1,nw)]==yes && d[enc(1,nw)] <= d[u]+1) )
						d[*tail++ = v] = d[u]+1, move[v] = 8, parent[v] = u, seen[v] = yes;
			}
			else {
				v = enc(1,(10*w+6)%n);
				if ( seen[v] != yes || d[v] > d[u]+1 )
					d[*tail++ = v] = d[u]+1, move[v] = 6, parent[v] = u, seen[v] = yes;
			}
		}
		if ( seen[enc(0,0)] != yes && seen[enc(1,0)] != yes )
			*qtr++ = '-', *qtr++ = '1', *qtr++ = '\n';
		else {
			if ( d[u = enc(0,0)] < d[v = enc(1,0)] )
				dump(u);
			else
				dump(v);
		}
	}
	*--qtr = '\0', puts(out);
	return 0;
}

