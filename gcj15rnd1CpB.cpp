/*
 */
#include <bits/stdc++.h>
#define N 0x80
#define A 26
using namespace std;

int n,m,L,p[N][A],prefix[N],cn[N],indeg[N][N];
char keyboard[N],word[N];
double P,prob[N],z[N][N],e[N][N];
bool dz[N][N][N];
map<pair<int,int>,int> indegree;

int main() {
	int i,j,k,l,ts,cs= 0,len;
	char tmp[N];
	for ( scanf("%d",&ts); ts--; ) {
		scanf("%d %d %d %s",&m,&L,&n,tmp), memset(cn,0,sizeof cn);
		for ( i= 0; i < m; ++cn[tmp[i++]-'A'] ) ;
		for ( k= 0, i= 'A'; i <= 'Z'; ++i )
			if ( cn[i-'A'] )
				prob[k]= (cn[i-'A']+0.00)/m, keyboard[k++]= i;
		m= k, scanf("%s",word+1);
		assert( strlen(word+1) == L );
		for ( j= prefix[0]= -1, i= 1; i <= L; prefix[i++]= ++j )
			for ( ;j+1 && word[i] != word[j+1]; j= prefix[j] ) ;
		assert( prefix[1] == 0 );
		for ( i= 0; i <= L-1; ++i )
			for ( j= 0; j < m; ++j ) {
				for ( k= i; k+1 && word[k+1] != keyboard[j]; k= prefix[k] ) ;
				if ( ++k == L )
					p[i][j]= prefix[L];
				else p[i][j]= k;
			}
		memset(z,0,sizeof z);
		queue<pair<int,int>> q;
		memset(indeg,0,sizeof indeg);

		for ( indegree.clear(), q.push({0,0}); !q.empty(); ) {
			pair<int,int> r= q.front(); q.pop();
			k= r.first, i= r.second;
			if ( k == n ) continue ;
			for ( j= 0; j < m; ++j ) {
				bool is_new= false;
				if ( !indegree.count(make_pair(k+1,p[i][j])) )
					indegree[make_pair(k+1,p[i][j])]= 0, is_new= true;
				++indegree[make_pair(k+1,p[i][j])];
				if ( is_new )
					q.push({k+1,p[i][j]});
			}
		}
		for ( map<pair<int,int>,int>::iterator it= indegree.begin(); it != indegree.end(); ++it )
			indeg[it->first.first][it->first.second]= it->second;
		memset(dz,0,sizeof dz);
		for ( z[0][0]= 1.00, dz[0][0][0]= true, q.push({0,0}); !q.empty(); ) {
			pair<int,int> r= q.front(); q.pop();
			k= r.first, i= r.second;
			if ( k == n ) continue ;
			for ( j= 0; j < m; ++j ) {
				z[k+1][p[i][j]]+= z[k][i]*prob[j];
				for ( l= 0; l <= k; ++l )
					if ( dz[k][i][l] ) {
						for ( auto t= 0; t <= l; ++t )
							dz[k+1][p[i][j]][t]= true;
						if ( i == L-1 && word[L] == keyboard[j] )
							dz[k+1][p[i][j]][l+1]= true;
					}
				if ( !--indeg[k+1][p[i][j]] )
					q.push({k+1,p[i][j]});
			}
		}
		for ( len= n; len >= 0; --len ) {
			for ( i= 0; i <= L-1; ++i )
				if ( dz[n][i][len] ) 
					goto br;
			continue ;
			br: break;
		}
		for ( map<pair<int,int>,int>::iterator it= indegree.begin(); it != indegree.end(); ++it )
			indeg[it->first.first][it->first.second]= it->second;
	
		memset(e,0,sizeof e);
		for ( q.push({0,0}); !q.empty(); ) {
			pair<int,int> r= q.front(); q.pop();
			k= r.first, i= r.second;
			if ( k == n ) continue ;
			for ( j= 0; j < m; ++j ) {
				e[k+1][p[i][j]]+= e[k][i]*prob[j];
				if ( i == L-1 && word[L] == keyboard[j] )
					e[k+1][p[i][j]]+= z[k][i]*prob[j];
				if ( !--indeg[k+1][p[i][j]] )
					q.push({k+1,p[i][j]});
				assert( indeg[k+1][p[i][j]] >= 0 );
			}
		}
		auto expected{0.00};
		for ( i= 0; i <= L-1; expected+= e[n][i++] ) ;
		printf("Case #%d: %lf\n",++cs,len-expected);
	}
	return 0;
}

