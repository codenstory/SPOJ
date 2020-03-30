/**
 * TOPIC: randomization
 * status: Accepted
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;

using i64= std::int64_t;

i64 rp( i64 L ) {
    static std::random_device R{};
    static std::default_random_engine e(R());
    static std::uniform_real_distribution<double> d(0,1);
    static auto dice= std::bind(d,e);
    auto res= dice();
    assert( 0 <= res and res <= 1 );
    return static_cast<i64>(res*(L+1));
}

bool valid_sol( i64 W, i64 L, const std::vector<i64> &cx, const std::vector<i64> &cy, const std::vector<i64> &r ) {
    int n= cx.size(),i,j;
    for ( i= 0; i < n; ++i ) {
        if ( cx[i] < 0 or cx[i] > W or cy[i] < 0 or cy[i] > L )
            return false ;
        for (j = i + 1; j < n; ++j)
            if ((cx[i] - cx[j]) * (cx[i] - cx[j]) + (cy[i] - cy[j]) * (cy[i] - cy[j]) < (r[i] + r[j]) * (r[i] + r[j]))
                return false;
    }
    return true ;
}

struct point {
    int idx;
    i64 r;
};

bool operator < ( const point &a, const point &b ) {
    if ( a.r == b.r )
        return a.idx < b.idx;
    return a.r > b.r;
}

void solve( int cs ) {
    int i,j,k,n;
    i64 L,W;
    std::vector<i64> r;
    is >> n >> W >> L, r.resize(n);
    for ( auto &v: r )
        is >> v;
    std::vector<point> p(n);
    for ( i= 0; i < n; ++i )
        p[i].r= r[i], p[i].idx= i;
    std::sort(p.begin(),p.end());
    std::vector<i64> cx(n),cy(n);
    for ( i = 0; i < n; ++i) {
        auto ff= [&cx,&cy,i,&p](){
            for (auto t = 0; t < i; ++t)
                if ((cx[p[t].idx]-cx[p[i].idx])*(cx[p[t].idx]-cx[p[i].idx])+
                (cy[p[t].idx]-cy[p[i].idx])*(cy[p[t].idx]-cy[p[i].idx]) < (p[t].r+p[i].r)*(p[t].r+p[i].r) )
                    return true;
            return false;
        };
        for ( cx[p[i].idx]= rp(W), cy[p[i].idx]= rp(L); ff(); cx[p[i].idx]= rp(W), cy[p[i].idx]= rp(L) ) ;
    }
    os << "Case #" << cs << ":";
    for ( i= 0; i < n; ++i )
        os << " " << cx[i] << " " << cy[i];
    os << std::endl;
}

int main() {
    int ts,cs= 0;
    for ( is >> ts; ts--; solve(++cs) ) ;
    return 0;
}   
 