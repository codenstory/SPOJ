/*
 * MSKYCODE: Sky code
 * TOPIC: pie
 * status:
 */
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#define tol (1e-13)
#define N 10100
#define MAXP 1240
#define SZ 7
using namespace std;
typedef long long i64;

int n,m;
bool isprime[N],table[N][MAXP];
vector<int> p,vec[N];
set<int> s;
map<int,int> mp;

void sieve() {
	int i,j,k;
	for ( isprime[2] = true, i = 3; i < N; i += 2 ) isprime[i] = true ;
	for ( i = 3; i < N; i += 2 )
		if ( isprime[i] )
			for ( j = i+i; j < N; isprime[j] = false, j += i );
	for ( p.clear(), i = 0; i < N; ++i )
		if ( isprime[i] )
			p.push_back(i);
}

int main() {
	int i,j,k;
	i64 num;
	for ( sieve(); 1 == scanf("%d",&m); ) {
		for ( s.clear(), i = 0; i < m; ++i ) {
			scanf("%lld",&num), vec[i].clear();
			for ( j = 0; j < (int)p.size() && p[j]*p[j] <= num; ++j ) {
				for ( k = 0; 0 == (num%p[j]); ++k, num /= p[j] );
				if ( (table[i][j] = (k>=1)) )
					s.insert(j), vec[i].push_back(j);
			}
			if ( num > 1 ) {
				for ( j = ((int)p.size())-1; j >= 0 && p[j] > num; --j ) ;
				assert( j >= 0 && p[j] == num );
				table[i][j] = true ;
				s.insert(j), vec[i].push_back(j);
			}
		}
		mp.clear();
		for ( set<int>::iterator it = s.begin(); it != s.end(); ++it ) 
			k = mp.size(), mp[*it] = k;
		for ( i = 0; i < m; ++i )
			for ( k = 0; k < (int)vec[i].size(); ++k )
				vec[i][k] = mp[vec[i][k]];
		assert( mp.size() < SZ );
	}
	return 0;
}

