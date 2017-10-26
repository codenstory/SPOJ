/*
 * GREMLINS
 * TOPIC: longest path in an implicit tree, matrix expo, trick with binary search
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
#define N 0x80
#define M 0x400
typedef unsigned long long u64;
using namespace std;
#define IN(x) ((x))
#define OUT(x) ((x)+n)
//#define oo ((u64)((1e+18)+1))
#define oo (0xffffffffffffffffull)

int m[N],delay[N],
	type[N][M],time_to_hatch[N][M];
u64 T,A[N][N],n;

u64 getnum() {
	register int ch,sign = 1;
	u64 n;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch == EOF ) return +oo;
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return n;
}

u64 ax[N][N];
u64 **power[64],**s,
	**left_matrix;
/* O(log(len) x n^3) */
u64 matrixpow( u64 len ) {
	int i,j,k;
	u64 x[N][N],t[N][N],ans = +oo;

	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			x[i][j] = A[i][j], ax[i][j] = (i==j?0:+oo);

	for ( ;len; len >>= 1 ) {
		if ( len & 1 ) {
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					for ( t[i][j] = +oo, k = 0; k < n; ++k )
						if ( ax[i][k] < +oo && x[k][j] < +oo )
							if ( ax[i][k]+x[k][j] < t[i][j] && ax[i][k]+x[k][j] <= T )
								t[i][j] = ax[i][k]+x[k][j];
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					ax[i][j] = t[i][j];
		}
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				for ( t[i][j] = +oo, k = 0; k < n; ++k )
					if ( x[i][k] < +oo && x[k][j] < +oo )
						if ( x[i][k]+x[k][j] < t[i][j] && x[i][k]+x[k][j] <= T )
							t[i][j] = x[i][k]+x[k][j];
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				x[i][j] = t[i][j];
	}

	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			if ( ans > (s[i][j] = ax[i][j]) )
				ans = ax[i][j];

	return ans;

}

u64 combine( u64 **a, u64 **b ) {
	u64 t[N][N],ans = +oo;
	int i,j,k;
	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			for ( t[i][j] = +oo, k = 0; k < n; ++k )
				if ( a[i][k] <= T && b[k][j] <= T )
					if ( a[i][k]+b[k][j] < t[i][j] )
						t[i][j] = a[i][k]+b[k][j];
	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			if ( (s[i][j] = t[i][j]) <= T )
				if ( s[i][j] < ans )
					ans = s[i][j];
	return ans;
}

void copy( u64 **dst, u64 **src ) {
	for ( int i = 0; i < n; ++i )
		for ( int j = 0; j < n; ++j )
			dst[i][j] = src[i][j];
}

void allocate( u64 ***x ) {
	(*x) = (u64 **)malloc(n*sizeof **x);
	for ( int i = 0; i < n; ++i )
		(*x)[i] = (u64 *)malloc(n*sizeof *(*x)[i]);
}

int main() {
	int i,j,l,k,t,mindelay,nt;
	u64 low, high, mid, prev, answer;
	for ( ;(n = getnum()) < +oo; ) {
		T = getnum();
		for ( mindelay = 0x400, i = 0; i < n; ++i ) {
			m[i] = getnum(), delay[i] = getnum(), mindelay = min(mindelay,delay[i]);
			for ( j = 0; j < m[i]; ++j )
				type[i][j] = getnum(), --type[i][j];
			for ( j = 0; j < m[i]; ++j ) 
				time_to_hatch[i][j] = getnum();
		}
		if ( T < mindelay ) {
			puts("0");
			break ;
		}
		for ( i = 0; i < n; ++i )
			for ( j = i; j < n; ++j )
				A[j][i] = A[i][j] = +oo;
		for ( i = 0; i < n; ++i ) 
			for ( l = 0; l < m[i]; ++l ) {
				j = type[i][l];
				if ( A[i][j] > (u64)time_to_hatch[i][l]+delay[i] )
					A[i][j] = time_to_hatch[i][l]+delay[i];
			}
		allocate(&s), allocate(&left_matrix);
		for ( t = 0; t < 64; ++t )
			allocate(&power[t]);
		//assert( matrixpow(T*1000+1) > T );
		//assert( matrixpow(0) <= T );
		bool stop = true;
		for ( t = 0, i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				if ( (power[t][i][j] = A[i][j]) <= T )
					stop = false;

		if ( stop ) {
			puts("0");
			continue ;
		}
	
		for ( ;!stop && ++t < 64; ) 
			for ( stop = true, i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					for ( power[t][i][j] = +oo, k = 0; k < n; ++k )
						if ( power[t-1][i][k] <= T && power[t-1][k][j] <= T )
							if ( power[t-1][i][k]+power[t-1][k][j] <= T )
								if ( power[t-1][i][k]+power[t-1][k][j] < power[t][i][j] )
									power[t][i][j] = power[t-1][i][k]+power[t-1][k][j], stop = false ;
		assert( t >= 1 );
		/* now power[t-1] is still good, but power[t] is bad */
		for ( answer = (1ULL<<(t-1)), copy(left_matrix,power[t-1]), nt = t-2; nt >= 0; --nt ) 
			if ( combine(left_matrix,power[nt]) <= T ) 
				copy(left_matrix,s), answer |= (1ULL<<nt);
		printf("%llu\n",answer);
		/*
		for ( low = 0, high = T+1; low+1 < high; ) {
			if ( matrixpow(mid = (low+high)>>1) <= T )
				low = mid;
			else high = mid;
		}
		printf("%llu\n",low);
		*/
	}
	return 0;
}

