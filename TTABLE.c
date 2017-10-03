/*
 * TTABLE
 * TOPIC: Dijkstra, DP
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SH 17
#define T 6
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define N (BIT(SH))
#define MAXE (BIT(22))
#define MMS(u) (((u)>>(SH+T+T))&MASK(T))
#define HHS(u) (((u)>>(SH+T+T+T))&MASK(T))
#define MMA(u) (((u)>>(SH))&MASK(T))
#define HHA(u) (((u)>>(SH+T))&MASK(T))
#define DEST_CITY(u) ((u)&MASK(SH))
#define oo (1<<30)
typedef unsigned long long u64;
#define MAXSTATES (BIT(21))
#define xchg(x,y) (((x)==(y))||((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]),xchg(heap[i],heap[j]))

int deg[N],n,V,last[N],src,sink,*top,o[MAXSTATES],
	heap[MAXSTATES<<1],cnt,pos[MAXSTATES];
u64 *adj[N],c[MAXSTATES];
unsigned int states[MAXSTATES];
int latest_we_could_start[MAXSTATES];

u64 enc( int t, int hhs, int mms, int hha, int mma ) {
	return (((u64)mma)) | (((u64)hha)<<T) | (((u64)t) << (T+T)) | (((u64)mms)<<(SH+T+T)) | (((u64)hhs)<<(SH+T+T+T));
}

int cmp( const void *a, const void *b ) {
	unsigned int x = *((unsigned int *)a),
				 y = *((unsigned int *)b);
	return x==y?0:x<y?-1:1;
}

int cmpu( const void *a, const void *b ) {
	u64 x = *((u64 *)a),
		y = *((u64 *)b);
	return x==y?0:x<y?-1:1;
}

int which_state( unsigned int u ) {
	int low, high, mid;
	if ( states[0] == u ) return 0;
	assert( states[0] < u );
	assert( states[V-1] >= u );
	for ( low = 0, high = V-1; low+1 < high; states[mid=(low+high)>>1] < u ? (low=mid):(high=mid) ) ;
	assert( states[high] == u );
	return high;
}

void push( int x ) {
	int i,j;
	if ( pos[x] < 0 )
		pos[heap[cnt] = x] = cnt, ++cnt;
	for ( j = pos[x]; j && latest_we_could_start[heap[i=(j-1)>>1]] < latest_we_could_start[heap[j]]; bubble, j = i ) ;
}

int pop() {
	int i,j,x = *heap;
	if ( cnt += (pos[x]=-1) )
		pos[*heap = heap[cnt]] = 0;
	for ( j = 0; (i=j,j<<=1,++j) < cnt; bubble ) {
		if ( j < cnt-1 && latest_we_could_start[heap[j]] < latest_we_could_start[heap[j+1]] ) ++j;
		if ( latest_we_could_start[heap[i]] >= latest_we_could_start[heap[j]] ) break ;
	}
	return x;
}

int main() {
	int x,y,i,j,k,t,ts,l,hhs,mms,hha,mma,m,vx,
		tm,current_time, low, high, mid;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( V = 0, m = 0, i = 0; last[i] = +oo, i < n; ++i ) {
			scanf("%d",&deg[i]);
			if ( !deg[i] ) continue ;
			adj[i] = (u64 *)malloc(deg[i]*sizeof *adj[i]);
			for ( k = 0; k < deg[i]; ++k ) {
				scanf("%d:%d %d:%d %d",&hhs,&mms,&hha,&mma,&j), --j;
				states[m++] = ((adj[i][k] = enc(j,hhs,mms,hha,mma)) & MASK(SH+T+T));
			}
		}
		qsort(states,m,sizeof *states,cmp), src = 0, sink = n-1;
		for ( i = 0; i < MAXSTATES; latest_we_could_start[i++] = -oo );
		for ( i = 0; i < m; latest_we_could_start[V] = -oo, pos[V] = -1, states[V++] = states[i], i = j ) 
			for ( j = i+1; j < m && states[j] == states[i]; ++j ) ;
		for ( i = 0; i < V; last[states[i]>>(T+T)] = j-1, i = j )
			for ( j = i+1; j < V && (states[j]>>(T+T)) == (states[i]>>(T+T)); ++j ) ;
		for ( i = 0; i < V; i = j ) {
			for ( j = i+1; j < V && (states[j]>>(T+T)) == (states[i]>>(T+T)); ++j ) ;
			for ( k = i; k+1 < j; ++k )
				assert( (states[k]&MASK(T+T)) <= (states[k+1]&MASK(T+T)) );
		}
		if ( last[sink] == +oo ) {
			puts("0");
			continue ;
		}
		for ( i = 0; i < n; ++i )
			for ( l = 0; l < deg[i]; ++l ) 
				adj[i][l] = (adj[i][l]>>(SH+T+T))|(((u64)which_state(adj[i][l]&MASK(SH+T+T)))<<(T+T));
		for ( cnt = 0, l = 0; l < deg[src]; ++l ) {
			y = (adj[src][l]>>(T+T)), tm = (adj[src][l]&MASK(T+T));
			assert( 0 <= y && y < V ) ;
			if ( latest_we_could_start[y] < tm )
				latest_we_could_start[y] = tm, push(y);
		}
		for (;cnt;) {
			x = pop(), current_time = (states[x]&MASK(T+T)), vx = (states[x]>>(T+T));
			/*printf("At station %d at %02d:%02d\n",vx,current_time>>T,current_time&MASK(T));*/
			if ( vx == sink ) 
				continue ;
			if ( deg[vx] && (adj[vx][deg[vx]-1]&MASK(T+T)) >= current_time ) {
				if ( current_time <= (adj[vx][0]&MASK(T+T)) )
					l = 0;
				else {
					for(low=0,high=deg[vx]-1;low+1<high;(adj[vx][mid=(low+high)>>1]&MASK(T+T))<current_time?(low=mid):(high=mid)) ;
					assert( (adj[vx][high]&MASK(T+T)) >= current_time );
					assert( (adj[vx][low]&MASK(T+T)) < current_time );
					l = high;
				}
				for ( ;l < deg[vx]; ++l ) 
					if ( latest_we_could_start[y=(adj[vx][l]>>(T+T))] < latest_we_could_start[x] && (states[y]>>(T+T)) != src )
						latest_we_could_start[y] = latest_we_could_start[x], push(y);
			}
			for ( y = x+1; last[vx] < +oo && y <= last[vx]; ++y ) {
				assert( (states[y]&MASK(T+T)) >= (states[y-1]&MASK(T+T)) );
				assert( (states[y]>>(T+T)) == (states[y-1]>>(T+T)) );
				if ( latest_we_could_start[y] < latest_we_could_start[x] && (states[y]>>(T+T)) != src )
					latest_we_could_start[y] = latest_we_could_start[x], push(y);
			}
		}
		for ( i = n-2; i >= 0 && last[i] == +oo; --i ) ;
		last[n-2] = (i>=0?last[i]:-1);
		for ( m = 0, l = last[n-2]+1; l <= last[sink]; ++l ) {
			assert( (states[l]>>(T+T)) == sink );
			if ( latest_we_could_start[l] >= 0 ) {
				c[m++] = (((u64)latest_we_could_start[l])<<(T+T))|(states[l]&MASK(T+T));
				assert( (states[l]>>(T+T)) == sink );
			}
		}
		if ( !m ) {
			puts("0");
			continue ;
		}
		qsort(c,m,sizeof *c,cmpu);
#define len(u) (((u)&MASK(T+T))-((u)>>(T+T)))
#define beginning(u) ((u)>>(T+T))
#define end(u) ((u) & MASK(T+T))
		for ( t = m, m = 0, i = 0; i < t; c[m++] = c[l], i = j ) {
			for ( j = i+1; j < t && beginning(c[i]) == beginning(c[j]); ++j ) ;
			for ( k = i+1, l = i; k < j; ++k )
				if ( end(c[k]) < end(c[l]) ) l = k;
		}
		for ( i = 0; i < m-1; ++i )
			assert( beginning(c[i]) < beginning(c[i+1]) );
		for ( top = o, *++top = 0, i = 1; i < m; *++top = i++ )
			for ( ;top > o && end(c[i]) <= end(c[*top]); --top ) ;
		for ( i = 1; i <= top-o-1; ++i )
			assert( beginning(c[o[i]]) < beginning(c[o[i+1]]) && end(c[o[i]]) < end(c[o[i+1]]) );
		for ( printf("%d\n",top-o), i = 1; i <= top-o; ++i )
			printf("%02llu:%02llu %02llu:%02llu\n",(u64)(c[o[i]]>>(T+T+T)),(c[o[i]]>>(T+T))&MASK(T),(c[o[i]]>>T)&MASK(T),c[o[i]]&MASK(T));
		for ( i = 0; i < n; ++i )
			if ( deg[i] ) 
				free(adj[i]);
	}
	return 0;
}

