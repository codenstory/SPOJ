/**
 * RIGHTTRI
 * status: W.I.P., TLE
 * TOPIC: analytical geometry, rotations, fractions, maths
 */
#include <bits/stdc++.h>
#include <boost/functional/hash.hpp>
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

struct frac {
    std::pair<i64,i64> val;
    std::pair<double,double> value;
    double w;
    explicit frac() {}
    explicit frac( i64 x, i64 y ) {
        val= {x,y};
        value= x==0?std::make_pair(0.00,0.00):std::make_pair(x+0.00,y+0.00);
        if ( x != 0 )
            w= value.first/value.second;
    }
};

bool operator < ( const frac &a, const frac &b ) {
    if ( a.val.first == 0 ) {
        return b.val.first != 0;
    }
    if ( b.val.first == 0 ) {
        return false ;
    }
    //return a.value.first/a.value.second < b.value.first/b.value.second;
    return a.w < b.w;
}

bool operator == ( const frac &a, const frac &b ) {
    return a.val == b.val;
}

struct angle {
    frac cosine;
    frac sine;
    int sc;
    int ss;
    mutable int q= -1;
    int quadrant() const {
        if ( q != -1 )
            return q;
        if ( sc >= 0 and ss >= 0 )
            return q= 1;
        if ( sc <= 0 and ss >= 0 )
            return q= 2;
        if ( sc >= 0 and ss <= 0 )
            return q= 4;
        return q= 3;
    }
};

std::set<std::pair<int,int>> rel= {{3,4},{4,1},{1,2}};
bool is_less[5][5];

bool operator < ( const angle &a, const angle &b ) {
    if ( a.quadrant() == b.quadrant() ) {
        switch ( a.quadrant() ) {
            case 1: return b.cosine < a.cosine;
            case 2: return b.sine < a.sine;
            case 3: return b.cosine < a.cosine;
            case 4: return a.cosine < b.cosine;
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
        .cosine= frac{co/cc,len/cc},
        .sine  = frac{si/ss,len/ss},
        .sc= sign_of(u),
        .ss= sign_of(v)
    };
    return ang;
}

angle rotate90( const angle &a ) {
    angle b{
        .cosine= a.sine,
        .sine= a.cosine,
        .sc= -a.ss,
        .ss= a.sc
    };
    return b;
}

template<>
struct std::hash<angle> {
    std::hash<i64> h{};
    std::size_t operator() ( const angle &a ) const {
        using boost::hash_value;
        using boost::hash_combine;

        // Start with a hash value of 0    .
        std::size_t seed = 0;

        // Modify 'seed' by XORing and bit-shifting in
        // one member of 'Key' after the other:
        hash_combine(seed,hash_value(a.sine.val.first));
        hash_combine(seed,hash_value(a.sine.val.second));
        hash_combine(seed,hash_value(a.cosine.val.first));
        hash_combine(seed,hash_value(a.cosine.val.second));

        // Return the result.
        return seed;
        //return h(a.sine.val.first)^h(a.sine.val.second)^h(a.cosine.val.first)^h(a.cosine.val.second);
    }
};

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

i64 ff( const int idx ) {
    i64 ans= 0;
    int i;
    std::unordered_map<angle,i64> cnt[3][3];
    for ( i= 0; i < n; ++i )
        if ( i != idx ) {
            auto z= get_ang(pts[idx],pts[i]);
            ++cnt[z.sc+1][z.ss+1][z];
        }
    for ( int sc= -1; sc <= 1; ++sc )
        for ( int ss= -1; ss <= 1; ++ss ) {
        for (const auto &[key, val]: cnt[sc+1][ss+1]) {
            auto T = rotate90(key);
            i64 kappa = cnt[T.sc+1][T.ss+1].count(T) ? cnt[T.sc+1][T.ss+1][T] : 0;
            ans += val * kappa;
        }
    }
    return ans;
}

int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    memset(is_less,0,sizeof is_less);
    for ( auto &it: rel )
        is_less[it.first][it.second]= true ;
    for ( int k= 1; k <= 4; ++k )
        for ( int i= 1; i <= 4; ++i )
            for ( int j= 1; j <= 4; ++j )
                if ( is_less[i][k] and is_less[k][j] )
                    is_less[i][j]= true ;
    for ( ;(is >> n) and n; ) {
        pts.resize(n);
        for ( auto &v: pts )
            is >> v.first >> v.second;
        i64 ans= 0;
        for ( int i= 0; i < n; ans+= ff(i++) ) ;
        os << ans << '\n';
    }
    return 0;
}
    