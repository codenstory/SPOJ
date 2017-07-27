/*
 * FONTSIZE
 * TOPIC: binary search
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N (1<<16)
#define tol (1e-11)
#define oo (1LL<<30)
typedef long long i64;

i64 W,H,len[N],pref[N];
char s[N],word[N];
int n;
double COEFF = (2.00/3.00);

int next_jump( int cur, double size ) {
	int i,j,k,low,high,mid;
	if ( size*COEFF*len[low=cur] > W ) return +oo;
	if ( size*COEFF*(pref[high=n]-pref[cur-1])+COEFF*size*(n-cur) <= W )
		return n+1;
	for(;low+1<high;mid=(low+high)/2,size*COEFF*(pref[mid]-pref[cur-1])+COEFF*size*(mid-cur)<=W?(low=mid):(high=mid));
	return high;
}

int num_lines( double size ) {
	int i,j,k,lines = 0;
	for ( i = 1; (k = next_jump(i,size)) < +oo && ++lines; )
		if ( (i = k) == n+1 ) break ;
	return i==n+1?lines:+oo;
}

int can_fit( double size ) {
	return num_lines(size)*size <= H;
}

int main() {
	int i,j,k;
	char *ptr;
	double good,bad,mid;
	for ( ;2 == scanf("%lld %lld\n",&W,&H); printf("%lf\n",good) ) {
		for ( ptr = fgets(s,sizeof s,stdin), n = 0; 1 == sscanf(ptr,"%s%n",word,&j); ptr += j, len[++n] = strlen(word), pref[n]=pref[n-1]+len[n] ) ;
		for ( good = 0, bad = H+1; fabs(bad-good) >= tol; can_fit(mid=(good+bad)/2)?(good=mid):(bad=mid) ) ;
	}
	return 0;
}

