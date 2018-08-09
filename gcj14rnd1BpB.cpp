/*
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 64
#define BIT(k) (1ULL<<(k))
#define MASK(k) (k==N?+oo:(BIT(k)-1ULL))
#define TST(k,i) (((k) >> (i)) & 1ULL)
typedef unsigned long long u64;
using namespace std;
#define oo (0xffffffffffffffffull)

u64 z[N+1][N+1][N+1],A,B,K,
	a[N+1][N+1],b[N+1][N+1],c[N+1],
	pw[N+1];
int M;

/**
 * returns the most significant "k" bits of a "w"-width word "n"
 */
u64 highest( u64 n, int k, int w ) { 
	assert( k <= w );
	return (n&MASK(w)) & (MASK(k)<<(w-k));
}

u64 calc_c( int m ) {
	if ( c[m] < +oo )
		return c[m];
	int t= 0;
	u64 KK= (K&MASK(m));
	c[m]= 0;
	for ( int i= 0; i < m; ++i )
		if ( TST(KK,m-i-1) )
			c[m]+= pw[t+1]*BIT(m-i-1)*BIT(m-i-1);
		else ++t;
	return c[m];
}

u64 calc_a( int j, int m ) {
	if ( j < 0 ) {
		assert( j == -1 );
		return calc_c(m);
	}
	if ( a[j][m] < +oo )
		return a[j][m];
	if ( m == 1 ) {
		assert( j == 0 );
		return a[j][m]= TST(K,j)?2:0;
	}
	assert( m >= 2 );
	u64 KK= (K & MASK(m)),
		AA= (A & MASK(m)),
		BB= (B & MASK(m));
	a[j][m]= 0;
	for ( int i= 0; i <= j; ++i ) {
		u64 lcp= highest(BB,i,m),
			kk = highest(KK,i+1,m);
		if ( lcp < kk )
			a[j][m]+= BIT(m-i-1)*BIT(m-j-1);
		else if ( lcp == kk )
			a[j][m]+= calc_a(j-i-1,m-i-1);
	}
	u64 lcp= highest(BB,j,m),
		kk=  highest(KK,j+1,m);
	if ( kk > lcp )
		a[j][m]+= BIT(m-j-1)*BIT(m-j-1);
	else if ( kk == lcp )
		a[j][m]+= calc_c(m-j-1);
	return a[j][m];
}

u64 calc_b( int i, int m ) {
	if ( i < 0 ) {
		assert( i == -1 );
		return calc_c(m);
	}
	if ( b[i][m] < +oo )
		return b[i][m];
	if ( m == 1 ) {
		assert( i == 0 );
		return b[i][m]= TST(K,i)?2:0;
	}
	assert( m >= 2 );
	u64 KK= (K & MASK(m)),
		AA= (A & MASK(m)),
		BB= (B & MASK(m));
	b[i][m]= 0;
	for ( int j= 0; j <= i; ++j ) {
		u64 lcp= highest(AA,j,m),
			kk = highest(KK,j+1,m);
		if ( lcp < kk )
			b[i][m]+= BIT(m-j-1)*BIT(m-i-1);
		else if ( lcp == kk )
			b[i][m]+= calc_b(i-j-1,m-j-1);
	}
	u64 lcp= highest(AA,i,m),
		kk=  highest(KK,i+1,m);
	if ( kk > lcp )
		b[i][m]+= BIT(m-i-1)*BIT(m-i-1);
	else if ( kk == lcp )
		b[i][m]+= calc_c(m-i-1);
	return b[i][m];
}

/**
 * Let's assume we are talking about only
 * the least significant "m" bits of the numbers,
 * forgetting altogether about the uppermost bits.
 * Let us denote these "abridged" numbers as A',B',K'
 * f(i,j,m) answers the question: 
 * For how many pairs m-bit words "a" and "b"
 * does it hold that "a" has longest common prefix of length "i" with A', where i < m
 * 					 "b" has longest common prefix of length "j" with B', where j < m
 * 					 and (a & b) < K'?
 */
u64 f( int i, int j, int m ) {
	if ( i < 0 )
		return calc_a(j,m);
	if ( j < 0 )
		return calc_b(i,m);
	assert( i >= 0 && j >= 0 );
	if ( z[i][j][m] < +oo )
		return z[i][j][m];
	u64 AA= (A & MASK(m)),
		BB= (B & MASK(m)),
		KK= (K & MASK(m));
	if ( !TST(A,m-i-1) || !TST(B,m-j-1) )
		return z[i][j][m]= 0ULL;
	int t= min(i,j);
	u64 lcp= highest(AA&BB,t,m),
		kk=  highest(KK,t+1,m);
	if ( kk > lcp )
		return z[i][j][m]= BIT(m-i-1)*BIT(m-j-1);
	if ( kk == lcp )
		return z[i][j][m]= f(i-t-1,j-t-1,m-t-1);
	return z[i][j][m]= 0ULL;
}

u64 bf( u64 m, u64 n, u64 k ) {
	u64 res= 0;
	for ( u64 i= 0; i < m; ++i )
		for ( u64 j= 0; j < n; ++j )
			if ( (i&j) < k )
				++res;
	return res;
}

int main() {
	int i,j,k,cs= 0,ts;
	u64 ans; pw[0]= 1ULL;
	for ( u64 u= 1; u <= N; ++u )
		pw[u]= pw[u-1]*3ULL;
	for ( scanf("%d",&ts); ts--; printf("Case #%d: %llu\n",++cs,ans) ) {
		scanf("%llu %llu %llu",&A,&B,&K), memset(z,0xffull,sizeof z);
		if ( A > B ) swap(A,B);
		for ( M= 0; (1ULL << M) <= max(K,max(A,B)); ++M ) ;
		memset(a,0xffull,sizeof a), memset(b,0xffull,sizeof b), memset(c,0xffull,sizeof c);
		for ( ans= 0, i= 0; i < M; ++i )
			for ( j= 0; j < M; ans+= f(i,j++,M) ) ;
	}
	return 0;
}

