/*
 * ILKQUERY2
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
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>
#define tol (1e-13)
#define N (1<<18)
#define LSB(k) ((k)&((~(k))+1ULL))
using namespace std;
enum { L, R };

int n,A[N];
unordered_map<int,int> mp,rmap;
bool is_active[N];

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
	int prefix( unsigned int i ) {
		int s = 0;
		for ( ;i; s += tr[i], i &= ~LSB(i) ) ;
		return s;
	}
	int sum( unsigned int i, unsigned int j ) {
		if ( i > j ) return 0;
		return prefix(j)-prefix(i-1);
	}
	BIT( int n ) {
		for ( this->n = n, K = 0; (1<<K) <= n; ++K ) ;
		tr = (int *)calloc((1<<K),sizeof *tr);
		for ( int i = 1; i <= n; update(i++,1) ) ;
	}
	int status_check( unsigned int i ) {
		return sum(i,i);
	}
	~BIT() { }
};

class Node {
private:
	int l,r,mv,*C;
	int map_left( int i ) const { return i-C[i];  };
	int map_right( int i ) const { return C[i]-1; };
	bool is_leaf;
	Node *son[2];
	BIT *left_bit, *right_bit, *leaf_bit;
	int active_left( int i ) const {
		return left_bit->prefix(i+1);
	}
	int active_right( int i ) const {
		return right_bit->prefix(i+1);
	}
	int active_leaf( int i ) const {
		return leaf_bit->prefix(i+1);
	}
	int query( int i, int val ) {
		if ( i < 0 ) return 0;
		if ( is_leaf )
			return l == val?leaf_bit->prefix(i+1):0;
		assert( l <= val && val <= r );
		return val<=mv ? son[L]->query(map_left(i),val) : son[R]->query(map_right(i),val);
	}
public:
	Node( int l, int r, int *A, int n ) {
		int i,j,k,lft = 0,*left,*right,llen,rlen;
		this->l = l, this->r = r, this->mv = (l+r)>>1, is_leaf = (l==r);

		assert( l <= r );

		if ( is_leaf ) {
			son[L] = son[R] = NULL;
			leaf_bit = new BIT(n);
			return ;
		}

		for ( i = 0; i < n; ++i )
			if ( A[i] <= mv && ++lft ) ;
		left = (int *)malloc((lft+1)*sizeof *left);
		right = (int *)malloc((n-lft+1)*sizeof *right);
		for ( llen = rlen = 0, i = 0; i < n; A[i]<=mv?(left[llen++]=A[i]):(right[rlen++]=A[i]), ++i ) ; 
		son[L] = new Node(l,mv,left,llen), son[R] = new Node(mv+1,r,right,rlen);
		free(left), free(right);
		C = (int *)calloc(n,sizeof *C);
		for ( C[0]=(A[0]>mv?1:0), i=1; i<n; C[i]=C[i-1]+(A[i]>mv?1:0), ++i ) ;
		//left_bit = new BIT(llen), right_bit = new BIT(rlen);
	}
	~Node() {
		//free(C);
	}
	void toggle( int val, int idx, bool flag ) {
		if ( is_leaf ) {
			leaf_bit->update(idx+1,flag?-1:1);
		}
		else {
			if ( val <= mv ) {
				//left_bit->update(idx+1,current_state?-1:1);
				son[L]->toggle(val,map_left(idx),flag);
			}
			else {
				//right_bit->update(idx+1,current_state?-1:1);
				son[R]->toggle(val,map_right(idx),flag);
			}
		}
	}
	int query( int i, int j, int val ) {
		return query(j,val)-query(i-1,val);
	}
} *root;

int main() {
	int i,j,l,k,qr,r,command,S;
	for ( ;; ) {
		n = getnum(), qr = getnum();
		for ( mp.clear(), rmap.clear(), i = 0; i < n; ++i ) {
			A[i] = getnum();
			if ( mp.find(A[i]) != mp.end() ) continue ;
			k = mp.size(), mp[A[i]] = k, rmap[k] = A[i];
		}
		for ( S = mp.size(), i = 0; i < n; A[i] = mp[A[i]], is_active[i] = true, ++i ) ;
		root = new Node(0,S-1,A,n);
		for ( ;qr--; ) {
			command = getnum();
			switch ( command ) {
				case 0: i = getnum(), l = getnum(), k = getnum();
						if ( i > l ) {
							puts("0");
							break ;
						}
						if ( i > l ) swap(i,l);
						if ( !mp.count(k) ) {
							puts("0");
							continue ;
						}
						k = mp[k];
						printf("%d\n",root->query(i,l,k));
						break ;
				default: r = getnum();
						 root->toggle(A[r],r,is_active[r]);
						 is_active[r] ^= true ;
						 break ;
			}
		}
		break ;
	}
	return 0;
}

