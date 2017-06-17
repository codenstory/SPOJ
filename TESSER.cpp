/*
 * TESSER: Finding the Tesserect
 * TOPIC: kmp
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
#define N (1<<18)
using namespace std;

int n,m,text[N],pattern[N],p[N];
char tmp[N];

int which( int x, int y ) { return (x==y?0:x<y?2:1); }

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d",&n);
		for ( i = 0; i < n; ++i, j = k ) {
			scanf("%d",&k);
			if ( i ) 
				text[i] = which(j,k);
		}
		scanf("%s",tmp);
		for ( --n, m = 0; tmp[m]; ++m )
			pattern[m+1] = (tmp[m]=='E'?0:tmp[m]=='G'?2:1);
		for ( p[0] = j = -1, i = 1; i <= m; p[i++] = ++j )
			for ( ;j+1 && pattern[j+1] != pattern[i]; j = p[j] ) ;
		for ( k = 0, i = 1; i <= n && k < m; ++i, ++k ) 
			for ( ;k+1 && text[i] != pattern[k+1]; k = p[k] ) ;
		puts(k==m?"YES":"NO");
	}
	return 0;
}

