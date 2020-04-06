/**
 * TOPIC: dynamic programming, correct bracket sequences
 * status:
 */
#include <bits/stdc++.h>
#define oo (1<<29)
#define A (10)
#define N 0x80

std::istream &is = std::cin;
std::ostream &os = std::cout;

std::vector<std::vector<std::vector<int>>> z;
std::vector<std::vector<std::vector<bool>>> seen;
std::vector<std::vector<std::vector<std::pair<int,int>>>> way_to_go;
int c[N],mn[N][N];

bool is_valid( const std::string &s ) {
    int sz= 0;
    for ( auto ch: s ) {
        if ( ch == '(' ) {
            ++sz;
            continue ;
        }
        if ( ch == ')' ) {
            assert( sz >= 1 );
            --sz;
            continue ;
        }
        assert( isdigit(ch) );
        if ( sz != ch-'0' )
            return false ;
    }
    return sz == 0;
}

template<typename T>
void init( std::vector<std::vector<std::vector<T>>> &e, int k, int m, int n, T val= T{} ) {
    e.resize(k);
    for ( auto &v: e ) {
        v.resize(m);
        for ( auto &u: v )
            u.resize(n), std::fill(u.begin(),u.end(),val);
    }
}

void init_z( int n ) {
    init(z,A,n+1,n+1,+oo), init(seen,A,n+1,n+1,false), init(way_to_go,A,n+1,n+1);
}

int calc_z( int d, int i, int j ) {

    if ( seen[d][i][j] )
        return z[d][i][j];

    seen[d][i][j]= true;

    if ( i > j )
        return z[d][i][j]= (d==0?0:+oo);

    if ( d > mn[i][j] )
        return z[d][i][j];

    if ( i == j ) {
        assert( d <= c[i] );
        return z[d][i][j] = 2*(c[i]-d)+(j-i+1);
    }

    assert( i < j );

    for ( int t= i; t+1 <= j; ++t )
        if ( calc_z(d,i,t) < +oo and calc_z(d,t+1,j) < +oo )
            if ( calc_z(d,i,t)+calc_z(d,t+1,j) < z[d][i][j] )
                z[d][i][j] = calc_z(d, i, t) + calc_z(d, t + 1, j), way_to_go[d][i][j]= {d,t};
    for ( int t= i; t+1 <= j; ++t )
        for ( int l= A-1; l >= d; --l ) {
            auto additional= l-d;
            if ( calc_z(l,i,t) < +oo and calc_z(l,t+1,j) < +oo )
                if ( z[d][i][j] > calc_z(l,i,t)+calc_z(l,t+1,j)+2*additional )
                    z[d][i][j]= calc_z(l,i,t)+calc_z(l,t+1,j)+2*additional, way_to_go[d][i][j]= {l,t};
        }
    for ( int l= A-1; l > d; --l )
        if ( calc_z(l,i,j) < +oo )
            if ( z[d][i][j] > calc_z(l,i,j)+2*(l-d) )
                z[d][i][j]= calc_z(l,i,j)+2*(l-d), way_to_go[d][i][j]= {l,j};
    return z[d][i][j];
}

std::string dump( int d, int i, int j ) {
    std::string res{};
    if ( i == j ) {
        assert( c[i] >= d );
        for ( int k= c[i]-d; k--; res.push_back('(') ) ;
        res.push_back(c[i]+'0');
        for ( int k= c[i]-d; k--; res.push_back(')') ) ;
        assert( res.size() == calc_z(d,i,j) );
        return res;
    }
    assert( i < j );
    auto t= way_to_go[d][i][j].second, l= way_to_go[d][i][j].first;
    if ( t == j ) {
        assert( l > d );
        auto middle= dump(l,i,j);
        for ( int k= l-d; k--; res.push_back('(') ) ;
        res.insert(res.begin(),middle.cbegin(),middle.cend());
        for ( int k= l-d; k--; res.push_back(')') ) ;
        assert( res.size() == calc_z(d,i,j) );
        return res;
    }
    assert( i <= t and t+1 <= j );
    auto left= dump(l,i,t), right= dump(l,t+1,j);
    auto additional= l-d;
    assert( additional >= 0 );
    for ( auto k= additional; k--; res.push_back('(') ) ;
    res.insert(res.end(),left.cbegin(),left.cend()), res.insert(res.end(),right.cbegin(),right.cend());
    for ( auto k= additional; k--; res.push_back(')') ) ;
    assert( res.size() == calc_z(d,i,j) );
    return res;
}

void solve( int cs ) {
    std::string s;
    int i,j,n;
    is >> s, init_z(n= s.size());
    for ( i= 1; i <= n; ++i ) c[i]= s[i-1]-'0';
    for ( i= 0; i <= n; ++i )
        for ( j= 0; j <= n; mn[i][j++]= +oo ) ;
    for ( i= 1; i <= n; ++i )
        for ( mn[i][i]= c[i], j= i+1; j <= n; ++j )
            mn[i][j]= std::min(mn[i][j-1],c[j]);
    auto res= (calc_z(0,1,n),dump(0,1,n));
    assert( is_valid(res) );
    os << "Case #" << cs << ": " << res << std::endl;
}

int main() {
    int cs = 0, ts;
    for ( is >> ts; ts--; solve(++cs) ) ;
    return 0;
}

