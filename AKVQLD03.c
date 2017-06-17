/*
 * AKVQLD03: How to Handle the Fans
 * TOPIC: BIT
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<20)
#define L(k) ((k) & ((~(k))+1))
typedef long long i64;

i64 tr[N<<1];
int n,K;

void update( unsigned int k, i64 dt ) {
	for ( ;k < (1<<K); tr[k] += dt, k += L(k) ) ;
}

i64 prefix( unsigned int k ) {
	i64 ans = 0;
	for ( ;k; ans += tr[k], k &= ~L(k) ) ;
	return ans;
}

i64 sum( unsigned int i, unsigned int j ) {
	return prefix(j)-prefix(i-1);
}

int main() {
	int i,j,k,cs=0,qr;
	char tmp[0x20];
	for ( ;2 == scanf("%d %d",&n,&qr); ) {
		if ( ++cs > 1 ) memset(tr,0,sizeof tr);
		for ( K = 0; (1<<K) <= n; ++K ) ;
		for ( ;qr-- && 1 == scanf("%s",tmp); ) {
			switch ( 0[tmp] ) {
				case 'f': scanf("%d %d",&i,&j);
						  printf("%lld\n",sum(i,j));
						  break ;
				default: scanf("%d %d",&i,&k);
						 update(i,k);
						 break ;
			}
		}
	}
	return 0;
}

