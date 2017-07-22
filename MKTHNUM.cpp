/*
 * MKTHNUM
 * TOPIC: persistent segment tree
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
#include <map>
#define MAXK (19)
#define N (1<<MAXK)
using namespace std;

int getnum() {
	int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

typedef struct cell {
	cell *left,*right;
	int card;
	cell( int card, cell *left, cell *right ) {
		this->card = card, this->left = left, this->right = right;
	}
} cell;

int n,weight[N],V;
set<int> s;
map<int,int> m,rm;
cell *null = new cell(0,NULL,NULL), *root[N];

cell *insert( cell *x, int i, int j, int val ) {
	if ( val < i || val > j ) return x;
	if ( i == j )
		return new cell(x->card+1,null,null);
	int k = (i+j)/2;
	return new cell(x->card+1,insert(x->left,i,k,val),insert(x->right,k+1,j,val));
}

int query( cell *x, cell *y, int i, int j, int k ) {
	if ( i == j ) return i;
	int cnt;
	if ( (cnt = y->left->card - x->left->card) >= k ) 
		return query(x->left,y->left,i,(i+j)/2,k);
	return query(x->right,y->right,(i+j)/2+1,j,k-cnt);
}

int main() {
	int i,j,k,qr;
	n = getnum(), qr = getnum();
	for ( V = 0, i = 0; i < n; s.insert(weight[i++] = getnum()) ) ;
	for ( set<int> :: iterator it = s.begin(); it != s.end(); rm[m[*it]=V++]=*it, ++it ) ;
	for ( null->left = null->right = null, root[0] = insert(null,0,V-1,m[weight[0]]), i = 1; i < n; ++i )
		root[i] = insert(root[i-1],0,V-1,m[weight[i]]);
	for ( ;qr--; i = getnum()-1, j = getnum()-1, k = getnum(), printf("%d\n",rm[query(i==0?null:root[i-1],root[j],0,V-1,k)]) ) ;
	return 0;
}

