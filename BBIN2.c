/*
 * TOPIC: binary search
 * status: Accepted
 */
#include <stdio.h>
#include <assert.h>
#define N (1<<17)

int n,x[N];

int main() {
	int i,j,k,qr,t,low,high,mid;
	for ( ;2 == scanf("%d %d",&n,&qr); ) {
		for ( i = 0; i < n; scanf("%d",&x[i++]) ) ;
		for ( ;qr-- && 1 == scanf("%d",&t); ) {
			if ( x[0] > t ) {
				puts("-1");
				continue ;
			}
			if ( x[n-1] == t ) {
				printf("%d\n",n-1);
				continue ;
			}
			if ( x[n-1] < t ) {
				puts("-1");
				continue ;
			}
			assert( x[0] <= t && x[n-1] > t ) ;
			for ( low = 0, high = n-1; low+1<high; x[mid=(low+high)/2]>t?(high=mid):(low=mid) ) ;
			printf("%d\n",x[low]==t?low:-1);
		}
	}
	return 0;
}

