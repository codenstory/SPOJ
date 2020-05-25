/*
 * JANE: Jane and Tarzan
 * TOPIC: bfs
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N BIT(SH)
#define SH 17
#define BIT(k) (1ULL<<(k))
#define MASK(k) (BIT(k)-1ULL)
#define Q BIT(21)
#define enc(x,y) ((x)|(((u64)(y))<<SH))
typedef unsigned long long u64;
using namespace std;

int n,D,h[N];
std::queue<u64> q;
std::set<u64> s;

int main() {
	int i,j,k,x,y,nx,ny;
	u64 u,v;
#ifndef ONLINE_JUDGE
	freopen("input.txt","r",stdin);
#endif
	for ( ;2 == scanf("%d %d",&n,&D) and (n or D); ) {
		for ( i = 0; i < n; scanf("%d",&h[i++]) ) ;
		for ( s.clear(), i = 0; i+1 < n; ++i )
			if ( abs(h[i]-h[i+1]) <= D )
				s.insert(u = enc(i+1,i)), q.push(u);
		for ( i = 0; i < n; s.insert(u=enc(i,i)), q.push(u), ++i ) ;
		for (  ;not q.empty(); q.pop() )
			for ( u = q.front(), x=(u&MASK(SH)), y = (u>>SH), nx = x-1; nx <= x+1; nx+= 2 )
				for ( ny = y-1; nx >= 0 && nx < n && ny <= y+1; ny+= 2 )
					if ( 0 <= ny && ny < n && abs(h[nx]-h[ny]) <= D )
						if ( s.find(v = enc(max(nx,ny),min(nx,ny))) == s.end() )
							s.insert(v), q.push(v);
		for ( set<u64>::iterator it = s.begin(); it != s.end(); ++it ) {
			x = (*it&MASK(SH)), y = (*it>>SH);
			if ( x != y && y < x ) 
				printf("%d %d\n",y+1,x+1);
		}
	}
	return 0;
}

