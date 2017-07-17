/*
 * YODANESS 
 * TOPIC: unordered_map, merge sort, inversions
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
#include <unordered_map>
typedef long long i64;
#define tol (1e-13)
#define N (1<<15)
using namespace std;

unordered_map<string,int> m;
char buff[1<<21],*ptr;
int n,a[N],b[N];

i64 f( int *x, int n ) {
	if ( n <= 1 ) return 0;
	int k = n/2,i,j,
		*L = (int *)malloc(n*sizeof *L), *ptr,*qtr,*str;
	for ( j = 0, i = 0; i < n; L[j++] = x[i++] ) ;
	i64 ans = f(L,k)+f(L+k,n-k);
	for ( str = x, ptr = L, qtr = L+k; ptr < L+k && qtr < L+n; )
		if ( *ptr <= *qtr ) 
			*str++ = *ptr++;
		else {
			assert( *ptr > *qtr ) ;
			ans += L+k-ptr;
			*str++ = *qtr++;
		}
	for ( ;ptr < L+k; *str++ = *ptr++ ) ;
	for ( ;qtr < L+n; *str++ = *qtr++ ) ;
	free(L);
	return ans;
}

int main() {
	int i,j,k,ts;
	char tmp[0x20];
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d\n",&n); ) {
		m.clear(), ptr = fgets(buff,sizeof buff,stdin);
		for ( i = 0; i < n; ++i ) {
			scanf("%s\n",tmp);
			string str(tmp);
			if ( !m.count(str) )
				k = m.size(), m[str] = k;
			b[i] = m[str];
		}
		for ( i = 0; 1 == sscanf(ptr,"%s%n",tmp,&j); ptr += j, ++i )
			a[i] = m[string(tmp)];
		printf("%lld\n",f(a,n));
	}
	return 0;
}

