/**
 * TOPIC: implementation, simulation, BFS
 * status: Accepted
 */
#include <bits/stdc++.h>
#define vc(x,y) (0<=(x) and (x)<m and 0<=(y) and (y)<n)

std::istream &is = std::cin;
std::ostream &os = std::cout;
#define EMPTY '.'
#define ROCK '#'
#define enc(x,y,l,r) ((x)|((y)<<6)|((l)<<12)|((r)<<18))
using state= unsigned int;
#define Q (1<<24)
#define oo (0xfffffffful)

std::vector<std::string> g;
unsigned int d[Q];
state q[Q],*head,*tail;
int m,n;

bool is_empty( int x, int y ) {
    return vc(x,y) and g[x][y] == EMPTY;
}

bool is_rock( int x, int y ) {
    return vc(x,y) and g[x][y] == ROCK;
}

void update( state from, state to, unsigned int w ) {
    assert( from < Q and to < Q );
    assert( d[from] < +oo );
    if ( d[to] > d[from]+w )
        d[*tail++= to]= d[from]+w;
}

void solve( int cs ) {
    int i,j,k,F,x,y,yleft,yright,t;
    os << "Case #" << cs << ": ";
    is >> m >> n >> F, g.resize(m);
    for ( auto &v: g )
        is >> v;
    assert( is_empty(0,0) and is_rock(1,0) );
    memset(d,0xff,sizeof d);
    unsigned int ans= +oo;
    state v, u= enc(0,0,0,0);
    for ( head= tail= q, d[*tail++= u]= 0; head < tail; ) {
        u= *head++, x= (u&0x3f), y= ((u>>6)&0x3f), yleft= ((u>>12)&0x3f), yright= (u>>18);
        if ( x == m-1 ) {
            ans= std::min(ans,d[u]);
            continue ;
        }
        assert( x+1 < m );
        auto ly= y-yleft, ry= y+yright;
        for ( ;ly-1 >= 0 and is_empty(x,ly-1); --ly ) ;
        for ( ;ry+1 < n and is_empty(x,ry+1); ++ry ) ;
        auto lh= y, rh= y;
        for ( ;lh >= ly and is_rock(x+1,lh); --lh ) ;
        for ( ;rh <= ry and is_rock(x+1,rh); ++rh ) ;
        if ( lh >= 0 and ly <= lh ) {
            int nnx= x+1, nny= lh;
            for ( ;is_empty(nnx,nny); ++nnx ) ;
            if ( nnx-1-x <= F ) {
                update(u, enc(nnx - 1, nny, 0, 0), 0);
            }
        }
        if ( rh < n and rh <= ry ) {
            int nnx= x+1, nny= rh;
            for ( ;is_empty(nnx,nny); ++nnx ) ;
            if ( nnx-1-x <= F )
                update(u,enc(nnx-1,nny,0,0),0);
        }
        for ( i= lh+2; i < rh; ++i )
            for ( j= i; j < rh; ++j ) {
                int nnx= x+2, nny= i;
                for ( ;is_empty(nnx,nny); ++nnx ) ;
                if ( nnx-1-x <= F ) {
                    if ( nnx-1 == x+1 )
                        update(u,enc(nnx-1,nny,0,j-i),j-i+1);
                    else update(u,enc(nnx-1,nny,0,0),j-i+1);
                }
            }
        for ( i= rh-2; i > lh; --i )
            for ( j= i; j > lh; --j ) {
                int nnx= x+2, nny= i;
                for ( ;is_empty(nnx,nny); ++nnx ) ;
                if ( nnx-1-x <= F ) {
                    if ( nnx-1 == x+1 )
                        update(u,enc(nnx-1,nny,i-j,0),i-j+1);
                    else update(u,enc(nnx-1,nny,0,0),i-j+1);
                }
            }
    }
    if ( ans < +oo )
        os << "Yes " << ans << std::endl;
    else os << "No" << std::endl;
}

int main() {
    int i, j, k, cs = 0, ts;
    for ( is >> ts; ts--; solve(++cs) ) ;
    return 0;
}   
 