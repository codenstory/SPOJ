/*
 * TTM
 * TOPIC: persistent segment tree, с прибаалением на отр otrezke
 * status: Accepted
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long i64;
#define N (1<<21)
#define oo (1LL<<45)

typedef struct cell {
	struct cell *left, 
				*right;
	int i,j;
	i64 sum,h;
	cell() { left= right= NULL, sum= h= 0LL; };
	cell( int i, int j ) : i(i), j(j) { left= right= NULL, sum= h= 0LL; }
	cell( const struct cell *other ) { this->i= other->i, this->j= other->j, sum= h= 0LL, left= right= NULL; }
	int length() const { return j-i+1; }
} cell;

int n,tick;
i64 A[N];
cell pool[N], *ptr, *r[N];
bool eof_reached;

cell *init( int i, int j ) {
	cell *x= ptr++;
	x->i= i, x->j= j, x->left= x->right= NULL, x->sum= x->h= 0;
	return x;
}

void build( cell **v, int i, int j ) {
	assert( i <= j );
	*v= init(i,j);
	if ( i == j ) {
		(*v)->sum= A[i];
		return ;
	}
	int k= (i+j)>>1;
	build(&((*v)->left),i,k), build(&((*v)->right),k+1,j);
	(*v)->sum= (*v)->left->sum+(*v)->right->sum;
}

/*
 * (*v) and "u" are both cells at the same role,
 * but at different trees in history
 */
void update( cell **v, cell *u, int qi, int qj, i64 dt ) {
	if ( qi > u->j || qj < u->i ) {
		*v= u;
		return ;
	}
	(*v)= init(u->i,u->j), (*v)->sum= u->sum, (*v)->h= u->h;
	if ( qi <= u->i && u->j <= qj ) {
		(*v)->h+= dt;
		(*v)->left= u->left, (*v)->right= u->right;
		return ;
	}
	update(&((*v)->left),u->left,qi,qj,dt), update(&((*v)->right),u->right,qi,qj,dt);
	(*v)->sum+= dt*(min(u->j,qj)-max(u->i,qi)+1);
}

i64 query( cell *u, int qi, int qj, i64 val ) {
	if ( u->i > qj || u->j < qi )
		return 0LL;
	val+= u->h;
	if ( qi <= u->i && u->j <= qj ) 
		return u->sum+val*u->length();
	i64 res= query(u->left,qi,qj,val)+query(u->right,qi,qj,val);
	return res;
}

i64 rint() {
	static int isdig[256]= {0};
	i64 ch,n=0,sign=1;
	if ( !isdig['0'] )
		for ( ch= '0'; ch <= '9'; isdig[ch++]= 1 ) ;
	for ( ;(ch= getchar_unlocked()) != EOF && !(ch == '-' || isdig[ch]); ) ;
	if ( ch == EOF ) {
		eof_reached= true ;
		return +oo;
	}
	if ( ch == '-' ) 
		sign= -1, ch= getchar_unlocked();
	for ( n=ch-'0'; (ch= getchar_unlocked()) != EOF && isdig[ch]; n= 10*n+(ch-'0') ) ;
	return n*sign;
}

int next_command() {
	int ch;
	for ( ;(ch= getchar_unlocked()) != EOF && !(ch=='B'||ch=='C'||ch=='H'||ch=='Q'); ) ;
	return ch;
}

int main() {
	int qr,i,j,k,left,right,timestamp,ch;
	i64 dt;
	char tmp[0x400];
	for ( eof_reached= false; (dt= rint()) < +oo; ) {
		n= dt, qr= rint();
		for ( i= 0; i < n; scanf("%lld",&A[i++]) ) ;
		for ( j= 0; j <= qr; r[j++]= NULL ) ;
		ptr= pool, tick= 0, build(r+tick,0,n-1);
		for (;qr--;) {
			ch= next_command();
			switch ( ch ) {
				case 'B': timestamp= rint();
						  assert( timestamp <= tick );
						  tick= timestamp, r[tick+1]= NULL;
						  break ;
				case 'C': 
						  left= rint(), right= rint(), dt= rint();
						  --left, --right;
						  if ( left > right ) swap(left,right);
						  ++tick, update(r+tick,r[tick-1],left,right,dt);
						  break ;
				case 'H': 
						  left= rint(), right= rint(), timestamp= rint();
						  --left, --right;
						  if ( left > right ) swap(left,right);
						  assert( timestamp <= tick );
						  printf("%lld\n",query(r[timestamp],left,right,0));
						  break ;
				case 'Q': 
						  left= rint(), right= rint();
						  --left, --right;
						  if ( left > right ) swap(left,right);
						  printf("%lld\n",query(r[tick],left,right,0));
						  break ;
				default: assert(0);
			}
		}
	}
	return 0;
}

