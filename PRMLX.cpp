/*
 * PRMLX
 * TOPIC: dp, profile, lex order
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#define N 32
#define FG (ptr=fgets(b,sizeof b,stdin))
typedef long long i64;
typedef unsigned long long u64;
#define MOD ((1ULL<<31)-1ULL)
#define M 11
using namespace std;

char b[N],*ptr;
int n,c[N],x[N],id[N],m;
i64 p[M] = {2,3,5,7,11,13,17,19,23,29,31};
vector<int> fact[N];

u64 g() {
	int sum = 0,i,j,k;
	u64 ans = 1ULL,x,ax;
	for ( i = 0; i < m; sum += c[i++] ) ;
	vector<int> vec;
	vec.clear();
	for ( i = 0; i < M; ++i )
		vec.push_back(fact[sum][i]);
	for ( i = 0; i < m; ++i ) 
		for ( j = 0; j < M; ++j )
			vec[j] -= fact[c[i]][j];
	for ( j = 0; j < M; ++j )
		assert( vec[j] >= 0 ) ;
	for ( i = 0; i < M; ans *= ax, ++i ) 
		for ( ax = 1ULL, x = p[i], k = vec[i]; k; k >>= 1, x *= x )
			if ( k&1 )
				ax *= x;
	return ans;
}

u64 f( int k, u64 rank ) {
	u64 w = 0;
	int i;
	if ( k == n ) 
		return rank;
	assert( k < n );
	for ( i = x[k]-1; i >= 0; --i ) 
		if ( c[i] ) 
			--c[i], w += g(), ++c[i];
	--c[x[k]];
	return f(k+1,rank+w);
}

int H( int n, i64 p ) {
	int s = 0;
	i64 x = p;
	for ( ;x<=n; s+=n/x, x*=p ) ;
	return s;
}

int main() {
	int i,j,k;

	for ( j = 0; j < M; fact[0].push_back(0), ++j ) ;
	for ( i = 1; i < N; ++i ) 
		for ( j = 0; j < M; fact[i].push_back(H(i,p[j])), ++j ) ;
	for (;FG&&*ptr!='#';) {
		if ( !('a' <= *ptr && *ptr <= 'z') ) {
			printf("%10d\n",1);
			continue ;
		}
		for ( n = 0; b[n] && b[n] != '\n'; ++n ) ;
		for ( memset(c,0,sizeof c), i = 0, b[n] = '\0'; i < n; ++c[b[i++]-'a'] ) ;
		for ( m = 0, memset(id,-1,sizeof id), i = 0; i < N; ++i )
			if ( c[i] ) 
				c[id[i] = m++] = c[i];
		for ( i = 0; i < n; x[i] = id[b[i]-'a'], ++i ) ;
		printf("%10llu\n",f(0,0)+1);
	}
	return 0;
}

