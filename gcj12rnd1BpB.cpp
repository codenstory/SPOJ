/**
 * TOPIC: BFS, Dijkstra, simulation
 * status: Accepted
 */
#include <bits/stdc++.h>
std::istream &is = std::cin;
std::ostream &os = std::cout;
#define enc(x,y) ((x)|((y)<<7))
#define Q (1<<14)
#define R (10.00)
#define L (50.00)

std::vector<std::optional<double>> d(Q);

void solve( int cs ) {
    int m,n,i,j,k,ni,nj;
    double H;
    std::vector<std::vector<int>> c,f;
    is >> H >> m >> n;
    c.resize(m), f.resize(m);
    for ( auto &v: c ) {
        v.resize(n);
        for ( auto &val: v )
            is >> val;
    }
    for ( auto &v: f ) {
        v.resize(n);
        for ( auto &val: v )
            is >> val;
    }
    using state= unsigned int;
    std::set<std::pair<double,state>> s{};
    std::vector<state> src{};
    std::queue<state> q{};
    std::set<state> seen{};
    state u,v;
    for ( seen.clear(), seen.insert(u= enc(0,0)), q.push(u); not q.empty(); ) {
        for (src.push_back(u= q.front()), q.pop(), i = (u & 127), j = (u >> 7), ni = i - 1; ni <= i+1; ++ni)
            for (nj = j - 1; nj <= j + 1; ++nj)
                if (abs(ni - i) + abs(nj - j) == 1)
                    if (0 <= ni and ni < m and 0 <= nj and nj < n)
                        if (H + L<= c[ni][nj] and std::max(f[i][j],f[ni][nj]) + L <= c[ni][nj] )
                            if (f[ni][nj] + L<= c[i][j])
                                if (not seen.count(v = enc(ni, nj)))
                                    q.push(v), seen.insert(v);
    }
    std::fill(d.begin(),d.end(),std::nullopt);
    for ( auto &sr: src )
        s.insert({*(d[sr]= 0.00),sr});
    while ( not s.empty() ) {
        auto it= s.begin();
        u= it->second, s.erase(it);
        i= (u&127), j= (u>>7);
        // pure water, above the floor level
        double water_thickness= std::max(0.00,std::max(0.00,H-f[i][j])-R*(*d[u]));
        for ( ni= i-1; ni <= i+1; ++ni )
            for ( nj= j-1; nj <= j+1; ++nj )
                if (abs(ni - i) + abs(nj - j) == 1)
                    if (0 <= ni and ni < m and 0 <= nj and nj < n) {
                        if (std::max(f[i][j],f[ni][nj]) + L <= c[ni][nj] )
                            if (f[ni][nj] + L<= c[i][j]) {
                                if ( water_thickness >= 1e-7 ) {
                                    double waiting_time =
                                            f[i][j]+water_thickness+L <= c[ni][nj] ? 0.00 : (f[i][j]+water_thickness+L-c[ni][nj])/R;
                                    assert(waiting_time >= 0);
                                    assert( water_thickness >= R*waiting_time );
                                    double new_thickness= std::max(0.00, water_thickness-R*waiting_time);
                                    if (new_thickness+f[i][j]+L <= c[ni][nj]) {
                                        double dt = new_thickness >= 20 ? 1 : 10;
                                        if (not d[v = enc(ni, nj)] or *d[v] > *d[u] + waiting_time + dt)
                                            s.insert({*(d[v] = *d[u] + waiting_time + dt), v});
                                    }
                                }
                                else {
                                    if (not d[v = enc(ni, nj)] or *d[v] > *d[u] + 10)
                                        s.insert({*(d[v] = *d[u] + 10), v});
                                }
                            }
                    }
    }
    os << "Case #" << cs << ": " << std::setprecision(6) << std::fixed << *(d[enc(m-1,n-1)]) << std::endl;
}

int main() {
    int cs = 0, ts;
    for ( is >> ts; cs < ts; solve(++cs) ) ;
    return 0;
}   
 