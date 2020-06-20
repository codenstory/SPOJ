/**
 * TOPIC: simulation, priority queue, STL, sweepline
 * status:  Accepted
 */
#include <bits/stdc++.h>
#include "cstdio"
#include <iostream>
#include <optional>
#include <iostream>
#include <functional>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <memory>
#include <cassert>
#include <istream>
#include <ostream>
#define inbound(x) ((x))
#define outbound(x) ((x)+offset)
enum { LEN= 0x10, DIGL= 5, BASE= 100000ll };

using std::vector;
using std::pair;
using std::string;
using i64= long long;
#define N (1 << 21)
#define INF_CAP (1ull<<60)
#define MOD (1000002013ll)

std::istream &is = std::cin;
std::ostream &os = std::cout;
int n;

class bignum {
private:
    i64 c[LEN];
public:
    bignum( i64 x ) {
        memset(c,0,sizeof c);
        for ( int i= 0; x; c[i++]= x%BASE, x/= BASE ) ;
    }
    bignum &operator = ( i64 x ) {
        memset(c,0,sizeof c);
        for ( int i= 0; x; c[i++]= x%BASE, x/= BASE ) ;
        return *this;
    }
    bignum() {
        memset(c,0,sizeof c);
    }
    i64 operator[] ( int i ) const {
        return c[i];
    }
    i64 &operator[] ( int i ) {
        return c[i];
    }
    bignum &operator += ( const bignum &b ) {
        for ( int i= 0; i < LEN; ++i )
            c[i]+= b[i];
        for ( int i= 0; i < LEN; ++i ) {
            if ( c[i] >= BASE )
                c[i+1]+= c[i]/BASE, c[i]%= BASE;
        }
        return *this;
    }
};

bignum operator + ( const bignum &a, const bignum &b ) {
    bignum c;
    for ( int i= 0; i < LEN; ++i )
        c[i]= (a[i]+b[i]);
    for ( int i= 0; i < LEN; ++i ) {
        if ( c[i] >= BASE )
            c[i+1]+= c[i]/BASE, c[i]%= BASE;
    }
    return c;
}

bignum operator - ( const bignum &a, const bignum &b ) {
    bignum c;
    for ( int i= 0; i < LEN; ++i )
        c[i]= (a[i]-b[i]);
    for ( int i= 0; i < LEN; ++i ) {
        if ( c[i] >= BASE )
            c[i+1]+= c[i]/BASE, c[i]%= BASE;
        for ( ;c[i] < 0; --c[i+1], c[i]+= BASE ) ;
    }
    return c;
}

bool operator < ( const bignum &a, const bignum &b ) {
    for ( int i= LEN-1; i >= 0; --i ) {
        if (a[i] == b[i])
            continue;
        return a[i] < b[i];
    }
    return false ;
}

bool operator > ( const bignum &a, const bignum &b ) {
    for ( int i= LEN-1; i >= 0; --i ) {
        if (a[i] == b[i])
            continue;
        return a[i] > b[i];
    }
    return false ;
}

bool operator == ( const bignum &a, const bignum &b ) {
    for ( int i= LEN-1; i >= 0; --i ) {
        if (a[i] == b[i])
            continue;
        return false ;
    }
    return true ;
}

class binary_heap {
    std::vector<int> heap,pos;
    int cnt,n;
#define bubble (std::swap(pos[heap[i]],pos[heap[j]]), std::swap(heap[i],heap[j]))
    void push( int x, const std::function<int(int,int)> &f) {
        int i,j;
        assert( 0 <= x and x < n );
        if ( pos[x] < 0 )
            pos[heap[cnt]= x]= cnt, ++cnt;
        assert( pos[x] >= 0 );
        for ( j= pos[x]; j and f(heap[i= (j-1)>>1],heap[j]) > 0; bubble, j= i ) ;
    }
    int pop( const std::function<int(int,int)> &f ) {
        assert( not empty() );
        int x= heap[0],i,j;
        if ( cnt+= (pos[x]= -1) )
            pos[heap[0]= heap[cnt]]= 0;
        for ( j= 0; (i= j, j<<= 1, ++j) < cnt; bubble ) {
            if ( j < cnt-1 and f(heap[j],heap[j+1]) > 0 ) ++j;
            if ( f(heap[i],heap[j]) <= 0 ) break ;
        }
        return x;
    }

#undef bubble
    const std::vector<bignum> &d;
    std::function<int(int,int)> f;
public:
    binary_heap( int n, const std::vector<bignum> &d ): d(d) {
        this->n= n, cnt= 0;
        heap.resize(4*n+7), pos.resize(n);
        std::fill(pos.begin(),pos.end(),-1);
        f= [this]( int x, int y ) {
            return this->d[x]==this->d[y]?0:this->d[x]<this->d[y]?-1:1;
        };
    }
    void push( int x ) {
        push(x,f);
    }
    int pop() {
        assert( not empty() );
        return pop(f);
    }
    bool empty() const {
        return cnt == 0;
    }
};

class mcmf {
    std::vector<std::optional<int>> last, next;
    std::vector<int> to,parent;
    std::vector<i64> cap,flow,w;
    std::vector<bignum> d,pi;
    int n,V,E,src,sink,yes;
    std::vector<int> seen;
    std::unique_ptr<binary_heap> heap;

    bool reachable() {
        int x,y;
        assert( heap != nullptr and heap->empty() );
        for ( d[src]= 0, seen[src]= ++yes, heap->push(src); not heap->empty(); ) {
            x= heap->pop();
            assert( 0 <= x and x < V );
            for (auto i = last[x]; i; i = next[*i])
                if (flow[*i] < cap[*i] and (seen[y = to[*i]] != yes or d[y] > d[x] + pi[x] + w[*i] - pi[y]))
                    seen[y] = yes, d[y] = d[x] + pi[x] + w[parent[y] = *i] - pi[y], heap->push(y);
        }
        for ( x= 0; x < V; ++x )
            if ( seen[x] == yes ) {
                pi[x] += d[x];
            }
        return seen[sink] == yes;
    }

public:

    void add_arc( int x, int y, i64 c, i64 weight ) {
        assert( 0 <= x and x < V );
        assert( 0 <= y and y < V );
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
        assert( w.size() > j );
        w[i]= weight, w[j]= -weight;
    }

    mcmf( int m ) {
        E= 0, V= (this->n= m+2), src= V-2, sink= V-1;
        last.resize(V), next.resize(N), to.resize(N);
        cap.resize(N), flow.resize(N), w.resize(N);
        std::fill(last.begin(),last.end(),std::nullopt);
        std::fill(flow.begin(),flow.end(),0);
        d.resize(V), pi.resize(V);
        seen.resize(V), std::fill(seen.begin(),seen.end(),0);
        parent.resize(V), yes= 0;
        std::fill(pi.begin(),pi.end(),0);
        heap= std::make_unique<binary_heap>(V,d);
    }

    i64 mincost_maxflow( i64 &T ) {
        i64 cost= 0ll, df;
        int x,y,i;
        for ( T= 0; reachable();) {
            df= INF_CAP;
            for ( auto z= sink; z != source(); ) {
                i= parent[z];
                assert( flow[i] < cap[i] );
                df= std::min(df,cap[i]-flow[i]), z= to[i^1];
            }
            assert( 0 < df and df < INF_CAP );
            T+= df;
            for ( auto z= sink; z != source();
            i= parent[z], flow[i]+= df, flow[i^1]-= df, z= to[i^1] ) ;
        }
        for ( i= 0; i < E; ++i )
            if ( flow[i] > 0 and w[i] != 0 ) {
                cost += (w[i] * flow[i])%MOD, cost %= MOD;
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
    return (((n*k)%MOD) + (MOD-(k*(k-1)%MOD))*500001007LL) % MOD;
}

void solve(int cs) {
    os << "Case #" << cs << ": ";
    std::vector<std::pair<std::pair<int,int>,i64>> vec{};
    i64 i,j,k,m,total= 0ll, flow= 0ll;
    is >> n >> m;
    vec.resize(m);
    for ( auto &z: vec )
        is >> z.first.first >> z.first.second >> z.second;
    std::sort(vec.begin(),vec.end());
    std::vector<int> vv{};
    std::vector<std::pair<std::pair<int,int>,i64>> aggr{};
    for ( i= 0; i < m; i= j ) {
        for ( k= 0, j= i; j < m and vec[i].first == vec[j].first; k+= vec[j++].second ) ;
        auto x= vec[i].first.first, y= vec[i].first.second;
        assert( x < y );
        aggr.emplace_back(vec[i].first,k);
        vv.push_back(x), vv.push_back(y);
        total+= k*calc_dist(x,y), total%= MOD;
        flow+= k;
    }
    std::sort(vv.begin(),vv.end());
    vv.erase(std::unique(vv.begin(),vv.end()),vv.end());
    int offset= vv.size();
    auto g= std::make_unique<mcmf>(2*offset);
    std::map<int,i64> supply_at{}, demand_at{};
#define rank_space(x) (std::lower_bound(vv.begin(),vv.end(),x)-vv.begin())
    for ( auto &z: aggr ) {
        auto x= z.first.first, y= z.first.second;
        if ( not supply_at.count(x) )
            supply_at[x]= 0;
        if ( not demand_at.count(y) )
            demand_at[y]= 0;
        assert( supply_at[x]+z.second < MOD );
        assert( demand_at[y]+z.second < MOD );
        supply_at[x]+= z.second, demand_at[y]+= z.second;
        auto wgt= calc_dist(x,y);
        x= rank_space(x), y= rank_space(y);
        g->add_arc(outbound(x),outbound(y),z.second,wgt);
    }
    for ( auto zz: vv ) {
        auto x= rank_space(zz);
        g->add_arc(g->source(),inbound(x),supply_at[zz],0);
        g->add_arc(inbound(x),outbound(x),supply_at[zz],0);
        g->add_arc(outbound(x),g->dest(),demand_at[zz],0);
        if ( supply_at.count(zz) )
        for ( auto tt: vv )
            if ( tt >= zz and demand_at.count(tt) ) {
                auto y= rank_space(tt);
                g->add_arc(outbound(x),outbound(y),INF_CAP,calc_dist(zz,tt));
            }
    }
    for ( i= 0; i < aggr.size(); ++i )
        for ( j= 0; j < aggr.size(); ++j ) {
            if ( i == j ) continue ;
            if ( aggr[i].first.second < aggr[j].first.first )
                continue ;
            if ( aggr[i].first.first > aggr[j].first.second )
                continue ;
            auto amount_x= aggr[i].second, amount_y= aggr[j].second;
            int rx,ry,rdx,rdy;
            auto x= (rx= aggr[i].first.first), y= (ry= aggr[j].first.first);
            auto dx= (rdx= aggr[i].first.second), dy= (rdy= aggr[j].first.second);
            x= rank_space(x), y= rank_space(y), dx= rank_space(dx), dy= rank_space(dy);
            if ( ry <= rdx ) {
                g->add_arc(outbound(x), inbound(y), amount_x, 0);
                //g->add_arc(outbound(y), 2 * offset + dx, INF_CAP, calc_dist(ry, rdx));
            }
            if ( rx <= rdy ) {
                g->add_arc(outbound(y),inbound(x),amount_y,0);
                //g->add_arc(outbound(x),2*offset+dy,INF_CAP,calc_dist(rx,rdy));
            }
        }
#undef rank_space
    i64 tt= 0;
    os << (total+MOD-g->mincost_maxflow(tt))%MOD << std::endl;
    assert( tt == flow );
}

using ride= std::tuple<int,int,int>;
std::vector<ride> e;

struct event {
    int time, idx;
    bool is_opening;
};

bool operator < ( const event &a, const event &b ) {
    if ( a.time < b.time )
        return true ;
    if ( a.time > b.time )
        return false ;
    assert( a.time == b.time );
    if ( a.is_opening == b.is_opening )
        return a.idx < b.idx;
    return a.is_opening;
}

struct pq_order {
    bool operator() ( const event &a, const event &b ) const {
        return b < a;
    }
};

struct set_order {
    bool operator() ( const event &a, const event &b ) const {
        if ( a.time == b.time )
            return a.idx < b.idx;
        return a.time > b.time;
    }
};

std::priority_queue<event,std::vector<event>,pq_order> pq;
std::set<event,set_order> s;

void solve2( int cs ) {
    int i= 0,j,k,m;

    is >> n >> m, e.resize(m);
    std::vector<int> remains(m);
    i64 total= 0ll;
    assert( pq.empty() );
    for ( auto &z: e ) {
        int op,en,zz;
        is >> op >> en >> zz;
        assert( op <= en );
        z= std::make_tuple(op,en,zz);
        assert( std::get<0>(z) <= std::get<1>(z) );
        total+= std::get<2>(z)*calc_dist(std::get<0>(z),std::get<1>(z));
        total%= MOD;
        event ev{
            .time= std::get<0>(z),
            .idx= i++,
            .is_opening= true
        };
        remains[i-1]= std::get<2>(z);
        pq.push(ev);
    }
    i64 cost= 0ll;
    for ( s.clear(); not pq.empty(); ) {
        auto z= pq.top(); pq.pop();
        if ( z.is_opening ) {
            auto nz= z;
            nz.time= std::get<1>(e[z.idx]), nz.is_opening= false;
            s.insert(z), pq.push(nz);
            continue ;
        }
        assert( not z.is_opening );
        auto current_station= z.time;
        auto how_many_need_to_exit= std::get<2>(e[z.idx]);
        auto kk= 0ll;
        using it_type= std::set<event,set_order>::iterator;
        std::vector<it_type> dd{};
        for ( auto it= s.begin(); it != s.end() and kk < how_many_need_to_exit; ) {
            if ( kk + remains[it->idx] <= how_many_need_to_exit ) {
                cost+= remains[it->idx]*calc_dist(it->time,current_station), cost%= MOD;
                kk+= remains[it->idx], dd.push_back(it++);
                continue ;
            }
            assert( kk < how_many_need_to_exit );
            assert( kk+remains[it->idx] > how_many_need_to_exit );
            auto excess= how_many_need_to_exit-kk;
            kk+= excess, cost+= excess*calc_dist(it->time,current_station), cost%= MOD;
            remains[it->idx]-= excess;
            assert( remains[it->idx] > 0 );
            break ;
        }
        assert( kk == how_many_need_to_exit );
        if ( not dd.empty() )
            s.erase(dd.front(),std::next(dd.back()));
    }
    for ( auto &it: s )
        assert( not remains[it.idx] );
    os << "Case #" << cs << ": " << (total+MOD-cost)%MOD << std::endl;
}

int main() {
    int cs = 0, ts;
    //freopen("A-small-practice.in","r",stdin);
    // solve() solves the small dataset using mincost maxflow
    // but it appears a shorter greedy solution exists
#if 1
    for (is >> ts; ts--; solve2(++cs));
#else
    for (is >> ts; ts--; solve(++cs));
#endif
    return 0;
}   
 