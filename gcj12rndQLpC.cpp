/*
 * status: Accepted
 */
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unordered_set>
using namespace std;
typedef long long i64;

void reverse( char *a, int i, int j ) {
	for ( int k= i, l= j; k < l; ++k, --l )
		swap(a[k],a[l]);
}

struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};

unordered_set<pair<int,int>,pair_hash> s;

int main() {
	int i,j,k,cs= 0,ts,A,B;
	i64 ans;
	for ( cin >> ts; ts--; ) {
		cin >> A >> B;
		for ( s.clear(), ans= 0, i= A; i <= B; ++i ) {
			char str[0x10],qtr[0x10],*ptr;
			sprintf(str,"%d",i), k= strlen(str);
			for ( j= 1; j <= k-1; ++j ) {
				if ( str[j] > str[0] ) continue ;
				strcpy(qtr,str), reverse(qtr,0,j-1), reverse(qtr,j,k-1), reverse(qtr,0,k-1);
				int t= atoi(qtr);
				if ( A <= t && t <= B && t < i ) {
					pair<int,int> p= {t,i};
					if ( !s.count(p) )
						++ans;
					s.insert(p);
				}
			}
		}
		printf("Case #%d: %lld\n",++cs,ans);
	}
	return 0;
}

