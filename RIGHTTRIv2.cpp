/**
 * RIGHTTRI
 * status: W.I.P., WA
 * TOPIC: analytical geometry, rotations, fractions, maths
 */
#include <bits/stdc++.h>
using i64= std::int64_t;
using cell= std::pair<i64,i64>;
i64 gcd( i64 x, i64 y ) { return not y ? x:gcd(y,x%y); }

std::istream &is = std::cin;
std::ostream &os = std::cout;

std::vector<cell> pts;
int n;

int sign_of( i64 x ) {
    if ( x == 0 )
        return 0;
    return x < 0 ? -1:1;
}

struct angle {
    std::pair<i64,i64> cosine;
    std::pair<i64,i64> sine;
    int sc;
    int ss;
    int quadrant() const {
        if ( sc >= 0 and ss >= 0 )
            return 1;
        if ( sc <= 0 and ss >= 0 )
            return 2;
        if ( sc >= 0 and ss <= 0 )
            return 4;
        return 3;
    }
};

struct comparator {
    bool operator() ( const std::pair<i64,i64> &a, const std::pair<i64,i64> &b ) const {
        assert( a.second >= 1 and b.second >= 1 );
        if ( a.first == 0 ) {
            return b.first != 0;
        }
        if ( b.first == 0 ) {
            return false ;
        }
        return a.first < a.second*(b.first/(b.second+0.00));
    }
};

comparator C{};

std::set<std::pair<int,int>> rel= {{3,4},{4,1},{1,2},{3,2},{3,1},{4,2}};
bool is_less[5][5];

bool compareFirstQuadrant( const angle &a, const angle &b ) {
    assert( a.quadrant() == 1 and b.quadrant() == 1 );
    return C(b.cosine,a.cosine);
}

bool compareSecondQuadrant( const angle &a, const angle &b ) {
    assert( a.quadrant() == 2 and b.quadrant() == 2 );
    return C(b.sine,a.sine);
}

bool compareThirdQuadrant( const angle &a, const angle &b ) {
    assert( a.quadrant() == 3 and b.quadrant() == 3 );
    return C(b.cosine,a.cosine);
}

bool compareFourthQuadrant( const angle &a, const angle &b ) {
    assert( a.quadrant() == 4 and b.quadrant() == 4 );
    return C(a.cosine,b.cosine);
}

bool operator < (const angle &a, const angle &b ) {
    if ( a.quadrant() == b.quadrant() ) {
        switch ( a.quadrant() ) {
            case 1: return compareFirstQuadrant(a,b);
            case 2: return compareSecondQuadrant(a,b);
            case 3: return compareThirdQuadrant(a,b);
            case 4: return compareFourthQuadrant(a,b);
            default: assert( false );
        }
    } else {
        return is_less[a.quadrant()][b.quadrant()];
    }
}

bool operator == ( const angle &a, const angle &b ) {
    //return not(a < b or b < a);
    return std::tie(a.cosine,a.sine,a.sc,a.ss) == std::tie(b.cosine,b.sine,b.sc,b.ss);
}

angle get_ang( const cell &a, const cell &b ) {
    i64 u= b.first-a.first, v= b.second-a.second;
    i64 co= u*u, si= v*v;
    i64 len= co + si;
    i64 cc= gcd(co,len), ss= gcd(si,len);
    angle ang{
        .cosine= {co/cc,len/cc},
        .sine  = {si/ss,len/ss},
        .sc= sign_of(u),
        .ss= sign_of(v)
    };
    return ang;
}

angle rotate90( angle &a ) {
    angle b{
        .cosine= a.sine,
        .sine= a.cosine,
        .sc= -a.ss,
        .ss= a.sc
    };
    return b;
}

i64 f( const int idx ) {
    i64 ans= 0;
    int i,j;
    std::vector<angle> c(n-1);
    for ( j= 0, i= 0; i < n; ++i )
        if ( i != idx )
            c[j++]= get_ang(pts[idx],pts[i]);
    std::sort(c.begin(),c.end());
    for ( i= 0; i < c.size(); i= j ) {
        for ( j= i+1; j < c.size() and c[j] == c[i]; ++j ) ;
        auto T= rotate90(c[i]);
        ans+= (j-i)*(std::upper_bound(c.begin(),c.end(),T)-std::lower_bound(c.begin(),c.end(),T));
    }
    return ans;
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    memset(is_less,0,sizeof is_less);
    for ( auto it: rel )
        is_less[it.first][it.second]= true ;
    for ( ;(is >> n) and n; ) {
        pts.resize(n);
        for ( auto &v: pts )
            is >> v.first >> v.second;
        i64 ans= 0;
        for ( int i= 0; i < n; ans+= f(i++) ) ;
        os << ans << '\n';
    }
    return 0;
}
    