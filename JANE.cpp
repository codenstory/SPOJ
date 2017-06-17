/*
 * JANE: Jane and Tarzan
 * TOPIC: bfs
 * status:
 */
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#define N BIT(SH)
#define SH 17
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define Q BIT(SH+1)
#define enc(x,y) ((x)|(((u64)(y))<<SH))
typedef unsigned long long u64;
using namespace std;

int n,D,h[N],m;
u64 q[Q],*head,*tail;
set<u64> s;

int main() {
	int i,j,k,x,y,nx,ny;
	u64 u,v;
	for ( ;2 == scanf("%d %d",&n,&D); ) {
		for ( i = 0; i < n; scanf("%d",&h[i++]) ) ;
		for ( s.clear(), head=tail=q, i = 0; i+1 < n; ++i )
			if ( abs(h[i]-h[i+1]) <= D )
				s.insert(*tail++ = enc(i+1,i));
		for ( i = 0; i < n; s.insert(*tail++=enc(i,i)), ++i ) ;
		for ( m = 0; head<tail; )
			for ( u = *head++, x=(u&MASK(SH)), y = (u>>SH), nx = x-1; nx <= x+1; nx += 2 )
				for ( ny = y-1; nx >= 0 && nx < n && ny <= y+1; ny += 2 )
					if ( 0 <= ny && ny < n && abs(h[nx]-h[ny]) <= D )
						if ( s.find(v = enc(max(nx,ny),min(nx,ny))) == s.end() )
							s.insert(*tail++ = v);
		for ( set<u64>::iterator it = s.begin(); it != s.end(); ++it ) {
			x = (*it&MASK(SH)), y = (*it>>SH);
			if ( x != y && y < x ) 
				printf("%d %d\n",y+1,x+1);
		}
	}
	return 0;
}

