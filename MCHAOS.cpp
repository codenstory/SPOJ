/*
 * MCHAOS
 * TOPIC: excellent interview bit, sorting, merge sort, inversions, encoding
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
#define L 10
#define N (1<<17)
#define x first
#define y second
using namespace std;
typedef unsigned long long u64;
#define gc (getchar_unlocked())

u64 read_string( u64 *v ) {
	u64 u = 0,d[0x10] = {0};
	int ch,i,j,k = 0;
	for ( ;(ch = gc) != EOF && !('a'<=ch&&ch<='z'); ) ;
	for ( d[k++] = ch-'a'+1; (ch = gc) != EOF && 'a' <= ch && ch <= 'z'; d[k++] = ch-'a'+1 ) ;
	assert( k <= L );
	for ( i = 0; i < L; ++i )
		u |= d[i]<<((L-i-1)*5);
	for ( i = 0, j = k-1; i < j; ++i, --j )
		swap(d[i],d[j]);
	for ( *v = 0, i = 0; i < L; ++i )
		*v |= d[i]<<((L-i-1)*5);
	return u;
}

u64 w[N],rw[N],res;
pair<u64,u64> p[N];
int n;

void merge_sort( u64 *x, int n ) {
	if ( n <= 1 )
		return ;
	int m = (n>>1),i,j,k;
	u64 *o = (u64 *)malloc(n*sizeof *o),
		*ptr = x, *qtr = x+m, *str = o;
	merge_sort(x,m), merge_sort(x+m,n-m);
	for ( ;ptr<x+m && qtr<x+n; *str++=*ptr<=*qtr?(*ptr++):((res+=(x+m-ptr)),*qtr++) ) ;
	for ( ;ptr<x+m; *str++=*ptr++ ) ;
	for ( ;qtr<x+n; *str++=*qtr++ ) ;
	for ( i = 0; i < n; x[i] = o[i], ++i ) ;
	free(o);
}

int main() {
	int i,j,k,ts;
	for ( ;1 == scanf("%d\n",&n); ) {
		for ( i = 0; i < n; w[i] = read_string(rw+i), ++i ) ;
		for ( i = 0; i < n; p[i] = make_pair(w[i],rw[i]), ++i ) ;
		sort(p,p+n);
		for ( i = 0; i < n; w[i] = p[i].second, ++i ) ;
		res = 0, merge_sort(w,n), printf("%llu\n",res);
	}
	return 0;
}



