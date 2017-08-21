/*
 * ILKQUERY
 * TOPIC: Wavelet Trees, Quantile Queries, nested binary search
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
#define N (1<<18)
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
	Cell *son[2];
	bool is_leaf;
	int l,r,mv,*C,n;
	int map_left( int i ) const { return i-C[i]; }
	int map_right( int i ) const { return C[i]-1; }
	int num_of_occurrences( int i, int val ) const {
		if ( is_leaf )
			return min(i+1,n);
		if ( val <= mv )
			return son[L]->num_of_occurrences(map_left(i),val);
		return son[R]->num_of_occurrences(map_right(i),val);
	}
public:
	Cell( int l, int r, int *A, int n ) {
		mv = ((this->l=l)+(this->r=r))>>1, this->n = n;
		if ( is_leaf = (l==r) ) {
			son[L] = son[R] = NULL;
			return ;
		}
		assert( l < r );
		int *x[2],cn[2] = {0},i,j,k,t;
		C = (int *)malloc(n*sizeof *C);
		for ( C[0]=(A[0]>mv?1:0), i=1; i<n; C[i]=C[i-1]+(A[i]>mv?1:0), ++i ) ;
		x[L] = (int *)malloc((n-C[n-1])*sizeof *x[L]);
		x[R] = (int *)malloc(C[n-1]*sizeof *x[R]);
		for ( i = 0; i < n; A[i]>mv?(x[R][cn[R]++]=A[i++]):(x[L][cn[L]++]=A[i++]) ) ;
		assert( cn[L] == n-C[n-1] );
		assert( cn[R] == C[n-1] );
		son[L] = new Cell(l,mv,x[L],cn[L]), son[R] = new Cell(mv+1,r,x[R],cn[R]);
		free(x[L]), free(x[R]);
	}
	int quantile_query( int i, int k ) const {
		if ( is_leaf )
			return l;
		if ( k > map_left(i) )
			return son[R]->quantile_query(map_right(i),k-map_left(i)-1);
		return son[L]->quantile_query(map_left(i),k);
	}
	int kth_occurrence( int val, int k ) const { 
		if ( num_of_occurrences(n-1,val) < k+1 )
			return -1;
		if ( num_of_occurrences(0,val) == k+1 )
			return 0;
		int low,high,mid;
		for ( high = n-1, low = 0; low+1<high; num_of_occurrences(mid=(low+high)/2,val)>=k+1?(high=mid):(low=mid) ) ;
		return high;
	}
} *root;

int n,A[N];
set<int> st;
unordered_map<int,int> mp,rmap;

int main() {
	int i,j,k,qr,t,l;
	n = getnum(), qr = getnum();
	for ( st.clear(), mp.clear(), rmap.clear(), i = 0; i < n; st.insert(A[i++]=getnum()) ) ;
	for ( set<int> :: iterator it = st.begin(); it != st.end(); rmap[k = mp.size()] = *it, mp[*it++] = k ) ;
	for ( i = 0; i < n; A[i] = mp[A[i]], ++i ) ;
	root = new Cell(0,mp.size()-1,A,n);
	for ( ;qr--; ) {
		k = getnum()-1, i = getnum(), l = getnum()-1;
		t = root->quantile_query(i,k);
		assert( t != -1 );
		printf("%d\n",root->kth_occurrence(t,l));
	}
	return 0;
}

