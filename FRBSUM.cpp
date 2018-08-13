/*
 * TOPIC: persistent range trees, binary search, constructive algorithms
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
#define N (1<<17)
#define Q (N<<4)
typedef int value_type;
typedef int size_type;

constexpr value_type neutral_element= 0;

typedef struct cell {
	struct cell *left, *right;
	value_type sum;
	size_type i,j;
	cell() {};
} cell;

cell pool[Q],*ptr;

cell *make_cell( cell *px= NULL ) {
	cell *x= ptr++;
	if ( px ) {
		x->i= px->i, x->j= px->j, x->sum= px->sum;
		x->left= px->left, x->right= px->right;
	}
	else x->left= x->right= NULL, x->sum= 0;
	return x;
}

int n;
pair<value_type,size_type> vec[N];
cell *root[N];

cell *update( cell *x, const size_type pos, const value_type val ) {
	assert( x->i <= pos && pos <= x->j );
	cell *nx= make_cell(x);
	nx->sum+= val;
	if ( x->i == x->j ) return nx;
	size_type k= (x->i+x->j)>>1;
	if ( pos <= k ) 
		nx->left = update(x->left, pos,val);
	else 
		nx->right= update(x->right,pos,val);
	return nx;
}

cell *build( size_type i, size_type j ) {
	cell *x= make_cell();
	if ( (x->i= i) == (x->j= j) ) 
		return x;
	int k= (i+j)>>1;
	x->left= build(i,k), x->right= build(k+1,j);
	return x;
}

value_type query( cell *x, const size_type qi, const size_type qj ) {
	if ( !x || x->i > qj || x->j < qi )
		return neutral_element;
	if ( qi <= x->i && x->j <= qj )
		return x->sum;
	return query(x->left,qi,qj)+query(x->right,qi,qj);
}

value_type 
solve_query( const size_type i, const size_type j ) {
	value_type sum, new_sum;
	size_type id;
	for ( ;; sum= new_sum ) {
		int low, high, mid;
		if ( vec[1].first > sum+1 )
			id= 1;
		else {
			assert( vec[low= 1].first <= sum+1 );
			if ( vec[high= n].first <= sum+1 )
				id= n+1;
			else {
				assert( vec[high].first > sum+1 );
				for ( ;low+1 < high; vec[mid= (low+high)>>1].first > sum+1 ? (high= mid) : (low= mid) ) ;
				id= high;
			}
		}
		if ( (new_sum= query(root[id-1],i,j)) == sum )
			break ;
	}
	return sum+1;
}

int main() {
	int i,j,k,qr;
	for (;1==scanf("%d",&n);) {
		for ( i= 1; i <= n; scanf("%d",&vec[i].first), vec[i].second= i, ++i ) ;
		sort(vec+1,vec+n+1), ptr= pool, root[0]= build(1,n);
		for ( i= 1; i <= n; root[i]= update(root[i-1],vec[i].second,vec[i].first), ++i ) ;
		for ( scanf("%d",&qr); qr-- && 2 == scanf("%d %d",&i,&j); ) {
			assert( 1 <= i && i <= n );
			assert( 1 <= j && j <= n );
			assert( i <= j );
			printf("%d\n",solve_query(i,j));
		}
	}
	return 0;
}

