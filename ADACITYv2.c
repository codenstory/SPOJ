/*
 * ADACITY
 * TOPIC: matching, floyd-warshall
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define xchg(x,y) (((x)==(y))||((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]),xchg(heap[i],heap[j]))
#define N (0x200)
#define Q (N<<1)
#define MAXE (1<<21)
#define oo (1LL<<60)
typedef long long i64;
i64 min( i64 x, i64 y ) { return x<y?x:y; }

i64 flow[MAXE],cap[MAXE],cost[MAXE],p[N],d[N],T,g[N][N],yes,seen[N];
int friends,cur[N],cnt,deg[N],adj[N][N],
	parent[N],n,m,mate[2][N],
	q[Q],head,tail;

int alt_path( int x ) {
	int i,y;
	if ( seen[x] == yes )
		return 0;
	for ( seen[x] = yes, i = 0; i < deg[x]; ++i )
		if ( mate[1][y = adj[x][i]] == -1 || alt_path(mate[1][y]) ) {
			mate[0][mate[1][y]=x]=y;
			return 1;
		}
	return 0;
}

int bpm() {
	int x,k,flag;
	for ( head = tail = 0, cnt = 0, x = 0; x < friends; ++x )
		q[tail++] = x, tail &= (Q-1), ++cnt;
	do {
		for ( flag = 0, k = cnt, ++yes; k--; ) {
			x = q[head++], head &= (Q-1), --cnt;
			if ( alt_path(x) ) flag = 1;
			else q[tail++] = x, tail &= (Q-1), ++cnt;
		}
	} while ( flag ) ;
	return friends-cnt;
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts-- && 4 == scanf("%d %d %d %lld",&n,&m,&friends,&T); ) {
		for ( i = 0; i < friends; scanf("%d",&cur[i++]), --cur[i-1] ) ;
		for ( i = 0; i < n; ++i )
			for ( j = i+1; j < N; ++j )
				g[i][j] = g[j][i] = +oo;
		for ( ;m-- && 3 == scanf("%d %d %d",&i,&j,&k); ) {
			if ( i == j ) continue ;
			--i, --j;
			if ( g[i][j] > k ) 
				g[i][j] = g[j][i] = k;
		}
		for ( k = 0; k < n; ++k )
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					if ( g[i][k] < +oo && g[k][j] < +oo && g[i][k]+g[k][j] <= T )
						if ( g[i][j] > g[i][k]+g[k][j] )
							g[i][j] = g[i][k]+g[k][j];
		for ( i = 0; i < friends; ++i )
			for ( deg[i] = 0, j = 0; j < n; ++j )
				if ( g[cur[i]][j] <= T )
					adj[i][deg[i]++] = j;
		for ( i = 0; i < friends; mate[0][i++] = -1 ) ;
		for ( i = 0; i < n; mate[1][i++] = -1 ) ;
		printf("%d\n",bpm());
	}
	return 0;
}

