/*
 * MUDDY
 * TOPIC: bipartite matching, minimum vertex cover, very neat
 * status: Accepted
 */
#include <iostream>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RR 0x40
#define MAXV (1<<21)
#define Q (MAXV)
#define MAXE (1<<21)
#define enc(x,y) ((x)|((y)<<6))
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define oo (1<<30)
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)
#define S (HOR)
#define T (VER)
#define NONE (-1)
enum { HOR, VER };
using namespace std;

char g[RR][RR];
int m,n,lst[2][MAXE],len[2],
	mate[2][MAXV],
	q[MAXV],head,tail,cnt,
	seen[MAXV],yes;
vector<int> adj[MAXV];

int neighbors( int x, int y ) {
	int nx,ny,k = 0;
	for ( nx = x-1; nx <= x+1; ++nx )
		for ( ny = y-1; ny <= y+1; ++ny )
			if ( abs(nx-x)+abs(ny-y) == 1 && vc(nx,ny) && g[nx][ny] == '*' )
				++k;
	return k;
}

int intersect( unsigned int u, unsigned int v ) {
	int a = (u&63), b = ((u>>6)&63), c = ((u>>12)&63), d = (u>>18);
	int x = (v&63), y = ((v>>6)&63), z = ((v>>12)&63), t = (v>>18);
	return !(c < x || z < a || t < b || d < y);
}

int length( unsigned int u ) {
	int a = (u&63), b = ((u>>6)&63), c = ((u>>12)&63), d = (u>>18);
	return abs(a-c)+abs(b-d)+1;
}

int alt_path( int x ) {
	int i,y;
	if ( seen[x] == yes )
		return 0;
	for ( seen[x] = yes, i = 0; i < (int)adj[x].size(); ++i ) 
		if ( mate[T][y = adj[x][i]-len[S]] == NONE || alt_path(mate[T][y]) ) {
			mate[T][mate[S][x] = y] = x;
			return 1;
		}
	return 0;
}

int bpm() {
	int x,flag,k;
	for ( head = tail = cnt = 0, x = 0; x < len[S]; ++x )
		q[tail++] = x, tail &= (Q-1), ++cnt;
	for ( k = S; k <= T; ++k )
		for ( x = 0; x < len[k]; mate[k][x++] = NONE ) ;
	do {
		for ( ++yes, flag = 0, k = cnt; k--; ) {
			x = q[head++], head &= (Q-1), --cnt;
			assert( mate[S][x] == NONE );
			if ( ++yes && alt_path(x) ) flag = 1;
			else q[tail++] = x, tail &= (Q-1), ++cnt;
		}
	} while ( flag );
	return len[S]-cnt;
}

int main() {
	int i,j,k,ts,ans;
	for ( scanf("%d",&ts); ts--; ) {
		for ( scanf("%d %d",&m,&n), i = 0; i < m; scanf("%s",g[i++]) ) ;
		memset(len,0,sizeof len);
		for ( ans = 0, i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				if ( g[i][j] == '*' && neighbors(i,j) == 0 ) 
					++ans, g[i][j] = '.';
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j ) {
				if ( g[i][j] == '.' ) continue ;
				if ( j == 0 || g[i][j-1] == '.' ) {
					for ( k = j+1; k < n && g[i][k] == '*'; ++k ) ;
					--k, lst[HOR][len[HOR]++] = (enc(i,j)|(enc(i,k)<<12));
				}
				if ( i == 0 || g[i-1][j] == '.' ) {
					for ( k = i+1; k < m && g[k][j] == '*'; ++k ) ;
					--k, lst[VER][len[VER]++] = (enc(i,j)|(enc(k,j)<<12));
				}
			}
		for ( i = 0; i < len[S]; adj[i++].clear() ) ;
		for ( i = 0; i < len[HOR]; ++i ) 
			for ( j = 0; j < len[VER]; ++j )
				if ( intersect(lst[HOR][i],lst[VER][j]) ) 
					adj[i].push_back(j+len[HOR]);
		/*printf("%d %d %d %d\n",len[S],len[T],bpm(),ans);
		 */
		printf("%d\n",bpm()+ans);
	}
	return 0;
}

