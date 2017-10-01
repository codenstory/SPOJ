/*
 * FILRTEST
 * TOPIC: KMP, greedy
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<21)
#define A (26)
#define oo (1<<30)

int m,n,p[N];
char s[N];

int main() {
	int i,j,k,ch,ans;
	for ( ;2==scanf("%d %s",&m,s+1) && m!=-1; ) {
		for ( n = strlen(s+1), j = p[0] = -1, i = 1; i <= n; p[i++] = ++j )
			for ( ;j+1 && s[i] != s[j+1]; j = p[j] ) ;
		if ( m < n ) {
			puts("0");
			continue ;
		}
		k = n-p[n], ans = 1, m -= n;
		ans += m/k;
		printf("%d\n",ans);
	}
	return 0;
}

