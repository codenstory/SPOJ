/*
 * IMPORT: Galactic Import
 * TOPIC: lazy dp, vertex splitting
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
#define tol 1e-13
#include <cfloat>
#define SINK (0)
#define N 0x80
using namespace std;
#define oo (DBL_MAX-0x400)
enum { D, T };

vector<int> adj[2][N];
int n,g[N][N],yes,willing[N],p[N][N],V;
double value[N],v[N],
	   z[N][N],seen[N][N];
set<char> s;
map<char,int> m;
char id[N];
char a[N][N];

double calc_z( int k, int x ) {
	assert( x >= 0 && x < V );
	assert( k >= 0 );
	if ( seen[k][x] == yes )
		return z[k][x];
	seen[k][x] = yes;
	if ( k == 0 ) {
		p[k][x] = x>=n+1?x-n:x;
		if ( x == 0 ) 
			return z[k][x] = 0;
		if ( 1 <= x && x <= n )
			return z[k][x] = 0;
		return z[k][x] = v[x-n];
	}
	assert( k >= 1 );
	z[k][x] = calc_z(k-1,x), p[k][x] = p[k-1][x];
	if ( 0 <= x && x <= n ) {
		for ( int l = 0; l < (int)adj[T][x].size(); ++l ) {
			int y = adj[T][x][l];
			if ( y != x+n && calc_z(k-1,y) > -oo ) {
				if ( calc_z(k-1,y) < z[k][x] ) continue ;
				if ( calc_z(k-1,y) > z[k][x] || fabs(calc_z(k-1,y)-z[k][x]) < tol && id[p[k-1][y]] < id[p[k][x]] )
					z[k][x] = calc_z(k-1,y), p[k][x] = p[k-1][y];
			}
		}
	}
	else {
		int y = x-n;
		assert( 1 <= y && y <= n );
		if ( calc_z(k-1,y) > -oo ) {
			if ( 19*calc_z(k-1,y)/20 > z[k][x] || fabs(19*calc_z(k-1,y)/20-z[k][x]) < tol && id[p[k-1][y]] < id[p[k][x]] )
				z[k][x] = 19*calc_z(k-1,y)/20, p[k][x] = p[k-1][y];
		}
	}
	return z[k][x];
}

int main() {
	int i,j,k,t;
	for ( ;1 == scanf("%d",&n); ) {
		for ( s.clear(), m.clear(), i = 0; i < n; ++i ) {
			char tmp[0x20];
			scanf("%s",tmp), scanf("%lf",&value[0[tmp]-'A']);
			s.insert(0[tmp]);
			scanf("%s",a[0[tmp]-'A']);
		}
		m['*'] = SINK;
		for ( set<char>::iterator it = s.begin(); it != s.end(); k=m.size(),v[m[*it]=k]=value[*it-'A'],id[k]=*it-'A',++it ) ;
		assert( n+1 == m.size() ) ;
		for ( ++yes, i = 1; i <= n; ++i ) 
			for ( k = strlen(a[id[i]]), j = 0; j < k; ++j ) {
				char tm = a[id[i]][j];
				if ( 'A' <= tm && tm <= 'Z' )
					g[i][m[tm]] = g[m[tm]][i] = yes;
				else {
					assert( tm == '*' );
					willing[i] = yes;
				}
			}
		for ( t = D; t <= T; ++t )
			for ( V = n+n+1, i = 0; i < V; adj[t][i++].clear() ) ;
		for ( i = 1; i <= n; ++i )
			for ( j = 1; j <= n; ++j )
				if ( g[i][j] == yes )
					adj[D][i+n].push_back(j), adj[T][j].push_back(i+n);
		for ( i = 1; i <= n; ++i ) {
			if ( willing[i] == yes )
				adj[D][i+n].push_back(SINK), adj[T][SINK].push_back(i+n);
			adj[D][i].push_back(i+n), adj[T][i+n].push_back(i);
		}
		for ( i = 0; i < V; ++i )
			for ( j = 0; j <= V+V; z[j++][i] = -oo ) ;
		calc_z(2*V,SINK), printf("Import from %c\n",id[p[2*V][SINK]]+'A');
	}
	return 0;
}

