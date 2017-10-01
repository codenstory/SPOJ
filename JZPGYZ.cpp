/*
 * JZPGYZ
 * TOPIC: Suffix Automaton, Offline Queries, BIT
 * status: Accepted
 * NOTE: with the help of https://github.com/miskcoo/oicode/blob/master/spoj/JZPGYZ.cpp
 * although the idea was the same, the author does not use delimiters
 */
#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#define N (1<<21)
#define oo (1<<30)
#define LEN 256
#define L(k) ((k)&((~(k))+1ULL))
#define FG (fgets(buff,sizeof buff,stdin))
#pragma comment(linker, ”/STACK:36777216“)
using namespace std;

int num_colors,leaf[N],*qtr,tr[N],m,occs[(int)1e6+5],ans[N],
	_in[N], _out[N], tick, card[N], dfn[N];
vector<int> adj[N];

void update( unsigned int i, int dt ) { for ( ;i < m; tr[i] += dt, i += L(i) ) ; }
int prefix( unsigned int i ) {
	int ans = 0;
	for ( ;i; ans += tr[i], i &= ~L(i) ) ;
	return ans;
}

struct cell {
		cell *slink;
		unordered_map<int,cell *> son;
		vector<cell *> e;
		int len,idx;
		bool flag;
		cell() {};
		void add_son( int ch, cell *y ) { son[ch] = y; }
		cell *get_son( int ch ) { return son.find(ch)==son.end()?NULL:son[ch]; }
} pool[N], *ptr = pool, *root, *sink, *q[N], **head, **tail;

class SA {
public:
#define is_solid(x,ch) ((x)->get_son(ch)->len == (x)->len+1)
	cell *newcell( int len, int i ) {
		cell *x = ptr++;
		adj[x-pool].clear();
		x->len = len, x->slink = NULL, x->son.clear();
		x->idx = i, x->flag = false, x->e.clear();
		return x;
	}
	cell *split( cell *x, int ch, int idx ) {
		cell *y = x->get_son(ch), *z = newcell(x->len+1,idx), *t;
		z->slink = y->slink, y->slink = z;
		for ( x->add_son(ch,z); (x=x->slink) && (t=x->get_son(ch)) && t == y; x->add_son(ch,z) ) ;
		for ( unordered_map<int ,cell *> :: iterator it = y->son.begin(); it != y->son.end(); ++it )
			z->add_son(it->first,it->second);
		return z;
	}
	void dfs( cell *x ) {
		cell *y;
		dfn[++tick] = x-pool, _in[x-pool] = tick;
		for ( int i = 0; i < (int)x->e.size(); dfs(x->e[i++]) ) ;
		_out[x-pool] = tick;
	}
	void init() { tick = 0, qtr = leaf, ptr = pool, root = sink = newcell(0,-1); }
	void reset() { sink = root; }
	SA() {};
	void update( int ch, int idx ) {

		if ( sink->get_son(ch) ) {
			if ( is_solid(sink,ch) ) 
				sink = sink->get_son(ch);
			else 
				sink = split(sink,ch,idx);
			return ;
		}

		cell *new_sink = newcell(sink->len+1,idx), *x;
		for ( (x=sink)->add_son(ch,new_sink); (x=x->slink) && !x->get_son(ch); x->add_son(ch,new_sink) ) ;
		new_sink->slink = !x?root:is_solid(x,ch)?x->get_son(ch):split(x,ch,idx), sink = new_sink;
	}

	void preprocess() {
		for ( cell *x = pool, *y; x-pool < ptr-pool; ++x )
			if ( y = x->slink )
				y->e.push_back(x);
		dfs(root);
		for ( m = 0; (1<<m) <= tick; ++m ) ;
		m = (1<<m);
	}
	pair<int,int> get_interval( int *s, int n ) {
		cell *x,*y;
		int i = 0;
		for ( x = root; i < n; x = y, ++i )
			if ( !(y = x->get_son(s[i])) ) 
				return make_pair(0,-1);
		return make_pair(_in[x-pool],_out[x-pool]);
	}
	int get_sink() { return sink-pool; }
} SA;

char buff[N];
int str[N];

struct ques_t {
	int id, l, r, ans;
	bool operator < ( const ques_t &a ) const {
		return r < a.r;
	}
} ques[360100];

int main() {
	int i,j,k,n,qr,len;
	for ( ;FG && 2 == sscanf(buff,"%d %d",&n,&qr); ) {
		for ( num_colors = n, SA.init(), i = 0; i < n; ++i ) 
			for ( FG, SA.reset(), j = 0; buff[j] && buff[j] != '\n'; SA.update(buff[j++],i), adj[SA.get_sink()].push_back(i) ) ;
		for ( SA.preprocess(), k = 1; k <= qr && FG; ++k ) {
			for ( i = 0; buff[i] && buff[i] != '\n'; str[i] = buff[i], ++i ) ;
			pair<int,int> r = SA.get_interval(str,i);
			ques[k].id = k;
			if ( r.second < r.first ) {
				ques[k].ans = 0, ques[k].l = -1;
				continue ;
			}
			ques[k].l = r.first, ques[k].r = r.second;
		}
		sort(ques+1,ques+qr+1);
		int now = 1;
		for ( i = 0; i < m; occs[i] = 0, tr[i++] = 0 ) ; 
		for ( ;now <= qr && ques[now].l == -1; ++now ) ;
		for ( i = 1; i <= tick && now <= qr; ++i ) {
			for ( int l = 0; l < (int)adj[dfn[i]].size(); ++l ) {
				int v = adj[dfn[i]][l];
				if ( occs[v] ) 
					update(occs[v],-1);
				update(occs[v] = i,1);
			}
			for ( ;now <= qr && ques[now].r == i; ++now )
				ques[now].ans = prefix(ques[now].r)-prefix(ques[now].l-1);
		}
		for ( int l = 1; l <= qr; ++l )
			ans[ques[l].id] = ques[l].ans;
		for ( i = 1; i <= qr; printf("%d\n",ans[i++]) ) ;
	}
	return 0;
}

