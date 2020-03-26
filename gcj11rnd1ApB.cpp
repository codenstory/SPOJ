#include <utility>

/**
 * TOPIC: DP, fast simulation, partition refinement, bitmasks, STL, sorting, bfs, very neat
 * status: Accepted
 */
#include <bits/stdc++.h>
using size_type= int;
#define A (26+1)
#define N (10000)

struct word {
    int weight;
    std::string s;
    int idx;
};

bool operator < ( const word &a, const word &b ) {
    return a.weight < b.weight or (a.weight == b.weight and a.idx < b.idx);
}

int n,qr,z[N][A];
unsigned int mask[N],profile[N],all_locations[N][A];
std::vector<word> D;

int solve( const std::string &L ) {
    int i,j,k;
    std::map<char,int> pos{};
    for ( i= 0; i < L.size(); ++i )
        pos[L[i]]= i;
    std::sort(D.begin(),D.end(),[](const auto &a, const auto &b) {
        return a.s.size() < b.s.size();
    });
    std::queue<std::tuple<int,int,int>> q{};
    for ( i= 0; i < n; q.push({0,i,j}), i= j )
        for ( j= i+1; j < n and D[i].s.size() == D[j].s.size(); ++j ) ;
    for ( i= 0; i < n; ++i )
        D[i].weight= pos[
            *std::max_element(D[i].s.begin(),D[i].s.end(),[&pos](auto x,auto y) {
                return pos[x] < pos[y];
            })];
    for ( i= 0; i < n; ++i )
        for ( j= 0; j <= L.size(); z[i][j++]= -1 ) ;
    for ( i= 0; i < n; ++i )
        z[i][0]= 0;
    for ( i= 0; i < n; ++i )
        for ( mask[i]= 0, j= 0; j < A; all_locations[i][j++]= 0 ) ;
    for ( const auto &w: D )
        for ( j= 0; j < w.s.size(); ++j ) {
            mask[w.idx] |= (1 << (w.s[j] - 'a'));
            all_locations[w.idx][w.s[j]-'a']|= (1<<j);
            profile[w.idx]= (1<<w.s.size())-1;
        }
    while ( not q.empty() ) {
        auto pr= q.front(); q.pop();
        k= std::get<0>(pr), i= std::get<1>(pr), j= std::get<2>(pr);
        assert( 0 <= i and j <= n );
        if ( j-i == 1 ) {
            // singleton bucket
            z[D[i].idx][k+1]= z[D[i].idx][k];
            if ( k+1 < L.size() )
                q.push({k+1,i,j});
            continue ;
        }
        assert( j-i >= 2 );
        unsigned int u= 0;
        for ( auto t= i; t < j; ++t )
            u|= mask[D[t].idx];
        unsigned int ll= (1ul<<(L[k]-'a'));
        for ( auto t= i; t < j; ++t ) {
            assert( z[D[t].idx][k+1] == -1 );
            z[D[t].idx][k+1] = z[D[t].idx][k]+((u&ll)?((mask[D[t].idx]&ll)?0:1):0);
            profile[D[t].idx]&= ~all_locations[D[t].idx][L[k]-'a'];
            mask[D[t].idx]&= ~ll;
        }
        auto &prof= profile;
        std::sort(D.begin()+i,D.begin()+j,[&prof](const auto &a, const auto &b) {
            return prof[a.idx] < prof[b.idx];
        });
        for ( int t= i, l; t < j; t= l ) {
            for ( l= t+1; l < j and profile[D[l].idx] == profile[D[t].idx]; ++l ) ;
            if ( k+1 < L.size() ) {
                q.push({k+1,t,l});
            }
        }
    }
    for ( i= 0; i < n; ++i )
        for ( j= 0; j <= L.size(); ++j )
            assert( z[i][j] >= 0 );
    int ans= -1;
    for ( i= 0; i < n; ++i )
        if ( ans < z[i][L.size()] )
            ans= z[k= i][L.size()];
    return k;

}

int main() {
    int i,cs= 0,ts;
    std::string L;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n >> qr, D.resize(n);
        memset(all_locations,0,sizeof all_locations);
        for ( i= 0; i < n; ++i ) {
            word w;
            is >> w.s, mask[w.idx= i]= 0, profile[w.idx]= (1<<(w.s.size()))-1;
            for ( int j= 0; j < w.s.size(); ++j ) {
                mask[w.idx] |= (1 << (w.s[j] - 'a'));
                all_locations[w.idx][w.s[j]-'a']|= (1<<j);
            }
            D[i]= std::move(w);
        }
        std::vector<int> res{};
        for (;qr--; is >> L, res.push_back(solve(L)) ) ;
        std::sort(D.begin(),D.end(),[](const auto &a,const auto &b) {
            return a.idx < b.idx;
        });
        os << "Case #" << ++cs << ":";
        for ( auto x: res )
            os << " " << D[x].s;
        os << "\n";
    }
    return 0;
}

