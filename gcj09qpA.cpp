/**
 * Problem A: Alien Language
 * TOPIC: STL, inner_product
 * status: Accepted
 */
#include <bits/stdc++.h>

int m,n;
std::vector<std::string> D;

int main() {
    int cs= 0,ts;
    std::istream &is= std::cin;
    is >> n >> m >> ts, D.resize(m);
    for ( auto &s: D ) {
        is >> s;
        assert( s.size() == n );
    }
    std::function<bool(const std::string&,const std::string&)>
    matches= [](const std::string &s, const std::string &p) {
        std::vector<std::set<char>> pattern{};
        for ( int i= 0,j; i < p.size(); ++i ) {
            std::set<char> ss{};
            if ( p[i] == '(' ) {
                for ( ; ++i < p.size() and p[i] != ')'; ss.insert(p[i]) ) ;
            }
            else
                ss.insert(p[i]);
            pattern.push_back(std::move(ss));
        }
        return std::inner_product(pattern.begin(),pattern.end(),s.begin(),true,
        [](bool status, bool x) {
            return status&= x;
        },
        [](std::set<char> &pt,char x) {
            return pt.count(x) > 0;
        });
    };
    for ( std::string s; ts-- and (is >> s);
    std::cout << "Case #" << ++cs << ": " << std::count_if(D.begin(),D.end(),[&s,&matches](auto z) {
            return matches(z,s);
        }) << std::endl ) ;
    return 0;
}
