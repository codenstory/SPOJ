/*
 * FATAWY
 * TOPIC: MST, DSU
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define N 0x200
#define MAXLEN (0x10)
using namespace std;

int n,th,lcs[N][N],len[N],parent[N],rank[N],yes,is_father[N];
char str[N][MAXLEN];

int LCS( int a, int b ) {
	int c[MAXLEN][MAXLEN] = {0};
	for ( int i = 1; i <= len[a]; ++i )
		for ( int j = 1; j <= len[b]; ++j ) {
			if ( str[a][i] == str[b][j] )
				c[i][j] = 1+c[i-1][j-1];
			c[i][j] = max(c[i][j],max(c[i-1][j],c[i][j-1]));
		}
	return c[len[a]][len[b]];
}

void link( int x, int y ) {
	if ( rank[x] > rank[y] )
		parent[y] = x;
	else {
		parent[x] = y;
		if ( rank[x] == rank[y] )
			++rank[y];
	}
}
int find( int x ) { return parent[x]==x?x:find(parent[x]); }
void merge( int x, int y ) {
	link(find(x),find(y));
}

int main() {
	int i,j,k,ts,cs = 0,th;
	for ( scanf("%d",&ts); ts-- && ++yes; ) {
		scanf("%d %d",&n,&th);
		for ( i = 0; i < n; scanf("%s",str[i++]+1), len[i-1] = strlen(1+str[i-1]) ) ;
		for ( i = 0; i < n; rank[parent[i]=i] = 0, ++i ) ;
		for ( i = 0; i < n; ++i )
			for ( j = i+1; j < n; ++j ) {
				lcs[j][i] = lcs[i][j] = LCS(i,j);
				bool g = min(lcs[i][j]/(0.00+len[i]),lcs[i][j]/(0.00+len[j]))*100.00 >= th;
				if ( g && find(i) != find(j) )
					merge(i,j);
			}
		for ( ++yes, i = 0; i < n; ++i )
			is_father[find(i)] = yes;
		for ( k = 0, i = 0; i < n; ++i )
			if ( is_father[i] == yes ) ++k;
		printf("Case #%d:\n%d\n",++cs,k);
	}
	return 0;
}
