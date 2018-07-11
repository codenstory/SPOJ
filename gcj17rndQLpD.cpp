/*
 * TOPIC: maxflow, lower bounds on capacities
 * status: Accepted
 */
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#define DIM (0x80)
#define N (1<<14)
#define MAXE (1<<21)
#define oo (1<<30)
#define R_IN(i) (i)
#define R_OUT(i) (R_IN(i)+n)
#define C_IN(j) (2*n+(j))
#define C_OUT(j) (C_IN(j)+n)
#define LD_IN(i) (4*n+(i))
#define LD_OUT(i) (LD_IN(i)+left)
#define RD_IN(j) (4*n+2*left+(j))
#define RD_OUT(j) (RD_IN(j)+right)
#define vc(x,y) (0 <= (x) && (x) < n && 0 <= (y) && (y) < n)
using namespace std;

struct answer {
	char ch;
	int i,j;
	answer() {};
	answer( char ch, int i, int j ) : ch(ch), i(i), j(j) {};
};

int n,left,right,cn[DIM][DIM],
	V,E,flow[MAXE],cap[MAXE],lb[MAXE],
	_next[MAXE],to[MAXE],last[MAXE],
	src,sink,visited[N],yes,d[N],seen[N],indeed,
	q[N],*head,*tail,
	lid[DIM+DIM],rid[DIM+DIM],
	mp[DIM+DIM][DIM+DIM],m;
char g[DIM][DIM],G[DIM][DIM];
double occupied_row[DIM],occupied_col[DIM],occupied_left[DIM+DIM],occupied_right[DIM+DIM];

void add_arcs( int x, int y, int c, int l, int ff ) {
	int i= E++, j= E++;
	assert( j < MAXE );
	to[i]= y, _next[i]= last[x], last[x]= i, cap[i]= c, lb[i]= l;
	to[j]= x, _next[j]= last[y], last[y]= j, cap[j]= -l, lb[j]= -c;
	flow[i]= ff, flow[j]= -ff;
}

bool bfs() {
	int x,y,i;
	for ( head= tail= q, visited[*tail++= sink]= ++yes, d[sink]= 0; head < tail; )
		for ( i= last[x= *head++]; i >= 0; i= _next[i] )
			if ( cap[i^1] > flow[i^1] && visited[y= to[i]] != yes )
				visited[*tail++= y]= yes, d[y]= d[x]+1;
	return visited[src] == yes;
}

int dfs( int x, int df ) {
	if ( x == sink && df > 0 )
		return df;
	if ( seen[x] == indeed )
		return 0;
	int y,i,dt;
	for ( seen[x]= indeed, i= last[x]; i >= 0; i= _next[i] )
		if ( visited[y= to[i]] == yes && d[y]+1 == d[x] )
			if ( cap[i] > flow[i] )
				if ( dt= dfs(y,min(df,cap[i]-flow[i])) ) {
					flow[i]+= dt, flow[i^1]-= dt;
					return dt;
				}
	return 0;
}

vector<int> src2row,
			src2diag;
vector<answer> ans;

int maxflow() {
	int total= 0;
	for (;bfs();)
		for (;++indeed && dfs(src,+oo);) ;
	for ( int i= 0; i < (int)src2row.size(); total+= flow[src2row[i++]] ) ;
	for ( int i= 0; i < (int)src2diag.size(); total+= flow[src2diag[i++]] ) ;
	return total;
}

int inflow( int x ) {
	int total= 0;
	for ( int i= 0; i < E; ++i )
		if ( to[i] == x )
			total+= flow[i];
	return total;
}

int outflow( int x ) {
	int total= 0;
	for ( int i= 0; i < E; ++i )
		if ( to[i^1] == x )
			total+= flow[i];
	return total;
}

void check_sanity() {
	int src_flow= 0,sink_flow= 0;
	for ( int x= 0; x < V; ++x )
		if ( x != src && x != sink ) {
			if ( inflow(x) != outflow(x) )
				printf("[%d %d] [%d,%d] This %d: %d %d\n",src,sink,n*n+2*left,n*n+2*left+2*right,x,inflow(x),outflow(x));
			assert( inflow(x) == outflow(x) );
		}
		else if ( x == src )
			src_flow= outflow(x);
		else sink_flow= inflow(x);
	assert( src_flow == sink_flow );
}

bool is_valid_board() {
	int i,j;
	memset(occupied_row,0,sizeof occupied_row);
	memset(occupied_col,0,sizeof occupied_col);
	memset(occupied_left,0,sizeof occupied_left);
	memset(occupied_right,0,sizeof occupied_right);
	for ( i= 0; i < n; ++i )
		for ( j= 0; j < n; ++j ) {
			if ( !g[i][j] ) continue ;
			switch ( g[i][j] ) {
				case 'o': if ( occupied_row[i] ) return false ;
						  if ( occupied_col[j] ) return false ;
						  if ( occupied_left[lid[i+j]] ) return false ;
						  if ( occupied_right[rid[i-j+n-1]] ) return false ;
						  occupied_left[lid[i+j]]= occupied_right[rid[i-j+n-1]]= occupied_row[i]= occupied_col[j]= true ;
						  break ;
				case 'x': if ( occupied_row[i] || occupied_col[j] )
								 return false ;
						  occupied_row[i]= occupied_col[j]= true ;
						  break ;
				default: assert( g[i][j] == '+' );
						  if ( occupied_left[lid[i+j]] || occupied_right[rid[i-j+n-1]] )
								return false ;
						 occupied_left[lid[i+j]]= occupied_right[rid[i-j+n-1]]= true ;
						 break ;

			}
		}
	return true ;
}

int alid[DIM+DIM],arid[DIM+DIM];

void find_cell( int ld, int rd, int *x, int *y ) {
	int a= alid[ld],
		b= arid[rd];
	*x= (a+b-n+1)/2, *y= (a-b+n-1)/2;
	assert( !((a+b-n+1)&1) );
	assert( !((a-b+n-1)&1) );
}

int main() {
	int i,j,k,ts,cs= 0,x,y,already_placed;
	char tmp[0x10];
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&already_placed), memset(g,'\0',sizeof g), memset(G,'\0',sizeof G);
		memset(occupied_row,0,sizeof occupied_row);
		memset(occupied_col,0,sizeof occupied_col);
		memset(occupied_left,0,sizeof occupied_left);
		memset(occupied_right,0,sizeof occupied_right);
		memset(lid,-1,sizeof lid), memset(rid,-1,sizeof rid);
		for ( left= right= 0, m= 0, i= 0; i < n; ++i )
			for ( j= 0; j < n; ++j ) {
				if ( lid[i+j] < 0 )
					alid[lid[i+j]= left++]= i+j;
				if ( rid[i-j+n-1] < 0 )
					arid[rid[i-j+n-1]= right++]= i-j+n-1;
			}
		assert( left == 2*n-1 );
		assert( right == 2*n-1 );
		for ( k= 0; k < already_placed; ++k ) {
			scanf("%s %d %d",tmp,&i,&j), --i, --j, g[i][j]= 0[tmp];
			switch ( g[i][j] ) {
				case 'o': occupied_left[lid[i+j]]= occupied_right[rid[i-j+n-1]]= occupied_row[i]= occupied_col[j]= true ;
						  break ;
				case 'x': occupied_row[i]= occupied_col[j]= true ;
						  break ;
				default: assert( g[i][j] == '+' );
						 occupied_left[lid[i+j]]= occupied_right[rid[i-j+n-1]]= true ;
						 break ;
			}
		}
		assert( is_valid_board() );
		/**
		 * initialisation of the graph
		 */
		for ( E= 0, V= 4*n+2*left+2*right+2, src= V-2, sink= V-1, x= 0; x < V; last[x++]= -1 ) ;
		/**
		 * adding connections from the source to rows and left diagonals
		 */
		for ( src2row.clear(), src2diag.clear(), i= 0; i < n; ++i )
			for ( j= 0; j < n; ++j ) {
				x= src, y= LD_IN(lid[i+j]);
				switch ( g[i][j] ) {
					case '+': add_arcs(x,y,1,1,1); break ;
					case 'o': add_arcs(x,y,1,1,1); break ;
					default: add_arcs(x,y,1,0,0); break ;
				}
				src2diag.push_back(E-2);

				x= src, y= R_IN(i);
				switch ( g[i][j] ) {
					case 'x': add_arcs(x,y,1,1,1); break ;
					case 'o': add_arcs(x,y,1,1,1); break ;
					default: add_arcs(x,y,1,0,0); break ;
				}
				src2row.push_back(E-2);
			}

		/*** Adding vertex capacities ***/
		/**
		 * adding vertex capacities for rows and for columns,
		 * as well as left and right diagonals
		 */
		for ( i= 0; i < n; ++i )
			add_arcs(R_IN(i),R_OUT(i),1,occupied_row[i]?1:0,occupied_row[i]?1:0);
		for ( j= 0; j < n; ++j )
			add_arcs(C_IN(j),C_OUT(j),1,occupied_col[j]?1:0,occupied_col[j]?1:0);
		for ( i= 0; i < left; ++i )
			add_arcs(LD_IN(i),LD_OUT(i),1,occupied_left[i]?1:0,occupied_left[i]?1:0);
		for ( j= 0; j < right; ++j )
			add_arcs(RD_IN(j),RD_OUT(j),1,occupied_right[j]?1:0,occupied_right[j]?1:0);
		/*** End of Adding vertex capacities ***/

		/**
		 * adding connections from rows to columns,
		 * as well as from left diagonals to the right ones
		 */
		for ( i= 0; i < n; ++i )
			for ( j= 0; j < n; ++j ) {
				switch ( g[i][j] ) {
					case 'x': add_arcs(R_OUT(i),C_IN(j),1,1,1); break ;
					case 'o': add_arcs(R_OUT(i),C_IN(j),1,1,1); break ;
					default: add_arcs(R_OUT(i),C_IN(j),1,0,0); break ;
				}
			}
		for ( i= 0; i < n; ++i )
			for ( j= 0; j < n; ++j ) {
				x= LD_OUT(lid[i+j]), y= RD_IN(rid[i-j+n-1]);
				switch ( g[i][j] ) {
					case '+': add_arcs(x,y,1,1,1); break ;
					case 'o': add_arcs(x,y,1,1,1); break ;
					default: add_arcs(x,y,1,0,0); break ;
				}
			}

		/**
		 * adding connections from right diagonals to the sink
	 	 * adding connections from columns to the sink
		 */
		for ( j= 0; j < n; ++j )
			add_arcs(C_OUT(j),sink,1,occupied_col[j]?1:0,occupied_col[j]?1:0);
		for ( j= 0; j < right; ++j )
			add_arcs(RD_OUT(j),sink,1,occupied_right[j]?1:0,occupied_right[j]?1:0);

		/**
		 * maxflow computation and output of results
		 */
		//puts("Doing pre-test");
		//check_sanity(); puts("Passed pre-test");
		printf("Case #%d: %d ",++cs,maxflow());
		//check_sanity(); puts("Passed post-test");
		memset(cn,0,sizeof cn);
		for ( i= 0; i < E; ++i ) {
			if ( !flow[i] ) continue ;
			x= to[i^1], y= to[i];
			if ( R_OUT(0) <= x && x <= R_OUT(n-1) && C_IN(0) <= y && y <= C_IN(n-1) ) {
				assert( flow[i] == 1 );
				assert( vc(x-n,y-2*n) );
				++cn[x-n][y-2*n], G[x-n][y-2*n]= 'x';
				continue ;
			}
			if ( LD_OUT(0) <= x && x <= LD_OUT(left-1) && RD_IN(0) <= y && y <= RD_IN(right-1) ) {
				find_cell(x-4*n-left,y-4*n-2*left,&x,&y);
				assert( flow[i] == 1 );
				assert( vc(x,y) );
				++cn[x][y], G[x][y]= '+';
				continue ;
			}
		}
		for ( ans.clear(), i= 0; i < n; ++i )
			for ( j= 0; j < n; ++j ) {
				if ( !g[i][j] && cn[i][j] || g[i][j] && g[i][j] != 'o' && cn[i][j] == 2 ) {
					if ( cn[i][j] == 2 )
						ans.push_back({'o',i,j}), g[i][j]= 'o';
					else ans.push_back({G[i][j],i,j}), g[i][j]= G[i][j];
				}
			}
		//assert( is_valid_board() );

		for ( printf("%lu\n",ans.size()), i= 0; i < (int)ans.size(); ++i )
			printf("%c %d %d\n",ans[i].ch,ans[i].i+1,ans[i].j+1);
	}
	return 0;
}

