/*
 * LQDRACE
 * TOPIC: Centroid Decomposition, IOI2011
 * LINK: http://wcipeg.com/problem/ioi1112
 * status: Accepted
 */
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#define SH (19)
#define BIT(k) (1ULL<<(k))
#define N BIT(SH)
#define MAXN (N)
#define MAXE (N<<1)
#define NONE (-1)
#define oo (1<<30)
#pragma comment(linker, "/STACK:16777216")
#define W (1<<21)
typedef long long i64;
using namespace std;

int max( int x, int y ) { return x<y?y:x; }
int min( int x, int y ) { return x>y?y:x; }

int getnum() {
	register int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch == EOF )
		return NONE;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

int KK,n,E,K,to[MAXE],last[MAXN],_next[MAXE],w[MAXE],
	label[N],link[N],card[N],seen[N],yes,parent[N],
	q[N],*head,*tail,anc[N][SH+1],st[N],*top;
i64 weight[N],d[N];

void add_arcs( int x, int y, int weight ) {
	int i = E++, j = E++;
	to[i] = y, _next[i] = last[x], last[x] = i, w[i] = weight;
	to[j] = x, _next[j] = last[y], last[y] = j, w[j] = weight;
}

int calc_size( int x ) {
	int i,y;
	assert( label[x] == NONE );
	if ( seen[x] == yes )
		return card[x];
	for ( seen[x] = yes, card[x] = 1, i = last[x]; i != NONE; i = _next[i] )
		if ( seen[y = to[i]] != yes && label[y] == NONE ) {
			//assert( anc[y][0] == x );
			parent[y] = i, card[x] += calc_size(y);
		}
	return card[x];
}

int determine_centroid( int x, int size_of_universe ) {
	int idx = NONE, max_weight = NONE, i,y;
	for ( i = last[x]; i != NONE; i = _next[i] )
		if ( parent[y = to[i]] == i && label[y] == NONE )
			if ( max_weight < card[y] )
				max_weight = card[idx=y];
	if ( 2*max(max_weight,size_of_universe-card[x]) <= size_of_universe )
		return x;
	assert( idx != -1 );
	return determine_centroid(idx,size_of_universe);
}

void centroid_decomposition( int x, int previous_centroid, int size_of_universe ) {
	int cx,c,i,y,z = NONE,wgt,prev;

	if ( size_of_universe == 1 ) {
		label[x]=(link[x]=previous_centroid)==NONE?0:label[previous_centroid]+1;
		return ;
	}

	assert( size_of_universe >= 2 );

	c=determine_centroid(x,size_of_universe),label[c]=(link[c]=previous_centroid)==NONE?0:label[previous_centroid]+1;
	//printf("%*d\n",label[c],size_of_universe);
	wgt = card[c];

	for ( i = last[c]; i != NONE; i = _next[i] ) {
		if ( label[y=to[i]]!=NONE ) continue ;
		if ( parent[y] == i )
			parent[y] = NONE, centroid_decomposition(y,c,card[y]);
		else {
			assert( parent[c] != NONE && to[1^parent[c]] == y );
			z = to[1^parent[c]];
		}
	}
	if ( c != x ) {
		assert( z != NONE );
		top = st;
		for ( cx = c; cx != x; ) {
			assert( parent[cx] != NONE );
			*++top = parent[cx], cx = to[1^parent[cx]];
		}
		assert( cx == x );
		parent[cx]=(1^(*top--));
		card[cx]-=card[to[1^parent[cx]]];
		for ( prev=cx; top > st; prev=cx ) {
		 	cx=to[1^parent[cx]];
			parent[cx]=(1^(*top--)),card[cx]+=card[prev],card[cx]-=card[to[1^parent[cx]]];
		}
		cx = to[1^parent[cx]];
		assert( cx == c );
		wgt = card[cx], card[cx] += card[prev];
		assert( z == prev );
		parent[c] = NONE;
		assert( card[z] == size_of_universe-wgt );
		parent[z] = NONE, centroid_decomposition(z,c,card[z]);
	}
	else {
		assert( z == NONE );
	}
}

int up( int x, unsigned int d ) {
	for ( int k = 0; d; d >>= 1, ++k )
		if ( d&1 ) x = anc[x][k];
	return x;
}

int lca( int x, int y ) {
	if ( d[x] > d[y] )
		return lca(up(x,d[x]-d[y]),y);
	if ( d[x] < d[y] )
		return lca(y,x);
	if ( d[x] == d[y] )
		return x;
	for ( int k = K-1; k; --k ) {
		assert( anc[x][k] == anc[y][k] );
		if ( anc[x][k-1] != anc[y][k-1] )
			x = anc[x][k-1], y = anc[y][k-1];
	}
	return anc[x][0];
}

i64 weighted_distance( int x, int y ) {
	return weight[x]+weight[y]-2*weight[lca(x,y)];
}

int _distance( int x, int y ) {
	return d[x]+d[y]-2*d[lca(x,y)];
}

vector<int> adj[N];
int dp[W+1],visited[W+1],answer,
	a[W+1],b[W+1],tick,T[N];

i64 calc_t( int x ) {
	if ( T[x] != NONE )
		return T[x];
	T[x] = 0;
	for ( int i = last[x]; i != NONE; i = _next[i] )
		if ( parent[to[i]] == i ) {
			if ( T[x]+w[i]+calc_t(to[i]) >= +oo )
				T[x] = +oo;
			else
				T[x] += w[i]+calc_t(to[i]);
		}
	return T[x];
}

void dfs( const int src, int x, int lane, int cur_dd, i64 cur_ww ) {
	int tin = ++tick, k = 0;
	seen[x] = yes;
	if ( cur_dd >= answer || cur_ww > KK )
		return ;
	if ( cur_ww == KK ) {
		answer = min(answer,cur_dd);
		return ;
	}
	if ( visited[KK-cur_ww] == yes && b[KK-cur_ww] < lane )
		answer = min(answer,dp[KK-cur_ww]+cur_dd);
	if ( cur_dd >= answer ) return ;
	//for ( int i = 0; i < (int)adj[x].size(); ++i )
	//	dfs(adj[x][i].first,cur_dd+adj[x][i].second.first,cur_ww+adj[x][i].second.second);
	for ( int i=last[x]; i!=NONE; i=_next[i] )
		if ( seen[to[i]] != yes && label[to[i]] > label[src] ) {
			if ( x == src ) dfs(src,to[i],k++,cur_dd+1,cur_ww+w[i]);
			else dfs(src,to[i],lane,cur_dd+1,cur_ww+w[i]);
		}
	if ( visited[cur_ww] != yes || dp[cur_ww] > cur_dd )
		a[cur_ww]=tin, b[cur_ww]=lane, dp[cur_ww] = cur_dd, visited[cur_ww] = yes;
}

int solve( const int root, int x ) {
	int ans = +oo;
	//if ( T[x] < KK ) return ans;
	for ( int i=0; i<(int)adj[x].size(); ans=min(ans,solve(root,adj[x][i++])) ) ;
	answer = min(answer,ans);
	++yes, tick = -1, dfs(x,x,NONE,0,0);
	//if ( visited[KK] == yes ) answer = min(answer,dp[KK]);
	/*
	for ( int k = 1; 2*k <= KK; ++k )
		if ( visited[k] == yes && visited[KK-k] == yes )
			if ( b[KK-k] < a[k] || b[k] < a[KK-k] )
				answer = min(answer,dp[k]+dp[KK-k]);*/
	answer = min(answer,ans);
	return answer;
}

int main() {
	int i,j,k,l,root,x,y,total;
	for ( ;(n = getnum()) != NONE; ) {
		for ( KK = getnum(), total = 0, E = 0, i = 0; i < n; adj[i].clear(), parent[i] = last[i] = label[i] = link[i] = NONE, T[i] = NONE, ++i ) ;
		for ( k = n-1; k--; i=getnum(),j=getnum(),l=getnum(),add_arcs(i,j,l), total+=l ) ;
		/*
		if ( total < KK ) {
			puts("-1");
			continue ;
		}*/
		for ( K = 0; BIT(K) <= n; ++K ) ;
		/*
		for ( i = 0; i < n; ++i )
			for ( k = 0; k < K; anc[i][k++] = NONE ) ;*/
		for ( head = tail = q, seen[*tail++ = 0] = ++yes; head < tail; )
			for ( i=last[x=*head++]; i!=NONE; i=_next[i] ) {
				if ( seen[y = to[i]] == yes ) 
					continue ;
				//for(seen[*tail++=y]=yes,weight[y]=weight[x]+w[i],d[y]=d[x]+1,anc[y][0]=x,k=1;anc[y][k-1]!=NONE;anc[y][k]=anc[anc[y][k-1]][k-1],++k);
				seen[*tail++=y] = yes;
			}
		++yes, calc_size(0), centroid_decomposition(0,NONE,n);
		for ( root = NONE, i = 0; i < n; ++i )
			if ( label[i] > 0 ) {
				assert( link[i] != NONE );
				//adj[link[i]].push_back(make_pair(i,make_pair(_distance(i,link[i]),weighted_distance(i,link[i]))));
				adj[link[i]].push_back(i);
			}
			else root = i;
		assert( root != NONE );
		answer = +oo;
		for ( head = tail = q, seen[*tail++ = root] = ++yes; head < tail; )
			for ( i=last[x=*head++]; i!=NONE; i=_next[i] ) {
				if ( seen[y = to[i]] == yes ) 
					continue ;
				seen[*tail++=y] = yes, parent[y] = i;
			}
		//calc_t(root);
		if ( (k=solve(root,root)) < +oo )
			printf("%d\n",k);
		else puts("-1");
	}
	return 0;
}

