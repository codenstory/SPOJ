/**
 * TOPIC: DP, fast simulation
 * status:
 */
#include <bits/stdc++.h>
using size_type= int;
#define A (26+1)
#define N (10000)

struct word {
    int weight;
    std::string s;
    int idx;
    word() {};
    word( int w, std::string s, int i ): weight(w), s(s), idx(i) {};
};

bool operator < ( const word &a, const word &b ) {
    return a.weight < b.weight or (a.weight == b.weight and a.idx < b.idx);
}

std::map<size_type,std::vector<word>> buckets;
int n,qr,z[N][A];
std::vector<std::string> D;

int main() {
    int i,j,k,cs= 0,ts;
    std::string L;
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    for ( is >> ts; ts--; ) {
        is >> n >> qr, D.resize(n);
        for ( buckets.clear(), i= 0; i < n; ++i ) {
            word w;
            is >> w.s, k= w.s.size();
            buckets[k].emplace_back(0,w.s,w.idx= i);
            D[i]= std::move(w.s);
        }
        std::vector<int> res{};
        while ( qr-- ) {
            is >> L;
            std::map<char,size_type> pos{};
            for ( i= 0; i < L.size(); ++i )
                pos[L[i]]= i;
            for ( auto &it: buckets ) {
                auto &v= it.second;
                for ( auto &w: v ) {
                    w.weight= pos[*std::max_element(w.s.begin(),w.s.end(),[&pos](auto a,auto b) {
                        return pos[a] < pos[b];
                    })];
                }
                std::sort(v.begin(),v.end());
            }
            std::map<size_type,std::pair<
                    typename std::vector<word>::iterator,
                    typename std::vector<word>::iterator
                    >> sliders{};
            for ( auto &it: buckets )
                sliders[it.first]= {it.second.begin(),it.second.end()};
            std::map<int,size_type> position{};
            for ( const auto &it: buckets ) {
                const auto &v= it.second;
                for ( i= 0; i < v.size(); ++i )
                    position[v[i].idx]= i;
            }

            assert( L.size() == A-1 );
            for ( i= 0; i < n; z[i++][0]= 0 ) ;
            for ( j= 1; j <= L.size(); ++j ) {
                auto ch= L[j-1];
                auto weight= pos[ch];
                for ( i= 0; i < n; ++i ) {
                    z[i][j]= z[i][j-1];
                    auto which_bucket= D[i].size();
                    auto &val= sliders[which_bucket];
                    if ( val.first == val.second or weight < val.first->weight  )
                        continue ;
                    assert( weight == val.first->weight );
                    for ( ;val.first < val.second and weight == val.first->weight; ++val.first ) ;
                    assert( position[i] < buckets[which_bucket].size() );
                    if ( val.second-val.first < buckets[which_bucket].size()-position[i] )
                        continue ; //we've guessed the word
                    ++z[i][j];
                }
            }
            int ans= -1;
            for ( k= 0, i= 0; i < n; ++i )
                if ( ans < z[i][L.size()] )
                    ans= z[k= i][L.size()];
            res.push_back(k);
            std::cerr << "For order " << L << " the weight is " << ans << std::endl;
        }
        os << "Case #" << ++cs << ":";
        for ( auto x: res )
            os << " " << D[x];
        os << "\n";
    }
    return 0;
}

