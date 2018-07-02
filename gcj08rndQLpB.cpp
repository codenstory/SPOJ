/*
 * status: Accepted
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <queue>
#define N 0x200
enum { S, T };
using namespace std;

int turnaround,na,nb,n[2],mate[2][N],adj[N][N],deg[N],visited[N],yes;
queue<int> q;

typedef struct mytime {
	int hh, mm;
	mytime() {};
	mytime( int hh, int mm ) : hh(hh), mm(mm) {};
	bool operator < ( const mytime &other ) const {
		return hh == other.hh && mm < other.mm || hh < other.hh;
	}
	bool operator == ( const mytime &other ) const {
		return hh == other.hh && mm == other.mm;
	}
} mytime;

mytime operator + ( const mytime &t, const int dt ) {
	int hh= t.hh, mm= t.mm+dt;
	if ( mm >= 60 )
		++hh, mm%= 60;
	return mytime(hh,mm);
}

mytime t[2][N];

int alt_path( int x ) {
	int i,y;
	if ( visited[x] == yes ) return 0;
	for ( visited[x]= yes, i= 0; i < deg[x]; ++i )
		if ( mate[T][y= adj[x][i]] < 0 || alt_path(mate[T][y]) ) {
			mate[T][mate[S][x]= y]= x;
			return 1;
		}
	return 0;
}

void bpm() {
	int i,j,k,x,y;
	bool flag;
	for (;!q.empty(); q.pop() ) ;
	for ( x= 0; x < na+nb; q.push(x++) ) ;
	do {
		for ( flag= false, k= q.size(), ++yes; k--; ) {
			x= q.front(), q.pop();
			if ( alt_path(x) ) flag= true ;
			else q.push(x);
		}
	} while ( flag ) ;
}

int main() {
	int i,j,k,ts,cs= 0;
	for ( scanf("%d\n",&ts); ts--; ) {
		scanf("%d\n",&turnaround);
		for ( scanf("%d %d\n",&na,&nb), i= 0; i < na; ++i ) 
			scanf("%d:%d %d:%d\n",&t[0][i].hh,&t[0][i].mm,&t[1][i].hh,&t[1][i].mm);
		for ( i= 0; i < nb; ++i ) 
			scanf("%d:%d %d:%d\n",&t[0][i+na].hh,&t[0][i+na].mm,&t[1][i+na].hh,&t[1][i+na].mm);
		memset(deg,0,sizeof deg);
		for ( memset(mate,-1,sizeof mate), i= 0; i < na+nb; ++i )
			for ( j= 0; j < na+nb; ++j )
				if ( !(i < na && j < na || i >= na && j >= na) ) {
					mytime tt= t[1][i]+turnaround;
					if ( tt < t[0][j] || tt == t[0][j] ) 
						adj[i][deg[i]++]= j;
				}
		int ans[2]= {0};
		for ( bpm(), i= 0; i < na+nb; ++i ) {
			if ( mate[T][i] < 0 ) 
				++ans[i<na?S:T];
		}
		printf("Case #%d: %d %d\n",++cs,ans[S],ans[T]);
	}
	return 0;
}

