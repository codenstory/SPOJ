/*
 * KATHTHI
 * TOPIC: 0/1 BFS
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (0x400)
#define vc(x,y) (0<=(x) && (x)<m && 0<=(y) && (y)<n)
#define Q (1<<22)
#define enc(x,y) ((x)|((y)<<10))

int m,n,ts,cnt,seen[1<<20],yes,
	dx[] = {-1,0,1,0},
	dy[] = {0,1,0,-1};
unsigned int q[Q],head,tail,d[1<<20];
char g[N][N];

void add_last( unsigned int u ) {
	q[tail] = u, tail = (tail+1)&(Q-1), ++cnt;
}
void add_first( unsigned int u ) {
	head = (head+Q-1)&(Q-1), q[head] = u, ++cnt;
}
unsigned int pop() {
	unsigned int u = q[head++];
	head &= (Q-1), --cnt;
	return u;
}

unsigned int 
bfs( unsigned int src, unsigned int dst ) {
	unsigned int u = -1,v;
	int x,y,nx,ny,i,j,k;
	for ( cnt = 0, head = tail = 0, seen[src] = ++yes, d[src] = 0, add_last(src); cnt && u != dst; )
		for ( u = pop(), x = (u&1023), y = (u>>10), i = 0; i < 4; ++i )
			if ( vc(nx = x+dx[i],ny = y+dy[i]) ) {
				k = (g[x][y]==g[nx][ny]?0:1);
				if ( seen[v = enc(nx,ny)] != yes || d[v] > d[u]+k ) {
					seen[v] = yes, d[v] = d[u]+k;
					if ( k ) add_last(v);
					else add_first(v);
				}
			}
	return d[dst];
}

int main() {
	int i,j,k;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&m,&n); ) {
		for ( i = 0; i < m; scanf("%s",g[i++]) ) ;
		printf("%u\n",bfs(enc(0,0),enc(m-1,n-1)));
	}
	return 0;
}

