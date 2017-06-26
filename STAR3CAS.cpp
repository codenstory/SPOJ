/*
 * TOPIC: bfs
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
#define N 21
using namespace std;

int x[N],n,seen[N],yes,d[N],
	q[N],*head,*tail;

int main() {
	int i,j,k,ts;
	for ( scanf("%d",&ts); ts--; ) {
		for ( scanf("%d",&n), i = 0; i < n; scanf("%d",&x[i++]) ) ;
		for ( head = tail = q, seen[*tail++ = 0] = ++yes, d[0] = 0; head < tail; ) {
			if ( (i = *head++) == n ) continue ;
			if ( seen[i+1] != yes || d[i+1] > d[i]+1 )
				seen[*tail++ = i+1] = yes, d[i+1] = d[i]+1;
			if ( (j = i+x[i]) <= n && j >= 0 )
				if ( seen[j] != yes || d[j] > d[i]+1 )
					seen[*tail++ = j] = yes, d[j] = d[i]+1;
		}
		printf("%d\n",seen[n] == yes?d[n]:-1);
	}
	return 0;
}

