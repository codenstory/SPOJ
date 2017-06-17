/*
 * KAOS: Kaos
 * TOPIC: bitmasks, data-structures
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<17)
typedef unsigned long long u64;

u64 conv( char *s ) {
	int i,j,n = strlen(s);
	u64 u = 0;
	for ( j = 50, i = 0; j >= 0 && i < n; ++i, j -= 4 )
		u |= (((u64)(s[i]-'a')) << j);
	return u;
}

int n;

int main() {
	int i,j,k;
	char s[0x10],t[0x10];
	for ( ;1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; ++i ) {
			for ( scanf("%s",s), j = strlen(s)-1, k = 0; j >= 0; t[k++] = s[j--] ) ;
		}
	}
	return 0;
}

