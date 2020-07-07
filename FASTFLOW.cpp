/**
 * FASTFLOW
 * TOPIC: maximum flow
 * status: Accepted
 */
#include <bits/stdc++.h>
#define MAXV (5007)
#define MAXE (30100*2)
using i64= std::int64_t;
using node_type= int;
using size_type= int;
using value_type= i64;

class flow_network {

    node_type to[MAXE], src, sink;
    size_type last[MAXV], next[MAXE], E, n;
    value_type cap[MAXE], flow[MAXE];
    const value_type oo= (1<<29);
    node_type q[MAXV], *head, *tail;
    size_type visited[MAXV], yes, d[MAXV], seen[MAXV], indeed;

    bool bfs() {
        for ( head= tail= q, visited[*tail++= sink]= ++yes, d[sink]= 0; head < tail; ) {
            auto x= *head++, y= x;
            for ( auto i= last[x]; i != -1; i= next[i] )
                if ( cap[i^1] > flow[i^1] and visited[y= to[i]] != yes )
                    visited[*tail++= y]= yes, d[y]= d[x]+1;
        }
        return visited[src] == yes;
    }

#define min_(x,y) ((x)<(y)?(x):(y))

    value_type dfs( node_type x, value_type df ) {
        value_type dt;
        node_type y;
        if ( x == sink )
            return df;
        if ( seen[x] == indeed )
            return 0;
        seen[x]= indeed;
        for ( auto i= last[x]; i != -1; i= next[i] )
            if ( cap[i] > flow[i] and seen[y= to[i]] != indeed )
                if ( visited[y] == yes and d[y]+1 == d[x] )
                    if ( (dt = dfs(y, min_(cap[i]-flow[i], df))) ) {
                        flow[i] += dt, flow[i ^ 1] -= dt;
                        return dt;
                    }
        return 0;
    }

public:

    flow_network() {};

    void init( size_type n ) {
        E= 0, this->n= n, src= 0, sink= n-1, yes= 0, indeed= 0;
        for ( auto x= 0; x < n; seen[x]= visited[x]= 0, last[x++]= -1 ) ;
    }

    void add_arc( node_type x, node_type y, value_type c ) {
        size_type i= E++, j= E++;
        to[i]= y, next[i]= last[x], last[x]= i;
        to[j]= x, next[j]= last[y], last[y]= j;
        cap[i]= c, cap[j]= c, flow[i]= flow[j]= 0;
    }

    value_type maxflow() {
        value_type total= 0, df;
        for (;bfs();)
            for (;++indeed and (df= dfs(src,+oo)); total+= df) ;
        return total;
    }
} mf;

int getnum() {
    static int dig[256]= {0};
    int n,ch;
    if ( not dig['0'] )
        for ( ch= '0'; ch <= '9'; dig[ch++]= 1 ) ;
    for ( ;(ch= getchar_unlocked()) != EOF and not dig[ch]; ) ;
    for ( n= ch-'0'; (ch= getchar_unlocked()) != EOF and dig[ch]; n= 10*n+ch-'0' ) ;
    return n;
}

value_type cap[MAXV][MAXV];

int main() {

    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif

    std::istream &is= std::cin;
    std::ostream &os= std::cout;

    for ( size_type n,m; ; ) {
        n= getnum(), m= getnum();
        mf.init(n);
        node_type x,y;
        value_type c;
        //for ( ;m--; x= getnum(), y= getnum(), c= getnum(), mf.add_arc(--x,--y,c), mf.add_arc(y,x,c) ) ;
        for ( ;m--;  ) {
            x= getnum()-1, y= getnum()-1, c= getnum();
            if ( x == y ) continue ;
            cap[x][y]+= c, cap[y][x]+= c;
        }
        for ( x= 0; x < n; ++x )
            for ( y= x+1; y < n; ++y )
                if ( cap[x][y] > 0 )
                    mf.add_arc(x,y,cap[x][y]);
        os << mf.maxflow() << '\n';
        break;
    }

    return 0;

}
