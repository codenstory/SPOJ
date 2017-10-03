/*
 * CATINV
 * TOPIC: sweepline
 * status: Accepted
 */
#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
using namespace std;
#define W (100001)
#define Q (1<<18)
#define N (Q>>1)
#define xchg(x,y) (((x)==(y))||((x)^=(y),(y)^=(x),(x)^=(y)))
#define bubble (xchg(pos[heap[i]],pos[heap[j]]),xchg(heap[i],heap[j]))
#define oo (1<<30)

int getnum() {
	register int ch,n = 0,sign = 1;
	static int dig[256] = {0};
	if ( !dig['0'] )
		for ( ch = '0'; ch <= '9'; dig[ch++] = 1 ) ;
	for ( ;(ch = getchar_unlocked()) != EOF && !(ch=='-'||dig[ch]); ) ;
	if ( ch=='-' )
		sign = -1, ch = getchar_unlocked();
	for ( n=ch-'0'; (ch=getchar_unlocked())!=EOF && dig[ch]; n=10*n+ch-'0' ) ;
	return sign*n;
}

typedef struct list {
	int to[N],next[N],
		last[W],E;
} list;

void init( list *l ) {
	int i;
	for ( l->E = 0, i = 0; i < W; l->last[i++] = -1 ) ;
}

void add( list *l, int val, int idx ) {
	int i = l->E++;
	l->to[i] = idx, l->next[i] = l->last[val], l->last[val] = i;
}

int heap[Q],pos[N],cnt,n,left[N],right[N],h[W],seen[W],yes;
list b,e;
pair<int,int> best[N];

pair<int,int> process( vector<pair<int,int> > &vec ) {
	int left, right, bl = +oo, br = 0, i;
	for ( left = vec[0].first, right = vec[0].second, i = 1; i < (int)vec.size(); ++i ) {
		if ( right < vec[i].first ) {
			if ( right-left > br-bl || right-left == br-bl && left < bl ) 
				bl = left, br = right;
			left = vec[i].first, right = vec[i].second;
		}
		else if ( right < vec[i].second )
			right = vec[i].second;	
	}
	if ( right-left > br-bl || right-left == br-bl && left < bl ) 
		bl = left, br = right;
	return make_pair(bl,br-1);
}

void push( int x ) {
	int i,j;
	if ( pos[x] < 0 ) 
		pos[heap[cnt] = x] = cnt, ++cnt;
	for ( j = pos[x]; j && left[heap[i=(j-1)>>1]] < left[heap[j]]; bubble, j = i ) ;
}

int pop() {
	int i,j,x = *heap;
	if ( cnt += (pos[x]=-1) )
		pos[*heap = heap[cnt]] = 0;
	for ( j = 0; (i=j,j<<=1,++j) < cnt; bubble ) {
		if ( j < cnt-1 && left[heap[j+1]] > left[heap[j]] ) ++j;
		if ( left[heap[i]] >= left[heap[j]] ) break ;
	}
	return x;
}

void erase( int x ) {
	left[x] = +oo, push(x);
	assert( x == pop() ) ;
}
int top() { return *heap; }

vector<pair<int,int> > vec[N];
int best_left[N],best_right[N];

int main() {
	int x,y,i,j,k,ts,qr,maxw,len,cs=0,prev;
	for ( ts=getnum(); ts--; ) {
		n=getnum(), qr=getnum();
		for ( maxw=0, cnt=0, init(&b), init(&e), i=0; i<n; pos[i++]=-1 ) {
			left[i] = getnum(), right[i] = getnum();
			assert ( left[i] <= right[i] );
			//++right[i];
			add(&b,left[i],i), add(&e,right[i],i);
			if ( right[i] > maxw ) maxw = right[i];
		}
		for ( x = 0; x < W; vec[x++].clear() ) ;
		for ( prev = 0, ++yes, x = 1; x <= maxw; ++x ) {
			if ( b.last[x] != -1 ) {
				if ( cnt ) {
					len = x-max(prev,left[top()]);
					if ( len > 0 ) {
						/*if ( seen[cnt] != yes || h[cnt] < len || h[cnt] == len && best_left[cnt] > max(prev,left[top()]) )
							seen[cnt] = yes, h[cnt] = len, best_left[cnt] = max(prev,left[top()]), best_right[cnt] = x-1;*/
						vec[cnt].push_back(make_pair(max(prev,left[top()]),x));
					}
				}
			}
			for ( i = b.last[x]; i >= 0; push(b.to[i]), i = b.next[i] ) ;
			if ( e.last[x] != -1 ) {
				len = x-max(prev,left[top()]);
				if ( len > 0 ) {
					/*if ( seen[cnt] != yes || h[cnt] < len || h[cnt] == len && best_left[cnt] > max(prev,left[top()]) )
						seen[cnt] = yes, h[cnt] = len, best_left[cnt] = max(prev,left[top()]), best_right[cnt] = x;*/
					vec[cnt].push_back(make_pair(max(prev,left[top()]),x));
				}
				prev = x;
			}
			for ( i = e.last[x]; i >= 0; erase(j), i = e.next[i] ) {
				j = e.to[i];
				assert( pos[j] >= 0 );
			}
		}
		for ( ++yes, x = 1; x <= maxw; ++x ) {
			if ( !vec[x].size() ) continue ;
			sort(vec[x].begin(),vec[x].end());
			best[x] = process(vec[x]);
			seen[x] = yes;
		}
		for ( printf("Case %d:\n",++cs); qr--; ) {
			if ( seen[k = getnum()] != yes ) puts("-1");
			else printf("%d %d\n",best[k].first,best[k].second);
			//assert( k > 0 );
		}
	}
	return 0;
}

