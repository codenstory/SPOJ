/*
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x40
#define MAXE (1<<20)
using namespace std;
enum { WHITE, GRAY, BLACK };

int last[N],_next[MAXE],to[MAXE],E,V,n,g[N][N],p[N],str[N][N],
	m,active[N],len,seen[N],yes,edge[N];
char zip[N][0x10],*ptr,out[N*5],color[N];
bool covered[N],traversable[MAXE],impassable[N];
set<int> s[N];

void add_arcs( int x, int y ) {
	int i= E++, j= E++;
	to[i]= y, _next[i]= last[x], last[x]= i;
	to[j]= x, _next[j]= last[y], last[y]= j;
	traversable[i]= traversable[j]= true ;
}

int dfs( int x ) {
	int i,y,res= covered[x]?0:1;
	assert( seen[x] != yes );
	for ( seen[x]= yes, i= last[x]; i >= 0; i= _next[i] )
		if ( seen[y= to[i]] != yes ) {
			if ( color[y] == WHITE ) {
				res+= dfs(y);
				continue ;
			}
			if ( color[y] == BLACK )
				continue ;
			assert( color[y] == GRAY );
			if ( s[y].count(i^1) ) 
				res+= dfs(y);
		}
	return res;
}

int main() {
	int i,j,k,cs= 0,ts,u,v,candidate,arc_id,t,idx,pu,parent,l,tmp;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&m);
		for ( E= 0, i= 0; i < n; last[i++]= -1 ) ;
		for ( i= 0; i < n; scanf("%s",zip[i++]) );
		memset(g,-1,sizeof g), memset(p,-1,sizeof p), memset(covered,0,sizeof covered);
		memset(edge,-1,sizeof edge);
		memset(impassable,0,sizeof impassable);
		for ( k= 0; k < m; ++k ) {
			scanf("%d %d",&i,&j), --i, --j;
			assert( g[i][j] < 0 );
			add_arcs(i,j), g[i][j]= E-2, g[j][i]= E-1;
		}
		for ( i= 0; i < n; s[i++].clear() ) ;
		for ( i= 0; i < n; ++i )
			for ( j= 0; j < n; ++j )
				str[i][j]= strcmp(zip[i],zip[j]);
		for ( v= -1, i= 0; i < n; ++i )
			if ( v == -1 || str[i][v] < 0 )
				v= i;
		int src= v;
		memset(color,WHITE,sizeof color);
		len= 0, covered[active[len++]= v]= true, color[v]= GRAY;
		for ( ptr= out, ptr+= sprintf(ptr,"%s",zip[v]), k= 1; k < n; ++k ) {
			for ( candidate= -1, l= len-1; l >= 0; --l )
				for ( i= last[u= active[l]]; i >= 0; i= _next[i] )
					if ( color[v= to[i]] == WHITE && !covered[v] ) {
						assert( !s[u].count(i) );
						++yes, s[u].insert(i);
						for ( j= len-1; j >= l+1; color[active[j--]]= BLACK ) ;
						if ( (t= dfs(v)) == n-k ) 
							if ( candidate == -1 || str[v][candidate] < 0 )
								candidate= v, arc_id= i, idx= l;
						for ( j= len-1; j >= l+1; color[active[j--]]= GRAY ) ;
						s[u].erase(i);
						if ( t < n-k ) break ;
					}
			assert( candidate != -1 );
			u= active[idx], color[v= candidate]= GRAY, covered[v]= true;
			for ( j= len-1; j >= idx+1; color[active[j--]]= BLACK ) ;
			len= idx+1, s[u].insert(arc_id), active[len++]= v, ptr+= sprintf(ptr,"%s",zip[v]);
		}
		*ptr= '\0', printf("Case #%d: %s\n",++cs,out);
	}
	return 0;
}

