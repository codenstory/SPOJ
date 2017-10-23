/*
 * GREMLINS
 * TOPIC: longest path in an implicit tree, matrix expo
 * status: TLE
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

/* O(log(len) x n^3) */
u64 matrixpow( u64 len ) {
	int i,j,k;
	u64 ax[N][N],x[N][N],t[N][N],ans = +oo;

	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			x[i][j] = A[i][j], ax[i][j] = (i==j?0:+oo);

	for ( ;len; len >>= 1 ) {
		if ( len & 1 ) {
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					for ( t[i][j] = +oo, k = 0; k < n; ++k )
						if ( ax[i][k] < +oo && x[k][j] < +oo )
							if ( ax[i][k]+x[k][j] < t[i][j] )
								t[i][j] = ax[i][k]+x[k][j];
			for ( i = 0; i < n; ++i )
				for ( j = 0; j < n; ++j )
					ax[i][j] = t[i][j];
		}
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				for ( t[i][j] = +oo, k = 0; k < n; ++k )
					if ( x[i][k] < +oo && x[k][j] < +oo )
						if ( x[i][k]+x[k][j] < t[i][j] )
							t[i][j] = x[i][k]+x[k][j];
		for ( i = 0; i < n; ++i )
			for ( j = 0; j < n; ++j )
				x[i][j] = t[i][j];
	}

	for ( i = 0; i < n; ++i )
		for ( j = 0; j < n; ++j )
			if ( ans > ax[i][j] )
				ans = ax[i][j];

	return ans;

}

int main() {
	int i,j,l,k,mindelay;
	u64 low, high, mid;
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
		//assert( matrixpow(T*1000+1) > T );
		//assert( matrixpow(0) <= T );
		for ( low = 0, high = T+1; low+1 < high; ) {
			mid = (low+high)>>1;
			if ( matrixpow(mid) <= T )
				low = mid;
			else high = mid;
		}
		printf("%llu\n",low);
		/*
		if ( low & 1 ) {
			printf("%llu\n",(low>>1));
		}
		else {
			int res = (low>>1);
			printf("%d\n",max(0,res-1));
		}
		*/
		//break ;
	}
	return 0;
}

