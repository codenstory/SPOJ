/*
 * ILKQUERYIII
 * TOPIC: Wavelet Trees, BIT, nested binary search, swaps, quantile queries
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
#include <set>
#include <unordered_map>
#define LSB(k) ((k)&((~(k))+1ULL))
#define N (1<<20)
enum { L, R };
using namespace std;

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

class BIT {
private:
	int n,K,*tr;
public:
	void update( unsigned int i, int dt ) {
		for ( ;i < (1<<K); tr[i] += dt, i += LSB(i) ) ;
	}
	BIT( int n ) {
		for ( this->n = n, K = 0; (1<<K) <= n; ++K ) ;
		tr = (int *)calloc((1<<K),sizeof *tr);
	}
	int prefix( unsigned int i ) const {
		int s = 0;
		for ( ;i; s += tr[i], i &= ~LSB(i) ) ;
		return s;
	}
	int sum( int i, int j ) const {
		if ( i > j ) return 0;
		if ( i <= 0 ) return prefix(j);
		return prefix(j)-prefix(i-1);
	}
};

int A[N];

class Cell {
private:
	int l,r,mv,n;
	bool is_leaf;
	Cell *son[2];
	BIT *bit;
	int map_left( int i ) const { return i-bit->prefix(i+1); }
	int map_right( int i ) const { return bit->prefix(i+1)-1; }
	int num_of_occurrences( int i, int val ) const {
		if ( is_leaf ) {
			//assert( val == l );
			return i+1;
		}
		return val<=mv?son[L]->num_of_occurrences(map_left(i),val):son[R]->num_of_occurrences(map_right(i),val);
	}
	void swp( int i, int x, int y ) {

		if ( is_leaf || x == y ) {
			return ;
		}

		if ( x <= mv && mv < y ) {
			bit->update(i+2,-1), bit->update(i+1,1);
			return ;
		}
		if ( y <= mv && mv < x ) {
			bit->update(i+1,-1), bit->update(i+2,1);
			return ;
		}
		//assert( x <= mv && y <= mv || x > mv && y > mv ) ;
		if ( x <= mv && y <= mv ) 
			son[L]->swp(map_left(i),x,y);
		else 
			son[R]->swp(map_right(i),x,y);
	}
public:
	Cell( int l, int r, int *A, int n ) {
		mv = ((this->l=l)+(this->r=r))>>1; this->n = n;
		if ( is_leaf = (l==r) ) {
			son[L] = son[R] = NULL, bit = NULL;
			return ;
		}
		int *x[2],cn[2] = {0},i,j,k,t;
		for ( bit = new BIT(n), i = 0; i < n; ++i )
			if ( A[i] > mv ) bit->update(i+1,1);
		x[L] = (int *)malloc((n-bit->prefix(n))*sizeof *x[L]), x[R] = (int *)malloc(bit->prefix(n)*sizeof *x[R]);
		for ( i = 0; i < n; (A[i]<=mv?x[L][cn[L]++]=A[i++]:x[R][cn[R]++]=A[i++]) ) ;
		son[L] = new Cell(l,mv,x[L],cn[L]), son[R] = new Cell(mv+1,r,x[R],cn[R]);
		free(x[L]), free(x[R]);
	}
	int kth_occurrence( int k, int val ) const {
		int low, high, mid;
		if ( num_of_occurrences(n-1,val) < k+1 )
			return -1;
		assert( num_of_occurrences(n-1,val) >= k+1 );
		for(low=0,high=n-1;low+1<high;num_of_occurrences(mid=(low+high)/2,val)<k+1?(low=mid):(high=mid));
		assert( num_of_occurrences(high,val) == k+1 );
		return high;
	}
	int quantile( int i, int k ) const {
		if ( is_leaf )
			return l;
		return k<=map_left(i)?son[L]->quantile(map_left(i),k):son[R]->quantile(map_right(i),k-map_left(i)-1);
	}
	void swp( int i ) { swp(i,A[i],A[i+1]), swap(A[i],A[i+1]); }
} *root;

set<int> st;
unordered_map<int,int> mp;
int n;

void read_input() {
	int i,k;
	for ( i = 0; i < n; st.insert(A[i++]=getnum()) ) ;
	for ( set<int> :: iterator it = st.begin(); it != st.end(); k = mp.size(), mp[*it++] = k ) ;
	for ( i = 0; i < n; A[i] = mp[A[i]], ++i ) ;
}

int main() {
	int i,j,k,l,qr,command,t;
	for ( n = getnum(), qr = getnum(), read_input(), root = new Cell(0,mp.size()-1,A,n); qr--; ) {
		command = getnum();
		switch( command ) {
			case 0: i = getnum(), l = getnum(), k = getnum()-1;
					t = root->quantile(i,k);
					printf("%d\n",root->kth_occurrence(l-1,t));
					break ;
			case 1: 0&&printf("Update operation\n");
					root->swp(i=getnum());
					break ;
			default: assert(0);
		}
	}
	return 0;
}

