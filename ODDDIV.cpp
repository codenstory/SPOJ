/*
 * ODDDIV
 * TOPIC: number theory, binary search
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
#define N (1<<17)
#define K (10100)
using namespace std;
typedef long long i64;

int m,len;
bool isprime[N];
i64 *p;
vector<i64> v[K];

i64 f( i64 n ) {
	i64 s = 1LL, i, k;
	for ( i = 0; i < len && p[i]*p[i] <= n; ++i, s *= (1+2*k) ) 
		for ( k = 0; !(n%p[i]); ++k, n /= p[i] ) ;
	if ( n > 1 ) s *= 3;
	return s;
}

void sieve() {
	i64 i,j,k;
	for ( isprime[2] = true, i = 3; i < N; isprime[i] = true, i += 2 ) ;
	for ( i = 3; i < N; i += 2 )
		for ( j = i*i; j < N && isprime[i]; isprime[j] = false, j += i ) ;
	for ( i = 0; i < N; ++i )
		if ( isprime[i] ) ++len;
	p = (i64 *)malloc(len*sizeof *p);
	for ( k = 0, i = 0; i < N; ++i )
		if ( isprime[i] ) 
			p[k++] = i;
	for ( i = 1; i < N; v[f(i)].push_back(i*i), ++i ) ;
}

int main() {
	int i,j,k,ts,good,bad,mid,left,right;
	i64 low,high;
	for ( sieve(), scanf("%d",&ts); ts-- && 3==scanf("%d %lld %lld",&m,&low,&high); ) {
		if ( !v[m].size() || high < v[m][0] || low > v[m][v[m].size()-1] ) {
			puts("0");
			continue ;
		}
		assert( high >= v[m][0] ) ;
		assert( low <= v[m][v[m].size()-1] ) ;
		if ( v[m][0] >= low )
			left = 0;
		else {
			assert( v[m][0] < low );
			for ( good = v[m].size()-1, bad = 0; bad+1<good; v[m][mid=(good+bad)/2]<low?(bad=mid):(good=mid) ) ;
			left = good;
		}
		if ( high >= v[m][v[m].size()-1] )
			right = v[m].size()-1;
		else {
			assert( high < v[m][v[m].size()-1] );
			for ( good = 0, bad = v[m].size()-1; good+1<bad; v[m][mid=(good+bad)/2]>high?(bad=mid):(good=mid) ) ;
			right = good;
		}
		printf("%d\n",right-left+1);
	}
	return 0;
}

