/*
 * DYZIO: Dyzio
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
#define tol (1e-13)
#define N (1<<15)
#define oo (1<<30)
#include <map>
typedef long long i64;
using namespace std;

int n,cuts,h[N],H[N],pref[N];
char str[N];

void rec( int height, int *pos ) {
	if ( *pos >= n || str[*pos] == '0' ) {
		++(*pos);
		return ;
	}
	++(*pos);
	if ( H[height] == -1 )
		H[height] = (*pos)-1;
	rec(height+1,pos), rec(height+1,pos);
}

int main() {
	int i,j,k;
	for (;2==scanf("%d %s",&n,str);) {
		for ( i = 0; i < N; H[i++] = -1 ) ;
		k = 0, rec(0,&k);
		for ( pref[0] = str[0]-'0', i = 1; i < N; ++i )
			pref[i] = pref[i-1]+(str[i]-'0');
		for ( k = -oo, i = n; i >= 0 && H[i] == -1; --i ) ;
		if ( i < 0 ) {
			puts("0");
			continue ;
		}
		printf("%d\n",pref[H[i]]);
	}
	return 0;
}

