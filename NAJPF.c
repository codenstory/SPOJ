/*
 * NAJPF: Pattern Find
 * TOPIC: kmp
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<21)

char pattern[N],text[N];
int m,n,p[N],*ptr,out[N];

int main() {
	int i,j,k,cs = 0,ts;
	for ( scanf("%d",&ts); ts--; ) {
		if ( ++cs > 1 ) putchar('\n');
		scanf("%s %s",text+1,pattern+1);
		m = strlen(pattern+1), n = strlen(text+1);
		for ( p[0] = -1, i = 1; i <= m; p[i++] = j+1 ) 
			for ( j = p[i-1]; j+1 && pattern[j+1] != pattern[i]; j = p[j] ) ;
		for ( ptr = out, k = 0, i = 1; i <= n; ++i, ++k ) {
			if ( k == m ) 
				*ptr++ = i-m;
			for ( ;k+1 && pattern[k+1] != text[i]; k = p[k] ) ;
		}
		if ( k == m ) *ptr++ = i-m;
		if ( out == ptr )
			puts("Not Found");
		else {
			printf("%d\n",ptr-out);
			for ( i = 0; i < ptr-out; ++i ) {
				if ( i ) putchar(' ');
				printf("%d",out[i]);
			}
			puts("");
		}
	}
	return 0;
}

