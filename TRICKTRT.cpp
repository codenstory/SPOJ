/*
 * TRICKTRT
 * TOPIC: ternary search
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#define tol (1e-7)
#define N (1<<16)
#define oo (1LL<<30)
using namespace std;
#define S(x) ((x)*(x))

double x[N],y[N];
int n;

double f( double xpos ) {
	double ans = -oo;
	for ( int i = 0; i < n; ++i )
		ans = max(ans,S(x[i]-xpos)+y[i]);
	return ans;
}

int main() {
	int i,j,k;
	double a,b,na,nb;
	for ( ;1 == scanf("%d",&n) && n; ) {
		a = +oo, b = -oo;
		for ( i = 0; i < n; ++i ) {
			scanf("%lf %lf",x+i,y+i), y[i] = y[i]*y[i];
			a = min(a,x[i]), b = max(b,x[i]);
		}
		for ( ;fabs(b-a) >= tol; ) {
			na = (2*a+b)/3, nb = (2*b+a)/3;
			if ( f(na) < f(nb) )
				b = nb;
			else a = na;
		}
		printf("%lf %lf\n",(a+b)/2,sqrt(f((a+b)/2)));
	}
	return 0;
}

