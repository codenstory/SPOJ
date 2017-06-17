/*
 * PATULJCI: Snow Whote and the N dwarfs
 * TOPIC: binary search, segment tree
 * status:
 */
#include <iostream>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 300100
#define MAXC 100100
using namespace std;

int h[MAXN],n,m,pics;
vector<int> pos[MAXC];

int main() {
	int i,j,k,low,high,mid;
	for ( ;2 == scanf("%d %d",&n,&m); ) {
		for ( i = 0; i < m; pos[i++].clear() ) ;
		for ( i = 0; i < n; ++i )
			scanf("%d",h+i), pos[--h[i]].push_back(i);
		for ( scanf("%d",&pics), k = 0; k < pics && 2 == scanf("%d %d",&i,&j); ++k ) {
			--i, --j;
		}
	}
	return 0;
}

