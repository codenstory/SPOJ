/*
 * VONNY
 * TOPIC: backtracking
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 7
#define N 8
#define L(k) ((k)&((~(k))+1ULL))
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(K)-1ULL)
#define vc(x,y) (0<=(x) && (x)<M && 0<=(y) && (y)<N)
typedef unsigned long long u64;
enum { WHITE, BLACK, NONE };

int n,c[M][N],color[M][N],q[M*N],*head,*tail,ans,
	stone[M][N],id[2][M][N],m[2],rid[2][M*N];
u64 adj[M*N];
char which[1<<21];

int who( unsigned int u ) {
	return u>=BIT(20)?20+which[u>>20]:which[u];
}

int min( int x, int y ) { return x<y ? x:y; }
int max( int x, int y ) { return x>y ? x:y; }

void rec( unsigned int covered, int x ) {
	unsigned int u,y,a,b;
	if ( x == m[WHITE] ) { ++ans; return ; }
	assert( x < m[WHITE] );
	for ( u = adj[x]&~covered; u && (y = who(L(u))) >= 0; u &= ~L(u) ) {
		a = min(c[rid[WHITE][x]&127][rid[WHITE][x]>>7],c[rid[BLACK][y]&127][rid[BLACK][y]>>7]);
		b = max(c[rid[WHITE][x]&127][rid[WHITE][x]>>7],c[rid[BLACK][y]&127][rid[BLACK][y]>>7]);
		if ( stone[a][b] ) 
			--stone[a][b], rec(covered|BIT(y),x+1),	++stone[a][b];
	}
}

int main() {
	int i,j,k,ts,t,x,y,nx,ny,u,v;
	for ( i = 0; i < 21; which[BIT(i)] = i, ++i ) ;
	for ( k = 0, i = 0; i < M; ++i )
		for ( j = i; j < M; ++j )
			stone[i][j] = 1;
	for ( scanf("%d",&ts); ts--; ) {
		for ( i = 0; i < M; ++i )
			for ( j = 0; j < N; ++j )
				scanf("%d",&c[i][j]), color[i][j] = NONE;
		memset(adj,0,sizeof adj), m[WHITE] = m[BLACK] = 0;
		for ( k = 0, head = tail = q, color[x=0][y=0] = WHITE, rid[WHITE][*tail++=(x|(y<<7))] = m[WHITE]++; head < tail; )
			for ( ++k, u = *head++, x=(u&127), y=(u>>7), t=color[x][y], nx=x-1; nx <= x+1; ++nx )
				for ( ny=y-1; ny <= y+1; ++ny )
					if ( vc(nx,ny) && abs(nx-x)+abs(ny-y) == 1 ) {
						if ( color[nx][ny] == NONE )
							color[nx][ny] = (t^1), *tail++=rid[t^1][id[t^1][nx][ny]=m[t^1]++]=(nx|(ny<<7));
						u = id[t][x][y], v = id[t^1][nx][ny];
						if ( t == WHITE )
							adj[u] |= BIT(v);
						else adj[v] |= BIT(u);
					}
		assert( k == M*N );
		ans = 0, rec(0,0);
		printf("%d\n",ans);
	}
	return 0;
}

