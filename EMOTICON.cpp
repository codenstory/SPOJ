/*
 * EMOTICON
 * TOPIC: Aho-Corasick, priority queue, line sweep, nailing the segments, greedy
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
#include <set>
#include <map>
#include <queue>
#define A 128
#define N 0x80
#define oo (1<<30)
#define FG (fgets(buff,sizeof buff,stdin))
using namespace std;

typedef struct cell {
	cell *slink;
	map<char,cell *> son;
	set<int> idx;
	cell() {};
} cell;

cell pool[1<<20],*ptr,*root,*q0,*base=pool;

cell *init() {
	cell *x = ptr++;
	x->son.clear(),x->slink=0,x->idx.clear();
	return x;
}

void push( char *s, int idx ) {
	cell *x;
	for ( x = root; x && *s; x = x->son[*s++] )
		if ( !x->son.count(*s) )
			x->son[*s] = init();
	x->idx.insert(idx);
}

typedef struct state {
	int time,sat;
	bool is_opening;
	state( int time, int sat, bool flag ) {
		this->time = time, this->sat = sat, this->is_opening = flag;
	}
} state;

bool operator < ( const state &a, const state &b ) {
	if ( a.time == b.time ) {
		if ( a.is_opening == b.is_opening )
			return false ;
		return a.is_opening ;
	}
	return a.time < b.time;
}

struct comparator {
	bool operator() ( const state &a, const state &b ) {
		return b < a;
	}
};

int m,n,L[N];
char buff[0x400];
queue<cell *> q;
priority_queue<state,vector<state>,comparator> pq;

int f( vector<pair<int,int> > &vec ) {
	int i,j,k,nail = -1,ans = 0;
	for ( i = 0; i < (int)vec.size(); ++i )
		pq.push(state(vec[i].first,vec[i].second,true));
	while ( !pq.empty() ) {
		state st = pq.top();
		pq.pop();
		if ( st.is_opening ) {
			pq.push(state(st.sat,st.time,false));
			continue ;
		}
		assert( st.sat <= st.time );
		if ( st.sat <= nail && nail <= st.time )
			continue ;
		else {
			assert( nail < st.sat ) ;
			nail = st.time, ++ans;
		}
	}
	return ans;
}

int main() {
	int i,j,k,len,qr,left,right,ans;
	char ch;
	cell *x,*y,*z;
	for ( ;2 == scanf("%d %d\n",&n,&qr) && (n||qr); ) {
		ptr = pool, q0 = init(), root = init();
		for ( i = 0; i < A; q0->son[i++] = root ) ;

		for ( i = 0; i < n; ++i ) {
			FG, L[i] = (k = strlen(buff))-1;
			assert( buff[k-1] == '\n' );
			buff[k-1] = '\0', push(buff,i);
		}

		for ( root->slink = q0, q.push(root); !q.empty(); ) {
			x = q.front(), q.pop();
			for ( map<char, cell *> :: iterator it = x->son.begin(); it != x->son.end(); ++it ) {
				y = it->second, ch = it->first;
				for ( z = x->slink; !z->son.count(ch); z = z->slink ) ;
				y->slink = z->son[ch];
				for ( set<int> :: iterator st = y->slink->idx.begin(); st != y->slink->idx.end(); y->idx.insert(*st++) ) ;
				q.push(y);
			}
		}
		vector<pair<int,int> > vec;
		for ( ans = 0, n = ptr-pool; qr--; ans += f(vec) ) {
			FG, buff[len = strlen(buff)] = '\0';
			for ( vec.clear(), x = root, i = 0; i < len; ++i ) {
				for ( ;!x->son.count(buff[i]); x = x->slink ) ;
				x = x->son[buff[i]];
				for ( set<int> :: iterator st = x->idx.begin(); st != x->idx.end(); ++st )
					vec.push_back(make_pair(i-L[*st]+1,i));
			}
		}
		printf("%d\n",ans);
	}
	return 0;
}

