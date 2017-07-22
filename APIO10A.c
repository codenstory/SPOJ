/*
 * APIO10A
 * TOPIC: convex hull trick, binary search
 * status: Accepted
 */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#define tol (1e-13)
#define N (1<<21)
#define oo (DBL_MAX-0x400)
#define S(k) ((k)*(k))
#define inf (1LL<<(60))
typedef long long i64;

i64 max( i64 x, i64 y ) { return x<y?y:x; }

int getnum() {
	int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

i64 a,b,c,x[N],pref[N],z[N];
int n,top,st[N];
double alpha[N],beta[N];

double intersect( int i, int j ) {
	double mi = -2*a*pref[i], pi = z[i]+a*S(pref[i])-b*pref[i],
		   mj = -2*a*pref[j], pj = z[j]+a*S(pref[j])-b*pref[j];
	return (pj-pi+0.00)/(mi-mj);
}

i64 f( int k ) {
	return a*S(pref[k])+b*pref[k]+c;
}

int bad( int i, int j, int k ) {
	i64 mi = -2*a*pref[i], 
		mj = -2*a*pref[j], 
		mk = -2*a*pref[k],
		pi = z[i]+a*S(pref[i])-b*pref[i],
		pj = z[j]+a*S(pref[j])-b*pref[j],
		pk = z[k]+a*S(pref[k])-b*pref[k];
	return log(mk-mj)+log(pi-pk) > log(mk-mi)+log(pj-pk);
}

int main() {
	int i,j,k,ts,low,high,mid,t;
	double u,v;
	for ( ts = getnum(); ts--; ) {
		n=getnum(),a=getnum(),b=getnum(),c=getnum();
		for ( i = 1; i <= n; pref[i] = pref[i-1]+(x[i]=getnum()), ++i ) ;
#if 1
		for ( z[1]=f(1), top=0, st[++top]=1, alpha[top]=-oo, beta[top]=+oo, k=1; k+1<=n; ++k ) {
			if ( pref[k+1] >= alpha[top] )
				t = st[top];
			else {
				assert( pref[k+1] < alpha[top] );
				for ( low=1, high=top; low+1<high; pref[k+1]<alpha[mid=(low+high)/2]?(high=mid):(low=mid) );
				t = st[low];
				assert( alpha[low] <= pref[k+1] );
			}
			z[k+1] = max(z[k]+a*S(x[k+1])+b*x[k+1]+c,f(k+1)-2*a*pref[t]*pref[k+1]+z[t]+a*S(pref[t])-b*pref[t]);
			z[k+1] = max(z[k+1],a*S(pref[k+1])+b*pref[k+1]+c);
			for (;top>=2;) 
				if ( bad(st[top-1],st[top],k+1) ) --top;
				else break ;
			beta[top]=intersect(st[top],k+1), st[++top]=(k+1), alpha[top]=beta[top-1], beta[top]=+oo;
		}
		printf("%lld\n",z[n]);
#else
		for ( z[1] = -a*S(x[1])+c, k = 0; k+1 <= n; ++k )
			for ( z[k+1] = z[k]-a*S(x[k+1])+c, t = 1; t <= k; ++t ) {
				//z[k+1] = max(z[k+1],z[t-1]-a*S(pref[k+1]-pref[t-1])+c);
				z[k+1] = max(z[k+1],z[t-1]-a*S(pref[k]-pref[t-1])-2*a*(pref[k]-pref[t-1])*x[k+1]-a*S(x[k+1])+c);
			}
		printf("%lld\n",z[n]+b*pref[n]);
#endif
	}
	return 0;
}

