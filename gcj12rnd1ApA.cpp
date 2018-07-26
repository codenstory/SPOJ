/*
 * status: Accepted
 */
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#define oo (DBL_MAX-0x400)
#define N (100100)
using namespace std;

int m,n;
double a[N],prefix[N];

double f( int k ) {
	return (n-k+1) + (1-prefix[k])*(n+1);
}

int main() {
	int i,j,k,cs= 0,ts;
	for ( cin >> ts; ts--; ) {
		for ( cin>>m>>n, i= 1; i <= m; cin >> a[i++] ) ;
		double variant1, variant2, variant3, p;
		for ( prefix[0]= 1, i= 1; i <= m; ++i )
			prefix[i]= prefix[i-1]*a[i];
		variant1= f(m);
		variant2= +oo;
		for ( i= m; i >= 1; --i ) 
			variant2= min(variant2, (m-i+1) + f(i-1));
		variant3= 2+n;
		printf("Case #%d: %lf\n",++cs,min(variant1,min(variant2,variant3)));
	}
	return 0;
}

