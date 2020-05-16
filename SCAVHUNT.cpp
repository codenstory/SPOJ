/**
 * SCAVHUNT
 * TOPIC: Topological sort
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N 0x200
enum { D, T };

std::istream &is = std::cin;
std::ostream &os = std::cout;

int n,seen[N],yes,st[N],*top;
std::map<std::string,int> m;
std::map<int,std::string> inverse;
std::vector<std::pair<int,int>> vec;
std::vector<int> adj[2][N];

int insert( std::string &s ) {
    if ( m.count(s) )
        return m[s];
    auto z= m.size();
    inverse[m[s]= z]= s;
    return z;
}

void dfs( int t, int x ) {
    assert( seen[x] != yes );
    seen[x]= yes;
    for ( auto y: adj[t][x] )
        if ( seen[y] != yes )
            dfs(t,y);
    *++top= x;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int i,j,k,ts,cs= 0;
    for ( is >> ts; ts--; ) {
        is >> n, m.clear(), inverse.clear(), vec.resize(--n);
        for ( i= 0; i < n; ++i ) {
            std::string a,b;
            is >> a >> b;
            j= insert(a), k= insert(b);
            vec[i]= std::make_pair(j,k);
        }
        for ( i= 0; i < n; ++i )
            for ( int t= D; t <= T; ++t )
                adj[t][i].clear();
        for ( i= 0; i < n; ++i )
            for ( j= 0; j < n; ++j )
                if ( vec[i].second == vec[j].first ) {
                    adj[D][i].push_back(j);
                    adj[T][j].push_back(i);
                }
        for ( ++yes, top= st, i= 0; i < n; ++i )
            if ( seen[i] != yes )
                dfs(D,i);
        os << "Scenario #" << ++cs << ":\n";
        for ( ;top > st; os << inverse[vec[*top--].first] << '\n' ) ;
        os << inverse[vec[*++top].second] << '\n' << std::endl;
    }
    return 0;
}
