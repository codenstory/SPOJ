/*
 * status: Accepted
 */
#include <cctype>
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#define N (0x400)
#define Q (1<<21)
#define oo (1<<30)
using namespace std;
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)
#define bubble (swap(pos[heap[i]],pos[heap[j]]),swap(heap[i],heap[j]))
#define enc(x,y) ((y)|((x)<<9))

int m,n,w[Q],c[N+1],pos[Q],heap[Q],cnt;
char g[N][N];

bool is_less( int x, int y ) {
	if ( w[x] == w[y] ) {
		assert( x != y );
		return x < y;
	}
	return w[x] > w[y];
}

void push( int x ) {
	int i,j;
	if ( pos[x] < 0 )
		pos[heap[cnt]= x]= cnt, ++cnt;
	for ( j= pos[x]; j && is_less(heap[j],heap[i=(j-1)>>1]); bubble, j= i ) ;
}

int pop() {
	int i,j,x= *heap;
	if ( cnt+= (pos[x]= -1) ) 
		pos[*heap= heap[cnt]]= 0;
	for ( j= 0; (i= j, j<<= 1, ++j) < cnt; bubble ) {
		if ( j < cnt-1 && is_less(heap[j+1],heap[j]) ) ++j;
		if ( is_less(heap[i],heap[j]) ) break ;
	}
	return x;
}

void remove( int x ) {
	int z;
	if ( pos[x] < 0 ) return ;
	w[x]= +oo, push(x), z= pop(), w[z]= -oo;
	assert( z == x );
}

bool inside( int a, int b, int c, int d, int p, int q ) {
	return a <= p && p <= c && b <= q && q <= d;
}

int main() {
	int i,j,k,cs= 0,ts,t,area,ni,nj,sz,ans,l,SZ,u,v,x,y;
	for ( cin >> ts; ts--; ) {
		cin >> m >> n;
		assert( !(n&3) );
		for ( i= 0; i < m; ++i ) {
			string tmp;
			cin >> tmp;
			assert( tmp.size() == n/4 );
			for ( j= 0; j < (n/4); ++j ) 
				for ( k= isdigit(tmp[j])?tmp[j]-'0':tmp[j]-'A'+10, t= 0; t < 4; ++t ) 
					g[i][4*j+t]= '0'+((k>>(3-t))&1);
		}
		for ( cnt= 0, i= 0; i < m; ++i )
			for ( j= 0; j < n; ++j )
				pos[enc(i,j)]= -1;
		memset(w,0,sizeof w);
		for ( i= 0; i < m; ++i )
			w[u= enc(i,0)]= 1, push(u);
		for ( j= 1; j < n; ++j )
			w[u= enc(0,j)]= 1, push(u);
		for ( i= 1; i < m; ++i )
			for ( j= 1; j < n; ++j ) {
				w[u= enc(i,j)]= 1;
				if ( g[i-1][j] != g[i][j] && g[i][j-1] != g[i][j] && g[i][j] == g[i-1][j-1] )
					w[u]= 1+min(min(w[enc(i-1,j)],w[enc(i,j-1)]),w[enc(i-1,j-1)]);
				push(u);
			}
		for ( memset(c,0,sizeof c); cnt; ) {
			++c[k= w[u= pop()]], x= (u>>9), y= (u&511);
			assert( pos[u] < 0 );
			assert( k >= 1 );
			for ( t= 0, i= x-k+1; i < x+k && i < m; ++i )
				for ( j= y-k+1; j < y+k && j < n; ++j, ++t ) {
					assert( vc(i,j) );
					if ( pos[enc(i,j)] < 0 ) continue ;
					if ( inside(x-k+1,y-k+1,x,y,i,j) ) {
						assert( w[enc(i,j)] < k );
						remove(v= enc(i,j)), w[v]= 0;
						assert( pos[v] < 0 );
						continue ;
					}
					assert( i > x || j > y );
					v= enc(i,j);
					assert( w[v] >= 1 );
					if ( i-w[v]+1 > x || j-w[v]+1 > y )
						continue ;
					if ( inside(x-k+1,y-k+1,x,y,i-w[v]+1,j) ) {
						assert( i > x );
						assert( w[v] > (i-x) );
						remove(v), w[v]= (i-x), push(v);
						continue ;
					}
					if ( inside(x-k+1,y-k+1,x,y,i,j-w[v]+1) ) {
						assert( w[v] > (j-y) );
						assert( j > y );
						remove(v), w[v]= (j-y), push(v);
						continue ;
					}
					if ( inside(x-k+1,y-k+1,x,y,i-w[v]+1,j-w[v]+1) ) {
						assert( max(i-x,j-y) > 0 );
						assert( w[v] > max(i-x,j-y) );
						remove(v), w[v]= max(i-x,j-y), push(v);
						continue ;
					}
				}
			//assert( t == (2*k-1)*(2*k-1) );
		}
		for ( ans= 0, i= 1; i <= m && i <= n; ++i )
			if ( c[i] ) ++ans;
		printf("Case #%d: %d\n",++cs,ans);
		for ( i= min(m,n); i >= 1; --i )
			if ( c[i] )
				printf("%d %d\n",i,c[i]);
	}
	return 0;
}

