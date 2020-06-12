#include <utility>

/**
 * PRHYME
 * TOPIC: Trie, greedy
 * status:
 */
#include <bits/stdc++.h>
#define oo (1<<29)

std::string brute_force( const std::vector<std::string> &corpus, const std::string &s ) {
    auto t= s;
    auto common_suffix= [&t](const std::string &x ) {
        int k= 0,i,j;
        for ( i= x.size()-1, j= t.size()-1; i >= 0 and j >= 0; --i, --j, ++k )
            if ( x[i] != t[j] )
                return k;
        return k;
    };
    return *std::min_element(corpus.begin(),corpus.end(),[&t,&common_suffix]( const auto &x, const auto &y ) {
        assert( x != y );
        if ( x == t ) return false ;
        if ( y == t ) return true ;
        auto ax= common_suffix(x), ay= common_suffix(y);
        if ( ax == ay )
            return x < y;
        return ax > ay;
    });
}

class Solution {

    struct cell {
        std::map<char, std::shared_ptr<cell>> son;
        std::shared_ptr<cell> p;
        bool flag;
        int idx, smallest, card;
        std::map<int, char> which;

        explicit cell(std::shared_ptr<cell> prnt= nullptr) {
            smallest = +oo, idx= -1, card= 0, flag = false, s= std::string{}, son.clear(), p = std::move(prnt), which.clear();
        }

        std::string s{};
    };

    std::shared_ptr<Solution::cell> root= std::make_shared<cell>();

    int traverse(std::shared_ptr<cell> &x) {
        if (x) {
            x->card= (x->flag ? 1:0);
            for (auto &[k, v]: x->son) {
                x->which[traverse(v)] = k;
                assert( v->smallest < +oo );
                x->smallest = std::min(x->smallest, v->smallest);
                x->card+= v->card;
            }
            assert( x->smallest < +oo );
            return x->smallest;
        }
        return +oo;
    }

    std::string smallest_in_subtree(std::shared_ptr<cell> &x, int avoid= -1) {
        if (x->smallest == x->idx and x->idx != avoid) {
            assert( x->flag );
            return x->s;
        }
        assert( not x->which.empty() );
        assert( x->son.size() == x->which.size() );
        for ( auto it= x->which.begin(); it != x->which.end(); ++it ) {
            auto z= x->son[it->second];
            if ( it->first != avoid or z->card > 1 )
                return smallest_in_subtree(z,avoid);
        }
        return std::string{};
    }

public:

    void push(const std::string &s, const int idx) {
        std::shared_ptr<cell> *hold = &root, x = root;
        for (auto ch: s) {
            if (not x->son.count(ch))
                (*hold)->son[ch] = std::make_shared<cell>(*hold);
            hold = &(x->son[ch]), x = x->son[ch];
        }
        assert(not x->flag); //no two words are equal
        x->flag = true, x->s = s, std::reverse(x->s.begin(), x->s.end());
        x->smallest= x->idx = idx;
        if ( x->s == "actinal" )
            std::cerr << "Here: " << x->idx << std::endl;
    }

    std::string find(const std::string &s) {
        auto x = root, y = x;
        for (auto ch: s) {
            if (x->son.count(ch)) {
                x = x->son[ch];
            } else {
                return smallest_in_subtree(x);
            }
        }
        if (not x->flag) {
            assert(not x->son.empty());
            return smallest_in_subtree(x);
        } else {
            auto o= x->idx;
            assert( o != -1 );
            if (not x->son.empty()) {
                assert( not x->which.empty() );
                assert( x->which.size() == x->son.size() );
                return smallest_in_subtree(x->son[x->which.begin()->second]);
            }
            assert( x->flag and x->son.empty() );
            for (y = x, x = x->p; x and (not x->flag and x->card == 1); y = x, x = x->p);
            assert(x);
            return smallest_in_subtree(x,o);
        }
    }

    void finalize() {
        traverse(root);
    }

};

std::string prep(const std::string &s) {
    std::string str{};
    for (auto ch: s)
        if (std::isalpha(ch))
            str.push_back(std::tolower(ch));
    return str;
}

void solve( std::istream &input, std::ostream &output ) {
    Solution sol;
    std::string s,t;
    std::vector<std::string> vec{};
    for (;std::getline(input,s) and not (t= prep(s)).empty(); vec.emplace_back(t) ) ;
    std::sort(vec.begin(),vec.end());
    for ( int i= 0; i < vec.size(); ++i ) {
        t= vec[i];
        if ( i ) assert( vec[i-1] < vec[i] );
        std::reverse(t.begin(),t.end());
        sol.push(t,i);
    }
    for ( sol.finalize(); std::getline(input,s); ) {
        auto orig= s,bf= s;
        s= prep(s), std::reverse(s.begin(),s.end());
        auto res= sol.find(s);
#if 1
        if ( res != (bf= brute_force(vec,orig)) ) {
            std::cerr << "Request: " << orig << std::endl;
            std::cerr << "These words: " << res << " vs " << bf << std::endl;
        }
        assert( res == bf );
#endif
        output << res << '\n';
        std::reverse(res.begin(),res.end());
        assert( not res.empty() );
        assert( res != s );
    }
}

void test_all() {
    {
        std::istringstream input(R"(perfect
rhyme
crime
time

crime
rhyme)");
        std::ostringstream out,gold(R"(time
crime
)");
        Solution sol;
        solve(input,out);
        assert( out.str() == gold.str() );
    }
    {
        std::istringstream input(R"(happy
crappy
time

time)");
        std::ostringstream out,gold(R"(crappy
)");
        Solution sol;
        solve(input,out);
        assert( out.str() == gold.str() );
    }
    {
        std::istringstream input(R"(abc
def

abc
def)");
        std::ostringstream out,gold(R"(def
abc
)");
        Solution sol;
        solve(input,out);
        assert( out.str() == gold.str() );
    }
    {
        std::istringstream input(R"(a
aa
aaa

aa
aaa
aaaa
a
)");

        std::ostringstream out,gold(R"(aaa
aa
aaa
aa
)");
        Solution sol;
        solve(input,out);
        assert( out.str() == gold.str() );
    }
}


int main() {
    std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    //test_all();
    std::istream &is = std::cin;
    std::ostream &os = std::cout;
    solve(is,os);
    return 0;
}
    