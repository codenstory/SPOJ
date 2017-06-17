/*
 * MOBIVINA
 * TOPIC: network flow, similar to COCONUTS
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x200
#define MAXV (N)
#define MAXE ((N+N+N*N)*2)
#define oo (1<<30)
#define min(x,y) ((x)<(y)?(x):(y))

int n,mobi[N],vina[N],c[N][N],seen[MAXV],indeed,visited[MAXV],yes,d[MAXV],q[MAXE],*head,*tail,
	last[MAXV],next[MAXE],to[MAXE],cap[MAXE],flow[MAXE],E,V,src,sink,isdig[256];

int getnum() {
	int ch,n = 0,sign = 1;
	for ( ;(ch = getchar()) != EOF && !isdig[ch] && ch != '-'; ) ;
	if ( ch == EOF )
		return EOF;
	if ( ch == '-' ) {
		sign = -1;
		ch = getchar();
	}
	for ( n=ch-'0'; (ch=getchar()) != EOF && isdig[ch]; n=10*n+(ch-'0') ) ;
	return sign*n;
}

int bfs() {
	int x,y,i;
	for ( head=tail=q, d[*tail++=sink] = 0, visited[sink] = ++yes; head < tail; )
		for ( i = last[x = *head++]; i >= 0; i = next[i] )
			if ( cap[i^1] > flow[i^1] && visited[y = to[i]] != yes ) 
				visited[*tail++ = y] = yes, d[y] = d[x]+1;
	return visited[src] == yes;
}

int dfs( int x, int df ) {
	int i,y,dt;
	if ( x == sink ) 
		return df;
	if ( seen[x] == indeed )
		return 0;
	for ( seen[x] = indeed, i = last[x]; i >= 0; i = next[i] )
		if ( visited[y = to[i]] == yes && cap[i] > flow[i] && seen[y] != indeed )
			if ( d[y]+1 == d[x] && (dt = dfs(y,min(df,cap[i]-flow[i]))) > 0 ) {
				flow[i] += dt, flow[i^1] -= dt;
				return dt;
			}
	return 0;
}

int p[MAXV];

int augment() {
	int i,j,k,x,y,df = +oo,dt;

	for ( head=tail=q, visited[*tail++=src]=++yes, d[src]=0; head<tail && visited[sink] != yes; )
		for ( i=last[x=*head++]; i>=0; i=next[i] ) 
			if ( cap[i] > flow[i] && visited[y=to[i]] != yes )
				visited[*tail++=y] = yes, d[y] = d[x]+1, p[y] = i;
	if ( visited[sink] != yes )
		return 0;
	for ( i=p[sink],x=to[i^1],df=cap[i]-flow[i]; x != src; i=p[x],x=to[i^1],df=min(df,cap[i]-flow[i]) );
	assert(df > 0);
	for ( i=p[sink],x=to[i^1],flow[i]+=df,flow[i^1]-=df; x != src; i=p[x],x=to[i^1],flow[i]+=df,flow[i^1]-=df );
	dt = df;
	for ( i=p[sink],x=to[i^1],df=cap[i]-flow[i]; x != src; i=p[x],x=to[i^1],df=min(df,cap[i]-flow[i]) );
	assert(df == 0);
	return dt;
}

int maxflow() {
	int T = 0, dt;
	for ( ;bfs(); )
		for ( ;++indeed && (dt = dfs(src,+oo)) > 0; T += dt ) ;
	/*
	for ( ;(dt = augment()); T += dt ) ;
	*/
	return T;
}

void add_arcs( int x, int y, int c ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i, cap[i] = c;
	to[j] = x, next[j] = last[y], last[y] = j, cap[j] = 0;
	flow[i] = flow[j] = 0;
}

int main() {
	int i,j,k,ii,jj;
	for ( i = '0'; i <= '9'; isdig[i++] = 1 ) ;
	for ( ;1 == scanf("%d",&n); ) {
		/*for ( i = 0; i < n; mobi[i++] = getnum() );
		for ( j = 0; j < n; vina[j++] = getnum() );
		*/
		for ( i = 0; i < n; ++i )
			scanf("%d",&mobi[i]);
		for ( i = 0; i < n; ++i )
			scanf("%d",&vina[i]);
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				scanf("%d",&c[i][j]);
		for ( E = 0, V = n+2, src = V-2, sink = V-1, i = 0; i < V; last[i++] = -1 ) ;
		for ( i = 0; i < n; ++i )
			for ( add_arcs(src,i,vina[i]), add_arcs(i,sink,mobi[i]), j = i+1; j < n; ++j ) {
				ii = E++, jj = E++;
				to[ii] = j, next[ii] = last[i], last[i] = ii, cap[ii] = c[i][j];
				to[jj] = i, next[jj] = last[j], last[j] = jj, cap[jj] = c[j][i];
				flow[ii] = flow[jj] = 0;
					/*add_arcs(i,j,c[i][j]);
					add_arcs(j,i,c[j][i]);*/
			}
		printf("%d\n",maxflow());
		break ;
	}	
	return 0;
}

