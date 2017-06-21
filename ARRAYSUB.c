/*
 * ARRAYSUB
 * TOPIC: deque, interview classic
 * status: Accepted
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N (1<<21)
#define NX(k) (((k)+1)&(N-1))
#define PR(k) (((k)+N-1)&(N-1))

int n,m,c[N],head,tail,cnt,q[N];

void add_to_front( int x ) { head = PR(head), ++cnt, q[head] = x; }
void add_to_rear( int x )  { q[tail] = x, tail = NX(tail), ++cnt; }
void pop_front() { head = NX(head), --cnt; }
void pop_back()  { tail = PR(tail), --cnt; }
int front() { return q[head]; }
int back()  { return q[PR(tail)]; }

int main() {
	int i,j,k;
	while ( 1 == scanf("%d",&n) ) {
		for ( i = 0; i < n; ++i )
			scanf("%d",&c[i]);
		scanf("%d",&m);
		for ( head=tail=cnt=0, add_to_rear(0), i = 1; i < m; ++i ) {
			if ( c[i] > c[back()] ) 
				add_to_rear(i);
			else {
				for ( ;cnt && c[i] >= c[front()]; pop_front() ) ;
				add_to_front(i);
			}
		}
		for ( printf("%d",c[back()]), j = 0, i = m; j+m < n; ++i, ++j ) {
			for ( ;cnt && back() < j+1; pop_back() ) ;
			if ( !cnt || c[i] > c[back()] )
				add_to_rear(i);
			else {
				for ( ;cnt && c[i] >= c[front()]; pop_front() ) ;
				add_to_front(i);
			}
			printf(" %d",c[back()]);
		}
		putchar('\n');
	}
	return 0;
}

