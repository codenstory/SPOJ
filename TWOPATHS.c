/*
 * TWOPATHS
 * TOPIC: DP on Tree, greedy, binary heap
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<17)
#define MAXE (N<<1)
#define xchg(x,y) (((x) == (y)) || ((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble(q) (xchg(pos[q->heap[i]],pos[q->heap[j]]),xchg(q->heap[i],q->heap[j]))
#define oo (1LL<<60)
typedef long long i64;

int getnum() {
	register int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch == EOF )
		return -1;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

int n,V,E,to[MAXE],next[MAXE],last[N],papa[N],
	pos[N],sons[N],
	que[N],*head,*tail,seen[N],yes,p[N];
i64 ad[N],bd[N],ans[N],a[N],b[N],diam[N],up[N],dp[N];

typedef struct pqueue { int *heap,cnt; } pqueue;

pqueue q[N];

void init( pqueue *q, int sz ) {
	q->heap = (int *)malloc(sz*sizeof *(q->heap));
	q->cnt = 0;
}

int peek( pqueue *q ) { return q->heap[0]; }

void push( pqueue *q, int x ) {
	int i,j;
	if ( pos[x] < 0 )
		pos[q->heap[q->cnt] = x] = q->cnt, ++q->cnt;
	for ( j = pos[x]; j && a[q->heap[i=(j-1)>>1]] < a[q->heap[j]]; bubble(q), j = i ) ;
}

int pop( pqueue *q ) {
	int i,j,x = q->heap[0];
	if ( q->cnt += (pos[x]=-1) )
		pos[q->heap[0] = q->heap[q->cnt]] = 0;
	for ( j = 0; (i = j, j <<= 1, ++j) < q->cnt; bubble(q) ) {
		if ( j < q->cnt-1 && a[q->heap[j]] < a[q->heap[j+1]] ) ++j;
		if ( a[q->heap[j]] <= a[q->heap[i]] ) break ;
	}
	return x;
}

void erase( pqueue *q, int x ) {
	int z;
	a[x] = +oo, push(q,x), z = pop(q);
	assert(z == x);
}

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i;
	to[j] = x, next[j] = last[y], last[y] = j;
}

i64 f( i64 len ) {
	if ( len&1 )
		return (len>>1)*(len>>1);
	return (len>>1)*((len>>1)-1);
}

void dfs( int x ) {
	int i,y,y0,y1;
	i64 D;
	if ( a[x] != -1 )
		return ;
	for ( ad[x]=bd[x]=a[x]=b[x]=diam[x]=ans[x]=0, i = last[x]; i >= 0; i = next[i] )
		if ( p[y = to[i]] == i ) {
			dfs(y), push(q+x,y);

			if ( 1+a[y] >= a[x] )
				b[x] = a[x], a[x] = 1+a[y];
			else if ( 1+a[y] > b[x] )
				b[x] = 1+a[y];

			if ( ans[y] > ans[x] ) ans[x] = ans[y];
			if ( diam[y] > diam[x] ) diam[x] = diam[y];

			if ( diam[y] >= ad[x] )
				bd[x] = ad[x], ad[x] = diam[y];
			else if ( diam[y] > bd[x] )
				bd[x] = diam[y];
		}
	if ( sons[x] >= 2 && a[x]+b[x] > diam[x] )
		diam[x] = a[x]+b[x];
	else if ( sons[x] == 1 && a[x] > diam[x] )
		diam[x] = a[x];
	if ( a[x] && f(a[x]) > ans[x] )
		ans[x] = f(a[x]);
	if ( ans[x] < ad[x]*bd[x] )
		ans[x] = ad[x]*bd[x];
	if ( sons[x] >= 2 )
		for ( i = last[x]; i >= 0; i = next[i] )
			if ( p[y = to[i]] == i ) {
				D = a[y], erase(q+x,y);
				if ( q[x].cnt >= 2 ) {
					y0 = pop(q+x), y1 = pop(q+x);
					if ( ans[x] < ((1+a[y0])+(1+a[y1]))*diam[y] )
						ans[x] = ((1+a[y0])+(1+a[y1]))*diam[y];
					push(q+x,y0), push(q+x,y1);
				}
				else {
					y0 = pop(q+x);
					if ( ans[x] < (1+a[y0])*diam[y] )
						ans[x] = (1+a[y0])*diam[y];
					push(q+x,y0);
				}
				a[y] = D, push(q+x,y);
			}
}

void calc_up( int x ) {
	int i,j,y,z;
	i64 D;
	if ( up[x] != -1 )
		return ;
	up[x] = 0;
	if ( p[x] != -1 ) {
		j = p[x], calc_up(z = to[j^1]);
		assert( papa[x] == z );
		D = a[x], erase(q+z,x);
		up[x] = 1+up[z];
		if ( q[z].cnt ) 
			if ( a[peek(q+z)]+2 > up[x] )
				up[x] = a[peek(q+z)]+2;
		a[x] = D, push(q+z,x);
	}
}

i64 g( int x ) {
	int j,z,y,y0,y1;
	i64 D;
	if ( dp[x] != -1 )
		return dp[x];
	dp[x] = ans[x];
	if ( up[x] ) {
		if ( diam[x] && diam[x]*(up[x]-1) > dp[x] )
			dp[x] = diam[x]*(up[x]-1);
		if ( ad[x] && ad[x]*up[x] > dp[x] )
			dp[x] = ad[x]*up[x];
	}
	if ( p[x] != -1 ) {
		j = p[x], z = to[j^1];
		D = a[x], erase(q+z,x);
		if ( q[z].cnt >= 2 ) {
			y0 = pop(q+z), y1 = pop(q+z);
			if ( dp[x] < (2+a[y0]+a[y1])*diam[x] )
				dp[x] = (2+a[y0]+a[y1])*diam[x];
			push(q+z,y0), push(q+z,y1);
		}
		else if ( q[z].cnt ) {
			y0 = pop(q+z);
			if ( dp[x] < (1+a[y0])*diam[x] )
				dp[x] = (1+a[y0])*diam[x];
			push(q+z,y0);
		}
		if ( q[z].cnt ) {
			y0 = pop(q+z);
			if ( dp[x] < (1+a[y0]+up[z])*diam[x] )
				dp[x] = (1+a[y0]+up[z])*diam[x];
			push(q+z,y0);
		}
		if ( dp[x] < up[z]*diam[x] )
			dp[x] = up[z]*diam[x];
		a[x] = D, push(q+z,x);
	}
	return dp[x];
}

int main() {
	int i,j,k,x,y;
	i64 w;
	for ( ;(n = getnum()) != -1; ) {
		for ( E = 0, i = 0; i < n; dp[i] = p[i] = up[i] = a[i] = pos[i] = -1, last[i++] = -1 ) ;
		for ( k = 0; k < n-1; i = getnum(), j = getnum(), add_arcs(--i,--j), ++k ) ;
		for ( head = tail = que, seen[*tail++ = 0] = ++yes; head < tail; init(q+x,(sons[x]<<1)+8) ) 
			for ( i = last[x = *head++], sons[x] = 0; i >= 0; i = next[i] )
				if ( seen[y = to[i]] != yes ) 
					seen[*tail++ = y] = yes, p[y] = i, ++sons[papa[y] = x];
		for ( dfs(0), i = 0; i < n; calc_up(i++) ) ;
		for ( w = -oo, i = 0; i < n; ++i )
			if ( g(i) > w )
				w = dp[i];
		printf("%lld\n",w);
	}
	return 0;
}

