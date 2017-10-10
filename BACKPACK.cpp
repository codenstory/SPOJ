/*
 * BACKPACK
 * TOPIC: DP, bfs
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
#define N 0x40
#define MAXVOL (32000)
using namespace std;
typedef long long i64;
#define oo (1LL<<60)
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define Q (1<<21)

vector<int> attachment[N];
int vol[N],weight[N],u[N],vx[N],*ptr,n,m,maxvol,not_main[N],
	seen[Q],yes;
i64 z[Q];
unsigned int q[1<<21],*head,*tail;

unsigned int enc( int i, int t, int v ) {
	return i | (v<<6);
}

int main() {
	int i,j,k,ts,x,t,v;
	i64 ans;
	for ( scanf("%d",&ts); ts-- && ++yes; ) {
		scanf("%d %d",&maxvol,&n);
		for ( i = 0; i <= n; ++i )
			attachment[i].clear();
		for ( i = 1; i <= n; ++i ) {
			scanf("%d %d %d",&vol[i],&weight[i],&u[i]);
			if ( u[i] )
				attachment[u[i]].push_back(i), not_main[i] = yes;
		}
		for ( ptr = vx, i = 1; i <= n; ++i )
			if ( attachment[i].size() || not_main[i] != yes )
				*++ptr = i;
		for ( m = ptr-vx, ans = -oo, head = tail = q, z[0] = 0, seen[0] = ++yes, *tail++ = enc(0,0,0); head < tail; ) {
			unsigned int uu = *head++, vv;
			x = (uu&MASK(6)), v = (uu>>6);
			ans = max(ans,z[uu]);
			if ( x == m ) 
				continue ;
			assert( x+1 <= m );
			vv = enc(x+1,0,v);
			if ( seen[vv] != yes || z[vv] < z[uu] )
				seen[*tail++ = vv] = yes, z[vv] = z[uu];
			if ( v+vol[vx[x+1]] <= maxvol ) {
				vv = enc(x+1,1,v+vol[vx[x+1]]);
				if ( seen[vv] != yes || z[vv] < z[uu]+weight[vx[x+1]]*vol[vx[x+1]] )
					seen[*tail++ = vv] = yes, z[vv] = z[uu]+weight[vx[x+1]]*vol[vx[x+1]];
			}
		    assert( attachment[vx[x+1]].size() <= 2 );
			if ( attachment[vx[x+1]].size() ) {
				for ( i = 0; i < (int)attachment[vx[x+1]].size(); ++i ) {
					k = attachment[vx[x+1]][i];
					if ( v+vol[vx[x+1]]+vol[k] <= maxvol ) {
						vv = enc(x+1,i==0?2:3,v+vol[vx[x+1]]+vol[k]);
						if ( seen[vv] != yes || z[vv] < z[uu]+weight[vx[x+1]]*vol[vx[x+1]]+weight[k]*vol[k] )
							seen[*tail++ = vv] = yes, z[vv] = z[uu]+weight[vx[x+1]]*vol[vx[x+1]]+weight[k]*vol[k];
					}
				}
				if ( attachment[vx[x+1]].size() == 2 ) {
					int aa = attachment[vx[x+1]][0], bb = attachment[vx[x+1]][1];
					if ( v+vol[vx[x+1]]+vol[aa]+vol[bb] <= maxvol ) {
						vv = enc(x+1,4,v+vol[vx[x+1]]+vol[aa]+vol[bb]);
						if ( seen[vv] != yes || z[vv] < z[uu]+weight[vx[x+1]]*vol[vx[x+1]]+weight[aa]*vol[aa]+weight[bb]*vol[bb] )
							seen[*tail++ = vv] = yes, z[vv] = z[uu]+weight[vx[x+1]]*vol[vx[x+1]]+weight[aa]*vol[aa]+weight[bb]*vol[bb];
					}
				}
			}
		}
		printf("%lld\n",ans);
	}
	return 0;
}

