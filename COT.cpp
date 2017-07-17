/*
 * COT
 * TOPIC: persistent segment tree, lca
 * status: Accepted
 */
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#define MAXK (20)
#define BIT(k) (1LL<<(k))
#define MAXV (BIT(MAXK))
#define MAXE (MAXV)
using namespace std;

set<int> ss;
map<int,int> m,rm;
int K,E,n,V,next[MAXE],to[MAXE],last[MAXV],weight[MAXV],p[MAXV],depth[MAXV],anc[MAXV][MAXK];
bool seen[MAXV];

int getnum() {
	int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

void add_arcs( int x, int y ) {
	int i = E++, j = E++;
	to[i] = y, next[i] = last[x], last[x] = i;
	to[j] = x, next[j] = last[y], last[y] = j;
}

typedef struct cell {
	cell *left, *right;
	int card;
	cell( int card, cell *left, cell *right ) {
		this->card = card, this->left = left, this->right = right;
	}
} cell;

cell *null = new cell(0,NULL,NULL);

cell *insert( cell *x, int i, int j, int val ) {
	assert( 0 <= val && val < V );
	if ( val < i || val > j ) return x;
	assert( x != NULL );
	if ( i == j ) 
		return new cell(x->card+1,null,null);
	int k = (i+j)/2;
	return new cell(x->card+1,insert(x->left,i,k,val),insert(x->right,k+1,j,val));
}

cell *root[MAXV];

void dfs( int x ) {
	assert( !seen[x] );
	root[x] = insert(p[x]==-1?null:root[anc[x][0]],0,V-1,m[weight[x]]);
	depth[x]=(p[x]==-1?0:depth[anc[x][0]]+1),seen[x]=true;
	for ( int y, k, i=last[x]; i>=0 && (y=to[i])>=0; i=next[i] ) 
		if ( !seen[y] ) {
			for ( p[y]=i, anc[y][0]=x, k=1; k<K && anc[y][k-1]!=-1; anc[y][k]=anc[anc[y][k-1]][k-1], ++k ) ;
			dfs(y);
		}
}

int up( int x, unsigned int d ) {
	int i;
	for ( i = 0; d; d >>= 1, ++i )
		if ( d&1 )
			x = anc[x][i];
	return x;
}

int lca( int x, int y ) {
	int k;
	if ( depth[x] > depth[y] )
		return lca(up(x,depth[x]-depth[y]),y);
	if ( depth[x] < depth[y] )
		return lca(y,x);
	if ( x == y )
		return x;
	assert( depth[x] == depth[y] && x != y ) ;
	for ( k = K-1; k; --k ) {
		/*
		if ( anc[x][k] != anc[y][k] ) 
			printf("K = %d, k = %d, x = %d, y = %d, %d %d\n",K,k,x,y,anc[x][k],anc[y][k]);*/
		assert( anc[x][k] == anc[y][k] ) ;
		if ( anc[x][k-1] != anc[y][k-1] )
			x = anc[x][k-1], y = anc[y][k-1];
	}
	return anc[x][0];
}

int q( cell *a, cell *b, cell *c, cell *d, int i, int j, int k ) {
	assert( i <= j && a->card+b->card-c->card-d->card >= k );
	if ( i == j ) 
		return i;
	int mid = (i+j)/2, cnt;
	if ( (cnt = a->left->card+b->left->card-c->left->card-d->left->card) >= k )
		return q(a->left,b->left,c->left,d->left,i,mid,k);
	return q(a->right,b->right,c->right,d->right,mid+1,j,k-cnt);
}

int query( int x, int y, int k ) {
	int z = lca(x,y), pz = (p[z]==-1?-1:anc[z][0]);
	cell *a = root[x], *b = root[y], *c = root[z], *d = pz==-1?null:root[pz];
	return rm[q(a,b,c,d,0,V-1,k)];
}

int main() {
	int i,j,k,qr;
	null->left = null->right = null;
	for ( E = 0, V = 0, n = getnum(), qr = getnum(), i = 0; i < n; ss.insert(weight[i++] = getnum()) ) ;
	for ( i = 0; i < n; p[i] = -1, last[i++] = -1 ) ;
	for ( K = 0; (1<<K) <= n; ++K ) ;
	for ( i = 0; i < n; ++i )
		for ( k = 0; k < K; anc[i][k++] = -1 ) ;
	for ( set<int> :: iterator it = ss.begin(); it != ss.end(); rm[m[*it] = V++] = *it, ++it ) ;
	for ( k = n-1; k--; add_arcs(getnum()-1,getnum()-1) ) ;
	for ( dfs(0); qr--; i=getnum()-1,j=getnum()-1,k=getnum(),printf("%d\n",query(i,j,k)) ) ;
	return 0;
}

