/*
 * GRASSPLA: Grass Planting
 * TOPIC: heavy-light decomposition, segment tree with lazy updates
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAXK (18)
#define N (1<<MAXK)
#define MAXE (N)
#define is_parent(x,y) (in[y] <= in[x] && out[x] <= out[y])
#define DBG 0
#define ASS 1
typedef long long i64;

int n,to[MAXE],next[MAXE],last[N],E,dig[256],idx[N],is_dad[N],
	parent[N],anc[N][MAXK],card[N],seen[N],yes,special_child[N],d[N],which_chain[N],
	pos_in_chain[N],arc_into[N],len,head[N],C,
	BS,K,in[N],out[N],tick,
	rank[N],p[N];
i64 tr[N<<2],h[N<<2];

void make_set( int x ) {
	rank[p[x] = x] = 0;
}

void link( int x, int y ) {
	if ( rank[x] > rank[y] )
		p[y] = x;
	else {
		p[x] = y;
		if ( rank[x] == rank[y] )
			++rank[y];
	}
}

int find( int x ) {
	return p[x] = (p[x]==x?x:find(p[x]));
}

void merge( int x, int y ) {
	link(find(x),find(y));
}

int num() {
	int ch,k = 0;
	/*
	for ( ;(ch = getchar_unlocked()) != EOF && !dig[ch]; ) ;
	if ( ch == EOF ) return EOF;
	for ( k = ch-'0'; (ch = getchar_unlocked()) != EOF && dig[ch]; k = 10*k+ch-'0' ) ;
	return k;
	*/
	if ( EOF == scanf("%d",&k) ) return -1;
	return k;
}

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	/*
	if ( x == 1 || y == 1 )
		printf("Thes %d --> %d\n",x+1,y+1);
	*/
	if ( !(0 <= x && x < n) ) return ;
	if ( !(0 <= y && y < n) ) {
		printf("Trouble: x = %d, n = %d, y = %d, %d\n",x,n,y,y-n);
		assert( 0 );
		return ;
	}
	to[i] = y, next[i] = last[x], last[x] = i;
	to[j] = x, next[j] = last[y], last[y] = j;
}

void dfs( int x ) {
	int i,k,y;
#if ASS
	assert( seen[x] != yes ) ;
#endif
	for ( in[x] = ++tick, seen[x] = yes, card[x] = 1, special_child[x] = -1, i = last[x]; i >= 0; i = next[i] ) 
		if ( seen[y = to[i]] != yes ) {
			parent[y] = i, d[y] = d[x]+1;
			for ( anc[y][0] = x, k = 1; anc[y][k-1] != -1; anc[y][k] = anc[anc[y][k-1]][k-1], ++k ) ;
			dfs(y), card[x] += card[y];
			if ( special_child[x] == -1 || card[y] > card[to[special_child[x]]] )
				special_child[x] = i;
		}
	out[x] = ++tick;
#if DBG
	if ( special_child[x] != -1 )
		printf("card[%d] = %d, special_link: %d --> %d\n",x,card[x],to[special_child[x]^1],to[special_child[x]]);
	else printf("card[%d] = %d\n",x,card[x]);
#endif
}

void hld( int x, int edge_id ) {
	int i,y;
	if ( head[C] == -1 ) head[C] = x;
#if ASS
	assert( seen[x] == yes );
#endif
	which_chain[x] = C, pos_in_chain[x] = len, arc_into[len++] = edge_id;
#if DBG
	printf("which_chain[%d] = %d\n",x,which_chain[x]);
#endif
	if ( special_child[x] != -1 )
		hld(to[special_child[x]],special_child[x]);
	for ( i = last[x]; i >= 0; i = next[i] )
		if ( parent[y=to[i]]==i && special_child[x]!=i ) 
			head[++C]=-1, hld(y,i);
}

int up( int x, unsigned int d ) {
	int k;
	for ( k = 0; k < K && d; ++k, d >>= 1 )
		if ( d&1 ) x = anc[x][k];
	return x;
}

int lca( int x, int y ) {
	int k;
	if ( d[x] < d[y] )
		return lca(y,x);
	if ( d[x] > d[y] )
		return lca(up(x,d[x]-d[y]),y);
#if ASS
	assert( d[x] == d[y] );
#endif
	if ( x == y )
		return x;
	for ( k = K; k; --k ) {
		if ( anc[x][k] != anc[y][k] )
			printf("k = %d, x = %d, y = %d, %d %d\n",k+1,x+1,y+1,anc[x][k],anc[y][k]);
		assert( anc[x][k] == anc[y][k] );
		if ( anc[x][k-1] != anc[y][k-1] )
			x = anc[x][k-1], y = anc[y][k-1];
	}
	return anc[x][0];
}

i64 query_tree( int v, int i, int j, int qi, int qj ) {
	int k = (i+j)/2;
	if ( qi > qj || qi > j || qj < i ) return 0;
	if ( i == j ) {
#if ASS
		assert( qi <= i & j <= qj );
#endif
		tr[v] += h[v], h[v] = 0;
		return tr[v];
	}
	if ( h[v] ) 
		h[2*v]+=(k-i+1)*h[v]/(j-i+1), h[2*v+1]+=(j-k)*h[v]/(j-i+1), tr[v]+=h[v],h[v]=0;
	if ( qi <= i && j <= qj ) 
		return tr[v];
	return query_tree(2*v,i,k,qi,qj)+query_tree(2*v+1,k+1,j,qi,qj);
}

i64 query_up( int u, int v ) {
	int uchain, vchain = which_chain[v];
   	i64	ans = 0;
	/*
	if ( !is_parent(u,v) )
		printf("These are not %d and %d\n",u+1,v+1);
	*/
#if ASS
	assert( is_parent(u,v) );
#endif
	for ( ;; ) {
		if ( (uchain = which_chain[u]) == vchain ) {
			ans += query_tree(1,0,len-1,pos_in_chain[v]+1,pos_in_chain[u]);
			break ;
		}
		ans += query_tree(1,0,len-1,pos_in_chain[head[uchain]],pos_in_chain[u]);
#if ASS
		assert( head[uchain] != 0 );
#endif
		u = anc[head[uchain]][0];
	}
	return ans;
}

i64 query( int x, int y ) {
	int z = lca(x,y);
	return query_up(x,z)+query_up(y,z);
}

void update_tree( int v, int i, int j, int qi, int qj ) {
	int k = (i+j)/2;
	if ( qi > qj || qi > j || qj < i ) return ;
#if 0
	if ( i == j ) {
		++tr[v];
		return ;
	}
	update_tree(2*v,i,k,qi,qj), update_tree(2*v+1,k+1,j,qi,qj);
#else
	if ( h[v] )
		h[2*v] += (k-i+1)*h[v]/(j-i+1), h[2*v+1] += (j-k)*h[v]/(j-i+1), tr[v] += h[v], h[v] = 0;
	if ( qi <= i && j <= qj ) {
		h[v] += (j-i+1);
		if ( h[v] )
			h[2*v] += (k-i+1)*h[v]/(j-i+1), h[2*v+1] += (j-k)*h[v]/(j-i+1), tr[v] += h[v], h[v] = 0;
		return ;
	}
	update_tree(2*v,i,k,qi,qj), update_tree(2*v+1,k+1,j,qi,qj);
#endif
}

void update_up( int u, int v ) {
	int uchain, vchain = which_chain[v];
#if ASS
	assert( is_parent(u,v) );
#endif
	for ( ;; ) {
		if ( (uchain = which_chain[u]) == vchain ) {
			update_tree(1,0,len-1,pos_in_chain[v]+1,pos_in_chain[u]);
			break ;
		}
#if ASS
		assert( pos_in_chain[head[uchain]] <= pos_in_chain[u] );
#endif
		update_tree(1,0,len-1,pos_in_chain[head[uchain]],pos_in_chain[u]);
#if ASS
		assert( head[uchain] != 0 );
#endif
		u = anc[head[uchain]][0];
	}
}

void update( int x, int y ) {
	int z = lca(x,y);
	update_up(x,z), update_up(y,z);
}

int main() {
	int i,j,k,l,t,qr,ch;
	for ( i='0'; i<='9'; dig[i++]=1 ) ;
	for ( ;1 == scanf("%d",&n); ) {
		for ( qr=num(), len=0, C=-1, E=0, i=0; i<n; parent[i]=-1, pos_in_chain[i] = -1,make_set(i),last[i++]=-1 ) ;
		for ( l=0; l<n-1; idx[l++]=E, i=(num()-1),j=(num()-1),add_arcs(i,j) ) ;
		/*
		for ( ++yes, i = 0; i < n; ++i )
			is_dad[find(i)] = yes;
		for ( k = 0, i = 0; i < n; ++i )
			if ( is_dad[i] )
				++k;
				*/
		/*printf("k = %d\n",k);*/
#if ASS
#endif
		for ( K = 0; (1<<K) <= n; ++K ) ;
		/*printf("n = %d\n",n);*/
		for ( i = 0; i < n; ++i )
			for ( k = 0; k <= K; anc[i][k++] = -1 ) ;
		tick=0,++yes,parent[0]=-1,d[0]=0,dfs(0);
		/*
		for ( i = 0; i < n; ++i )
			if ( seen[i] != yes ) {
				printf("This %d\n",i);
				assert( seen[i] == yes );
			}
			*/
		/*printf("n = %d, card[%d] = %d\n",n,0,n-1);*/
		/*assert( card[0] == n ) ;*/
		head[++C]=-1,hld(0,-1);
		memset(tr,0,sizeof tr), memset(h,0,sizeof h);
		/*printf("pos_in_chain[%d] = %d\n",1,pos_in_chain[1]);*/
		/*
		for ( i = 1; i < len; ++i )
			printf("%d --> %d\n",to[arc_into[i]^1],to[arc_into[i]]);
		*/
		for ( l = 0; l < qr; ++l ) {
			for ( ;(ch = getchar_unlocked()) != 'P' && ch != 'Q'; ) ;
			i = num()-1, j = num()-1;
			//printf("ch = %c, i = %d, j = %d\n",ch,i+1,j+1);
			switch ( ch ) {
				case 'P': 
							update(i,j); 
						  break ;
				case 'Q': 
						    printf("%lld\n",query(i,j)); 
						  break ;
				default: assert( 0 );
			}
		}
	}
	return 0;
}

