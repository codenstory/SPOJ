/*
 * PDECODE
 * TOPIC: matrix expo, permutation exponentiation
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 0x400

int m,n,p[N],invp[N];
char s[N];

void perm_pow( int *p, int n, int m ) {
	int ax[N],x[N],i,j,k,t[N];
	for ( i = 1; i <= n; ++i ) x[i] = p[i], ax[i] = i;
	for ( ;m; m >>= 1 ) {
		if ( m&1 ) {
			for ( i = 1; i <= n; ++i )
				t[i] = ax[x[i]];
			for ( i = 1; i <= n; ++i )
				ax[i] = t[i];
		}
		for ( i = 1; i <= n; ++i )
			t[i] = x[x[i]];
		for ( i = 1; i <= n; ++i )
			x[i] = t[i];
	}
	for ( i = 1; i <= n; ++i )
		p[i] = ax[i];
}


int main() {
	int i,j,k,ch;
	char t[N],*ptr;
	for ( ;2 == scanf("%d %d\r\n",&n,&m) && (m||n); ) {
		for ( i = 1; i <= n; ++i )
			scanf("%d",&p[i]), invp[p[i]] = i;
		perm_pow(invp,n,m);
		for ( i = 1; i <= n; s[i++] = ch ) 
			for ( ;(ch = getchar_unlocked()) == '\r' || ch == '\n' || ch == EOF; ) ;
		for ( ;(ch = getchar_unlocked()) != '\r' && ch != '\n' && ch != EOF; ) ;
		for ( i = 1; i <= n; ++i )
			t[i] = s[invp[i]];
		t[n+1] = '\0', puts(t+1);
	}
	return 0;
}

