/**
 * RIGHTTRI
 * status: Accepted
 * TOPIC: analytical geometry, rotations, fractions, maths
 */
#include <bits/stdc++.h>
#include <boost/functional/hash.hpp>
using i64= std::int64_t;
using cell= std::pair<i64,i64>;
i64 gcd( i64 x, i64 y ) { return (not y) ? x:gcd(y,x%y); }

std::istream &is = std::cin;
std::ostream &os = std::cout;

i64 getint() {
    int ch;
    i64 n= 0, sign= 1;
    for ( ;(ch= getchar()) != EOF and not(ch == '-' or std::isdigit(ch)); ) ;
    if ( ch == EOF ) {
        return EOF;
    }
    if ( ch == '-' ) {
        sign= -1, ch= getchar();
    }
    for ( n= ch-'0'; (ch= getchar()) != EOF and std::isdigit(ch); n= 10*n+(ch-'0') ) ;
    return n*sign;
}

std::vector<cell> pts;
int n;

int sign_of( i64 x ) {
    if ( x == 0 )
        return 0;
    return x < 0 ? -1:1;
}

struct frac {
    std::pair<i64,i64> val;
    //std::pair<double,double> value;
    double w;
    explicit frac() {}
    explicit frac( i64 x, i64 y ) {
        val= {x,y};
        if ( x == 0 ) {
            y= 1;
        }
        if ( y < 0 ) {
            x= -x, y= -y;
        }
        assert( y >= 0 );
        //value= x==0?std::make_pair(0.00,0.00):std::make_pair(x+0.00,y+0.00);
        if ( x != 0 )
            w= val.first/(val.second+0.00);
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
    frac slope;
};

std::set<std::pair<int,int>> rel= {{3,4},{4,1},{1,2}};
bool is_less[5][5];

bool operator < ( const angle &a, const angle &b ) {
    assert( a.slope.val.second > 0 );
    assert( b.slope.val.second > 0 );
    return a.slope.val.first*b.slope.val.second < a.slope.val.second*b.slope.val.first;
}

bool operator == ( const angle &a, const angle &b ) {
    //return not(a < b or b < a);
    return a.slope.val == b.slope.val;
}

angle get_ang( const cell &a, const cell &b ) {
    i64 u= b.first-a.first, v= b.second-a.second;
    i64 co= labs(u), si= labs(v);
    i64 g= gcd(co,si);
    u/= g, v/= g;
    angle ang{
        .slope= frac{v,u}
    };
    return ang;
}

angle rotate90( const angle &a ) {
    i64 co= a.slope.val.second, si= a.slope.val.first;
    angle b{
        .slope= frac{co,-si}
    };
    return b;
}

angle rotatem90( const angle &a ) {
    i64 co= a.slope.val.second, si= a.slope.val.first;
    angle b{
            .slope= frac{-co,si}
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
        hash_combine(seed,hash_value(a.slope.val.first));
        hash_combine(seed,hash_value(a.slope.val.second));

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
    i64 ans= 0, infx= 0, infy= 0;
    int i;
    std::unordered_map<angle,i64> cnt;
    for ( i= 0; i < n; ++i )
        if ( i != idx ) {
            if ( pts[idx].first == pts[i].first ) {
                ++infx;
                continue ;
            }
            if ( pts[idx].second == pts[i].second ) {
                ++infy;
                continue ;
            }
            ++cnt[get_ang(pts[idx],pts[i])];
        }
    for ( const auto &[key, val]: cnt ) {
        auto T = rotate90(key);
        i64 kappa = cnt.count(T) ? cnt[T] : 0;
        //T= rotatem90(key);
        //i64 tau= cnt.count(T) ? cnt[T] : 0;
        i64 tau= 0;
        ans += val * (kappa+tau);
    }
    return ans+infx*infy;
}

int main() {
    //std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
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
    for ( ;; ) {
        n= getint();
        pts.resize(n);
        for ( auto &v: pts )
            //is >> v.first >> v.second;
            v.first= getint(), v.second= getint();
        i64 ans= 0;
        for ( int i= 0; i < n; ans+= ff(i++) ) ;
        os << ans << '\n';
        break ;
    }
    return 0;
}
    