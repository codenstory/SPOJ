/*
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (1<<21)
#define oo (1<<30)
using namespace std;
typedef long long i64;

char s[N],pattern[N],text[N<<1];
int m,n,p[N];
vector<pair<int,int>> vec,v;
i64 F[N];

int main() {
	int i,j,k,cs= 0,ts;
	i64 ans,total;
	for ( i64 t= 2; t < N; ++t )
		F[t]= (t*(t-1))>>1;
	for ( cin>>ts; ts-- && 2==scanf("%s %d",s+1,&m); ) {
		for ( n= strlen(s+1), i= 1; i <= n; s[i]= (s[i]=='a'||s[i]=='e'||s[i]=='i'||s[i]=='o'||s[i]=='u')?'0':'1', ++i ) ;
		total= F[n+1];
		for ( i= 1; i <= m; pattern[i++]= '1' ) ;
		for ( j= 1, i= 1; i <= m; text[j++]= pattern[i++] ) ;
		for ( text[j++]= '#', i= 1; i <= n; text[j++]= s[i++] ) ;
		for ( j= p[0]= -1, i= 1; i <= m+1+n; p[i++]= ++j )
			for ( ;j+1 && text[j+1] != text[i]; j= p[j] ) ;
		for ( vec.clear(), i= m+2; i <= m+1+n; ++i )
			if ( p[i] >= m ) 
				vec.push_back({i-m+1-(m+1),i-(m+1)});
		int _left= +oo, _right= -oo;
		for ( v.clear(), sort(vec.begin(),vec.end()), i= 0; i < (int)vec.size(); ++i ) {
			if ( _right < vec[i].first ) {
				if ( _left <= _right )
					v.push_back({_left,_right});
				_left= vec[i].first, _right= vec[i].second;
				continue ;
			}
			if ( _right < vec[i].second ) 
				_right= vec[i].second ;
		}
		if ( _left <= _right )
			v.push_back({_left,_right});
		if ( !v.size() ) {
			printf("Case #%d: %lld\n",++cs,0LL);
			continue ;
		}
		if ( v.size() && v[0].first > 1 ) 
			v.insert(v.begin(),{1,0});
		if ( v.size() && v[v.size()-1].second <= n-1 )
			v.push_back({n+1,n});
		/*
		for ( i= 0; i < v.size(); ++i ) {
			printf("[%d -- %d]\n",v[i].first,v[i].second);
		}
		*/
		for ( ans= 0LL, n= v.size(), i= 1; i <= n-1; ++i ) {
			i64 _left_length= v[i-1].second-v[i-1].first+1;
			i64 _right_length= v[i].second-v[i].first+1;
			//assert( _left_length >= m );
			//assert( _right_length >= m );
			assert( v[i-1].second < v[i].first );
			ans+= ((i64)min(m-1,(int)_right_length)) * min(m-1,(int)_left_length);
			i64 gap_length= v[i].first-v[i-1].second-1;
			ans+= gap_length*(gap_length+1)/2;
			ans+= min(m-1,(int)_left_length)*gap_length + min(m-1,(int)_right_length)*gap_length;
		}
		for ( i= 0; i < n; ++i ) {
			i64 len= v[i].second-v[i].first+1;
			if ( len < m ) continue ;
			ans+= (len-m+2)*(m-1)+F[m-1];
		}
		printf("Case #%d: %lld\n",++cs,total-ans);
	}
	return 0;
}

