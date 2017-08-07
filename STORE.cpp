/*
 * STORE
 * TOPIC: Dijkstra, bfs, technical
 * status: Accepted
 */
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#define SH (7)
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)
#define is_free(x,y) (vc(x,y) && g[x][y]!='S')
#define NIE (puts("NO"))
#define N BIT(SH)
#define Q (BIT(SH+SH+4))
#define oo (0xfffffffful)
#define e(x,y) ((x)|((y)<<SH))
typedef unsigned int state;
using namespace std;
typedef unsigned long long u64;
#define xchg(x,y) (((x)==(y)) || ((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]),xchg(heap[i],heap[j]))

state enc( int x, int y, int t ) { return x|(y<<SH)|((t&3)<<(SH+SH)); }

int m,n,seen[N],yes,pos[Q],cnt,inq[Q],aha,
	dx[] = {-1,0,1,0},
	dy[] = {0,-1,0,1};
char g[N][N];
unsigned int d[Q];
state heap[Q<<1];

void heap_push( state x ) {
	int i,j;
	if ( inq[x] != aha )
		inq[x] = aha, pos[heap[cnt]=x] = cnt, ++cnt;
	else {
		assert( pos[x] != -1 );
	}
	for ( j=pos[x]; j && d[heap[i=(j-1)>>1]] > d[heap[j]]; bubble, j = i ) ;
}

state pop() {
	state x = *heap;
	int i,j;
	if ( cnt += (pos[x]=-1) )
		pos[*heap=heap[cnt]] = 0;
	for ( inq[x] = 0, j = 0; (i=j,j<<=1,++j) < cnt; bubble ) {
		if ( j < cnt-1 && d[heap[j]] > d[heap[j+1]] ) ++j;
		if ( d[heap[i]] <= d[heap[j]] ) break ;
	}
	return x;
}

void find( char ch, int *x, int *y ) {
	for ( int i = 0; i < m; ++i )
		for ( int j = 0; j < n; ++j )
			if ( g[i][j] == ch ) { *x = i, *y = j; return ; }
}

class Graph {
	private:
			unsigned int dist[BIT(SH+SH)];
			map<u64,unsigned int> s;
			unsigned int q[BIT(SH+SH)],*head,*tail;
			int visited[BIT(SH+SH)],indeed;
	public:
			Graph() { s.clear(),indeed=0,memset(visited,0,sizeof visited); };
			void reset() { s.clear(),indeed=0,memset(visited,0,sizeof visited); }
			unsigned int 
			bfs( const unsigned int &parcel, const unsigned int &src, const unsigned int &dst ) {
				int x,y,nx,ny,i,j,k;
				u64 u,v;
				if ( s.count(u=(((u64)parcel)|(((u64)src)<<(SH+SH))|(((u64)dst)<<(SH+SH+SH+SH)))) )
					return s[u];
				for ( dist[dst]=+oo, head=tail=q,visited[*tail++=src]=++indeed,dist[src]=0; head<tail && visited[dst]!=indeed; assert(e(x,y) != parcel) )
					for ( u=*head++,x=(u&MASK(SH)),y=(u>>SH),i=0; i<4; ++i )
						if ( is_free(nx=x+dx[i],ny=y+dy[i]) && (v=e(nx,ny))!=parcel )
							if ( visited[v]!=indeed )
								visited[*tail++=v]=indeed, dist[v]=dist[u]+1;
				u=((u64)parcel) | (((u64)src)<<(SH+SH)) | (((u64)dst)<<(SH+SH+SH+SH));
				v=((u64)parcel) | (((u64)dst)<<(SH+SH)) | (((u64)src)<<(SH+SH+SH+SH));
				return s[u]=s[v]=dist[dst];
			}
} G;

unsigned int bfs( state src, state dst ) {
	int i,x,y,nx,ny,t;
	state u,v;
	unsigned int w, best = +oo;
	for ( cnt=0,++aha,d[src]=0,heap_push(src),seen[src]=++yes; cnt; ) {
		u = pop(), t = (u>>(SH+SH)), x=(u&MASK(SH)), y=((u>>SH)&MASK(SH));
		assert( 0 <= t && t <= 3 );
		if ( x==(dst&MASK(SH)) && y==(dst>>SH) ) {
			best = min(best,d[u]);
			continue ;
		}
		if ( best <= d[u] ) continue ;
		if ( is_free(nx=x+dx[(t+2)&3],ny=y+dy[(t+2)&3]) )
			if ( seen[v=enc(nx,ny,t)] != yes || d[v] > d[u]+1 )
				seen[v] = yes, d[v]=d[u]+1, heap_push(v);
		assert( is_free(x+dx[t],y+dy[t]) );
		if ( is_free(x+dx[(t+1)&3],y+dy[(t+1)&3]) && is_free(x+dx[(t+3)&3],y+dy[(t+3)&3]) ) {
			if ( (w = G.bfs(e(x,y),e(x+dx[t],y+dy[t]),e(x+dx[(t+1)&3],y+dy[(t+1)&3]))) < +oo )
				if ( seen[v=enc(x,y,t+1)] != yes || d[v] > d[u] ) 
					seen[v] = yes, d[v] = d[u], heap_push(v);
			if ( (w = G.bfs(e(x,y),e(x+dx[t],y+dy[t]),e(x+dx[(t+3)&3],y+dy[(t+3)&3]))) < +oo )
				if ( seen[v=enc(x,y,t+3)] != yes || d[v] > d[u] ) 
					seen[v] = yes, d[v] = d[u], heap_push(v);
		}
		if ( is_free(x+dx[(t+2)&3],y+dy[(t+2)&3]) ) 
			if ( (w = G.bfs(e(x,y),e(x+dx[t],y+dy[t]),e(x+dx[(t+2)&3],y+dy[(t+2)&3]))) < +oo )
				if ( seen[v=enc(x,y,t+2)] != yes || d[v] > d[u] ) 
					seen[v] = yes, d[v] = d[u], heap_push(v);
	}
	return best;
}

int main() {
	int i,ts,sx,sy,tx,ty,px,py,x,y;
	unsigned int w,ww,ans;
	for ( scanf("%d",&ts); ts-- && 2==scanf("%d %d",&m,&n) && (m||n); ) {
		for ( G.reset(), i = 0; i < m; scanf("%s",g[i++]) ) ;
		find('M',&sx,&sy), find('P',&px,&py), find('K',&tx,&ty);
		for ( ans = +oo, i = 0; i < 4; ++i )
			if ( is_free(x=px+dx[i],y=py+dy[i]) ) {
				w = G.bfs(e(px,py),e(sx,sy),e(x,y));
				if ( w < +oo && (ww = bfs(enc(px,py,i),e(tx,ty))) < +oo && ans > ww )
					ans = ww;
			}
		if ( ans < +oo )
			printf("%u\n",ans);
		else NIE;
	}
	return 0;
}

