/*
 */
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#define tol (1e-9)
#define N 0x200
using namespace std;

double s[N],S,ans[N];
int n,id[N];

bool is_safe( int i, double p ) {
	int j;
	double sum= 0;
	for ( j= n-1; j >= 0; --j ) 
		if ( j != i )
			sum+= min(1.00,max(0.00,(S*p+s[id[i]]-s[id[j]])/S));
	return sum+p > 1.00;
}

int main() {
	int i,j,k,cs= 0,ts;
	for ( cin >> ts; ts--; ) {
		for ( S= 0, (cin>>n), i= 0; i < n; (cin>>s[i++]), S+= s[i-1] ) ;
		printf("Case #%d:",++cs);
		/*
		for ( i= 0; i < n; ++i ) {
			printf(" %lf",100*min(1.00,max(0.00,(2.00/n-s[i]/S))));
		}
		putchar('\n');
		*/
		for ( i= 0; i < n; id[i]= i, ++i ) ;
		for ( j= 1; j; )
			for ( j= 0, i= 0; i < n-1; ++i )
				if ( s[id[i]] > s[id[i+1]] )
					swap(id[i],id[i+1]), ++j;
		for ( i= 0; i < n; ++i ) {
			double good, bad, mid;
			if ( is_safe(i,0.00) ) {
				ans[id[i]]= 0.00;
				continue ;
			}
			for ( good= 1.00, bad= 0.00; fabs(good-bad) >= tol; ) 
				is_safe(i,mid= (good+bad)/2)?(good= mid):(bad= mid);
			ans[id[i]]= good;
		}
		for ( i= 0; i < n; printf(" %lf",100*min(1.00,max(0.00,ans[i++]))) ) ;
		putchar('\n');
	}
	return 0;
}

