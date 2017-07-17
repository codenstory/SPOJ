/*
 * WATER: Water among cubes
 * TOPIC: bfs, hard, greedy, iterative
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BIT(k) (1ull<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define N BIT(SH)
#define SH 7
#define enc(x,y) ((x)|((y)<<SH))
#define Q (BIT(SH+SH))
#define oo 0xfffffffful
#define vc(x,y) (0<=(x) && (x)<m && 0<=(y) && (y)<n)
typedef long long i64;

unsigned int d[Q],q[Q],*head,*tail,
			 h[N][N];
int m,n,seen[Q],yes,bad[Q],indeed,mark,height[Q],pool_id[Q],
	dx[] = {-1,0,1,0},
	dy[] = {0,1,0,-1};

void mark_bad_cells() {
	int i,j,k,x,y,nx,ny;
	unsigned int u,v;
	for ( ++indeed, ++yes, head=tail=q, i = 0; i < m; ++i )
		for ( j = 0; j < n; ++j )
			if ( 0 == i || i == m-1 || 0 == j || j == n-1 ) 
				seen[*tail++=u=enc(i,j)]=yes, bad[u]=indeed;
	for ( ;head < tail; )
		for ( u = *head++, x=(u&MASK(SH)), y=(u>>SH), i = 0; i < 4; ++i ) {
			nx = x+dx[i], ny = y+dy[i];
			if ( vc(nx,ny) && h[nx][ny] >= h[x][y] && seen[v=enc(nx,ny)] != yes )
				seen[*tail++=v] = yes, bad[v] = indeed;
		}
}

int find_lowest_bad_cell_in_circumference( unsigned int src ) {
	int i,j,k,x,y,nx,ny,ans = (1<<30);
	unsigned int u,v;
	for ( head=tail=q, seen[*tail++=src]=yes; head<tail; )
		for ( pool_id[u=*head++]=mark, x=(u&MASK(SH)), y=(u>>SH), i = 0; i < 4; ++i )
			if ( vc(nx=x+dx[i],ny=y+dy[i]) ) {
				if ( bad[v = enc(nx,ny)] == indeed ) {
					if ( h[nx][ny] < ans )
						ans = h[nx][ny];
				}
				else {
					if ( h[nx][ny] <= h[x][y] && seen[v] != yes )
						seen[*tail++ = v] = yes;
				}
			}
	assert( ans < (1<<30) );
	return ans;
}

int suitable_start( int x, int y ) {
	int i,nx,ny;
	for ( i = 0; i < 4; ++i )
		if ( vc(nx = x+dx[i],ny = y+dy[i]) )
			if ( bad[enc(nx,ny)] != indeed )
				return 1;
	return 0;
}

unsigned int get_neighbor( int x, int y ) {
	int i,nx,ny;
	for ( i = 0; i < 4; ++i )
		if ( vc(nx = x+dx[i],ny = y+dy[i]) )
			if ( bad[enc(nx,ny)] != indeed )
				return enc(nx,ny);
	return +oo;
}

void floodfill( unsigned int src, int H ) {
	int x,y,nx,ny,i,j,k;
	unsigned int u,v;
	for ( head=tail=q, seen[*tail++=src] = yes; head < tail; )
		for ( pool_id[u=*head++]=mark, x=(u&MASK(SH)),y=(u>>SH), i=0; i < 4; ++i )
			if ( vc(nx=x+dx[i],ny=y+dy[i]) && seen[v = enc(nx,ny)] != yes && h[nx][ny] <= H )
				seen[*tail++ = v] = yes;
}

int main() {
	int i,j,k,ts,flag,best_height;
	i64 ans;
	unsigned int src;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&m,&n); ) {
		for ( ans = 0, i = 0; i < m; ++i )
			for ( j = 0; j < n; scanf("%u",&h[i][j++]) ) ;
		do {
			mark_bad_cells(), flag = 0;
			for ( best_height = (1<<30), src = +oo, mark = -1, i = 0; i < m; ++i )
				for ( j = 0; j < n; ++j )
					if ( bad[enc(i,j)] == indeed && suitable_start(i,j) && best_height > h[i][j] ) 
						/*++mark, height[mark] = find_lowest_bad_cell_in_circumference(enc(i,j));*/
						best_height = h[i][j], src = enc(i,j);
			if ( src < +oo )
				++mark, height[mark] = h[src&MASK(SH)][src>>SH], floodfill(get_neighbor(src&MASK(SH),src>>SH),height[mark]);
			if ( mark >= 0 ) flag = 1;
			for ( i = 0; i < m; ++i )
				for ( j = 0; j < n; ++j )
					if ( bad[enc(i,j)] != indeed && seen[enc(i,j)] == yes ) {
						assert( pool_id[enc(i,j)] <= mark );
						if ( height[pool_id[enc(i,j)]] > h[i][j] )
							ans += height[pool_id[enc(i,j)]]-h[i][j], h[i][j] = height[pool_id[enc(i,j)]];
					}
		} while ( flag ) ;
		printf("%lld\n",ans);
	}
	return 0;
}

