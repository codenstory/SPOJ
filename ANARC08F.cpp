/*
 * status: WA
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
#define tol (1e-13)
#define N 0x80
#define oo (1LL<<60)
using namespace std;
typedef long long i64;

map<string,int> m;
map<int,string> rm;
int n,cars,roads,V,r;
vector<int> e;
i64 g[N][N];

int main() {
	int i,j,k,cs = 0;
	i64 ans;
	char tmp[0x20],a[0x20],b[0x20],arr1[0x10],arr2[0x10];
	for ( ;3 == scanf("%d %d %d\r\n",&n,&cars,&roads) && !(!n&&!cars&&!roads); ) {
		for ( e.clear(), V = 0, rm.clear(), m.clear(), i = 0; i <= cars;  ++i ) {
			scanf("%s\r\n",tmp);
			if ( m.count(string(tmp)) ) {
				e.push_back(m[string(tmp)]);
				continue ;
			}
			rm[m[string(tmp)]=V++]=string(tmp);
			e.push_back(V-1);
		}
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				g[i][j] = +oo;
		for ( ;roads--; ) {
			scanf("%[A-Za-z] %[^A-Za-z0-9] %d %[^A-Za-z0-9] %[A-Za-z]\r\n",a,arr1,&k,arr2,b);
			/*printf("%s %s %d %s %s\n",a,arr1,k,arr2,b);*/
			if ( !m.count(string(a)) ) rm[m[string(a)] = V++] = string(a);
			if ( !m.count(string(b)) ) rm[m[string(b)] = V++] = string(b);
			i = m[string(a)], j = m[string(b)];
			if ( strcmp(arr1,"--") == 0 ) {
				if ( 0 == strcmp(arr2,"--") ) continue ;
				g[i][j] = k;
			}
			else if ( strcmp(arr1,"<-") == 0 ) {
				if ( 0 == strcmp(arr2,"--") ) 
					g[j][i] = k;
				else {
					g[i][j] = g[j][i] = k;
				}
			}
		}
		assert( m.size() <= n );
		for ( i = 0; i < n; g[i][i] = 0, ++i ) ;
		for ( k = 0; k < n; ++k )
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					if ( g[i][k] < +oo && g[k][j] < +oo && g[i][j] > g[i][k]+g[k][j] )
						g[i][j] = g[i][k]+g[k][j], 0&&printf("Relaxed %s-->%s --> %lld\n",rm[i].c_str(),rm[j].c_str(),g[i][j]);
		for ( ans = 0, i = 1; i < (int)e.size(); ++i ) {
			/*printf("%s --> %s with cost %lld, %s --> %s with cost %lld\n",rm[0].c_str(),rm[i].c_str(),g[0][i],rm[i].c_str(),rm[0].c_str(),g[i][0]);*/
			assert( g[0][e[i]] < +oo );
			assert( g[e[i]][0] < +oo );
			ans += g[0][e[i]]+g[e[i]][0];
		}
		printf("%d. %lld\n",++cs,ans);
	}
	return 0;
}

