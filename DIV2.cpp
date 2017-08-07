/*
 * DIV2
 * TOPIC: sieve
 * status: Accepted
 * NOTES: \sum_{i=1}^{N}N/i ~ O(NlogN)
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define tol (1e-13)
#define N (1000001)
using namespace std;

bool isprime[N],is_bad[N];
int numdivs[N];
vector<int> p;

int main() {
	int i,j,k,t;
	for ( i = 0; i < N; numdivs[i++] = 1 );
	for ( i = 3; i < N; isprime[i] = true, i += 2 ) ;
	for ( isprime[2] = true, i = 2; i < N; ++i )
		for ( j = i+i; isprime[i] && j < N; isprime[j] = false, numdivs[j] *= (t+1), j += i ) 
			for ( k = j, t = 0; !(k%i); ++t, k /= i ) ;
	for ( i = 2; i < N; ++i )
		for ( k = 2; k*i < N; ++k )
			if ( numdivs[k*i] <= 3 || (numdivs[k*i]%numdivs[i]) )
				is_bad[k*i] = true ;
	for ( k = 0, p.clear(), i = 2; i < N; ++i )
		if ( numdivs[i] >= 4 && !is_bad[i] )
			if ( (++k) == 108 )
				p.push_back(i), k = 0;
	for ( i = 0; i < (int)p.size(); ++i )
		printf("%d\n",p[i]);
	return 0;
}

