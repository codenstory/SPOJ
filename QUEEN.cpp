/*
 * QUEEN
 * TOPIC: bfs with optimizations
 */
#include <algorithm>
#include <queue>
#include <deque>
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SH 10
#define N BIT(SH)
#define BIT(k) (1<<(k))
#define MASK(k) (BIT(k)-1)
#define enc(x,y,t) (e(x,y)|((t)<<(SH+SH)))
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)
#define oo 0xfffffffful
#define Q BIT(SH+SH+3)
#define QS BIT(SH+SH+4)
#define NEXT(x) (((x)+1)&(QS-1))
#define PREV(x) (((x)+QS-1)&(QS-1))
#define ADD_TO_FRONT(x) (head=PREV(head),++cnt,q[head]=(x))
#define ADD_TO_REAR(x)  (q[tail]=(x),++cnt,tail=NEXT(tail))
#define e(x,y) ((x)|((y)<<SH))
using namespace std;

char g[N][N];
int seen[Q],yes,m,n,cnt,
	dir[8][8],len[8],
	dx[] = {-1,-1,0,1,1,1,0,-1},
	dy[] = {0,1,1,1,0,-1,-1,-1};
unsigned int d[Q],ans,dist[N][N];
deque<unsigned int> q;

int main() {
	int i,j,k,t,ts,x,y,nx,ny;
	unsigned int src,sink,u,v;
	for ( t = 0; t < 8; ++t ) 
		for ( len[t] = 0, i = 0; i < 8; ++i )
			/*if ( i != t && i != ((t+4)&7) )*/
			if ( i != t )
				dir[t][len[t]++] = i;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&m,&n); ) {
		for ( i = 0; i < m; scanf("%s",g[i++]) ) ;
		for ( src = sink = +oo, i = 0; i < m && (src==+oo || sink==+oo); ++i )
			for ( j = 0; j < n && (src==+oo || sink==+oo); ++j )
				if ( g[i][j] == 'S' )
					src = e(i,j);
				else if ( g[i][j] == 'F' )
					sink = e(i,j);
		assert( src < +oo && sink < +oo );
		if ( src == sink ) {
			puts("0");
			continue ;
		}
		for ( x = 0; x < n; ++x )
			for ( y = 0; y < n; ++y )
				dist[x][y] = +oo;
		while ( !q.empty() ) q.pop_front();
		for ( ans = +oo, ++yes, x=(src&MASK(SH)),y=(src>>SH), dist[x][y]=0, t = 0; t < 8; ++t ) {
			d[u=enc(x,y,t)] = 0, seen[u] = yes;
			nx=x+dx[t], ny=y+dy[t];
			assert( nx != x || ny != y );
			if ( vc(nx,ny) && g[nx][ny] != 'X' )
				d[u=enc(nx,ny,t)]=1, seen[u]=yes, q.push_back(u);
		}
		while ( !q.empty() ) {
			u = q.front(), q.pop_front(), x=(u&MASK(SH)),y=((u>>SH)&MASK(SH));
			if ( e(x,y) == sink ) {
				if ( ans > d[u] )
					ans = d[u];
				continue ;
			}
			if ( d[u] >= ans )
				continue ;
			t=(u>>(SH+SH)), nx=x+dx[t],ny=y+dy[t];
			if ( vc(nx,ny) && g[nx][ny] != 'X' && (seen[v=enc(nx,ny,t)] != yes || d[v]>d[u]) )
				seen[v] = yes, d[v]=d[u], q.push_front(v);
			if ( dist[x][y] <= d[u] ) continue ;
			dist[x][y] = d[u];
			for ( i = 0; i < len[t]; ++i ) {
				j = dir[t][i], nx=x+dx[j], ny=y+dy[j];
				assert( nx != x || ny != y );
				assert( j != t );
				/*assert( j != ((t+4)&7) );*/
				if ( vc(nx,ny) && g[nx][ny] != 'X' && (seen[v=enc(nx,ny,j)] != yes || d[v]>d[u]+1) )
					seen[v] = yes, d[v]=d[u]+1, q.push_back(v);
			}
		}
		if ( ans < +oo )
			printf("%u\n",ans);
		else puts("-1");
	}
	return 0;
}

