/*
 * TREEORD
 * TOPIC: tree traversals, recursion
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 8086

int pre[N],post[N],in[N],n,pos[N];

int f( int len, int i, int j, int k ) {
	int root,t,left,right;
	if ( len == 0 ) return 1;
	if ( len == 1 ) 
		return pre[i] == post[j] && post[j] == in[k];
	assert( len >= 2 );
	root = pre[i];
	if ( post[j+len-1] != root )
		return 0;
	t = pos[root];
	if ( !(k <= t && t <= k+len-1) ) return 0;
	left = t-k, right = k+len-1-t;
	return f(left,i+1,j,k) && f(right,i+1+left,j+left,t+1);
}

int main() {
	int i,j,k;
	for ( ;1 == scanf("%d",&n); ) {
		for ( i = 0; i < n; ++i )
			scanf("%d",&pre[i]), --pre[i];
		for ( i = 0; i < n; ++i )
			scanf("%d",&post[i]), --post[i];
		for ( i = 0; i < n; ++i )
			scanf("%d",&in[i]), pos[--in[i]] = i;
		puts(f(n,0,0,0)?"yes":"no");
	}
	return 0;
}

