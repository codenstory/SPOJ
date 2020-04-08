/**
 * TOPIC: DP, trie
 * status: Accepted
 */
#include <bits/stdc++.h>
#define N (0x400*4)
#define L 5

std::istream &is = std::cin;
std::ostream &os = std::cout;

std::vector<std::vector<int>> matches[N][N];

struct cell {
    std::unordered_map<char,std::shared_ptr<cell>> son;
};

void push( std::shared_ptr<cell> &root, std::string &s ) {
    std::shared_ptr<cell> *hold= &root, x= root;
    assert( root );
    for ( char ch: s ) {
        if ( not x->son.count(ch) )
            (*hold)->son[ch] = x->son[ch]= std::make_shared<cell>();
        hold= &x->son[ch], x= x->son[ch];
    }
}

void find_all_matches(
        std::shared_ptr<cell> &root, const std::string &s,
        std::vector<int> &cur,
        int idx, std::vector<std::vector<int>> &vec ) {
    if (idx == s.size()) {
        std::vector<int> entry{};
        entry.insert(entry.end(),cur.begin(),cur.end());
        vec.push_back(std::move(entry));
        return;
    }
    assert(idx < s.size());
    assert(root);
    for (auto &[key, val]: root->son) {
        if (key != s[idx]) {
            if ( not cur.empty() and idx-cur.back() < L )
                continue ;
            cur.push_back(idx);
        }
        find_all_matches(val, s, cur, idx + 1, vec);
        if (key != s[idx]) {
            assert( cur.back() == idx );
            cur.pop_back();
        }
    }
}

std::map<int,std::shared_ptr<cell>> roots{};

void read_dict( const std::string& filename ) {
    std::ifstream ifs(filename);
    int i= 0;
    for ( std::string s; std::getline(ifs,s); ) {
        auto n= s.size();
        if ( not roots.count(n) )
            roots[n]= std::make_shared<cell>();
        push(roots[n],s);
    }
    ifs.close();
}

auto valid_match= []( const std::vector<int> &v )-> bool {
    for ( int i= 0; i+1 < v.size(); ++i ) {
        assert( v[i+1] > v[i] );
        if (v[i + 1] - v[i] < L)
            return false;
    }
    return true ;
};

std::string subs[N][N], s;
int n;

void preprocess() {
    int i,j,k;
    auto vm= valid_match;
    is >> s, n= s.size();
    for ( i= 0; i < n; ++i )
        for ( k= 1; i+k-1 < n; ++k ) {
            matches[i][k].clear(), subs[i][k] = s.substr(i, k);
            std::vector<int> c{};
            if ( not roots[k] ) continue ;
            find_all_matches(roots[k],subs[i][k],c,0,matches[i][k]);
            assert( c.empty() );
            auto it= std::remove_if(matches[i][k].begin(),matches[i][k].end(),[&vm](auto &cc) {
                return not valid_match(cc);
            });
            matches[i][k].erase(it,matches[i][k].end());
            std::sort(matches[i][k].begin(),matches[i][k].end(),[](const auto &a, const auto &b) {
                return a.size() < b.size();
            });
            if ( matches[i][k].empty() ) continue ;
            auto &vec= matches[i][k];
            if ( vec.front().empty() ) {
                vec.erase(std::next(vec.begin()),vec.end());
            }
            else {
            }
        }
}

void init() {
    int i,k;
    is >> s, n= s.size();
    for ( i= 0; i < n; ++i )
        for ( k= 1; i+k-1 < n; ++k ) {
            matches[i][k].clear();
        }
}

using i64= std::int64_t;
std::optional<i64> z[N][L];
#define oo (1ll<<60)

std::optional<i64> calc_z( int i, int k ) {
    if ( i < 0 )
        return 0;
    assert( i >= 0 );
    if ( z[i][k] ) return z[i][k];
    z[i][k]= +oo;
    for ( int len, j= 0; j <= i; subs[j][len].clear(), matches[j++][len].clear() ) {
        if ( not roots[len= i-j+1] ) continue ;
        {
            subs[j][len]= s.substr(j,len);
            std::vector<int> cur{};
            find_all_matches(roots[len], subs[j][len], cur, 0, matches[j][len]);
            auto vm= valid_match;
            matches[j][len].erase(std::remove_if(matches[j][len].begin(),matches[j][len].end(),[&vm](const auto &c) {
                return not vm(c);
            }),matches[j][len].end());
        }
        for ( const auto &c: matches[j][len] ) {
            if ( c.empty() ) {
                if ( k == L-1 ) {
                    for ( int kk= 0; kk <= L-1; ++kk )
                        if ( kk+len >= L-1 )
                            if ( calc_z(j-1,kk) and *calc_z(j-1,kk) < +oo )
                                if ( *z[i][k] > *calc_z(j-1,kk)+c.size() )
                                    z[i][k]= *calc_z(j-1,kk)+c.size();
                }
                else {
                    for (int kk = 0; kk <= L - 1; ++kk)
                        if (kk + len == k)
                            if (calc_z(j - 1, kk) and *calc_z(j - 1, kk) < +oo)
                                if (*z[i][k] > *calc_z(j - 1, kk) + c.size())
                                    z[i][k] = *calc_z(j - 1, kk) + c.size();
                }
                continue ;
            }
            if ( k == len-1-c.back() or (k == L-1 and c.back() < len-1-k) ) {
                for ( int kk= 0; kk <= L-1; ++kk )
                    if ( kk+c.front()+1 >= L )
                        if ( calc_z(j-1,kk) and *calc_z(j-1,kk) < +oo )
                            if ( *z[i][k] > *calc_z(j-1,kk)+c.size() )
                                z[i][k]= *calc_z(j-1,kk)+c.size();
            }
        }
    }
    return z[i][k];
}

void solve( int cs ) {
    int i,j,k;
    for ( init(), i= 0; i <= n; ++i )
        for ( k= 0; k < L; z[i][k++]= std::nullopt ) ;
    i64 ans= +oo;
    for ( k= 0; k < L; ++k )
        if ( calc_z(n-1,k) )
            ans= std::min(ans,*calc_z(n-1,k));
    os << "Case #" << cs << ": " << ans << std::endl;
}

int main() {
    int cs= 0,ts;
    read_dict("garbled_email_dictionary.txt");
    for ( is >> ts; ts--; solve(++cs) ) ;
    return 0;
}   
 