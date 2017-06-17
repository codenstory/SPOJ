/*
 * BBIN: Busqueda Binaria
 * TOPIC: binary search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<18)

int n,c[N];

int main() {
	int i,j,k,qr,low,high,mid;
	for ( ;2 == scanf("%d %d",&n,&qr); ) {
		for ( i = 0; i < n; ++i ) scanf("%d",c+i);
		for ( ;qr-- && 1 == scanf("%d",&k); ) {
			if ( c[0] > k || c[n-1] < k ) {
				puts("-1");
				continue ;
			}
			if ( c[0] == k ) { printf("%d\n",0); continue; }
			for ( low = 0, high = n-1; low+1 < high; c[mid = (low+high)/2] < k ? (low=mid):(high=mid) ) ;
			printf("%d\n",c[high] == k?high:-1);
		}
	}
	return 0;
}

