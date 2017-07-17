/*
 * TOPIC: stacks, largest rectangle in histogram
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
#include <stack>
#include <queue>
#define N (1<<20)
#define tol (1e-13)
typedef long long i64;
using namespace std;

deque<int> q;
i64 n,h[N],_left[N],_right[N];

int main() {
	int i,j,k;;
	i64 ans;
	for ( ;1 == scanf("%lld",&n) && n; ) {
		for ( i = 1; i <= n; ++i ) scanf("%lld",&h[i]);
		for ( h[0] = -1, q.push_front(0), i = 1; i <= n; _left[i] = i-q.front(), q.push_front(i++) )
			for ( ;!q.empty() && h[q.front()] >= h[i]; q.pop_front() ) ;
		for ( ;!q.empty(); q.pop_front() ) ;
		for ( h[n+1] = -1, q.push_front(n+1), i = n; i >= 1; _right[i] = q.front()-i, q.push_front(i--) )
			for ( ;!q.empty() && h[q.front()] >= h[i]; q.pop_front() ) ;
		for ( ans = 0, i = 1; i <= n; ++i )
			ans = max(ans,(_right[i]+_left[i]-1)*h[i]);
		printf("%lld\n",ans);
	}
	return 0;
}

