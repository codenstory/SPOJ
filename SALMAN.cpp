/*
 * SALMAN (Raduyev)
 * TOPIC: dfs, preorder, segment tree, lazy propagation, subtree updates
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#define N (1<<21)
#define MAXE (N)
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
typedef long long i64;
#define oo (1LL<<30)
using namespace std;

int E,n,to[MAXE],last[N],next[MAXE],p[N],boss[N],left[N],right[N],
	preorder_sequence[N],*ptr,seen[N],yes,pos[N],card[N];
i64 salary[N],a[N];

void add_arcs( int x, int y ) {
	int i = E++;
	to[i] = y, next[i] = last[x], last[x] = i;
}

void preorder( int x ) {
	int i;
	assert( seen[x] != yes ) ;
	for ( card[x] = 1, left[x] = ptr-preorder_sequence, seen[*ptr++ = x] = yes, i = last[x]; i >= 0; preorder(to[i]), card[x] += card[to[i]], i = next[i] ) ;
	right[x] = ptr-1-preorder_sequence;
}

class ST {
private:
	int n;
	i64 *a;
	i64 *sum,*min_salary,*dt;

	void push_up( int v, int i, int j ) {
		if ( i < j ) {
			min_salary[v] = min(min_salary[L(v)],min_salary[R(v)]);
			sum[v] = sum[L(v)]+sum[R(v)];
		}
	}
	void build( int v, int i, int j ) {
		dt[v] = 0;
		if ( i == j ) {
			min_salary[v] = sum[v] = this->a[i];
			return ;
		}
		int k = (i+j)>>1;
		build(L(v),i,k), build(R(v),k+1,j), push_up(v,i,j);
	}
	void add_value( int v, int i, int j, i64 ddt ) {
		if ( ddt ) {
			sum[v] += (j-i+1)*ddt, min_salary[v] += ddt;
			if ( i < j ) dt[v] += ddt;
		}
	}
	void push_down( int v, int i, int j ) {
		if ( dt[v] ) {
			assert( i < j );
			int k = (i+j)>>1;
			add_value(L(v),i,k,dt[v]), add_value(R(v),k+1,j,dt[v]), dt[v] = 0;
		}
	}
	void update( int v, int i, int j, int qi, int qj, i64 ddt ) {
		if ( qi > j || qj < i ) return ;
		push_down(v,i,j);
		if ( qi <= i && j <= qj ) {
			add_value(v,i,j,ddt);
			return ;
		}
		int k = (i+j)>>1;
		update(L(v),i,k,qi,qj,ddt), update(R(v),k+1,j,qi,qj,ddt), push_up(v,i,j);
	}
	i64 min_query( int v, int i, int j, int qi, int qj ) {
		if ( qj < i || qi > j ) return +oo;
		push_down(v,i,j);
		if ( qi <= i && j <= qj ) 
			return min_salary[v];
		int k = (i+j)>>1;
		i64 left = min_query(L(v),i,k,qi,qj), right = min_query(R(v),k+1,j,qi,qj);
		push_up(v,i,j);
		return min(left,right);
	}
	i64 sum_query( int v, int i, int j, int qi, int qj ) {
		if ( qj < i || qi > j ) return 0LL;
		push_down(v,i,j);
		if ( qi <= i && j <= qj )
			return sum[v];
		int k = (i+j)>>1;
		i64 left = sum_query(L(v),i,k,qi,qj), right = sum_query(R(v),k+1,j,qi,qj);
		push_up(v,i,j);
		return left+right;
	}
public:
	ST( i64 *a, int n ) {
		int m = (n*3)+0x400;
		sum = new i64[m], dt = new i64[m], min_salary = new i64[m];
		memset(dt,0,sizeof dt);
		this->a = a, this->n = n, build(1,0,n-1);
	}
	~ST() {
		delete sum, delete dt, delete min_salary;
	}
	i64 get_sum( int qi, int qj ) {
		return qi <= qj ? sum_query(1,0,n-1,qi,qj):0LL;
	}
	i64 get_min( int qi, int qj ) {
		return qi <= qj ? min_query(1,0,n-1,qi,qj):+oo;
	}
	void update( int qi, int qj, i64 ddt ) {
		if ( ddt ) update(1,0,n-1,qi,qj,ddt);
	}
} *st;

int main() {
	int i,j,k,cs = 0, ts,qr,comm;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d",&n,&qr);
		for ( E = 0, i = 0; i < n; last[i++] = -1 ) ;
		for ( i = 0; i < n; ++i ) {
			scanf("%d %lld",&j,&salary[i]);
			if ( --j >= 0 )
				add_arcs(boss[i] = j,i);
		}
		++yes, ptr = preorder_sequence, preorder(0);
		for ( i = 0; i < n; ++i ) {
			pos[preorder_sequence[i]] = i;
			assert( card[i] == right[i]-left[i]+1 ) ;
			assert( left[preorder_sequence[i]] == i );
			a[i] = salary[preorder_sequence[i]];
		}
		/*
		for ( i = 0; i < n; ++i )
			printf("%d ",preorder_sequence[i]);
		puts("");*/
		st = new ST(a,n);
		for ( printf("Case %d:\n",++cs); qr--; ) {
			scanf("%d %d",&comm,&i), --i;
			if ( comm == 1 ) 
				printf("%lld\n",st->get_sum(left[i],right[i]));
			else {
				i64 ddt = st->get_min(left[i],right[i]);
				st->update(left[i],right[i],min(1000LL,ddt));
			}
		}
		delete st;
	}
	return 0;
}

