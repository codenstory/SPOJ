/*
 * PATULJCI
 * TOPIC: range mode queries
 * status: Accepted
 * NOTES: http://spoj-solutions.blogspot.com/2015/10/patuljci-snow-white-and-n-dwarfs.html
 */
#include <cassert>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#define MAXC (100100)
#define N (300100)
#define Q (N<<4)
#define tol (1e-9)
#define L(v) ((v)<<1)
#define R(v) (1|L(v))
using namespace std;

vector<int> pos[MAXC];
int n,c[N],num_colors,
	candidate[Q],_count[Q];

void build( int v, int i, int j ) {
	if ( i == j ) {
		candidate[v]= c[i], _count[v]= 1;
		return ;
	}
	int k= (i+j)>>1;
	build(L(v),i,k), build(R(v),k+1,j);
	if ( candidate[L(v)] == candidate[R(v)] )
		candidate[v]= candidate[L(v)], _count[v]= _count[L(v)]+_count[R(v)];
	else if ( _count[L(v)] > _count[R(v)] )
		candidate[v]= candidate[L(v)], _count[v]= _count[L(v)]-_count[R(v)];
	else 
		candidate[v]= candidate[R(v)], _count[v]= _count[R(v)]-_count[L(v)];
}

pair<int,int> 
query( int v, int i, int j, int qi, int qj ) {
	if ( qi > j || qj < i )
		return {0,0};
	if ( qi <= i && j <= qj ) 
		return {candidate[v],_count[v]};
	int k= (i+j)>>1;
	pair<int,int> left= query(L(v),i,k,qi,qj),
				  right= query(R(v),k+1,j,qi,qj);
	if ( left.first == right.first )
		return {left.first,left.second+right.second};
	else if ( left.second > right.second )
		return {left.first,left.second-right.second};
	else return {right.first,right.second-left.second};
}

int freq( int x, int i ) {
	if ( !pos[x].size() ) return 0;
	int n= pos[x].size();
	if ( pos[x][0] > i ) return 0;
	assert( pos[x][0] <= i );
	if ( pos[x][n-1] <= i )
		return n;
	assert( pos[x][n-1] > i );
	int low, high, mid;
	for ( low= 0, high= n-1; low+1<high; pos[x][mid=(low+high)>>1]<=i?(low=mid):(high=mid) ) ;
	return high;
}

int occ( int x, int i, int j ) { return freq(x,j)-freq(x,i-1); }

int main() {
	int i,j,k,qr;
	for ( ;2 == scanf("%d %d",&n,&num_colors); ) {
		for ( i= 1; i <= num_colors; pos[i++].clear() ) ;
		for ( i= 0; i < n; scanf("%d",&c[i++]), pos[c[i-1]].push_back(i-1) ) ;
		build(1,0,n-1);
		for ( scanf("%d",&qr); qr-- && 2 == scanf("%d %d",&i,&j); ) {
			pair<int,int> res= query(1,0,n-1,i-1,j-1);
			if ( res.first == 0 || res.second == 0 )
				puts("no");
			else {
				int occurrences= occ(res.first,i-1,j-1);
				if ( occurrences*2 > (j-i+1) )
					printf("yes %d\n",res.first);
				else puts("no");
			}
		}
	}
	return 0;
}

