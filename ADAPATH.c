/*
 * ADAPATH: Ada and Path
 * TOPIC: maxflow, matching
 * status:
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x80
#define MAXV (N*N*2)
#define MAXE (1<<21)
#define vc(x,y) (0<=(x)&&(x)<n&&0<=(y)&&(y)<n)
#define enc(x,y) ((x)*n+(y))
#define min(x,y) ((x)<(y)?(x):(y))
#define   LEFT_IN(i,j)    (enc(i,j))
#define   LEFT_OUT(i,j)   (enc(i,j)+n*n)
#define   RIGHT_IN(i,j)   (2*n*n+enc(i,j))
#define   RIGHT_OUT(i,j)  (3*n*n+enc(i,j))
#define oo (1<<30)

int dig[256],m,n,g[N][N],
	to[MAXE],last[MAXE],cap[MAXE],flow[MAXE],next[MAXE],
	d[MAXV],seen[MAXV],visited[MAXV],yes,indeed,
	q[MAXV],*head,*tail,
	src,sink,V,E;

int getnum() {
	int n = 0,ch;
	for (;(ch = getchar()) != EOF && !('0'<=ch && ch<='9'); );
	for ( n = ch-'0'; (ch = getchar()) != EOF && '0' <= ch && ch <= '9'; n = 10*n+ch-'0' );
	return n;
}

void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c, flow[i] = 0;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0, flow[j] = 0;
}

int bfs() {
	int x,y,i;
	for ( head=tail=q,d[*tail++=sink]=0, visited[sink]=++yes; head<tail;)
		for ( i=last[x=*head++]; i>=0; i=next[i] )
			if ( cap[i^1]>flow[i^1] && visited[y=to[i]]!=yes )
				visited[*tail++=y]=yes, d[y]=d[x]+1;
	return visited[src]==yes;
}

int dfs( int x, int df ) {
	int i,y,dt;
	if ( x == sink )
		return df;
	if ( seen[x] == indeed )
		return 0;
	for ( seen[x] = indeed, i = last[x]; i >= 0; i = next[i] )
		if ( cap[i] > flow[i] && visited[y=to[i]] == yes && d[y]+1 == d[x] )
			if ( dt=dfs(y,min(df,cap[i]-flow[i])) ) {
				flow[i]+=dt,flow[i^1]-=dt;
				return dt;
			}
	return 0;
}

int maxflow() {
	int T = 0,dt;
	for(;bfs();)
		for(;++indeed&&(dt=dfs(src,+oo));T+=dt);
	return T;
}

int main() {
	int i,j,k,ts,ni,nj,ones,
		di[] = {-1,0,1,0},
		dj[] = {0,1,0,-1};
	for ( k = '0'; k <= '9'; dig[k++] = 1 ) ;
	for ( ts = getnum(); ts--; ) {
		for ( n = getnum(), i = 0; i < n; ++i )
			for ( j = 0; j < n; g[i][j++] = getnum() );
		for ( E = 0, V = 2+2*n*n+2*n*n, src = V-2, sink = V-1, i = 0; i < V; last[i++] = -1 ) ;
		for ( ones = 0, i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j ) {
				add_arcs(src,LEFT_IN(i,j),1), add_arcs(RIGHT_OUT(i,j),sink,1);
				if ( g[i][j] == 1 ) 
					++ones;
				for ( k = 0; k < 4; ++k ) {
					ni = i+di[k], nj = j+dj[k];
					if ( vc(ni,nj) && g[ni][nj] == 1+g[i][j] ) 
						add_arcs(LEFT_OUT(i,j),RIGHT_IN(ni,nj),+oo);
				}
			}
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				add_arcs(LEFT_IN(i,j),LEFT_OUT(i,j),1), add_arcs(RIGHT_IN(i,j),RIGHT_OUT(i,j),1);
		k = maxflow();
		/*printf("k = %d\n",k);*/
		puts(n*n-k == ones?"YES":"NO");
	}
	return 0;
}

