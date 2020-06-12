/**
 * PRHYME
 * TOPIC: Trie, greedy
 * status:
 */
#include <bits/stdc++.h>
#define oo (1<<29)

std::istream &is = std::cin;
std::ostream &os = std::cout;

class Solution {
    struct cell {
        std::map<char, std::shared_ptr<cell>> son;
        std::shared_ptr<cell> p;
        bool flag;
        int idx, smallest;
        std::map<int, char> which;

        cell() { smallest = +oo, flag = false, son.clear(), p = nullptr, which.clear(); }

        std::string s;
    };

    std::shared_ptr<Solution::cell> root= std::make_shared<cell>();

    int traverse(std::shared_ptr<cell> &x) {
        if (x) {
            x->smallest = (x->flag ? x->idx : +oo);
            for (auto &[k, v]: x->son) {
                x->which[traverse(v)] = k;
                x->smallest = std::min(x->smallest, v->smallest);
            }
            return x->smallest;
        }
        return +oo;
    }

    std::string smallest_in_subtree(std::shared_ptr<cell> &x) {
        if (x->flag and x->smallest == x->idx) {
            return x->s;
        }
        return smallest_in_subtree(x->son[x->which.begin()->second]);
    }

public:

    void push(const std::string &s, int idx) {
        std::shared_ptr<cell> *hold = &root, x = root;
        for (auto ch: s) {
            if (not x->son.count(ch))
                (*hold)->son[ch] = x->son[ch] = std::make_shared<cell>();
            hold = &(x->son[ch]), (*hold)->p = x, x = x->son[ch];
        }
        assert(not x->flag); //no two words are equal
        x->flag = true, x->s = s, std::reverse(x->s.begin(), x->s.end());
        x->idx = idx;
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
            if (not x->son.empty()) {
                return smallest_in_subtree(x->son[x->which.begin()->second]);
            }
            for (y = x, x = x->p; x and (not x->flag and x->son.size() == 1); y = x, x = x->p);
            assert(x);
            if (x->flag)
                return x->s;
            for (auto it = x->which.begin(); it != x->which.end(); ++it)
                if (x->son[it->second] != y)
                    return smallest_in_subtree(x->son[it->second]);
            assert(false);
        }
        return std::string{};
    }
    void finalize() {
        traverse(root);
    }
};

std::string prep(const std::string &s) {
    std::string str{};
    for (auto ch: s)
        if (std::islower(ch))
            str.push_back(ch);
    return str;
}

void solve( std::istream &input, std::ostream &output ) {
    Solution sol;
    std::string s;
    std::vector<std::string> vec{};
    for ( ;std::getline(input,s) and not s.empty(); vec.emplace_back(prep(s)) ) ;
    std::sort(vec.begin(),vec.end());
    for ( int i= 0; i < vec.size(); ++i ) {
        auto t= vec[i];
        std::reverse(t.begin(),t.end());
        sol.push(t,i);
    }
    for ( sol.finalize(); std::getline(input,s) and not s.empty(); std::reverse(s.begin(),s.end()),s= prep(s), output << sol.find(s) << '\n' ) ;
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
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    test_all();
    solve(is,os);
    return 0;
}
    