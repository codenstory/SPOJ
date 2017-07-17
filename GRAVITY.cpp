/*
 * GRAVITY
 * TOPIC: bfs
 * status: WA
 * NOTES: not sure about "by 10%"
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include <queue>
#include <set>
#define N 0x80
#define vc(x,y) (0<=(x) && (x)<m && 0<=(y) && (y)<n)
#define tol (1e-13)
#define SH 7
using namespace std;

char g[N][N];
int m,n,O,seen[1<<20],yes,d[1<<20];
queue<int> q;

int main() {
	int i,j,k,ts,src,sink,nx,ny,x,y,u,v;
	for ( scanf("%d",&ts); ts-- && 3 == scanf("%d %d %d\n",&O,&m,&n); ) {
		for ( i = 0; i < m; fgets(g[i],sizeof g[i],stdin), ++i ) ;
		src = sink = -1;
		for ( i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
			if ( g[i][j] == 'S' )
				src = (i|(j<<SH));
			else if ( g[i][j] == 'T' )
				sink = (i|(j<<SH));
		assert( src != -1 );
		assert( sink != -1 );
		for ( q.push(src), d[src] = 0, seen[src] = ++yes; !q.empty(); )
			for ( u=q.front(),q.pop(),x=(u&127),y=(u>>SH),i=-1; i<=1; ++i )
				for ( j=-1; j<=1; ++j )
					if ( (i||j) && vc(nx=x+i,ny=y+j) && g[nx][ny] != '#' )
						if ( seen[v=nx|(ny<<SH)] != yes )
							seen[v] = yes, d[v] = d[u]+1, q.push(v);
		puts(seen[sink] == yes && (int)((O+1)*pow(0.99,d[sink])) > O?"Possible":"Impossible");
	}
	return 0;
}

