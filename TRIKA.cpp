/*
 * TRIKA
 * TOPIC: dp, dijkstra, interview-level
 * status: Accepted
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#define tol (1e-13)
#define N 0x20
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)
using namespace std;
#define oo (1<<30)

int m,n,g[N][N],d[N][N];
typedef struct cell {
	int x,y,w;
	cell( int x, int y, int w ) {
		this->x = x, this->y = y, this->w = w;
	}
	cell() {};
} cell;

struct comparator {
	bool operator () ( const cell &a, const cell &b ) {
		if ( a.w == b.w ) {
			if ( a.x == b.x ) 
				return a.y < b.y;
			return a.x < b.x;
		}
		return a.w < b.w;
	}
};

priority_queue<cell,vector<cell>,comparator> pq;

int main() {
	int i,j,k,sx,sy;
	cell u,v;
	for ( ;2 == scanf("%d %d",&m,&n); ) {
		for ( scanf("%d %d",&sx,&sy), --sx, --sy, i = 0; i < m; ++i )
			for ( j = 0; j < n; ++j )
				scanf("%d",&g[i][j]), d[i][j] = -oo;
		for ( d[sx][sy] = g[sx][sy], pq.push(cell(sx,sy,g[sx][sy])); !pq.empty(); )  {
			u = pq.top(), pq.pop();
			if ( vc(u.x+1,u.y) && d[u.x+1][u.y] < d[u.x][u.y]-g[u.x+1][u.y] && d[u.x][u.y] >= g[u.x+1][u.y] ) {
				v = cell(u.x+1,u.y,d[u.x+1][u.y]=d[u.x][u.y]-g[u.x+1][u.y]);
				pq.push(v);
			}
			if ( vc(u.x,u.y+1) && d[u.x][u.y+1] < d[u.x][u.y]-g[u.x][u.y+1] && d[u.x][u.y] >= g[u.x][u.y+1] ) {
				v = cell(u.x,u.y+1,d[u.x][u.y+1]=d[u.x][u.y]-g[u.x][u.y+1]);
				pq.push(v);
			}
		}
		if ( d[m-1][n-1] == -oo )
			puts("N");
		else printf("Y %d\n",d[m-1][n-1]);
	}
	return 0;
}

