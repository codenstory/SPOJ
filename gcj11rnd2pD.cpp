/**
 * TOPIC: graph theory, DP, bfs
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (0x200)
#define oo (1<<29)

int n,m;
std::vector<std::set<int>> adj;

int delta( int x, int y, int z ) {
    int ans= 0;
    for ( auto t: adj[z] )
        if ( not adj[x].count(t) )
            if ( not adj[y].count(t) )
                if ( t != x and t != y )
                    ++ans;
    return ans;
}

int main() {
    int i, j, k, x,y,cs = 0, ts;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n >> m, adj.resize(n);
        for ( auto &v: adj )
            v.clear();
        for ( ;m--; ) {
            std::string tmp;
            is >> tmp;
            auto pos= tmp.find(',');
            assert( pos != std::string::npos );
            x= std::strtol(tmp.c_str(),nullptr,10), y= std::strtol(tmp.c_str()+pos+1,nullptr,10);
            adj[x].insert(y), adj[y].insert(x);
        }
        if ( adj[0].count(1) ) {
            os << "Case #" << ++cs << ": 0 " << adj[0].size() << std::endl;
            continue ;
        }
        using state= std::pair<int,int>;
        std::queue<int> q{};
        std::vector<int> dist(n,+oo);
        for ( dist[0]= 0, q.push(0); not q.empty(); ) {
            x= q.front(), q.pop();
            if ( x == 1 ) break ;
            for ( auto z: adj[x] )
                if ( dist[z] > dist[x]+1 )
                    dist[z]= dist[x]+1, q.push(z);
        }
        std::vector<int> vx;
        for ( x= 0; x < n; ++x )
            if ( dist[x] < +oo )
                vx.push_back(x);
        std::sort(vx.begin(),vx.end(),[&dist](auto x,auto y) {
            return dist[x] < dist[y];
        });
        assert( not vx.empty() and vx.front() == 0 );
        std::map<state,int> dp{};
        for ( auto z: adj[0] )
            if ( dist[0]+1 == dist[z] ) {
                dp[{0,z}]= adj[0].size();
                for ( auto tt: adj[z] )
                    if ( not adj[0].count(tt) )
                        ++dp[{0,z}];
            }
        for ( i= 1; i < vx.size(); ++i ) {
            auto z= vx[i];
            for ( j= 0; j < i; ++j ) {
                auto pz= vx[j];
                if ( adj[z].count(pz) and dist[pz]+1 == dist[z] ) {
                    for ( k= 0; k < j; ++k ) {
                        auto ppz= vx[k];
                        if ( adj[pz].count(ppz) and dist[ppz]+1 == dist[pz] ) {
                            state s{ppz,pz};
                            assert( dp.count(s) );
                            int candidate= dp[s]+delta(ppz,pz,z);
                            if ( not dp.count({pz,z}) or dp[{pz,z}] < candidate )
                                dp[{pz,z}]= candidate;
                        }
                    }
                }
            }
        }
        int a2= -oo;
        for ( auto z: adj[1] ) {
            if ( dist[z]+1 == dist[1] ) {
                for ( auto pz: adj[z] )
                    if ( dist[pz]+1 == dist[z] )
                        if ( dp.count({pz,z}) ) {
                            if ( a2 < dp[{pz,z}] ) {
                                a2= dp[{pz,z}];
                            }
                        }
            }
        }
        os << "Case #" << ++cs << ": " << dist[1]-1 << " " << a2-dist[1] << std::endl;
    }
    return 0;
}   
 