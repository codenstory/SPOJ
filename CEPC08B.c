/*
 * CEPC08B
 * TOPIC: data structures, intervals, merging, DP
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SH (21)
#define N BIT(SH)
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1LL)
#define POS(u) ((u)&MASK(SH))
#define H(u) ((u) >> SH)
typedef long long i64;

int n,as_right[N],as_left[N],actives,
	used[N],seen[N],left[N],right[N],m,qr,z[N],marked[N],yes,inactive[N];
i64 c[N],h[N];

int cmp( const void *a, const void *b ) {
	i64 *x = (i64 *)a,
		*y = (i64 *)b;
	if ( H(*x) == H(*y) )
		return (int)(POS(*x)-POS(*y));
	return H(*x)<H(*y)?-1:1;
}

void check_consistency( int i ) {
	assert( left[i] <= right[i] ) ;
	assert( as_left[left[i]] == i ) ;
	assert( as_right[right[i]] == i ) ;
}

void merge( int i, int j ) {
	assert( i != j );
	assert( inactive[i] != yes );
	assert( inactive[j] != yes );
	assert( i < m && j < m );
	assert( right[i] == left[j] );
	assert( left[i] <= right[i] );
	assert( left[j] <= right[j] );
	assert( right[j] > right[i] );
	assert( left[i] < left[j] );
	assert( as_right[right[i]] == i );
	as_right[right[i]] = -1, as_right[right[i] = right[j]] = i;
	as_left[left[j]] = -1, as_left[left[i]] = i, --actives;
	assert( left[i] <= right[i] );
	assert( as_left[left[i]] == i );
	assert( as_right[right[i]] == i );
	inactive[j] = yes;
	check_consistency(i);
}

int main() {
	int i,j,k,ts,l,t,low,high,mid;
	for ( scanf("%d",&ts); ts-- && 2 == scanf("%d %d",&n,&qr) && ++yes; ) {
		for ( m=0,actives=0, i=0; i<n; as_left[i]=as_right[i]=-1, scanf("%lld",&h[i++]) ) ;
		for ( i = 0; i < n; c[i] = (i|(h[i]<<SH)), ++i ) ;
		qsort(c,n,sizeof *c,cmp);
		for ( z[n]=0, k=n-1; k>=0; marked[j+1]=yes,z[j+1]=actives, k=j ) {
			for ( j = k; j >= 0 && H(c[j]) == H(c[k]); --j ) {
				t = POS(c[j]);
				assert( used[t] != yes );
				used[t] = yes;
				assert( 0 <= t && t < n );
				if ( !(t >= 1 && as_right[t-1] >= 0 || t+1 < n && as_left[t+1] >= 0) ) {
					left[m] = right[m] = t, as_left[t] = as_right[t] = m, ++m, ++actives;
					seen[m-1] = yes;
					assert( left[m-1] <= right[m-1] );
					check_consistency(m-1);
					continue ;
				}
				if ( t >= 1 && (i=as_right[t-1]) != -1 ) {
					assert( seen[i] == yes );
					assert( right[i] == t-1 );
					as_right[t-1] = -1, as_right[right[i] = t] = i;
					check_consistency(i);
				}
				if ( t+1 < n && (i=as_left[t+1]) != -1 ) {
					assert( inactive[i] != yes );
					as_left[t+1] = -1, as_left[left[i] = t] = i;
					assert( left[i] <= right[i] );
					check_consistency(i);
				}
				if ( as_left[t] >= 0 && as_right[t] >= 0 ) 
					merge(as_right[t],as_left[t]);
			}
		}
		for ( l = 0; qr--; ++l ) {
			if ( l ) putchar(' ');
			scanf("%d",&t);
			if ( H(c[n-1]) <= t ) {
				putchar('0');
				continue ;
			}
			if ( H(c[0]) > t ) {
				putchar('1');
				continue ;
			}
			assert( H(c[0]) <= t && H(c[n-1]) > t ) ;
			for ( low = 0, high = n-1; low+1 < high; H(c[mid=(low+high)>>1]) > t ? (high=mid):(low=mid) ) ;
			assert( H(c[high]) > t );
			assert( H(c[low]) <= t );
			assert( marked[high] == yes );
			printf("%d",z[high]);
		}
		putchar('\n');
	}
	return 0;
}

