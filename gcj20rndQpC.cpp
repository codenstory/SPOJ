/**
 * TOPIC:
 * status:
 */
#include <bits/stdc++.h>
enum { W, H };

std::istream &is = std::cin;
std::ostream &os = std::cout;

int n;
std::vector<std::vector<int>> adj;
std::vector<int> colors;
std::vector<bool> seen;
bool ok;

bool overlap( std::pair<int,int> &a, std::pair<int,int> &b ) {
    return not(a.second <= b.first or b.second <= a.first);
}

bool dfs( int x, int c ) {
    assert( not seen[x] );
    seen[x]= true, colors[x]= c;
    for ( auto y: adj[x] ) {
        if (seen[y] and colors[y] == c)
            return ok = false;
        if ( not seen[y] and not dfs(y,c^1) )
            return false ;
    }
    return ok;
}

void solve( int cs ) {
    int i,j;
    is >> n;
    std::vector<std::pair<int,int>> v(n);
    for ( auto &z: v )
        is >> z.first >> z.second;
    adj.resize(n);
    for ( auto &u: adj )
        u.clear();
    for ( i= 0; i < n; ++i )
        for ( j= i+1; j < n; ++j )
            if ( overlap(v[i],v[j]) )
                adj[i].push_back(j), adj[j].push_back(i);
    colors.resize(n);
    std::fill(colors.begin(),colors.end(),W);
    seen.resize(n), std::fill(seen.begin(),seen.end(),false);
    for ( ok= true, i= 0; i < n and ok; ++i )
        if ( not seen[i] )
            dfs(i,W);
    os << "Case #" << cs << ": ";
    if ( not ok ) {
        os << "IMPOSSIBLE" << std::endl;
        return ;
    }
    std::string res{};
    std::transform(colors.begin(),colors.end(),std::back_inserter(res),[](auto x) {
        return x==W?'C':'J';
    });
    os << res << std::endl;
}

int main() {
    int cs = 0, ts;
    for ( is >> ts; ts--; solve(++cs) ) ;
    return 0;
}   
 