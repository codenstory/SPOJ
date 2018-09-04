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
#include <set>
#include <map>
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define L(k) ((k)&((~(k))+1ULL))
#define tol (1e-13)
#define A 26
#define N 0x80
using namespace std;
typedef unsigned long long u64;
typedef long long i64;

int m,n;
char g[256][256];
bool are_opposed[256][256];
char s[N];

void print_list( char *s ) {
	int i,j,k,n= strlen(s);
	if ( !n ) {
		puts("[]");
		return ;
	}
	if ( n == 1 ) {
		printf("[%c]\n",s[0]);
		return ;
	}
	for ( printf("[%c,",s[0]), i= 1; i < n; ++i ) {
		if ( i < n-1 ) printf(" %c,",s[i]);
		else printf(" %c]\n",s[i]);
	}
}

int main() {
	int i,j,k,ts,cs = 0;
	for ( scanf("%d",&ts); ts--; ) {
		cin >> m;
		memset(g,'\0',sizeof g);
		memset(are_opposed,0,sizeof are_opposed);
		for ( i= 0; i < m; ++i ) {
			string tmp;
			cin >> tmp;
			g[tmp[0]][tmp[1]]= g[tmp[1]][tmp[0]]= tmp[2];
		}
		cin >> n;
		for ( i= 0; i < n; ++i ) {
			string tmp;
			cin >> tmp;
			are_opposed[tmp[0]][tmp[1]]= are_opposed[tmp[1]][tmp[0]]= true ;
		}
		scanf("%d %s",&n,s);
		char t[N];
		for ( k= 0, i= 0; i < n; ++i ) {
			char ch= s[i];
			int changes= 0;
			if ( !k ) {
				t[k++]= ch;
				continue ;
			}
			if ( g[t[k-1]][ch] ) {
				t[k-1]= g[t[k-1]][ch];
				continue ;
			}
			for ( j= 0; j < k && !are_opposed[t[j]][ch]; ++j ) ;
			if ( j < k ) {
				k= 0;
				continue ;
			}
			t[k++]= ch;
		}
		t[k]= '\0', printf("Case #%d: ",++cs), print_list(t);
	}
	return 0;
}

