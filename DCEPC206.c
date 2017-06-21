/*
 * DCEPC206
 * TOPIC: mergesort, neat interview
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<17)
typedef long long i64;

int n;
i64 c[N],ans;

void mergesort( i64 *x, int n ) {
	i64 *a,*b,*ptr,*qtr,*str = x;
	int i,j,k,m = (n>>1);
	if ( n <= 1 ) return ;
	ptr = a = (i64 *)malloc(m*sizeof *a);
	qtr = b = (i64 *)malloc((n-m)*sizeof *b);
	for ( k = 0, i = 0; i < m; a[k++] = x[i++] ) ;
	for ( k = 0; i < n; b[k++] = x[i++] ) ;
	assert( k == n-m ) ;
	for ( mergesort(a,m), mergesort(b,n-m); ptr < a+m && qtr < b+n-m; )
		if ( *ptr < *qtr ) 
			ans += (b+n-m-qtr)*(*ptr), *str++ = *ptr++;
		else *str++ = *qtr++;
	for ( ;ptr < a+m; *str++ = *ptr++ ) ;
	for ( ;qtr < b+n-m; *str++ = *qtr++ ) ;
	free(a), free(b);
}

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; scanf("%lld",&c[i++]) ) ;
		ans = 0, mergesort(c,n), printf("%lld\n",ans);
	}
	return 0;
}

