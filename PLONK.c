/*
 * PLONK
 * TOPIC: sorting, binary search, excellent for interviews
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<14)
#define oo (1<<30)
typedef long long i64;

int n,x[N],y[N],ox[N],oy[N];
i64 px[N],tx[N],py[N],ty[N];

int cmp( const void *a, const void *b ) {
	int *x = (int *)a,
		*y = (int *)b;
	return *x-*y;
}

int main() {
	int i,j,k,t,ts,idx,idy,low,high,mid;
	i64 ans,w;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( i = 1; i <= n; ++i )
			scanf("%d %d",&ox[i],&oy[i]), x[i] = ox[i], y[i] = oy[i];
		x[0] = y[0] = -oo, qsort(x+1,n,sizeof *x,cmp), qsort(y+1,n,sizeof *y,cmp);
		for ( i = 1; i <= n; ++i )
			px[i] = px[i-1]+x[i], py[i] = py[i-1]+y[i];
		for ( tx[n+1] = ty[n+1] = 0, i = n; i >= 1; --i )
			tx[i] = tx[i+1]+x[i], ty[i] = ty[i+1]+y[i];
		for ( ans = (1LL<<60), t = 1; t <= n; ++t ) {
			for ( low = 0, high = n; low+1<high; x[mid=(low+high)/2]<ox[t]?(low=mid):(high=mid), idx=high ) ;
			for ( low = 0, high = n; low+1<high; y[mid=(low+high)/2]<oy[t]?(low=mid):(high=mid), idy=high ) ;
			assert( x[idx] == ox[t] );
			assert( y[idy] == oy[t] );
			assert( x[idx-1] < ox[t] );
			assert( y[idy-1] < oy[t] );
			w = tx[idx+1]-px[idx]+(idx*ox[t]-(n-idx)*ox[t]) + ty[idy+1]-py[idy]+(idy*oy[t]-(n-idy)*oy[t]);
			if ( w < ans ) ans = w;
		}
		printf("%lld\n",ans);
	}
	return 0;
}

