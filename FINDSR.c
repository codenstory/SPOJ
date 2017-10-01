/*
 * FINDSR
 * TOPIC: KMP, periods
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<20)

int n,p[N],m;
char s[N];

int main() {
	int i,j,k;
	for ( ;1 == scanf("%s",s+1) && 1[s] != '*'; ) {
		for ( n = strlen(s+1), j = p[0] = -1, i = 1; i <= n; p[i++] = ++j ) 
			for ( ;j+1 && s[i] != s[j+1]; j = p[j] ) ;
		m = p[n];
		if ( p[n] == 0 ) {
			puts("1");
			continue ;
		}
		if ( 0 == (n%(n-p[n])) ) {
			printf("%d\n",n/(n-p[n]));
			continue ;
		}
		puts("1");
	}
	return 0;
}

