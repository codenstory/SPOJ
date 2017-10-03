/*
 * QTREE5
 * TOPIC: Centroid Decomposition
 * status: Accepted
 * NOTES: may need a faster set, since the numbers are in the range 1..N,
 * and thus we can speed up
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_map>
#include <vector>
#define SH (18)
#define BIT(k) (1ULL<<(k))
#define MAXN (BIT(SH))
#define MAXE (MAXN<<1)
#define MASK(k) (BIT(k)-1ULL)
#define NONE (-1)
#define oo (1<<30)
#include <set>
#pragma comment(linker, "/STACK:16777216")
using namespace std;
enum { B, W };

int max( int x, int y ) { return x < y ? y:x; }
int min( int x, int y ) { return y < x ? y:x; }

int getnum() {
	register int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch == EOF ) return NONE;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

int n,K,E,to[MAXE],_next[MAXE],last[MAXN],
	color[MAXN],label[MAXN],link[MAXN],
	card[MAXN],seen[MAXN],yes,parent[MAXN],
	*head,*tail,q[MAXN],d[MAXN],anc[MAXN][SH+1],
	dist[MAXN][SH+1];
unordered_map<int,int> mp[MAXN];
set<int> s[MAXN];

int calc_size( int x ) {
	int i,y;
	assert( label[x] == NONE );
	assert( seen[x] != yes );
	for ( seen[x] = yes, card[x] = 1, i = last[x]; i >= 0; i = _next[i] )
		if ( label[y = to[i]] == NONE && seen[y] != yes ) 
			parent[y] = i, card[x] += calc_size(y);
	return card[x];
}

int determine_centroid( int x, const int size_of_universe ) {
	int i,y,max_weight = -1, idx = NONE;

	for ( i = last[x]; i >= 0; i = _next[i] )
		if ( label[y = to[i]] == NONE && parent[y] == i ) 
			if ( card[y] > max_weight )
				max_weight = card[idx = y];
	if ( 2*max(max_weight,size_of_universe-card[x]) <= size_of_universe )
		return x;
	assert( idx != NONE );
	return determine_centroid(idx,size_of_universe);
}

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	to[i] = y, _next[i] = last[x], last[x] = i;
	to[j] = x, _next[j] = last[y], last[y] = j;
}

void centroid_decomposition( int x, int previous_centroid, int size_of_universe ) {
tail:
	int c,zz = NONE;
	
	if ( size_of_universe == 1 ) {
		label[x] = (previous_centroid==NONE?0:label[previous_centroid]+1), link[x] = previous_centroid;
		/*printf("label[%d] = %d, link[%d] = %d\n",x+1,label[x],x+1,link[x]+1);*/
		return ;
	}

	assert( size_of_universe >= 2 );

	c = determine_centroid(x,size_of_universe);
	label[c] = (previous_centroid==NONE?0:label[previous_centroid]+1), link[c] = previous_centroid;
	/*printf("label[%d] = %d, link[%d] = %d\n",c+1,label[c],c+1,link[c]+1);*/
	for ( int y,i = last[c]; i >= 0; i = _next[i] ) {
		if ( label[y = to[i]] != NONE ) continue ;
		if ( parent[y] == i )
			centroid_decomposition(y,c,card[y]);
		else {
			assert( parent[c] == (i^1) );
			if ( c != x )
				zz = x;
		}
	}
	if ( zz != NONE ) {
		++yes, calc_size(zz), x = zz, previous_centroid = c, size_of_universe -= card[c];
		goto tail;
	}
	//centroid_decomposition(y,c,size_of_universe-card[c]);
}

int up( int x, unsigned int d ) {
	int k = 0;
	for ( ;d; ++k, d >>= 1 )
		if ( d&1 ) x = anc[x][k];
	return x;
}

int lca( int x, int y ) {
	int k;
	if ( d[x] > d[y] )
		return lca(up(x,d[x]-d[y]),y);
	if ( d[y] > d[x] )
		return lca(y,x);
	if ( x == y ) return x;
	for ( k = K-1; k; --k ) {
		assert( anc[x][k] == anc[y][k] );
		if ( anc[x][k-1] != anc[y][k-1] )
			x = anc[x][k-1], y = anc[y][k-1];
	}
	return anc[x][0];
}

int ancestor[MAXN][SH+1],whites;

int my_distance( int x, int y ) { return d[x]+d[y]-2*d[lca(x,y)]; }
int _distance( int x, int k ) { 
	return dist[x][k]==NONE?dist[x][k] = my_distance(x,ancestor[x][k]):dist[x][k]; 
}
void insert( unordered_map<int,int> &m, set<int> &s, int r ) {
	if ( m.find(r) == m.end() || m[r] == 0 )
		m[r] = 1, s.insert(r);
	else 
		m[r] = m[r]+1;
}
void remove( unordered_map<int,int> &m, set<int> &s, int r ) {
	if ( m.find(r) == m.end() )
		return ;
	assert( m[r] >= 1 );
	m[r] = m[r]-1;
	if ( m[r] == 0 )
		s.erase(r);
}

void flip_color( int x ) {
	int k = label[x], y = x;
	if ( color[x] ^= 1 ) 
		for ( ++whites, insert(mp[x],s[x],0); --k >= 0; y = link[y], insert(mp[y],s[y],_distance(x,k)) );
	else 
		for ( --whites, remove(mp[x],s[x],0); --k >= 0; y = link[y], remove(mp[y],s[y],_distance(x,k)) );
}

int Q( int x ) {
	set<int> :: iterator it = s[x].begin();
	if ( it == s[x].end() ) 
		return +oo;
	return *it;
}

int query( int x ) {
	int y = x, k = label[x], ans = Q(x);
	for ( ;--k >= 0; y=link[y], ans = min(ans,Q(y)+_distance(x,k)) ) ;
	return ans;
}

int main() {
	int i,j,k,qr,x,y;
	for ( ;(n = getnum()) != NONE; ) {
		for ( E = 0, i = 0; i < n; s[i].clear(), mp[i].clear(), card[i] = parent[i] = label[i] = last[i] = NONE, color[i++] = B ) ;
		for ( K = 0; (1<<K) < n; ++K ) ;
		for ( i = 0; i < n; ++i )
			for ( k = 0; k < K; dist[i][k] = NONE, anc[i][k++] = NONE ) ;
		for ( k = n-1; k--; add_arcs(getnum()-1,getnum()-1) ) ;
		++yes, calc_size(0), centroid_decomposition(0,NONE,n);
		for ( head=tail=q, seen[*tail++=0]=++yes; head<tail; )
			for ( i=last[x=*head++]; i>=0; i=_next[i] )
				if ( seen[y=to[i]]!=yes ) 
					for ( seen[*tail++=y]=yes, d[y]=d[x]+1, anc[y][0]=x, k=1; anc[y][k-1]!=NONE; anc[y][k]=anc[anc[y][k-1]][k-1], ++k ) ;
		for ( x = 0; x < n; ++x )
			for ( y = x, k = label[x], ancestor[x][k] = x; --k >= 0; y = link[y], ancestor[x][k] = y ) ;
		for ( qr = getnum(); qr--; ) {
			 k = getnum(), x = getnum()-1;
			 assert( label[x] != NONE );
			 switch ( k ) {
				case 0:	flip_color(x);
						break ;
				default: k = !whites?+oo:query(x);
						 if ( k < +oo )
								printf("%d\n",k);
						 else puts("-1");
			 }
		}
		break ;
	}
	return 0;
}

