/*
 * GOODG
 * TOPIC: convex hull trick, binary search
 * status: TLE
 */
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<20)
#define oo (1LL<<60)
#define inf (DBL_MAX-0x400)
typedef long long i64;

typedef struct line {
	i64 m,l;
	double left;
} line;

i64 max( i64 x, i64 y ) { return x<y?y:x; }

line L[N];
int n,top,s[N],q[N],best[N];
i64 a[N],d[N],z[N];

double intersection( const line *a, const line *b ) {
	return (b->l-a->l+0.00)/(a->m-b->m);
}

int getnum() {
	register int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch == EOF ) return -1;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

int bad( const line *a, const line *b, const line *c ) {
	/*return intersection(a,c) >= intersection(b,c);*/
	assert( c->m > a->m && c->m > b->m );
	return (a->l-c->l)*(c->m-b->m) >= (b->l-c->l)*(c->m-a->m);
}

void add( int k ) {
	int i,t,low,high,mid;

	if ( top == 0 ) {
		L[s[++top]=k].left=-inf;
		return ;
	}

	if ( L[k].m >= L[s[top]].m ) {
		if ( L[k].m == L[s[top]].m ) {
			if ( L[k].l <= L[s[top]].l ) return ;
			--top;
		}
		for ( ;top>=2 && bad(L+s[top-1],L+s[top],L+k); --top ) ;
		s[++top] = k, L[s[top]].left = top==1?-inf:intersection(L+s[top-1],L+s[top]);
		return ;
	}

	assert( top >= 1 && L[k].m < L[s[top]].m );

	if ( L[k].m == L[s[1]].m ) {
		if ( L[k].l <= L[s[1]].l ) return ;
		for ( t = 0, L[q[++t] = k].left = -inf, i = 2; i <= top; ++i ) {
			for ( ;t>=2 && bad(L+q[t-1],L+q[t],L+s[i]); --t ) ;
			q[++t] = s[i], L[q[t]].left = t==1?-inf:intersection(L+q[t-1],L+q[t]);
		}
		for ( top = 0, i = 1; i <= t; s[++top] = q[i++] ) ;
		return ;
	}

	if ( L[k].m < L[s[1]].m ) {
		for ( t = 0, L[q[++t] = k].left = -inf, i = 1; i <= top; ++i ) {
			for ( ;t>=2 && bad(L+q[t-1],L+q[t],L+s[i]); --t ) ;
			q[++t] = s[i], L[q[t]].left = t==1?-inf:intersection(L+q[t-1],L+q[t]);
		}
		for ( top = 0, i = 1; i <= t; s[++top] = q[i++] ) ;
		return ;
	}
	
	assert( L[k].m > L[s[1]].m );
	assert( L[k].m < L[s[top]].m );
	for ( low = 1, high = top; low+1<high; L[s[mid=(low+high)/2]].m < L[k].m ? (low = mid) : (high = mid) ) ;
	assert( L[k].m > L[s[low]].m );
	assert( L[k].m <= L[s[high]].m );
	if ( L[s[high]].m == L[k].m ) {
		if ( L[k].l <= L[s[high]].l ) return ;
		for ( t = high-1; t>=2 && bad(L+s[t-1],L+s[t],L+k); --t ) ;
		s[++t] = k, L[s[t]].left = t==1?-inf:intersection(L+s[t-1],L+s[t]);
		for ( i = high+1; i <= top; ++i ) {
			for (;t >= 2 && bad(L+s[t-1],L+s[t],L+s[i]); --t ) ;
			assert( t+1 <= i );
			s[++t] = s[i], L[s[t]].left = t==1?-inf:intersection(L+s[t-1],L+s[t]);
		}
		top = t;
		return ;
	}
	
	assert( L[k].m < L[s[high]].m ) ;
	assert( L[k].m > L[s[low]].m ) ;

	for ( t = low; t >= 2 && bad(L+s[t-1],L+s[t],L+k); --t ) ;
	for ( i = 1; i <= t; q[i] = s[i], ++i ) ;
	q[++t] = k, L[q[t]].left = t==1?-inf:intersection(L+q[t-1],L+q[t]);
	for ( i = high; i <= top; ++i ) {
		for ( ;t >= 2 && bad(L+q[t-1],L+q[t],L+s[i]); --t ) ;
		q[++t] = s[i], L[q[t]].left = t==1?-inf:intersection(L+q[t-1],L+q[t]);
	}
	for ( top = 0, i = 1; i <= t; s[++top] = q[i++] );
}

int brute_force( int k ) {
	int i,idx = -1;
	i64 best = -oo;
	for ( i = 1; i <= k; ++i )
		if ( L[i].m*k+L[i].l > best )
			best = L[i].m*k+L[i].l, idx = i;
	return idx;
}

int main() {
	int i,j,k,t,low,high,mid,cs = 0,idx;
	for ( ;;  ) {
		n = getnum();
		if ( n == -1 || n == 0 ) return 0;
		for ( i = 1; i <= n; a[i] = getnum(), d[i] = getnum(), ++i ) ;
		for ( top = 0, k = 1; k <= n; best[k] = s[t], ++k ) {
			L[k].m = -d[k], L[k].l = d[k]*(k-1)+z[k-1]+a[k], add(k);
			/*
			for ( i = 1; i <= top; ++i )
				printf("%lldx+%lld\n",L[s[i]].m,L[s[i]].l);
			puts("");*/
			assert( L[s[1]].left <= k );
			if ( L[s[top]].left <= k )
				t = top;
			else {
				assert( L[s[1]].left <= k && L[s[top]].left > k );
				for(low=1,high=top;low+1<high;L[s[mid=(low+high)/2]].left>k?(high=mid):(low=mid));
				t = low;
			}
			assert( L[s[t]].left <= k && (t==top||L[s[t+1]].left > k) );
			idx = brute_force(k), z[k] = max(0,L[idx].m*k+L[idx].l);
			//z[k]=max(0,L[s[t]].m*k+L[s[t]].l);
		}
		printf("%lld\n",z[n]);
	}
	return 0;
}

