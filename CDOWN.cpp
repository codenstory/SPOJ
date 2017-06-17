/*
 * CDOWN: countdown
 * TOPIC: recursion
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
#include <map>
#include <set>
#define N (0x400)
#define tol (1e-13)
using namespace std;
#define FG (ptr = fgets(buff,sizeof buff,stdin))
#define oo (1<<30)

char buff[0x400],*ptr;
int n,D,z[N],seen[N],yes,ans[N][N],visited[N][N];
bool in[N];
map<string,int> m;
string name[N];
vector<int> adj[N];

int calc_z( int x ) {
	int i,y;
	if ( seen[x] == yes )
		return z[x];
	seen[x] = yes;
	for ( z[x] = 0, i = 0; i < (int)adj[x].size(); ++i )
		z[x] += 1+calc_z(y=adj[x][i]);
	return z[x];
}

int calc_ans( int x, int depth ) {
	if ( visited[x][depth] == yes )
		return ans[x][depth];
	visited[x][depth] = yes;
	if ( depth == 1 ) 
		return ans[x][depth] = adj[x].size();
	ans[x][depth] = 0;
	for ( int i = 0; i < (int)adj[x].size(); ++i )
		ans[x][depth] += calc_ans(adj[x][i],depth-1);
	return ans[x][depth];
}

vector<string> des[N];

int main() {
	int i,j,k,ts,len,cs = 0;
	char a[0x20],b[0x20];
	for	( scanf("%d",&ts); ts--; ) {
		if ( cs >= 1 ) putchar('\n');
		scanf("%d %d",&n,&D), m.clear();
		for ( i = 0; i < N; ++i )
			adj[i].clear();
		memset(in,0,sizeof in);
		for ( ++yes, i = 0; i < n; ++i ) {
			scanf("%s",a);
			string A = string(a);
			if ( m.find(A) == m.end() )
				k = m.size(), m[A] = k, name[k] = A;
			scanf("%d",&len);
			k = m[A];
			for ( int l = 0; l < len; ++l ) {
				scanf("%s",a);
				string B = string(a);
				if ( m.find(B) == m.end() )
					j = m.size(), m[B] = j, name[j] = B;
				j = m[B];
				adj[k].push_back(j), in[j] = true ;
			}
		}
		for ( i = 0; i <= m.size(); ++i )
			des[i].clear();
		for ( i = 0; i < m.size(); ++i ) {
			k = calc_ans(i,D);
			des[k].push_back(name[i]);
		}
		printf("Tree %d:\n",++cs);
		int printed = 0;
		for ( k = m.size(); k >= 1 && printed <= 2; --k ) {
			if ( des[k].size() == 0 ) continue ;
			sort(des[k].begin(),des[k].end());
			for ( i = 0; i < (int)des[k].size(); ++i, ++printed )
				printf("%s %d\n",des[k][i].c_str(),k);
		}
	}
	return 0;
}

