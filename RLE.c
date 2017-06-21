/*
 * RLE
 * TOPIC: ad hoc
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<20)

char s[N],e[N],*ptr;
void wr( int x ) { (x<10)?(*ptr++=x+'0'):(wr(x/10),*ptr++=(x%10)+'0'); }

int main() {
	int i,j,k,n;
	while ( 1 == scanf("%s",s) ) {
		for ( n = strlen(s), ptr = e, i = 0; i < n; i = j ) {
			for ( j = i+1; j < n && s[i] == s[j]; ++j ) ;
			if ( j-i >= 4 )
				wr(j-i), *ptr++='!', *ptr++=s[i];
			else for ( k = j-i; k--; *ptr++ = s[i] ) ;
		}
		*ptr = '\0', puts(e);
	}
	return 0;
}

