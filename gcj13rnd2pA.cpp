/**
 * TOPIC: mincost maxflow
 * status:
 */
#include <bits/stdc++.h>
using std::vector;
using std::pair;
using std::string;
using i64= std::int64_t;
#define N (1<<21)
#define INF_CAP (1ull<<60)
#define MOD (1000002013ll)

std::istream &is = std::cin;
std::ostream &os = std::cout;
int n;

class binary_heap {
    std::vector<int> heap,pos;
    int cnt,n;
#define bubble (std::swap(pos[heap[i]],pos[heap[j]]), std::swap(heap[i],heap[j]))
    void push( int x, const std::function<int(int,int)> &f) {
        int i,j;
        if ( pos[x] < 0 )
            pos[heap[cnt]= x]= cnt, ++cnt;
        for ( j= pos[x]; j and f(heap[i= (j-1)>>1],heap[j]) > 0; bubble, j= i ) ;
    }
    int pop( const std::function<int(int,int)> &f ) {
        int x= heap[0],i,j;
        if ( cnt+= (pos[x]= -1) )
            pos[heap[0]= heap[cnt]]= 0;
        for ( j= 0; (i= j, j<<= 1, ++j) < cnt; bubble ) {
            if ( j < cnt-1 and f(heap[j],heap[j+1]) > 0 ) ++j;
            if ( f(heap[i],heap[j]) <= 0 ) break ;
        }
        return x;
    }
    const std::function<int(int,int)> &f;
public:
    binary_heap( int n, const std::function<int(int,int)> &f ): f(f) {
        this->n= n, cnt= 0;
        heap.resize(3*n+7), pos.resize(n);
        std::fill(pos.begin(),pos.end(),-1);
    }
    void push( int x ) {
        push(x,f);
    }
    int pop() {
        return pop(f);
    }
    bool empty() const {
        return cnt == 0;
    }
};

class mcmf {
    std::vector<std::optional<int>> last, next;
    std::vector<int> to,parent;
    std::vector<i64> cap,flow,w,d,pi;
    int n,V,E,src,sink,yes;
    std::vector<int> seen;
    std::unique_ptr<binary_heap> heap;

    bool reachable() {
        int x,y;
        assert( heap != nullptr and heap->empty() );
        for ( d[src]= 0, seen[src]= ++yes, heap->push(src); not heap->empty(); )
            for ( auto i= last[x= heap->pop()]; i; i= next[*i] )
                if ( flow[*i] < cap[*i] and (seen[y= to[*i]] != yes or d[y] > d[x]+pi[x]-pi[y]+w[*i]) )
                    seen[y]= yes, d[y]= d[x]+pi[x]-pi[y]+w[parent[y]= *i], heap->push(y);
        for ( x= 0; x < n+2 and seen[sink] == yes; ++x )
            if ( seen[x] == yes )
                pi[x]+= d[x];
        return seen[sink] == yes;
    }

public:

    void add_arc( int x, int y, i64 c, i64 weight ) {
        int i= E++, j= E++;
        if ( j >= to.size() )
            to.resize(j+1);
        if ( j >= cap.size() )
            cap.resize(j+1);
        if ( j >= flow.size() )
            flow.resize(j+1);
        if ( j >= w.size() )
            w.resize(j+1);
        to[i]= y, next[i]= last[x], last[x]= i;
        to[j]= x, next[j]= last[y], last[y]= j;
        cap[i]= c, cap[j]= 0, flow[i]= flow[j]= 0;
        w[i]= weight, w[j]= -weight;
    }

    mcmf( int n ) {
        E= 0, V= this->n= n+2, src= V-2, sink= V-1;
        last.resize(V), next.resize(V*V), to.resize(V*V);
        cap.resize(V*V), flow.resize(V*V), w.resize(V*V);
        std::fill(last.begin(),last.end(),std::nullopt);
        d.resize(V), pi.resize(V);
        seen.resize(V), std::fill(seen.begin(),seen.end(),0);
        parent.resize(V);
        std::fill(pi.begin(),pi.end(),0);
        auto &D= d;
        auto comparison_function= [&D](int x, int y) {return D[x] < D[y]?-1:(D[x]==D[y]?0:1);};
        heap= std::make_unique<binary_heap>(V,comparison_function);
    }

    i64 mincost_maxflow() {
        i64 cost= 0ll, df;
        int x,y,i;
        for (;reachable();) {
            df= INF_CAP;
            for ( auto z= sink; z != source(); ) {
                i= parent[z];
                assert( flow[i] < cap[i] );
                df= std::min(df,cap[i]-flow[i]), z= to[i^1];
            }
            assert( 0 < df and df < INF_CAP );
            for ( auto z= sink; z != source();
            i= parent[z], cost+= df*w[i], cost%= MOD, flow[i]+= df, flow[i^1]-= df, z= to[i^1] ) ;
        }
        return cost;
    }

    int source() const { return src; }

    int dest() const { return sink; }

};

i64 calc_dist( int x, int y ) {
    assert( x <= y );
    if ( x == y )
        return 0;
    i64 k= y-x;
    return (n+(n-(k-1)))*k/2;
}

void solve(int cs) {
    os << "Case #" << cs << ": ";
    std::vector<std::pair<std::pair<int,int>,i64>> vec{};
    i64 i,j,k,m,total= 0ll;
    is >> n >> m;
    vec.resize(m);
    for ( auto &z: vec )
        is >> z.first.first >> z.first.second >> z.second;
    std::sort(vec.begin(),vec.end());
    auto g= std::make_unique<mcmf>(n+n);
    std::vector<int> lhs{},rhs{};
    std::vector<std::pair<std::pair<int,int>,i64>> aggr{};
    for ( i= 0; i < m; i= j ) {
        for ( k= 0, j= i; j < m and vec[i].first == vec[j].first; k+= vec[j++].second ) ;
        auto x= vec[i].first.first, y= vec[i].first.second;
        assert( x < y );
        aggr.emplace_back(vec[i].first,k);
        lhs.push_back(x), rhs.push_back(y);
        total+= k*calc_dist(x,y), total%= MOD;
    }
    std::sort(lhs.begin(),lhs.end());
    std::sort(rhs.begin(),rhs.end());
    lhs.erase(std::unique(lhs.begin(),lhs.end()),lhs.end());
    rhs.erase(std::unique(rhs.begin(),rhs.end()),rhs.end());
    for ( auto &z: aggr ) {
        auto x= z.first.first, y= z.first.second;
        auto pp= z.second;
        x= std::lower_bound(lhs.begin(),lhs.end(),x)-lhs.begin();
        y= std::lower_bound(rhs.begin(),rhs.end(),y)-rhs.begin();
        assert( lhs[x] == z.first.first );
        assert( rhs[y] == z.first.second );
        g->add_arc(g->source(),x,pp,0ll);
        g->add_arc(y+lhs.size(),g->dest(),pp,0ll);
    }
    for ( i= 0; i < lhs.size(); ++i )
        for ( j= 0; j < rhs.size(); ++j )
            if ( lhs[i] <= rhs[j] )
                g->add_arc(i,j+lhs.size(),INF_CAP,calc_dist(lhs[i],rhs[j]));
    for ( i= 0; i < lhs.size(); ++i )
        for ( j= 0; j < lhs.size(); ++j )
            if ( i > j )
                g->add_arc(i,j,INF_CAP,0ll);
    for ( i= 0; i < rhs.size(); ++i )
        for ( j= 0; j < rhs.size(); ++j )
            if ( i > j )
                g->add_arc(i+lhs.size(),j+lhs.size(),INF_CAP,0ll);

    os << (total+MOD-g->mincost_maxflow())%MOD << std::endl;
}

int main() {
    int cs = 0, ts;
    for (is >> ts; ts--; solve(++cs));
    return 0;
}   
 