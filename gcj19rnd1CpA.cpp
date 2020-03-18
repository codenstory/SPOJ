/**
 * TOPIC: trie, constructive, greedy
 */
#include <bits/stdc++.h>
#define N (256)
using i64= std::int64_t;

int n;
std::string str[N];

struct cell {
    std::unordered_map<char,std::shared_ptr<cell>> son;
    int idx;
    cell() { son.clear(), idx= -1; }
};

void push( std::shared_ptr<cell> *root, const std::string &s, int idx) {
    if ( not *root )
        *root= std::make_shared<cell>();
    auto x= root;
    for ( auto ch: s ) {
        if ( not (*x)->son.count(ch) ) {
            (*x)->son[ch]= std::make_shared<cell>();
        }
        x= &(*x)->son[ch];
    }
    (*x)->idx= idx;
}

i64 gcd( i64 x, i64 y ) { return (not y) ? x:gcd(y,x%y); }

i64 lcm( std::vector<i64> &sizes ) {
    if ( sizes.size() == 1 )
        return sizes.front();
    auto b= sizes.back();
    sizes.pop_back();
    auto L= lcm(sizes);
    return L*b/gcd(L,b);
}

std::unordered_map<char,char> is_beaten_by;

int main() {
    int i,j,k,cs= 0,ts;
    std::istream &is= std::cin;
    is_beaten_by['P']= 'S', is_beaten_by['S']= 'R', is_beaten_by['R']= 'P';
    for ( is >> ts; ts--; ) {
        is >> n;
        for ( i= 0; i < n; is >> str[i++] ) ;
        std::vector<i64> sizes(n);
        for ( i= 0; i < n; ++i )
            sizes[i]= str[i].size();
        std::list<int> alive{};
        for ( i= 0; i < n; alive.push_back(i++) ) ;
        std::string res{};
        for ( i= 0; not alive.empty(); ++i ) {
            std::vector<typename std::list<int>::iterator> elim{};
            std::vector<std::pair<char,typename std::list<int>::iterator>> pairs{};
            for ( std::list<int>::iterator it= alive.begin(); it != alive.end(); ++it )
                pairs.emplace_back(str[*it][i%sizes[*it]],it);
            std::sort(pairs.begin(),pairs.end(),[](auto x, auto y) {
                return x.first < y.first;
            });
            auto end_of_P= std::find_if_not(pairs.begin(),pairs.end(),[](auto pr) {
                return pr.first == 'P';
            });
            auto end_of_R= std::find_if_not(end_of_P,pairs.end(),[](auto pr) {
                return pr.first == 'R';
            });
            auto pp= end_of_P-pairs.begin();
            auto rr= end_of_R-end_of_P;
            auto ss= pairs.end()-end_of_R;
            if ( pp and rr and ss ) {
                goto IMP;
            }
            if ( pp and rr ) {
                res.push_back('P');
                std::for_each(pairs.begin(),pairs.end(),[&elim](auto x) {
                    if ( x.first == 'R' )
                        elim.push_back(x.second);
                });
            }
            else if ( pp and ss ) {
                res.push_back('S');
                std::for_each(pairs.begin(),pairs.end(),[&elim](auto x) {
                    if ( x.first == 'P' )
                        elim.push_back(x.second);
                });
            }
            else if ( rr and ss ){
                res.push_back('R');
                std::for_each(pairs.begin(),pairs.end(),[&elim](auto x) {
                    if ( x.first == 'S' )
                        elim.push_back(x.second);
                });
            }
            else {
                if ( pp ) {
                    res.push_back('S');
                    break ;
                }
                if ( rr ) {
                    res.push_back('P');
                    break ;
                }
                assert( ss );
                res.push_back('R');
                break ;
            }
            for ( auto e: elim )
                alive.erase(e);
        }
        std::cout << "Case #" << ++cs << ": " << res << std::endl;
        continue ;
        IMP:
        std::cout << "Case #" << ++cs << ": IMPOSSIBLE\n";
    }
    return 0;
}

