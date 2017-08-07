/*
 * DIV
 * TOPIC: sieve
 * status: Accepted
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
int first[N],second[N],numdivs[N];
vector<int> p;

int main() {
	int i,j,k,t;
	for ( i = 0; i < N; first[i] = second[i] = -1, numdivs[i++] = 1 );
	for ( i = 3; i < N; isprime[i] = true, i += 2 ) ;
	for ( isprime[2] = true, i = 2; i < N; ++i )
		for ( j = i+i; isprime[i] && j < N; isprime[j] = false, numdivs[j] *= (t+1), j += i ) {
			for ( k = j, t = 0; !(k%i); ++t, k /= i ) ;
			if ( t == 1 ) {
				if ( first[j] == -1 ) 
					first[j] = i;
				else if ( second[j] == -1 ) {
					assert( i != first[j] );
					second[j] = i;
				}
				else {
					assert( i != second[j] && i != first[j] );
					is_bad[j] = true ;
				}
			}
			else is_bad[j] = true;
		}
	for ( k = 0, p.clear(), i = 2; i < N; ++i )
		if ( !is_bad[numdivs[i]] && (first[numdivs[i]] != second[numdivs[i]] && first[numdivs[i]] != -1 && second[numdivs[i]] != -1) )
			if ( (++k) == 9 )
				p.push_back(i), k = 0;
	for ( i = 0; i < (int)p.size(); ++i )
		printf("%d\n",p[i]);
	return 0;
}

