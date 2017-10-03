/*
 * SAMER08B
 * TOPIC: number bases, polynomials, finding roots
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
#include <cfloat>
#include <set>
#define oo (DBL_MAX-(1<<20))
#define tol (1e-11)
#define L 0x400
using namespace std;
typedef long long i64;

int maxdig;
i64 gcd( i64 x, i64 y ) { return !y?x:gcd(y,x%y); }

class Poly {
private:
	int n;
	i64 *c;
public:
	Poly( i64 x ) {
		vector<int> v;
		if ( x == 0 ) {
			c = (i64 *)malloc(sizeof *c);
			c[0] = 0, n = 1;
			return ;
		}
		for ( v.clear(); x; v.push_back(x%10), x /= 10 ) ;
		c = (i64 *)malloc(v.size()*sizeof *c);
		for ( int i = 0; i < (int)v.size(); c[i] = v[i], ++i ) ;
		n = v.size();
	}
	Poly( i64 *d, int m ) {
		c = (i64 *)malloc(m*sizeof *c), n = m;
		for ( int i = 0; i < n; ++i )
			c[i] = d[i];
		for ( ;n && !c[n-1]; --n ) ;
		if ( !n ) n = 1;
	}
	Poly( vector<int> &v ) {
		int m = v.size();
		c = (i64 *)malloc(m*sizeof *c), n = m;
		for ( int i = 0; i < n; ++i )
			c[i] = v[i];
		int i = n-1;
		for ( ;i && !c[i]; --i ) ;
		if ( i < 0 ) n = 1;

	}
	i64 operator [] ( int i ) const {
		return i<n?c[i]:0;
	}
	int deg() const {
		return n-1;
	}
	bool is_zero() {
		int i;
		for ( i = n-1; i >= 0 && !c[i]; --i ) ;
		return i < 0;
	}
	i64 eval( i64 t ) const {
		i64 s = 0;
		for ( int i = deg(); i >= 0; --i )
			s *= t, s += c[i];
		return s;
	}
	double eval_dbl( double t ) const {
		double s = 0;
		for ( int i = deg(); i >= 0; --i )
			s *= t, s += c[i];
		return s;
	}

	bool has_negative() const {
		for ( int i = 0; i < n; ++i )
			if ( c[i] < 0 )
				return true ;
		return false ;
	}
	void adjust( int idx, i64 g ) {
		c[idx] /= g;
	}
	Poly *derivative() const {
		i64 x[L];
		int i,j,k,m = 0;
		if ( deg() == 0 )
			return new Poly(0);
		for ( j = deg()-1, i = deg(); j >= 0; x[j] = i*c[i], --j, --i ) ;
		return new Poly(x,deg());
	}
	int signum( double x ) const {
		if ( x < 0 ) return -1;
		if ( x > 0 ) return 1;
		return 0;
	}
	double find_root( double a, double b ) const {
		int signa = signum(eval_dbl(a)), signb = signum(eval_dbl(b));
		if ( signa == 0 ) return a;
		if ( signb == 0 ) return b;
		if ( signa*signb > 0 ) return +oo;
		for (;fabs(b-a)>=tol;) {
			double mid = (a+b)/2;
			if ( signum(eval_dbl(mid)) == signa )
				a = mid;
			else b = mid;
		}
		return (a+b)/2;
	}
	void solve( set<double> &roots ) const {
		roots.clear();
		if ( deg() == 1 ) {
			roots.insert((-c[0]+0.00)/c[1]);
			return ;
		}
		if ( !deg() ) return ;
		assert( deg() >= 2 );
		Poly *r = this->derivative();
		set<double> rt;
		r->solve(rt);
		double prev;
		rt.insert(+oo);
		for ( set<double> :: iterator it = rt.begin(); it != rt.end(); prev = *it++ ) {
			if ( it == rt.begin() )
				prev = -oo;
			double root = find_root(prev,*it);
			if ( root < +oo )
				roots.insert(root);
		}
	}
};

Poly *operator - ( const Poly &a, const Poly &b ) {
	int i,j,k;
	i64 c[L] = {0};
	for ( i = 0; i <= a.deg() && i <= b.deg(); ++i )
		c[i] = a[i]-b[i];
	for ( ;i <= a.deg(); c[i] += a[i], ++i ) ;
	for ( ;i <= b.deg(); c[i] -= b[i], ++i ) ;
	return new Poly(c,i);
}

Poly *operator + ( const Poly &a, const Poly &b ) {
	int i,j,k;
	i64 c[L] = {0};
	for ( i = 0; i <= a.deg() && i <= b.deg(); c[i] = a[i]+b[i], ++i ) ;
	for ( ;i <= a.deg(); c[i] += a[i], ++i ) ;
	for ( ;i <= b.deg(); c[i] += b[i], ++i ) ;
	return new Poly(c,i);
}

Poly *operator * ( const Poly &a, const Poly &b ) {
	int i,j,k;
	i64 c[L] = {0};
	for ( i = 0; i <= a.deg(); ++i )
		for ( j = 0; j <= b.deg(); ++j )
			c[i+j] = a[i]*b[j];
	return new Poly(c,a.deg()+b.deg()+2);
}

typedef struct cell {
	struct cell *left, *right;
	char op;
	Poly *val;
} cell;

char buff[0x400], *ptr;

cell *f00() {
	cell *x;
	if ( '0' <= *ptr && *ptr <= '9' ) {
		x = (cell *)malloc(sizeof *x);
		vector<int> vec; vec.clear();
		for ( ;'0' <= *ptr && *ptr <= '9'; maxdig = max(maxdig,*ptr-'0'), vec.push_back(*ptr++-'0') ) ;
		reverse(vec.begin(),vec.end());
		x->left = x->right = NULL, x->op = '7', x->val = new Poly(vec);
		return x;
	}
	return NULL;
}

cell *f01( cell *left ) {
	cell *x;
	if ( *ptr == '*' ) {
		x = (cell *)malloc(sizeof *x);
		x->op = *ptr++, x->left = left, x->right = f00();
		return f01(x);
	}
	return left;
}

cell *f02( cell *left ) {
	cell *x;
	if ( *ptr == '+' ) {
		x = (cell *)malloc(sizeof *x);
		x->op = *ptr++, x->left = left, x->right = f01(f00());
		return f02(x);
	}
	return left;
}

Poly *eval( cell *x ) {
	if ( !x ) return NULL;
	if ( x->op == '7' )
		return x->val;
	if ( x->op == '+' ) 
		return *(eval(x->left)) + *(eval(x->right));
	return (*(eval(x->left)))*(*(eval(x->right)));
}

int is_integer( double x ) {
	return fabs(x-(i64)(x+tol)) < tol;
}

int main() {
	Poly *LHS,*RHS;
	for ( ;(ptr = fgets(buff,sizeof buff,stdin)) && *ptr != '='; ) {
		maxdig = 0, LHS = eval(f02(f01(f00())));
		assert( *ptr == '=' );
		++ptr, RHS = eval(f02(f01(f00())));
		Poly *res = *RHS-*LHS;
		if ( res->is_zero() ) {
			printf("%d+\n",max(2,maxdig+1));
			continue ;
		}

		if ( !res->has_negative() || res->deg() == 0 ) {
			puts("*");
			continue ;
		}

		i64 g = labs((*res)[0]);
		for ( int i = 1; i <= res->deg(); ++i )
			g = gcd(g,(*res)[i]);
		for ( int i = 0; i <= res->deg(); ++i )
			res->adjust(i,g);

		vector<i64> sol;
		sol.clear();
		/*
		i64 c0 = labs((*res)[0]);
		for ( i64 k = max(2,maxdig+1); k <= c0; ++k )
			if ( !(c0%k) && !(((*res)[0]/k+(*res)[1])%k) )
				if ( 0 == res->eval(k) )
					sol.push_back(k);
		*/
		set<double> roots;
		res->solve(roots);
		for ( set<double> :: iterator it = roots.begin(); it != roots.end(); ++it )
			if ( is_integer(*it) )
				if ( (i64)(*it+tol) >= max(2,maxdig+1) )
					sol.push_back(((i64 )(*it+tol)));
		if ( !sol.size() )
			puts("*");
		else {
			int i;
			for ( i = 0; i+1 < (int)sol.size(); ++i )
				printf("%lld ",sol[i]);
			printf("%lld\n",sol[i]);
		}
	}
	return 0;
}

