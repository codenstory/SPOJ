/*
 * KQUERY
 * TOPIC: Wavelet Trees
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
#include <set>
#define tol (1e-13)
#define N (1<<16)
using namespace std;
enum { L, R };

int getnum() {
	register int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

class Cell {
private:
	int l,r,mv,*C,n;
	bool is_leaf;
	Cell *son[2];
	int map_left( int i ) const { return i-C[i]; }
	int map_right( int i ) const { return C[i]-1; }
	int greater_or_equal( int i, int val ) const {
		if ( i < 0 ) return 0;
		if ( is_leaf ) 
			return l>=val?i+1:0;
		if ( val <= mv )
			return son[L]->greater_or_equal(map_left(i),val)+map_right(i)+1;
		return son[R]->greater_or_equal(map_right(i),val);
	}
public:
	Cell( int l, int r, int *A, int n ) {
		this->l = l, this->r = r, mv = (l+r)>>1, this->n = n;
		if ( this->is_leaf = (l==r) ) {
			son[L] = son[R] = NULL;
			return ;
		}
		assert( l < r );
		int *x[2],cn[2] = {0},i,j,k,t;
		for ( i = 0; i < n; ++i )
			if ( A[i] <= mv ) ++cn[L];
			else ++cn[R];
		for ( t = L; t <= R; ++t )
			x[t] = (int *)malloc(cn[t]*sizeof *x[t]);
		C = (int *)malloc(n*sizeof *C);
		for ( C[0] = (A[0]>mv?1:0), i = 1; i < n; C[i]=C[i-1]+(A[i]>mv?1:0), ++i ) ;
		for ( cn[L] = cn[R] = 0, i = 0; i < n; ++i )
			if ( A[i] <= mv ) x[L][cn[L]++] = A[i];
			else x[R][cn[R]++] = A[i];
		son[L] = new Cell(l,mv,x[L],cn[L]), son[R] = new Cell(mv+1,r,x[R],cn[R]);
		free(x[L]), free(x[R]);
	}
	int query( int i, int j, int val ) const {
		int a = greater_or_equal(j,val), b = greater_or_equal(i-1,val);
		return a-b;
	}
} *root;

int n,A[N],S,values[N],m;
set<int> s;
unordered_map<int,int> mp;

int successor( int k ) {
	int low,high,mid;

	if ( mp.count(k) ) {
		if ( values[m-1] <= k )
			return -1;
		return mp[k]+1;
	}

	if ( m == 1 ) {
		if ( values[0] <= k ) return -1;
		return 0;
	}
	assert( m >= 2 );
	if ( values[m-1] <= k ) 
		return -1;
	assert( values[m-1] > k );
	if ( values[0] == k )
		return 1;
	if ( values[0] > k )
		return 0;
	assert( values[0] < k );
	for ( low=0, high=m-1; low+1<high; values[mid=(low+high)/2]>k?(high=mid):(low=mid) ) ;
	assert( values[high] > k );
	assert( values[low] <= k );
	return high;
}

int main() {
	int i,j,k,qr,low,high,res,bf;
	for ( m = 0, s.clear(), mp.clear(), n = getnum(), i = 0; i < n; s.insert(A[i++] = getnum()) ) ;
	for ( set<int> :: iterator it = s.begin(); it != s.end(); values[m++] = *it, k=mp.size(), mp[*it++] = k ) ;
	for ( S = mp.size(), i = 0; i < n; A[i] = mp[A[i]], ++i ) ;
	for ( root = new Cell(0,S-1,A,n), qr = getnum(); qr--; ) {
		i = getnum()-1, j = getnum()-1;
		assert( i <= j );
		k = successor(getnum());
		if ( k == -1 ) {
			puts("0");
			continue ;
		}
		printf("%d\n",res = root->query(i,j,k));
		/*
		for ( bf = 0, low = i; low <= j; ++low )
			if ( A[low] >= k ) ++bf;
		assert( bf == res );*/
	}
	return 0;
}

