/*
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
#include <queue>
#define tol (1e-13)
typedef long long i64;
using namespace std;

char mass[256],s[0x400];
int n;
deque<i64> q;

int main() {
	i64 i,j,k;
	mass['H'] = 1, mass['C'] = 12, mass['O'] = 16;
	for ( ;1 == scanf("%s",s); ) {
		for ( n = strlen(s), i = 0; i < n; ++i ) {
			if ( s[i] == '(' )
				q.push_front(0);
			else if ( s[i] == ')' ) {
				for ( j = 0; !q.empty() && q.front() != 0; j += q.front(), q.pop_front() ) ;
				if ( !q.empty() ) q.pop_front();
				q.push_front(j);
			}
			else if ( mass[s[i]] ) {
				q.push_front(mass[s[i]]);
			}
			else if ( '0' <= s[i] && s[i] <= '9' ) {
				assert( '2' <= s[i] && (i+1 == n || !('0' <= s[i+1] && s[i+1] <= '9')) );
				for ( k = 0; i < n && '0' <= s[i] && s[i] <= '9'; ++i )
					k = 10*k+s[i]-'0';
				j = q.front(); q.pop_front(); --i;
				q.push_front(j*k);
			}
			else break ;
		}
		for ( k = 0; !q.empty(); k += q.front(), q.pop_front() );
		printf("%lld\n",k);
		return 0;
	}
	return 0;
}

