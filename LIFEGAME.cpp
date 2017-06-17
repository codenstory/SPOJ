/*
 * LIFEGAME: Life Game
 * TOPIC: matrix exponentiation
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <queue>
#define tol 1e-13
#define N 0x80
typedef long long i64;
using namespace std;

const long double pi = 2*acos(0.00);

typedef struct cell {
	long double x,y;
	cell() { x=y=0; }
	cell( long double x, long double y ) {
		this->x = x, this->y = y;
	}
} cell;

cell operator - ( const cell &a, const cell &b ) {
	return cell(a.x-b.x,a.y-b.y);
}

cell operator + ( const cell &a, const cell &b ) {
	return cell(a.x+b.x,a.y+b.y);
}

long double operator ^ ( const cell &a, const cell &b ) {
	return a.x*b.y-a.y*b.x;
}

long double operator , ( const cell &a, const cell &b ) {
	return a.x*b.x+a.y*b.y;
}

long double operator | ( const cell &a, const cell &b ) {
	long double A = sqrt((a,a)), B = sqrt((b,b)),
		   co = (a,b)/A/B, si = (a^b)/A/B;
	if ( co >= 0 && si >= 0 )
		return acos(co);
	if ( co <= 0 && si >= 0 )
		return acos(co);
	if ( co >= 0 && si <= 0 )
		return asin(si);
	return -pi+acos(-co);
}

bool operator < ( const cell &a, const cell &b ) {
	if ( fabs(a.y-b.y) < tol && fabs(a.x-b.x) < tol )
		return false;
	if ( fabs(a.y-b.y) < tol ) 
		return a.x<b.x;
	return a.y>b.y;
}

bool operator == ( const cell &a, const cell &b ) {
	return !(a < b || b < a);
}

bool operator != ( const cell &a, const cell &b ) {
	return !(a == b);
}

cell operator ^ ( const cell &a, const long double t ) {
	long double co = cosl(t), si = sinl(t);
	return cell(a.x*co-a.y*si,a.x*si+a.y*co);
}

struct cell_comparator {
	bool operator () ( const cell &a, const cell &b ) {
		return a<b;
	}
};

i64 MOD,L,g[N*N],A[0x80][0x80];
map<cell,int,cell_comparator> m,dist;
set<cell,cell_comparator> s;
int n;
cell UNIT(1,0),ZERO,id[N*N];
queue<cell> q;
set<int> adj[N*N];

int main() {
	int i,j,k,cs = 0;
	cell u,v,dir[6];
	i64 ans,dans;
	for ( dir[k=0] = UNIT; ++k < 6; dir[k] = dir[k-1]^(pi/3) ) ;
	for (;3 == scanf("%d %lld %lld",&n,&MOD,&L) && (n||MOD||L);) {
		for ( i = 0; i <= 3*n*n-3*n+1; adj[i++].clear() ) ;
		assert( q.empty() );
		for ( s.clear(),s.insert(ZERO),dist.clear(),m.clear(),m[ZERO]=0,q.push(ZERO),dist[ZERO] = 0; !q.empty(); ) {
			u = q.front(), q.pop();
			if ( dist[u] == n ) continue ;
			for ( k = 0; k < 6; ++k ) {
				v = u+dir[k];
				if ( s.find(v) == s.end() ) {
					if ( dist[u] == n-1 ) continue ;
					assert( dist.find(v) == dist.end() );
					s.insert(v), q.push(v), dist[v] = dist[u]+1;
					i = m.size(), m[v] = i;
				}
				adj[m[u]].insert(m[v]), adj[m[v]].insert(m[u]);
			}
		}
		assert( m.size() == 3*n*n-3*n+1 ) ;
		memset(g,-1,sizeof g);
		for ( i = 0; i < m.size(); ++i ) {
			k = -1;
			bool found = false;
			for ( set<cell,cell_comparator>::iterator it = s.begin(); it != s.end(); ++it ) 
				if ( (++k) == i ) { found = true; u = *it; break ; }
			assert( found );
			assert( g[m[u]] < 0 );
			assert( 1 == scanf("%lld",&g[m[u]]) );
			g[m[u]] %= MOD;
		}
		memset(A,0,sizeof A);
		for ( map<cell,int,cell_comparator>::iterator it = m.begin(); it != m.end(); ++it ) {
			i = it->second, A[i][i] = 1;
			for ( set<int>::iterator jt = adj[i].begin(); jt != adj[i].end(); ) {
				j = *jt++, ++A[i][j]; 
				if ( i < j && false ) 
					printf("%d %d\n",i,j);
			}
		}
		i64 tmp[0x80][0x80],ax[0x80][0x80];
		for ( i = 0; i < m.size(); ++i )
			for ( j = 0; j < m.size(); ++j )
				ax[i][j] = (i==j?1:0);
		for ( ;L; L >>= 1 ) {
			if ( (L&1) ) {
				for ( i = 0; i < m.size(); ++i )
					for ( j = 0; j < m.size(); ++j )
						for ( tmp[i][j] = 0, k = 0; k < m.size(); ++k )
							tmp[i][j] += ax[i][k]*A[k][j], tmp[i][j] %= MOD;
				for ( i = 0; i < m.size(); ++i )
					for ( j = 0; j < m.size(); ++j )
						ax[i][j] = tmp[i][j];
			}
			for ( i = 0; i < m.size(); ++i )
				for ( j = 0; j < m.size(); ++j )
					for ( tmp[i][j] = 0, k = 0; k < m.size(); ++k )
						tmp[i][j] += A[i][k]*A[k][j], tmp[i][j] %= MOD;
			for ( i = 0; i < m.size(); ++i )
				for ( j = 0; j < m.size(); ++j )
					A[i][j] = tmp[i][j];
		}
		for ( ans = 0, i = 0; i < m.size(); ++i, ans += (dans%MOD) )
			for ( dans = 0, j = 0; j < m.size(); ++j )
				dans += ax[i][j]*g[j], dans %= MOD;
		printf("Case %d: %lld\n",++cs,ans);
	}
	return 0;
}

