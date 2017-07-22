/*
 * GOODG
 * TOPIC: convex hull trick, binary search, DP
 * status: Accepted
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
#include <cfloat>
#include <set>
#define tol (1e-7)
#define inf (DBL_MAX-0x400)
#define N (1<<20)
#define oo (1LL<<61)
typedef long long i64;
using namespace std;

i64 a[N],d[N],z[N];
int n;

typedef struct line {
	i64 m,b; // y = mx+b
	double left;
	line( i64 m, i64 b ): m(m), b(b), left(-inf) {}
	line() {};
	double eval( const double &t ) const { return m*t+b; }
	int sign( const double &t ) const {
		double x = eval(t);
		if ( fabs(x) < 1e-5 ) return 0;
		return x>0?1:-1;
	}
	double intersection_with( const line &other ) const { 
		assert( other.m != this->m );
		return (other.b-this->b+0.00)/(this->m-other.m); 
	}
} line;

line L[N];

struct cmp_by_slope {
	bool operator () ( const int &p, const int &q ) {
		return L[p].m < L[q].m;
	}
};

struct cmp_by_left{
	bool operator () ( const int &p, const int &q ) {
		return L[p].left < L[q].left;
	}
};

bool is_above( const line &p, const line &q, const double &t ) {
	return p.eval(t) > q.eval(t);
}

bool bad( const line &p, const line &q, const line &r ) {
	i64 mi = (p.m-q.m), mj = (p.m-r.m),
		bi = (q.b-p.b), bj = (r.b-p.b);
	assert( mi > 0 ); assert( mj > 0 );
	return bi*mj <= bj*mi;
}

class set_of_lines {
private:
	set<int,cmp_by_slope> *s;
   	set<int,cmp_by_left> *p;
	void erase( int k ) { 
		assert( s->find(k) != s->end() );
		assert( p->find(k) != p->end() ) ;
		if ( s->size() == 1 ) {
			s->clear(), p->clear();
			return ;
		}
		set<int,cmp_by_left> :: iterator it = p->find(k), jt, kt;
		if ( it == p->begin() ) {
			p->erase(k), jt = p->begin(), p->erase(*jt), L[*jt].left = -inf, p->insert(*jt);
			s->erase(k);
		}
		else {
			jt = it, --jt, kt = it, ++kt;
			s->erase(k), p->erase(k);
			if ( kt != p->end() ) {
				p->erase(*kt), L[*kt].left = L[*kt].intersection_with(L[*jt]);
				if ( p->find(*kt) != p->end() ) {
					jt = p->find(*kt);
					printf("The line %lldx+%lld, with left = %lf versus %lldx+%lld, with left = %lf\n",L[*jt].m,L[*jt].b,L[*jt].left,L[*kt].m,L[*kt].b,L[*kt].left);
				}
				assert( p->find(*kt) == p->end() );
				p->insert(*kt);
			}
		}
		assert( s->size() == p->size() );
	}
	void insert( int k ) { 
		assert( s->find(k) == s->end() );
		assert( p->find(k) == p->end() ) ;
		s->insert(k), p->insert(k);
	}
	void add_from_end( const int k ) {
		if ( !s->size() ) {
nx:
			L[k].left = -inf, insert(k);
			assert( s->size() == p->size() );
			return ;
		}
		set<int,cmp_by_slope> :: reverse_iterator it = s->rbegin(), jt;
		if ( L[*it].m == L[k].m ) {
			//printf("Found duplicate for %lldx+%lld\n",L[k].m,L[k].b);
			if ( L[*it].b >= L[k].b ) return ;
			erase(*it);
		}
		if ( !s->size() ) goto nx;
		for ( ;s->size() >= 2; ) {
			it = s->rbegin(), jt = it, ++jt;
			assert( L[k].m != L[*it].m );
			assert( L[k].m != L[*jt].m );
			//if ( L[k].intersection_with(L[*it]) <= L[k].intersection_with(L[*jt]) )
			assert( L[k].m > L[*it].m );
			if ( bad(L[k],L[*it],L[*jt]) )
				erase(*it);
			else break ;
		}
		assert( s->size() == p->size() );
		it = s->rbegin();
		if ( it == s->rend() ) L[k].left = -inf;
		else L[k].left = L[k].intersection_with(L[*it]);
		insert(k);
		assert( s->size() == p->size() );
	}
	void add_from_beginning( const int k ) {
		assert( s->size() );
		set<int,cmp_by_slope> :: iterator it = s->begin(), jt;
		assert( L[*it].m > L[k].m ) ;
		for ( ;s->size() >= 2; ) {
			it = s->begin(), jt = it, ++jt;
			assert( L[k].m != L[*jt].m );
			assert( L[*jt].m != L[*it].m );
			//if ( L[*jt].intersection_with(L[*it]) <= L[*jt].intersection_with(L[k]) ) erase(*it);
			assert( L[*jt].m > L[*it].m );
			if ( bad(L[*jt],L[*it],L[k]) ) erase(*it);
			else break ;
		}
		it = s->begin();
		assert( s->size() == p->size() );
		p->erase(*it), L[*it].left = L[*it].intersection_with(L[k]), p->insert(*it);
		assert( s->size() == p->size() );
		L[k].left = -inf, insert(k);
		assert( s->size() == p->size() );
	}
public:
	set_of_lines() {
		s = new set<int,cmp_by_slope>(), p = new set<int,cmp_by_left>();
	}
	void add( const int k ) {

		if ( !s->size() ) {
			L[k].left = -inf, insert(k);
			return ;
		}
	
		set<int,cmp_by_slope> :: iterator kt = s->lower_bound(k);

		if ( kt != s->end() && L[*kt].m == L[k].m ) {
			if ( L[*kt].b >= L[k].b ) return ;
			erase(*kt);
		}

		
		assert( s->find(k) == s->end() );

		set<int,cmp_by_slope> :: iterator it = s->upper_bound(k);

		if ( it == s->end() ) {
			add_from_end(k);
			return ;
		}
		if ( it == s->begin() ) {
			add_from_beginning(k);
			return ;
		}

		int i,j,sgn;

		assert( s->size() >= 2 );

		if ( !is_above(L[k],L[*it],L[*it].left) ) return ;
		int sign = L[k].sign(L[*it].left);
		vector<int> to_remove;
		set<int,cmp_by_slope> :: iterator pt = it, nt = it, jt;

		/*if ( L[*--pt].m == L[k].m ) {
			if ( L[*pt].b >= L[k].b ) return ;
			erase(*pt);
			goto up;
		}

		assert( L[*pt].m < L[k].m );*/
		assert( pt != s->begin() );

		for ( --pt; pt != s->begin(); ) {
			jt = pt, --jt;
			//if ( L[k].intersection_with(L[*jt]) >= L[k].intersection_with(L[*pt]) ) erase(*pt);
			assert( L[k].m > L[*pt].m );
			if ( bad(L[k],L[*pt],L[*jt]) ) {
				erase(*pt);
				pt = jt;
			}
			else break ;
		}
		for ( nt = s->upper_bound(k); nt != s->end(); ) {
			jt = nt, ++jt;
			if ( jt == s->end() ) break ;
			//if ( L[*jt].intersection_with(L[*nt]) <= L[*jt].intersection_with(L[k]) ) {
			assert( L[*jt].m > L[*nt].m );
			if ( bad(L[*jt],L[*nt],L[k]) ) {
					/*
				printf("k = %d\n",k);
				printf("Going to erase %lldx+%lld, leaving %lldx+%lld\n",L[*nt].m,L[*nt].b,L[*jt].m,L[*jt].b);*/
				erase(*nt);
				/*printf("Ok with %d\n",k);*/
				nt = jt;
			}
			else break ;
		}
		//printf("Done with %d\n",k);
		/*
		for ( set<int,cmp_by_slope> :: iterator ut = s->begin(); ut != s->end(); ++ut )
			printf("%lldx+%lld, ",L[*ut].m,L[*ut].b);
		puts("");*/
		pt = s->lower_bound(k);
		assert( pt == s->end() || L[*pt].m > L[k].m );
		--pt;
		nt = s->upper_bound(k);
		if ( nt == s->end() ) {
			add_from_end(k);
			return ;
		}
		if ( nt == s->begin() ) {
			add_from_beginning(k);
			return ;
		}
		assert( nt != s->end() && L[*nt].m > L[k].m );
		assert( s->size() == p->size() );
		p->erase(*nt), L[*nt].left = L[*nt].intersection_with(L[k]), p->insert(*nt);
		/*
		if ( p->find(*nt) != p->end() ) {
			set<int,cmp_by_left> :: iterator ut = p->find(*nt), bt = p->begin();
			set<int,cmp_by_left> :: reverse_iterator et = p->rbegin();
			printf("Already in the set: %lldx+%lld with left = %lf, while we tried to insert %lldx+%lld, with left = %lf\n",L[*ut].m,L[*ut].b,L[*ut].left,L[*nt].m,L[*nt].b,L[*nt].left);
			printf("The new entry line is %lldx+%lld\n",L[k].m,L[k].b);
			printf("Begin %lldx+%lld\n",L[*bt].m,L[*bt].b);
			printf("End %lldx+%lld\n",L[*et].m,L[*et].b);
		}*/
		assert( s->size() == p->size() );
		L[k].left = L[k].intersection_with(L[*pt]), insert(k);
		assert( s->size() == p->size() );
	}

	int query( const double t ) const {
		set<int,cmp_by_left> :: reverse_iterator it = p->rbegin();
		if ( L[*it].left <= t ) return *it;
		L[n+1].left = t;
		set<int,cmp_by_left> :: iterator jt = p->lower_bound(n+1);
		if ( fabs(L[*jt].left-t) < tol )
			return *jt;
		return *--jt;
	}
	void display() {
		for ( set<int,cmp_by_slope> :: iterator it = s->begin(); it != s->end(); ++it )
			printf("%lldx+%lld\n",L[*it].m,L[*it].b);
	}
};

int main() {
	int i,j,k,t;
	for ( ;1 == scanf("%d",&n) && n > 0; ) {
		for ( i = 1; i <= n; scanf("%lld %lld",a+i,d+i), ++i ) ;
		set_of_lines *s = new set_of_lines();
		for ( k = 1; k <= n; ++k ) {
			L[k].m = -d[k], L[k].b = d[k]*(k-1)+a[k]+z[k-1];
			s->add(k), t = s->query(k);
			/*s->display();
			puts("");*/
			z[k] = max(0LL,L[t].m*k+L[t].b);
		}
		printf("%lld\n",z[n]), delete s;
	}
	return 0;
}

