/*
 */
#include <bits/stdc++.h>
#define N 0x80
using namespace std;

int n,m[N];
char s[N][N];

struct profile {
	vector<pair<char,int>> vec;
	profile( char *s ) {
		vec.clear();
		for ( auto j= 0,i= 0; s[i]; vec.push_back({s[i],j-i}), i= j ) 
			for ( j= i; s[j] && s[i] == s[j]; ++j ) ;
	}
	int len() const { return vec.size(); } 
	bool operator & ( const profile &other ) {
		if ( other.len() != len() )
			return false ;
		for ( auto i= 0; i < vec.size(); ++i )
			if ( vec[i].first != other.vec[i].first )
				return false ;
		return true ;
	}
	profile() { vec.clear(); }
};

profile p[N];

int main() {
	int i,j,k,cs= 0,ts;
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d",&n);
		for ( i= 0; i < n; ++i ) {
			scanf("%s",s[i]), m[i]= strlen(s[i]);
			p[i]= profile(s[i]);
		}
		printf("Case #%d: ",++cs);
		bool ok= false;
		vector<int> vec[N];
		for ( i= 0; i < n; ++i )
			for ( j= i+1; j < n; ++j )
				if ( !(p[i] & p[j]) ) {
					ok= false;
					goto next;
				}
		for ( i= 0; i < n; ++i )
			vec[i].clear();
		for ( k= 0, i= 0; i < n; ++i )
			for ( j= 0; j < p[i].len(); ++j )
				vec[j].push_back(p[i].vec[j].second);
		for ( j= 0; j < p[0].len(); ++j ) {
			sort(vec[j].begin(),vec[j].end());
			auto median1= vec[j][vec[j].size()/2],
				 median2= vec[j][vec[j].size()>=1?(vec[j].size()-1)/2:vec[j].size()/2];
			int sum1= 0, sum2= 0;
			for ( i= 0; i < vec[j].size(); ++i )
				sum1+= abs(vec[j][i]-median1), sum2+= abs(vec[j][i]-median2);
			k+= min(sum1,sum2);
		}
		printf("%d\n",k);
		continue ;
		next:
		puts("Fegla Won");
		continue ;
	}
	return 0;
}

