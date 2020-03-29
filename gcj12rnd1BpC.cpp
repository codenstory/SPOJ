/**
 * TOPIC: randomization, probability, birthday paradox, random sampling, reservoir sampling
 * status: Accepted
 */
#include <bits/stdc++.h>
using i64= int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;


std::vector<i64> random_subset( const std::vector<i64> &c, size_t s ) {
    static std::random_device r{};
    static std::default_random_engine engine(r());
    static std::uniform_real_distribution<double> dist(0.00,1.00);
    std::vector<i64> res(s);
    for ( int i= 0; i < s; res[i]= c[i], ++i ) ;
    for ( int k= s; k < c.size(); ++k ) {
        double t= dist(engine);
        int j= (int)(0+t*k);
        if ( j < s )
            res[j]= c[k];
    }
    return res;
}

void solve( int cs ) {
    int n;
    is >> n;
    std::vector<i64> a(n);
    for ( auto &v: a )
        is >> v;
    auto T= std::accumulate(a.begin(),a.end(),0ll);
    int m= 2*10000000, num_runs= 1;
    for ( auto it= 0; it < num_runs; ++it ) {
        //m= (rand() % n)+1;
        std::vector<std::vector<i64>> subsets(m);
        std::map<i64,std::vector<i64>> mp{};
        for ( auto &v: subsets ) {
            v = random_subset(a,6);
            std::sort(v.begin(),v.end());
            auto sum= std::accumulate(v.begin(),v.end(),0ll);
            if ( mp.count(sum) and not(mp[sum] == v) ) {
                bool flag= false;
                os << "Case #" << cs << ":\n";
                for ( auto u: mp[sum] ) {
                    if ( flag ) os << " ";
                    os << u;
                    flag= true ;
                }
                os << "\n";
                flag= false;
                for ( auto u: v ) {
                    if ( flag ) os << " ";
                    os << u;
                    flag= true ;
                }
                os << "\n";
                return ;
            }
            else if ( mp[sum] == v ) {
                //std::cerr << "Two vectors are the same" << std::endl;
            }
            mp[sum]= std::move(v);
        }
    }
    os << "Case #" << cs << ":\nImpossible\n";
}

int main() {
    int i,ts;
    for ( is >> ts, i= 1; i <= ts; solve(i++) );
    return 0;
}   
 