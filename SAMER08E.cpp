/**
 * SAMER08E
 * TOPIC: dates, STL, inner_product
 * status: Accepted
 */
#include <bits/stdc++.h>

std::istream &is = std::cin;
std::ostream &os = std::cout;

bool divisible(int x,int m)  {
    return 0 == (x%m);
}

bool is_leap( int y ) {
    if ( divisible(y,4) and not divisible(y,100) )
        return true ;
    return divisible(y,400);
}

std::vector<int> M;

struct date {
    int d,m,y;
    date() { d= m= y= 0; }
    date( int d, int m, int y ): d(d), m(m), y(y) {}
    date operator ++ () const {
        date x= *this;
        int limit= (x.m!=2 or (x.m==2 and not is_leap(x.y))) ? M[x.m] : 29;
        if ( ++x.d <= limit )
            return x;
        x.d= 1;
        if ( ++x.m == 13 )
            x.m= 1, ++x.y;
        return x;
    }
};

bool operator == ( const date &a, const date &b ) {
    return a.d == b.d and a.m == b.m and a.y == b.y;
}

std::istream &operator >> ( std::istream &is, date &z ) {
    return is >> z.d >> z.m >> z.y;
}

int n;

int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int xx[] = {-1,31,28,31,30,31,30,31,31,30,31,30,31};
    for ( int i= 0; i < 13; ++i )
        M.push_back(xx[i]);
    for ( ;(is >> n) and n; ) {
        std::vector<std::pair<date,int> > c,sc;
        c.resize(n);
        for ( int i= 0; i < n; ++i )
            is >> c[i].first >> c[i].second;
        sc= c;
        std::rotate(sc.begin(),sc.begin()+1,sc.end());
        /*
        auto combine= [](int &k, const std::pair<date,int> &a, const std::pair<date,int> &b) -> int {
            if ( ++a.first == b.first )
                return ++k, b.second-a.second;
            return 0;
        };
        */
        int k= 0,res= 0;
        /*
        res= std::inner_product(c.begin(),c.end()-1,sc.begin(),0,
        [](int acc,int dt) {
            return acc+dt;
        },
        [&k,combine](const auto &ita, const auto &itb)->int {
            return combine(k,ita,itb);
        });
         */
        for ( int i= 0; i+1 < c.size(); ++i ) {
            if ( ++c[i].first == sc[i].first )
                ++k, res+= sc[i].second-c[i].second;
        }
        os << k << " " << res << std::endl;
    }
    return 0;
}
    