/*
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <iostream>
#define oo (1<<30)
using namespace std;

int n,total;
string s;
vector<pair<int,int>> vec;

int simulate( int k ) {
	int how_many_standing= k+vec[0].second, i,j;
	for ( i= 1; i <= n; ++i ) 
		if ( how_many_standing >= i )
			how_many_standing+= vec[i].second;
		else break ;
	return how_many_standing;
}

int main() {
	int i,j,k,cs= 0,ts;
	for ( cin >> ts; ts--; ) {
		cin >> n >> s;
		for ( vec.clear(), total= 0, i= 0; i <= n; ++i ) 
			vec.push_back({i,s[i]-'0'}), total+= s[i]-'0';
		int good, bad, mid;
		if ( simulate(bad= 0) >= total+0 ) {
			printf("Case #%d: 0\n",++cs);
			continue ;
		}
		for ( good= +oo; bad+1 < good; ) {
			mid= (good+bad)/2;
			simulate(mid) >= total+mid ? (good= mid):(bad= mid);
		}
		printf("Case #%d: %d\n",++cs,good);
	}
	return 0;
}

