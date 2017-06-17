/*
 * DYZIO: Dyzio
 * TOPIC: recursion
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
#define tol (1e-13)
#define N (1<<15)
#define oo (1<<30)
#include <map>
typedef long long i64;
using namespace std;

i64 len[16];
int n,cuts,deg[16],pref[N],processed[N],yes;
char str[N];

int which_level( int pos ) {
	int low,high,mid;
	if ( pos < len[0] ) return 0;
	assert( len[low = 0] <= pos );
	assert( len[high = 15] > pos );
	for(;low+1<high;len[mid=(low+high)/2]<=pos?(low=mid):(high=mid));
	return high;
}

int which_son( int pos ) {
	int level = which_level(pos);
	return pos&(deg[level]-1);
}

void rec( int pos ) {
	if ( pos >= N ) return ;
	if ( processed[pos] == yes ) {
		assert( str[pos] == '0' );
		return ;
	}
	processed[pos] = yes;
	str[pos] = '0';
	int level = which_level(pos), idx = which_son(pos),
		left_son_pos = len[level]+2*idx, right_son_pos = left_son_pos+1;
	rec(left_son_pos), rec(right_son_pos);
}

int sum( int i, int j ) {
	return pref[j]-(i==0?0:pref[i-1]);
}

int main() {
	int i,j,k;
	for ( deg[0] = 1, k = 1; k < 16; deg[k] = (deg[k-1]<<1), ++k ) ;
	for ( len[0] = deg[0], i = 1; i < 16; ++i )
		len[i] = len[i-1]+deg[i];
	for (;2==scanf("%d %s",&n,str) && ++yes;) {
		for ( i = n; i < N; str[i++] = '0' ) ;
		for ( i = 0; i < n; ++i )
			if ( processed[i]!=yes && str[i]=='0' )
				rec(i);
		for ( pref[0]=(str[0]-'0'), i=1; i<N; ++i )
			pref[i]=pref[i-1]+(str[i]-'0');
		if ( sum(0,0) == 0 ) {
			for ( i = 1; i < N; ++i )
				assert( str[i] == '0' );
			puts("0");
			continue ;
		}
		printf("n = %d\n",n);
		for ( i = 0; i < n; ++i )
			putchar(str[i]);
		puts("");
		for ( i = 0; i <= 14; ++i ) 
			if ( sum(len[i],len[i+1]) == 0 )
				break ;
		cuts = pref[i==0?0:len[i-1]];
		printf("%d\n",cuts+1);
	}
	return 0;
}

