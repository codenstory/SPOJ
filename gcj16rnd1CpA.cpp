/*
 */
#include <bits/stdc++.h>
#define A 26
#define N 0x400
#define xchg(x,y) (swap(x,y))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]),xchg(heap[i],heap[j]))
using namespace std;

int n,m[A],pos[N],heap[N<<1],cnt;

void push( int x ) {
	int i,j;
	if ( pos[x] < 0 ) 
		pos[heap[cnt]= x]= cnt, ++cnt;
	for ( j= pos[x]; j && m[heap[i= (j-1)>>1]] < m[heap[j]]; bubble, j= i ) ;
}

int top() { return *heap; }

int pop() {
	int i,j,x= *heap;
	if ( cnt+= (pos[x]=-1) )
		pos[*heap= heap[cnt]]= 0;
	for ( j= 0; (i=j,j<<=1,++j) < cnt; bubble ) {
		if ( j < cnt-1 && m[heap[j]] < m[heap[j+1]] ) ++j;
		if ( m[heap[i]] >= m[heap[j]] ) break ;
	}
	return x;
}

int main() {
	int i,j,k,ts,cs= 0,x,y;
	for ( scanf("%d",&ts); ts-- && 1 == scanf("%d",&n); ) {
		for ( k= 0, i = 0; i < n; ++i )
			scanf("%d",m+i), k+= m[i];
		for ( i= 0; i < n; ++i )
			assert( 2*m[i] <= k ) ;
		for ( cnt= 0, i= 0; i < n; pos[i++]= -1 ) ;
		for ( i= 0; i < n; push(i++) ) ;
		printf("Case #%d:",++cs);
		for ( ;cnt; ) {
			x= pop();
			assert( cnt >= 1 );
			if ( 2*max(m[top()],(m[x]-1)) <= k-1 ) {
				if ( --m[x] ) push(x);
				--k, printf(" %c",x+'A');
				continue ;
			}
			y= pop();
			printf(" %c%c",x+'A',y+'A'), k-= 2;
			if ( --m[x] ) push(x);
			if ( --m[y] ) push(y);
		}
		putchar('\n');
	}
	return 0;
}

