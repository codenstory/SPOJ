/*
 * REPAIR1
 * TOPIC: expectation, DP, very hard, recurrences, maths, derivation, inequalities
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define tol (1e-13)
#define N (1<<20)
typedef long long i64;

long double p,A,B,a[N],b[N];
int n;

long double f( int m ) {
	long double q = (1-p), qm = pow(q,m);
	if ( m <= 1 ) return 0.00;
	return (B/p)*((m-1)*p*(1-qm)-q*(1-m*qm/q+(m-1)*qm));
}

long double dp( int m ) {
	long double tmp;
	int k;
	for ( a[0] = b[0] = 0, k = 1; k <= m; ++k ) {
		tmp = a[k-1]<=b[k-1]+A?a[k-1]:b[k-1]+A;
		a[k] = B+tmp;
		b[k] = (1-p)*b[k-1]+p*tmp;
	}
	return b[m];
}

int find( int m ) {
	long double tmp;
	int k;
	for ( a[0] = b[0] = 0, k = 1; k <= m; ++k ) {
		tmp = a[k-1]<=b[k-1]+A?a[k-1]:b[k-1]+A;
		if ( fabs(tmp-b[k-1]-A) < tol )
			return k-1;
		a[k] = B+tmp;
		b[k] = (1-p)*b[k-1]+p*tmp;
	}
	return m;
}

int main() {
	i64 m;
	long double t;
	for ( ;4==scanf("%d %Lf %Lf %Lf",&n,&p,&A,&B) && !(!n&&fabs(p)<tol&&fabs(A)<tol&&fabs(B)<tol); ) {
		if ( fabs(p/=100.00) < tol || (fabs(A) < tol && fabs(B) < tol) ) {
			puts("0.0000");
			continue ;
		}
		if ( fabs(p-1) < tol ) {
			printf("%.4Lf\n",(n-1)*(A<B?A:B));
			continue ;
		}
		if ( B <= p*A ) {
			printf("%.4Lf\n",f(n));
			continue ;
		}
		/*
		if ( A <= B ) {
nx:
			printf("%.4Lf\n",p*A*(n-1));
			continue ;
		}*/
		/*
		if ( (B-p*A) <= B*pow(1-p,n) ) {
			printf("%.4Lf\n",f(n));
			continue ;
		}*/
		t = log(1-p*(A/B))/log(1-p);
		m = (i64)(floor(log(1-p*A/B)/log(1-p)+tol));
		/*
		if ( !((B-p*A) <= B*pow(1-p,m) && (B-p*A) > B*pow(1-p,m+1)) ) {
			for ( m += 4; m >= 0 && !((B-p*A) <= B*pow(1-p,m) && (B-p*A) > B*pow(1-p,m+1)); --m ) ;
		}
		if ( m < 0 ) {
			goto nx;
		}
		assert( (B-p*A) <= B*pow(1-p,m) );
		assert( (B-p*A) > B*pow(1-p,m+1) );
		*/
		m = find(m)+1;
		if ( n <= m ) {
			printf("%.4Lf\n",f(n));
			continue ;
		}
		assert( n > m );
		if ( m <= 10 ) {
			printf("%.4Lf\n",p*A*(n-m)+dp(m));
		}
		else printf("%.4Lf\n",p*A*(n-m)+f(m));
	}
	return 0;
}

